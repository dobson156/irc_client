
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BASIC_HPP 
#define BASIC_HPP

#include "colour_pair.hpp"

#include <ncurses.h>

#include <vector>
#include <string>
#include <memory> //unique_ptr
#include <ostream>
#include <utility> //pair

namespace cons {


struct point { 
	int x, y; 
	point()                       =default;
	point(const point&)           =default;
	point& operator=(const point&)=default;
};
std::ostream& operator<<(std::ostream& os, const point& p);
bool  operator==(const point& l, const point& r);
point operator+ (const point& l, const point& r);
point operator- (const point& l, const point& r);

using unique_window_ptr=std::unique_ptr<WINDOW, decltype(&::delwin)>;

class base {
public:
	virtual ~base() { }
	virtual void clear()                              =0;
	virtual void refresh()                            =0;
	virtual void set_position(const point& position)  =0;
	virtual void set_dimension(const point& dimension)=0;
	virtual point get_position() const                =0;
	virtual point get_dimension() const               =0;

	virtual unique_window_ptr reset(unique_window_ptr)=0;
}; //class base

using unique_base_ptr  =std::unique_ptr<base>;

//helper
int destroy_parent(WINDOW *win);
//Makes a new parent windows
unique_window_ptr make_window();
//Makes a sub window from an existing window
unique_window_ptr make_window(WINDOW *parent, const point& position, 
                                              const point& dimension);
//Makes a new pad
unique_window_ptr make_pad(const point& dimension);

class output_pane {
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

class frame : public base, public output_pane {
	unique_window_ptr handle;
	colour_pair       colours;

	point get_cursor() const; 
	void set_cursor(const point& p);

	void apply_colour();
	bool is_attr_on(int attr_on) const;
	void set_attribute(bool set, int attr);
public:
//Ctor
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

#endif // BASIC_HPP
