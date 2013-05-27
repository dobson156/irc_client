#include "assert.hpp"
#include "async_input_box.hpp"

#include <algorithm>

namespace cons {

async_input_box::async_input_box(unique_window_ptr ptr, 
                                 boost::asio::io_service& io_service)
:	frame_        { std::move(ptr) }
,	in_manager    { make_tty_manager(io_service) }
,	io_service    { &io_service    }
{	
	set();
}

void async_input_box::stop() {
	in_manager.stop();
}

point async_input_box::calc_cursor_position() const {
	auto dim=get_dimension();
	return {
		int(value.size()) % dim.x,
		int(value.size()) / dim.x,
	};
}

void async_input_box::handle_read_error(const boost::system::error_code&) {

}

async_input_box::arrow_key async_input_box::is_arrow_key(
                                        std::string::const_iterator it, 
					                    std::string::const_iterator last) {
	if(std::distance(it, last) >= 3
	&& it[0] == 0x1b && it[1] == 0x5b) {
		switch(it[2]) {
		default:   return arrow_key::none;
		case 0x43: return arrow_key::right;
		case 0x44: return arrow_key::left;
		}
	}
	else {
		return arrow_key::none;
	}
}

void async_input_box::handle_read_complete(std::string str) {
	bool do_refresh=false;

	for(auto it = str.begin(); it != str.end(); ++it) {

		char c = *it;
		arrow_key is_arrow_key_;

		if((is_arrow_key_=is_arrow_key(it, str.end())) != arrow_key::none) {
			switch(is_arrow_key_) {
			case arrow_key::left: { 
				it+=2; 
				if(pos > 0) 
					--pos; 
				break;
			} 
			case arrow_key::right: { 
				it+=2; 
				if(pos < value.size()) 
					++pos; 
				break;
			}
			default: 
				break;
			}
		}
		else if(c==0x08 || c==0x7f) {
			if(!value.empty() && pos != 0) {
				value.erase(pos-1, 1);
				--pos;
				//value.pop_back();
				do_refresh=true;		
			}
		}
		else if(c=='\r') {
			if(on_input) on_input(value);
		}
		else if(std::isprint(c)) {
			value.insert(value.begin()+pos, c);
			++pos;
			do_refresh=true;		
		}
	}	
	if(do_refresh) refresh();
	set();
}

void async_input_box::set() {
	auto pos=calc_cursor_position();

	wmove(frame_.get_handle(), pos.y, pos.x);
	frame_.refresh();

	in_manager.async_read(
		std::bind(&async_input_box::handle_read_complete, this, ph::_1)
	//	std::bind(&async_input_box::handle_read_error,    this, ph::_1)
	);
}

bool async_input_box::grow(point pt) {
	return on_grow 
		 ? on_grow(pt) 
		 : false;
}

void async_input_box::refresh() {
	auto dim=get_dimension();
	int y=stencil.required_y(dim.x, value.size());
	point required { dim.x, y };

	frame_.clear();

	if(value.empty()) {
		if(dim.y!=1 && grow({dim.x, 1})) {
			//In theory this should invoke a recursive refresh
			//we should just guard the function so that can't happen
			//and explicitly reresh on the first iteration
			return;
		}
		frame_.refresh();
		return;
	}
	if(y <= dim.y) {
		stencil.write_to(frame_, value);
	}
	else if(!grow(required)) {	
		//TODO: a pad based draw
	}
	frame_.refresh();
}

void async_input_box::clear() {
	pos=0;
	value.clear();
	frame_.clear();
}

void async_input_box::set_value(const std::string& str) { 
	value=str; 
	pos=str.size();
}
const std::string& async_input_box::get_value() const { 
	return value; 
}

//watch thread safety
//what happens if a thread on this callback modifies this value?
void async_input_box::reg_on_grow(grow_cb gcb) {
	on_grow=std::move(gcb);
}
void async_input_box::reg_on_input(input_cb cb) {
	on_input=std::move(cb);
}

//Overrides
void async_input_box::set_position(const point& position) { 
	auto dim=frame_.get_dimension();
	WINDOW *p=wgetparent(frame_.get_handle());

	CONS_ASSERT(p, "parent is null");

	frame_=frame { make_window(p, position, dim) };
	touchwin(frame_.get_handle());
	//frame_.set_position(position); 
}
void async_input_box::set_dimension(const point& dimension) { 
	frame_.set_dimension(dimension); 
	touchwin(frame_.get_handle());
}
point async_input_box::get_position()  const { 
	return frame_.get_position();  
}
point async_input_box::get_dimension() const { 
	return frame_.get_dimension(); 
}
unique_window_ptr async_input_box::reset(unique_window_ptr handle) {
	return frame_.reset(std::move(handle));
}
//Colour
void async_input_box::set_background(short bg) { 
	frame_.set_background(bg); 
}
void async_input_box::set_foreground(short fg) { 
	frame_.set_foreground(fg); 
}
short async_input_box::get_background() const { 
	return frame_.get_background(); 
}
short async_input_box::get_foreground() const { 
	return frame_.get_foreground(); 
}

} //namespace cons
