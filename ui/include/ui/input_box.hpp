
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef INPUT_BOX_HPP
#define INPUT_BOX_HPP

#include "basic.hpp"
#include "string_stencil.hpp"

namespace cons {

//enum class { no_input, new_line, new_char, control_char };

class input_box : public base {
public:
	using grow_cb =std::function<bool(point)>;
private:
	grow_cb        on_grow;
	frame          frame_;
	std::string    value;
	string_stencil stencil;
public:
	input_box(unique_window_ptr ptr)
	:	frame_ { std::move(ptr) }
	{	}

	point calc_cursor_position() const {
		auto dim=get_dimension();
		return {
			int(value.size()) % dim.x,
			int(value.size()) / dim.x,
		};
	}

	int get_char() {
		noecho();
		auto pos=calc_cursor_position();
		int c=mvwgetch(frame_.get_handle(), pos.y, pos.x);
		if(c==0x08) {
			if(!value.empty()) {
				value.pop_back();
				refresh();
			}
		}
		else if(std::isprint(c)) {
			value+=c;
			refresh();
		}
		return c;
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
	void reg_on_grow(grow_cb gcb) {
		on_grow=std::move(gcb);
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
}; //class input_box

} //namespace cons

#endif //INPUT_BOX_HPP
