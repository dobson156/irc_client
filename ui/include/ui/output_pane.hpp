#ifndef CONS_OUTPUT_PANE_HPP
#define CONS_OUTPUT_PANE_HPP

#include "point.hpp"
#include "colour_pair.hpp"

#include <string>

namespace cons {

class output_pane {
/**
 * Output pane, is the inteface for a UI element that be be written onto
 * like a canvas, for example like a frame.
 *
 * The reason why we need this generic interface rather than just use
 * frame itself is because we can use the interface to narrow down on
 * subsections of a frame but still providing the same frame interface
 *
 * @see frame and scrolling_frame
 */
public:
	virtual point get_position()  const                                    =0;
	virtual point get_dimension() const                                    =0;

	virtual point write(const point& pt, char ch)                          =0;
	virtual point write(const point& pt, const std::string& str)           =0;
	virtual point write(const point& pt, const std::string& str, int n )   =0;
	virtual point write(const point& pt, std::string::const_iterator first, 
	                                     std::string::const_iterator last) =0;
	virtual bool is_underlined() const                                     =0;
	virtual bool is_blinking()   const                                     =0;
	virtual bool is_dim()        const                                     =0;
	virtual bool is_bold()       const                                     =0;
	virtual void set_underlined(bool set=true)                             =0;
	virtual void set_blinking  (bool set=true)                             =0;
	virtual void set_dim       (bool set=true)                             =0;
	virtual void set_bold      (bool set=true)                             =0;
	virtual void set_colour    (const colour_pair& cp)                     =0;
}; //class output_pane

} //namespace cons

#endif //CONS_OUTPUT_PANE_HPP
