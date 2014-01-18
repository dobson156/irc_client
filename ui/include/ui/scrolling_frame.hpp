
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SCROLLING_FRAME_HPP
#define SCROLLING_FRAME_HPP

/*
 * The scolling_frame is a frame adaptor that facilities scroling
 * should the user write off the bottom of the frame then the frame
 * scrolls so that-that section becomes the bottom line,
 * any data at the top is deleted
 *
 * There is also an offset, the offset allows position translation
 * so that use can be made to write to specific areas.
 *
 * Together this means that if the offset is set to the height of 
 * the frame, the write to {0,0} will make it scroll, which will
 * effectively append the data to the end
 */

#include "basic.hpp"

namespace cons {

class scrolling_frame : public output_pane {
	int y_offset;
	frame& frame_;

	point translate_position(point pos) const;
	point scroll_if_necessary(point pos);
	point prepare_position(point pos); 
	point revert_position(point pos) const;
public:
	scrolling_frame(frame& frame, int y_offset_);

	point write(const point& pos, char c)                          override;
	point write(const point& pos, const std::string& str)          override;
	point write(const point& pt, std::string::const_iterator first, 
	                             std::string::const_iterator last) override;
	point write(const point& pos, const std::string& str, int n)   override;

	point get_position()  const override;
	point get_dimension() const override;

	bool is_underlined()  const override;
	bool is_blinking()    const override;
	bool is_dim()         const override;
	bool is_bold()        const override;

	void set_underlined(bool set=true);
	void set_blinking  (bool set=true);
	void set_dim       (bool set=true);
	void set_bold      (bool set=true);
	void set_colour    (const colour_pair& cp);

	frame& get_underlying_frame();
	const frame& get_underlying_frame() const;
}; //class scrolling_frame

} //namespace cons

#endif //SCROLLING_FRAME_HPP
