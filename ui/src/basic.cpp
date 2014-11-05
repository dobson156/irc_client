
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//#include "assert.hpp"
//#include "uilib.hpp"

#include "console.hpp"
#include "util.hpp"

#include <string>
#include <ostream>
#include <utility>
#include <sstream>
#include <stdexcept>

#ifdef CONS_FAST_COMPILE
	template class cons::stenciled_frame<cons::string_stencil>;
	template class cons::stenciled_list<cons::string_stencil>;
	template class cons::anchor_view<cons::anchors::top>;
	template class cons::anchor_view<cons::anchors::bottom>;
	template class cons::anchor_view<cons::anchors::left>;
	template class cons::anchor_view<cons::anchors::right>;
#endif

namespace cons {

std::ostream& operator<<(std::ostream& os, const point& p) {
	return os << "x: " << p.x << " y: " << p.y;
}
bool operator==(const point& l, const point& r) {
	return l.x == r.x && l.y == r.y;
}
point operator+(const point& l, const point& r) {
	return { l.x + r.x, l.y + r.y };
}
point operator-(const point& l, const point& r) {
	return { l.x - r.x, l.y - r.y };
}

bool operator<(const point& l, const point& r) {
	return l.x < r.x && l.y < r.y;
}

int destroy_parent(WINDOW *win) {
	delwin(win);
	endwin();
	return 0;
}

unique_window_ptr make_window() {
	unique_window_ptr parent {
		initscr(),
		&destroy_parent
		//&::delwin
	};
	start_color();
	curs_set(0);
	//TODO: leaveok()
	if(parent==nullptr) {
		throw CONS_MAKE_EXCEPTION("Unable to initialise screen");
	}
	return parent;
}
unique_window_ptr make_window(WINDOW *parent, const point& position, 
                                              const point& dimension) {
	CONS_ASSERT(parent, "invalid parent handle");
	unique_window_ptr child {
		derwin(
			parent,
			dimension.y, dimension.x,
			position.y,  position.x
		),
		&::delwin
	};
	if(!child) {
		std::ostringstream oss;
		oss << "unable to create new window at: " << position 
		    << " with dimension: " << dimension;
		throw CONS_MAKE_EXCEPTION(oss.str());
	}
	if(touchwin(parent)==ERR)
		throw CONS_MAKE_EXCEPTION("unable to touchwin");
	return child;
}
unique_window_ptr make_pad(const point& dimension) {
	unique_window_ptr handle {
		newpad(dimension.y, dimension.x),
		&::delwin
	};
	if(!handle) {
		std::ostringstream oss;
		oss << "unable to create pad with dimension: " << dimension;
		throw CONS_MAKE_EXCEPTION(oss.str());
	}
	return handle;
}


/*
//ATTRIBUTE
attribute::attribute(int attribute_id_)
:	attribute_id { attribute_id_ }
{	}

	CONS_ASSERT(handle!=nullptr);
	if(wattron(handle, attribute_id)==ERR) {
		throw CONS_MAKE_EXCEPTION("Unable to apply attribute to frame");
	}
}

int attribute::get_id() const    { return attribute_id;     }
//STATIC MEMBER FUNCTIONS
attribute attribute::normal()    { return { WA_NORMAL };    }
attribute attribute::underline() { return { WA_UNDERLINE }; }
attribute attribute::reverse()   { return { WA_REVERSE };   }
attribute attribute::blink()     { return { WA_BLINK };     }
attribute attribute::dim()       { return { WA_DIM };       }
attribute attribute::bold()      { return { WA_BOLD };      }
*/

} //namespace cons
