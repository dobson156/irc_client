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

//CLASS colour_pair
//STATIC
std::vector<bool> colour_pair::used_slots;

short colour_pair::get_free_id() {
	auto it=std::find(used_slots.begin(), used_slots.end(), false);
	if(it==used_slots.end()) {
		used_slots.push_back(true);
		return used_slots.size();
	}
	*it=true;
	return std::distance(used_slots.begin(), it)+1;
}
void colour_pair::release_id(short id) {
	--id;
	CONS_ASSERT(std::size_t(id) < used_slots.size(), "invalid id");
	CONS_ASSERT(used_slots[id], "id already released");
	used_slots[id]=false;
}
//END STATIC

std::pair<short, short> colour_pair::get_pair() const {
	CONS_ASSERT(id!=0, "invalid colour id");
	short f, b;
	pair_content(id, &f, &b);
	return { f, b };
}
void colour_pair::init_pair_(short fg, short bg) {
	CONS_ASSERT(id!=0, "invalid colour id");
	if(init_pair(id, fg, bg)==ERR) {
		std::ostringstream oss;
		oss << "Unable to set colour pair id " << id 
		    << " with fg: " << fg << " bg: " << bg;
		throw CONS_MAKE_EXCEPTION(oss.str());
	}
}

colour_pair::colour_pair()
:	id { get_free_id() }
{
	//see man use_default_colors for details
 	use_default_colors();
	init_pair_(-1, -1);
}
colour_pair::colour_pair(short foreground, short background)
:	id { get_free_id() }
{	
	init_pair_(foreground, background);
}
colour_pair::colour_pair(colour_pair&& other) 
:	id { other.id }
{
	other.id=0; 
}
colour_pair::colour_pair(const colour_pair& other) 
:	id { get_free_id() }
{
	const auto& p=other.get_pair();
	init_pair_(std::get<0>(p), std::get<1>(p));
}
colour_pair::~colour_pair() {
	if(id!=0) {
		attroff(COLOR_PAIR(id));
		release_id(id);
	}
}
colour_pair& colour_pair::operator=(colour_pair other) {
	swap(other);
	return *this;
}
void colour_pair::set_foreground(short foreground) {
	CONS_ASSERT(id!=0, "invalid colour id");
	init_pair_(foreground, get_background());
}
void colour_pair::set_background(short background) {
	CONS_ASSERT(id!=0, "invalid colour id");
	init_pair_(get_foreground(), background);
}
short colour_pair::get_foreground() const {
	return std::get<0>(get_pair());
}
short colour_pair::get_background() const {
	return std::get<1>(get_pair());
}
short colour_pair::get_id() const {
	return COLOR_PAIR(id);
}
bool colour_pair::operator==(const colour_pair& other) const {
	return other.get_pair() == get_pair();
}
void colour_pair::swap(colour_pair& other) {
	CONS_ASSERT(id!=0 && other.id!=0, "invalid colour id");
	std::swap(id, other.id);
}
void swap(colour_pair& a, colour_pair& b) {
	a.swap(b);	
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
	if(!handle)
		throw CONS_MAKE_EXCEPTION("unable to create pad");
	
	return handle;
	
}

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
{
}

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
		throw CONS_MAKE_EXCEPTION("Unable to write char");
	}
	return get_cursor();
}
point frame::write(const point& pt, const std::string& str) {
	CONS_ASSERT(handle, "inavlid handle");
	set_cursor(pt); //throws
	if(waddstr(handle.get(), str.c_str())==ERR) {
		throw CONS_MAKE_EXCEPTION("Unable to write string to frame");
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
		throw CONS_MAKE_EXCEPTION("Unable to write string to frame");
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
//	if(wcolor_set(handle.get(), COLOR_PAIR(colours.get_id()), nullptr)==ERR) {
//		throw CONS_MAKE_EXCEPTION("Unable to set background of frame");
//	}
}
void frame::set_foreground(short fg) {
	CONS_ASSERT(handle, "invalid handle");
	colours.set_foreground(fg);	
	//if(wattron(handle.get(), COLOR_PAIR(colours.get_id()))==ERR) {
	apply_colour();
	//if(wcolor_set(handle.get(), COLOR_PAIR(colours.get_id()), nullptr)==ERR) {
	//	throw CONS_MAKE_EXCEPTION("Unable to set background of frame");
	//}
}
short frame::get_background() const {
	return colours.get_background();
}
short frame::get_foreground() const {
	return colours.get_foreground();
}

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
                 dst_lower_right.y, dst_lower_right.x, true)==ERR) {

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


point  bordered::calc_element_dimension() const {
	auto dim=get_dimension() -  point{ right+left, top+bottom };
	return {
		std::max(dim.x, 0),
		std::max(dim.y, 0)
	};
}

point bordered::calc_element_position() const {
	return { int(left), int(top) };
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

namespace anchors {
top::top(point dimension_, int n_)
:	dimension ( dimension_ )
,	n         { n_         }
{	}
point top::get_fill_position() const {
	auto used=get_anchor_dimension();
	return { 0, used.y };
}
point top::get_fill_dimension() const {
	auto used=get_anchor_dimension();
	return { dimension.x, dimension.y - used.y };
}
point top::get_anchor_position() const {
	return { 0, 0 };
}
point top::get_anchor_dimension() const {
	return { dimension.x, std::min(n, dimension.y) };
}
void top::set_dimension(point dimension_) { dimension=dimension_; }
void top::set_n(int n_) { n=n_; }
int top::get_n() const { return n; }

left::left(point dimension_, int n_)
:	dimension ( dimension_ )
,	n         { n_         }
{	}
point left::get_fill_position() const {
	auto used=get_anchor_dimension();
	return { used.x, 0 };
}
point left::get_fill_dimension() const {
	auto used=get_anchor_dimension();
	return { dimension.x - used.x, dimension.y };
}
point left::get_anchor_position() const {
	return { 0, 0 };	
}
point left::get_anchor_dimension() const {
	return { std::min(n, dimension.x), dimension.y };
}
void left::set_dimension(point dimension_) { dimension=dimension_; }
void left::set_n(int n_) { n=n_; }
int left::get_n() const { return n; }

bottom::bottom(point dimension_, int n_)
:	dimension ( dimension_ )
,	n         { n_         }
{	}
point bottom::get_fill_position() const {
	return { 0, 0 };	
}
point bottom::get_fill_dimension() const {
	auto used=get_anchor_dimension();
	return { dimension.x, dimension.y - used.y };
}
point bottom::get_anchor_position() const {
	return { 0, std::max(dimension.y-n, 0)  };
}
point bottom::get_anchor_dimension() const {
	return { dimension.x, std::min(n, dimension.y) };
}
void bottom::set_dimension(point dimension_) { dimension=dimension_; }
void bottom::set_n(int n_) { n=n_; }
int bottom::get_n() const { return n; }

right::right(point dimension_, int n_)
:	dimension ( dimension_ )
,	n         { n_         }
{	}
point right::get_fill_position() const {
	return { 0, 0 };	
}
point right::get_fill_dimension() const {
	auto used=get_anchor_dimension();
	return { dimension.x - used.x, dimension.y };
}
point right::get_anchor_position() const {
	return { std::max(dimension.x-n, 0), 0 };
}
point right::get_anchor_dimension() const {
	return { std::min(n, dimension.x), dimension.y };
}
void right::set_dimension(point dimension_) { dimension=dimension_; }
void right::set_n(int n_) { n=n_; }
int right::get_n() const { return n; }

} //namespace anchors

//ATTRIBUTE
attribute::attribute(int attribute_id_)
:	attribute_id { attribute_id_ }
{	}

/*
	CONS_ASSERT(handle!=nullptr);
	if(wattron(handle, attribute_id)==ERR) {
		throw CONS_MAKE_EXCEPTION("Unable to apply attribute to frame");
	}
}
*/

int attribute::get_id() const    { return attribute_id;     }
//STATIC MEMBER FUNCTIONS
attribute attribute::normal()    { return { WA_NORMAL };    }
attribute attribute::underline() { return { WA_UNDERLINE }; }
attribute attribute::reverse()   { return { WA_REVERSE };   }
attribute attribute::blink()     { return { WA_BLINK };     }
attribute attribute::dim()       { return { WA_DIM };       } 
attribute attribute::bold()      { return { WA_BOLD };      }

} //namespace cons
