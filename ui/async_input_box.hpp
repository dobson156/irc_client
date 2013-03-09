#ifndef ASYNC_INPUT_BOX_HPP
#define ASYNC_INPUT_BOX_HPP

#include "input_manager.hpp"

#include "basic.hpp"
#include "string_stencil.hpp"

namespace cons {

//enum class { no_input, new_line, new_char, control_char };

class async_input_box : public base {
public:
	using grow_cb =std::function<bool(point)>;
	using input_cb=std::function<void(std::string)>;
private:
	grow_cb                  on_grow;
	input_cb                 on_input;

	frame                    frame_;
	std::string              value;
	string_stencil           stencil;
	input_manager            in_manager;
	boost::asio::io_service *io_service;



public:
	async_input_box(unique_window_ptr ptr, boost::asio::io_service& io_service)
	:	frame_        { std::move(ptr) }
	,	in_manager    { make_tty_manager(io_service) }
	,	io_service    { &io_service    }
	{	
		set();
	}

	point calc_cursor_position() const {
		auto dim=get_dimension();
		return {
			int(value.size()) % dim.x,
			int(value.size()) / dim.x,
		};
	}

	void handle_read_error(const boost::system::error_code&) {
	
	}

	void handle_read_complete(std::string str) {
		bool do_refresh=false;

		for(char c : str) {
			if(c==0x08) {
				if(!value.empty()) {
					value.pop_back();
					do_refresh=true;		
				}
			}
			else if(c=='\r') {
				if(on_input) on_input(value);
			}
			else if(std::isprint(c)) {
				value+=c;
				do_refresh=true;		
			}
		}	
		if(do_refresh) refresh();
		set();
	}

	void set() {
		auto pos=calc_cursor_position();

		wmove(frame_.get_handle(), pos.y, pos.x);

		in_manager.async_read(
			std::bind(&async_input_box::handle_read_complete, this, ph::_1)
		//	std::bind(&async_input_box::handle_read_error,    this, ph::_1)
		);
	}

	bool grow(point pt) {
		return on_grow 
		     ? on_grow(pt) 
			 : false;
	}

	void refresh() {
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

	void clear() {
		value.clear();
		frame_.clear();
	}

	std::string&       get_value()       { return value; }
	const std::string& get_value() const { return value; }

	//watch thread safety
	//what happens if a thread on this callback modifies this value?
	void reg_on_grow(grow_cb gcb) {
		on_grow=std::move(gcb);
	}
	void reg_on_input(input_cb cb) {
		on_input=std::move(cb);
	}

//Overrides
	void set_position(const point& position) override { 
		auto dim=frame_.get_dimension();
		WINDOW *p=wgetparent(frame_.get_handle());

		CONS_ASSERT(p, "parent is null");

		frame_=frame { make_window(p, position, dim) };
		touchwin(frame_.get_handle());
		//frame_.set_position(position); 
	}
	void set_dimension(const point& dimension) override { 
		frame_.set_dimension(dimension); 
		touchwin(frame_.get_handle());
	}
	point get_position()  const override { return frame_.get_position();  }
	point get_dimension() const override { return frame_.get_dimension(); }

	unique_window_ptr reset(unique_window_ptr handle) override {
		return frame_.reset(std::move(handle));
	}
//Colour
	void set_background(short bg) { frame_.set_background(bg); }
	void set_foreground(short fg) { frame_.set_foreground(fg); }
	short get_background() const  { return frame_.get_background(); }
	short get_foreground() const  { return frame_.get_foreground(); }
}; //class async_input_box

} //namespace cons

#endif //ASYNC_INPUT_BOX_HPP
