
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "assert.hpp"
#include "async_input_box.hpp"

#include "ctrl_char_parser.hpp"

#include <algorithm>

#include <fstream>

namespace cons {

async_input_box::async_input_box(unique_window_ptr ptr, 
                                 boost::asio::io_service& io_service)
:	frame_        { std::move(ptr)               }
,	in_manager    { make_tty_manager(io_service) }
,	io_service    { &io_service                  }
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

void async_input_box::handle_read_complete(std::string str) {
	bool do_refresh=false;

	for(auto it=str.cbegin(); it < str.cend(); ++it) {

		char c = *it;

		ctrl_char cht;
		std::tie(cht, it)=parse_ctrl_char(it, str.cend());

		switch(cht) {
		case ctrl_char::none:
			if(std::isprint(c)) {
				value.insert(value.begin()+pos, *it);
				++pos;
				do_refresh=true;		
			}
			break;
		case ctrl_char::newline: //fall through
		case ctrl_char::carriage_ret:
			on_input(value);
			break;
		case ctrl_char::arrow_right:
			if(pos < value.size()) 
				++pos; 
			break;
		case ctrl_char::arrow_left:
			if(pos > 0) 
				--pos; 
			break;
		case ctrl_char::backspace:
			if(!value.empty() && pos != 0) {
				value.erase(pos-1, 1);
				--pos;
				do_refresh=true;		
			}
			break;
		default:
			//All other cases are passed on to the user
			on_ctrl_char(cht);
			break;
		}
	}	
	if(do_refresh) refresh();
	set();
}

void async_input_box::set() {
	auto pos=calc_cursor_position();

	wmove(frame_.get_handle(), pos.y, pos.x);
	frame_.refresh();

	//TODO: handle  error
	in_manager.async_read(
		std::bind(&async_input_box::handle_read_complete, this, ph::_1));
}

bool async_input_box::grow(point pt) {
	//TODO ensure this works as expected in the
	//true, false and unconnected cases
	return on_grow(pt);
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
