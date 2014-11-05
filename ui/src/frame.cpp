#include "frame.hpp"
#include "assert.hpp"
#include "exception.hpp"

#include <ncurses.h>

#include <string>
#include <sstream>

namespace cons {

//CLASS frame
point frame::get_cursor() const {
	CONS_ASSERT(handle, "invalid handle");
	int y, x;
	getyx(handle.get(), y, x);
	return { x, y };
}
void frame::set_cursor(const point& pt) {
	CONS_ASSERT(handle, "invalid handle");
	if(wmove(handle.get(), pt.y, pt.x)==ERR) {
		std::ostringstream oss;
		oss << "Unable to move cursor to " << pt
		    << "\tFrame dimensions are " << get_dimension();
		throw CONS_MAKE_EXCEPTION(oss.str());
	}
}
void frame::apply_colour() {
	if(wbkgd(handle.get(), colours.get_id())==ERR) {
		throw CONS_MAKE_EXCEPTION("Unable to set background of frame");
	}
}

//Ctor
frame::frame(unique_window_ptr handle_)
:	base()
,	handle { std::move(handle_) }
{	}

unique_window_ptr frame::reset(unique_window_ptr handle_) {
	//we don't actually care if it was previously invalid
	CONS_ASSERT(handle_, "can not assign a invalid handle");
	std::swap(handle_, handle);
	apply_colour();
	return handle_;
}

//Content
point frame::write(const point& pt, char ch) {
	CONS_ASSERT(handle, "inavlid handle");
	set_cursor(pt); //throws
	if(waddch(handle.get(), ch)==ERR) {
		std::ostringstream oss;
		oss << "Unable to write char to frame at point: " << pt
			<< " frame size: " << get_dimension();
		throw CONS_MAKE_EXCEPTION(oss.str());
	}
	return get_cursor();
}
point frame::write(const point& pt, const std::string& str) {
	CONS_ASSERT(handle, "inavlid handle");
	set_cursor(pt); //throws
	if(waddstr(handle.get(), str.c_str())==ERR) {
		std::ostringstream oss;
		oss << "Unable to write string to frame at point: " << pt
		    << " string size: " << str.size()
			<< " frame size: " << get_dimension();
		throw CONS_MAKE_EXCEPTION(oss.str());
	}
	return get_cursor();
}
point frame::write(const point& pt, const std::string& str, int n) {
	CONS_ASSERT(handle, "invalid handle");
	CONS_ASSERT(n>=0 && std::size_t(n) <= str.size(),
		"n is greater than string length");
	set_cursor(pt); //throws
	if(waddnstr(handle.get(), str.c_str(), n)==ERR) {
		throw CONS_MAKE_EXCEPTION("Unable to write string to frame");
	}
	return get_cursor();
}
point frame::write(const point& pt, std::string::const_iterator first,
                                    std::string::const_iterator last) {
	CONS_ASSERT(handle, "invalid handle");
	set_cursor(pt); //throws
	if(waddnstr(handle.get(), &*first, std::distance(first, last))==ERR) {

		std::ostringstream oss;
		oss << "Unable to write string to frame   no. of chars: "
		    << std::distance(first, last)
			<< " size: " << get_dimension()
			<< " pos " << pt;
		throw CONS_MAKE_EXCEPTION(oss.str());
	}
	return get_cursor();
}


//Overrides
void frame::clear() {
	CONS_ASSERT(handle, "inavlid handle");
	if(werase(handle.get())==ERR) {
		throw CONS_MAKE_EXCEPTION("Unable to erase frame");
	}
}
void frame::refresh() {
	CONS_ASSERT(handle, "inavlid handle");
	touchwin(handle.get());
	if(wrefresh(handle.get())==ERR) {
		throw CONS_MAKE_EXCEPTION("Unable to refresh frame");
	}
}
void frame::set_position(const point& position) {
	CONS_ASSERT(handle, "inavlid handle");
	if(mvderwin(handle.get(), position.y, position.x)==ERR) {
		std::ostringstream oss;
		oss << "Unable to move frame to: " << position;
		throw CONS_MAKE_EXCEPTION(oss.str());
	}
	touchwin(handle.get());
}
void frame::set_dimension(const point& dimension) {
	CONS_ASSERT(handle, "inavlid handle");
	if(wresize(handle.get(), dimension.y, dimension.x)==ERR) {
		std::ostringstream oss;
		oss << "Unable to resize frame from: " << get_dimension()
		    << " to "                          << dimension;
		throw CONS_MAKE_EXCEPTION(oss.str());
	}
	touchwin(handle.get());
}
point frame::get_dimension() const {
	point p;
	getmaxyx(handle.get(), p.y, p.x);
	return p;
}
point frame::get_position() const {
	point p;
	getparyx(handle.get(), p.y, p.x);
	return p;
}

//Colours
void frame::set_background(short bg) {
	CONS_ASSERT(handle, "invalid handle");
	colours.set_background(bg);
	apply_colour();
}
void frame::set_foreground(short fg) {
	CONS_ASSERT(handle, "invalid handle");
	colours.set_foreground(fg);
	apply_colour();
}
short frame::get_background() const {
	return colours.get_background();
}
short frame::get_foreground() const {
	return colours.get_foreground();
}


void frame::set_attribute(bool set, int attr) {
	CONS_ASSERT(handle, "invalid handle");
	int ret;
	if(set)
		ret=wattron(handle.get(), attr);
	else 
		ret=wattroff(handle.get(), attr);

	if(ret!=OK) {
		std::ostringstream oss;
		oss << "failed to " << ( set ? "" : "un" )
		    << "set attribute value: " << attr;
		throw CONS_MAKE_EXCEPTION(oss.str());
	}
}

void frame::set_underlined(bool set) { set_attribute(set, A_UNDERLINE); }
void frame::set_blinking  (bool set) { set_attribute(set, A_BLINK); }
void frame::set_dim       (bool set) { set_attribute(set, A_DIM); }
void frame::set_bold      (bool set) { set_attribute(set, A_BOLD); }
void frame::set_colour(const colour_pair& cp) { set_attribute(true, cp.get_id()); }
void frame::set_colour(const colour_pair& cp, bool set) { set_attribute(set, cp.get_id()); }


bool frame::is_attr_on(int attr_on) const {
	CONS_ASSERT(handle, "invalid handle");
	attr_t attr;
	short pair;
	wattr_get(handle.get(), &attr, &pair, nullptr);
	return attr & attr_on;
}

bool frame::is_underlined() const { return is_attr_on(A_UNDERLINE); }
bool frame::is_blinking()   const { return is_attr_on(A_BLINK); }
bool frame::is_dim()        const { return is_attr_on(A_DIM); }
bool frame::is_bold()       const { return is_attr_on(A_BOLD); }

WINDOW *frame::get_handle() {
	CONS_ASSERT(handle, "invalid handle");
	return handle.get();
}

//this is a higher level error checked version of copywin
void copy(frame& src, frame& dst,
          const point& src_upper_left,
          const point& dst_upper_left,
          const point& size) {

	auto dst_lower_right=dst_upper_left + size - point{1,1};

#ifndef NDEBUG
	{
		//this like all high arity functions is a bitch to get right
		//hence this debug block
		auto dst_dim=dst.get_dimension();
		auto src_dim=src.get_dimension();
		auto src_lower_right=src_upper_left + size - point{1,1};

		CONS_ASSERT(dst_dim.x > dst_lower_right.x
		         && dst_dim.y > dst_lower_right.y,
		            "copy area is larger than destination"
		);
		CONS_ASSERT(src_dim.x > src_lower_right.x
		         && src_dim.y > src_lower_right.y,
	 	            "copy area is larger than source: "
		);
		CONS_ASSERT(src.get_handle(), "source frame is not valid");
		CONS_ASSERT(dst.get_handle(), "destination frame is not valid");
	}
#endif

	if(::copywin(src.get_handle(),  dst.get_handle(),
                 src_upper_left.y,  src_upper_left.x,
                 dst_upper_left.y,  dst_upper_left.x,
                 dst_lower_right.y, dst_lower_right.x, false)==ERR) {

		std::ostringstream oss;
		oss << "unable to copy window"
		    << "\nsource size " << src.get_dimension()
		    << "\ndestination size " << dst.get_dimension()
			<< "\nupper left source position " << src_upper_left
			<< "\nupper left destination position " << dst_upper_left
			<< "\ndimension to copy " << size
			<< "\ncalculated destination lower right " << dst_lower_right;
		throw CONS_MAKE_EXCEPTION(oss.str());
	}
}

} //namespace cons
