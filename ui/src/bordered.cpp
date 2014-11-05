
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "frame.hpp"
#include "point.hpp"
#include "window.hpp"
#include "bordered.hpp"
#include "exception.hpp"

#include <ncurses.h>

#include <algorithm> //min, max

namespace cons {

point bordered::calc_element_dimension() const {
	auto dim=get_dimension() -  point{ right+left, top+bottom };
	return {
		std::max(dim.x, 0),
		std::max(dim.y, 0)
	};
}

point bordered::calc_element_position() const {
	return { int(left), int(top) };
}

unique_window_ptr bordered::make_element_window() {
	return make_window(
		frame_.get_handle(),
		calc_element_position(),
		calc_element_dimension()
	);
}

void bordered::clear() {
	frame_.clear();
	if(element) element->clear();
}
void bordered::refresh() {
	frame_.clear();
 	auto dim=get_dimension();

	if(right
	&& mvwvline(frame_.get_handle(), 0, dim.x-1, '|', dim.y)) {
		throw CONS_MAKE_EXCEPTION("unable to draw right border");
	}
	if(left
	&& mvwvline(frame_.get_handle(), 0, 0, '|', dim.y)) {
		throw CONS_MAKE_EXCEPTION("unable to draw left border");
	}
	if(top
	&& mvwhline(frame_.get_handle(), 0, 0, '-', dim.x)) {
		throw CONS_MAKE_EXCEPTION("unable to draw top border");
	}
	if(bottom
	&& mvwhline(frame_.get_handle(), dim.y-1, 0, '-', dim.x)) {
		throw CONS_MAKE_EXCEPTION("unable to draw bottom border");
	}
	frame_.refresh();
	if(element) element->refresh();
}

void bordered::set_position(const point& position) {
	//IMPLICATIONS ON CHILDREN???
	frame_.set_position(position);
	if(element) element->set_position(calc_element_position());
}
void bordered::set_dimension(const point& dimension) { 
	frame_.set_dimension(dimension); 
	if(element) {
		//element->set_position(calc_element_position());
		element->set_dimension(calc_element_dimension());
		touchwin(frame_.get_handle());
	}

}
point bordered::get_dimension()  const { return frame_.get_dimension();   }
point bordered::get_position()   const { return frame_.get_position();    }
short bordered::get_background() const  { return frame_.get_background(); }
short bordered::get_foreground() const  { return frame_.get_foreground(); }
void bordered::set_background(short bg) { frame_.set_background(bg);      }
void bordered::set_foreground(short fg) { frame_.set_foreground(fg);      }

unique_window_ptr bordered::reset(unique_window_ptr handle) {
	handle=frame_.reset(std::move(handle));
	element->reset(make_element_window());
	return handle;
}

} //namespace cons
