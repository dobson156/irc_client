
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "scrolling_frame.hpp"
#include "assert.hpp"
#include "exception.hpp"

#include <limits>

namespace cons {

point scrolling_frame::translate_position(point pos) const {
	pos.y+=y_offset;

	auto frame_dim=frame_.get_dimension();
	CONS_ASSERT(pos.x < frame_dim.x, 
		"the fales position is outside of the frame dimensions");

	return pos;
}
point scrolling_frame::scroll_if_necessary(point pos) {
	int y=pos.y;
	auto y_midx=frame_.get_dimension().y - 1; //max index

	if(y > y_midx) {
		int diff=y-y_midx;
		if(wscrl(frame_.get_handle(), diff) == ERR) {
			throw CONS_MAKE_EXCEPTION("Could not scroll frame");
		}
		y_offset-=diff; //the offset should shink as the buffer scrolls
		pos.y=y_midx;
	}
	return pos;
}
point scrolling_frame::prepare_position(point pos) {
	auto act_pos=translate_position(std::move(pos));
	act_pos=scroll_if_necessary(act_pos);
	return act_pos;
}
point scrolling_frame::revert_position(point pos) const {
	return { pos.x, pos.y-y_offset };
}

scrolling_frame::scrolling_frame(frame& frame, int y_offset_)
:	y_offset { y_offset_ }
,	frame_   ( frame     )
{	
	scrollok(frame.get_handle(), true);
}

point scrolling_frame::write(const point& pos, char c) {
	return revert_position(frame_.write(prepare_position(pos), c));
}
point scrolling_frame::write(const point& pos, const std::string& str) {
	return revert_position(frame_.write(prepare_position(pos), str));
}
point scrolling_frame::write(const point& pt, std::string::const_iterator first,
                                              std::string::const_iterator last) {
	return revert_position(frame_.write(prepare_position(pt), first, last));
}
point scrolling_frame::write(const point& pos, const std::string& str, int n) {
	return revert_position(frame_.write(prepare_position(pos), str, n));
}

point scrolling_frame::get_position() const {
	return frame_.get_position();
}
point scrolling_frame::get_dimension() const {
	return { frame_.get_dimension().x, std::numeric_limits<int>::max() };
}

bool scrolling_frame::is_underlined() const { 
	return frame_.is_underlined(); 
}
bool scrolling_frame::is_blinking() const { 
	return frame_.is_blinking(); 
}
bool scrolling_frame::is_dim() const { 
	return frame_.is_dim(); 
}
bool scrolling_frame::is_bold() const { 
	return frame_.is_bold(); 
}

void scrolling_frame::set_underlined(bool set) { 
	frame_.set_underlined(set); 
}
void scrolling_frame::set_blinking(bool set) { 
	frame_.set_blinking(set); 
}
void scrolling_frame::set_dim(bool set) { 
	frame_.set_dim(set); 
}
void scrolling_frame::set_bold(bool set) { 
	frame_.set_bold(set); 
}
void scrolling_frame::set_colour(const colour_pair& cp) { 
	frame_.set_colour(cp); 
}

frame& scrolling_frame::get_underlying_frame() { return frame_; }
const frame& scrolling_frame::get_underlying_frame() const { return frame_; }

} //namespace cons
