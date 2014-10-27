
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CONS_BASIC_HPP 
#define CONS_BASIC_HPP

#include "base.hpp"
#include "colour_pair.hpp"
#include "output_pane.hpp"

#include <ncurses.h>

#include <vector>
#include <string>
#include <memory> //unique_ptr
#include <ostream>
#include <utility> //pair

namespace cons {


class frame : public base, public output_pane {
/**
 * frame is the most basic UI element and implementation of output_pane
 * 
 * it simply puts an iterface over the underlying curses windows,
 * this system deals entirely with string and characters and has no
 * default structure to the data
 *
 * Frame is highly useful for creating more strucutured UI elements,
 * by wrapping frame you can use it's inteface to output data with formatting
 * such as colour information and test attributes such bas bold and blinking
 * 
 * @see stenciled_frame for an examples
 */
	unique_window_ptr handle;
	colour_pair       colours;

	void apply_colour();
	bool is_attr_on(int attr_on) const;
	void set_attribute(bool set, int attr);
public:
//Ctor

	point get_cursor() const; 
	void set_cursor(const point& p);

	frame()            =delete;
	frame(const frame&)=delete;
	//This is safe as both colour and unique_ptr are move safe
	frame(frame&&)     =default;
	frame(unique_window_ptr handle_);
//Operator
	frame& operator=(frame&&)=default;
//Content
	point write(const point& pt, char ch)                          override;
	point write(const point& pt, const std::string& str)           override;
	point write(const point& pt, const std::string& str, int n)    override;
	point write(const point& pt, std::string::const_iterator first, 
	                             std::string::const_iterator last) override;
//Overrides
	void clear()                               override;
	void refresh()                             override;
	void set_position(const point& position)   override;
	void set_dimension(const point& dimension) override;
	point get_position()                 const override;
	point get_dimension()                const override;

	unique_window_ptr reset(unique_window_ptr handle_) override;
//Colour
	void set_background(short bg);
	void set_foreground(short fg);
	short get_background() const;
	short get_foreground() const;

//Atributes
	void set_underlined(bool set=true)         override;
	void set_blinking  (bool set=true)         override;
	void set_dim       (bool set=true)         override;
	void set_bold      (bool set=true)         override;
	void set_colour    (const colour_pair& cp) override;
	void set_colour    (const colour_pair& cp, bool set); //TODO override

	bool is_underlined() const override;
	bool is_blinking()   const override;
	bool is_dim()        const override;
	bool is_bold()       const override;

	WINDOW *get_handle();

	friend void copy(frame& src, frame& dst, 
	                 const point& src_upper_left,  
	                 const point& dst_upper_left, 
	                 const point& size);
}; //class frame

class attribute {
	int attribute_id;
public:
	attribute(int attribute_id_);
	int get_id() const;

	static attribute normal();
	//ncurses has a lower case MACRO called standout :/
	//static attribute standout();
	static attribute underline();
	static attribute reverse();
	static attribute blink();
	static attribute dim();
	static attribute bold();
}; //class atrribute

} //namespace cons

#endif // CONS_BASIC_HPP
