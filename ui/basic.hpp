#ifndef BASIC_HPP 
#define BASIC_HPP

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

class colour_pair {
	//As this lib is single thread async this is not a data race
	static std::vector<bool> used_slots;	
	static short get_free_id(); 
	static void release_id(short id); 

	short id { 0 };
	std::pair<short, short> get_pair() const;
	void init_pair_(short fg, short bg);
public:
//CTOR
	colour_pair();
	colour_pair(short foreground, short background);
	colour_pair(colour_pair&& other);
	colour_pair(const colour_pair& other);
	~colour_pair();
//OPS
	colour_pair& operator=(colour_pair pair);
	bool operator==(const colour_pair& other) const;
//Mutator
	void set_foreground(short foreground);
	void set_background(short background);
	void swap(colour_pair& other);
//Accessor
	short get_foreground() const;
	short get_background() const;
	short get_id() const;
}; //class colour_pair

void swap(colour_pair& a, colour_pair& b);
class base {
public:
	virtual ~base() { }
	virtual void clear()                              =0;
	virtual void refresh()                            =0;
	virtual void set_position(const point& position)  =0;
	virtual void set_dimension(const point& dimension)=0;
	virtual point get_position() const                =0;
	virtual point get_dimension() const               =0;
}; //class base

using unique_base_ptr  =std::unique_ptr<base>;
using unique_window_ptr=std::unique_ptr<WINDOW, decltype(&::delwin)>;

//helper
int destroy_parent(WINDOW *win);
//Makes a new parent windows
unique_window_ptr make_window();
//Makes a sub window from an existing window
unique_window_ptr make_window(WINDOW *parent, const point& position, 
                                              const point& dimension);
//Makes a new pad
unique_window_ptr make_pad(const point& dimension);

class frame : public base {
	unique_window_ptr handle;
	colour_pair       colours;

	point get_cursor() const; 
	void set_cursor(const point& p);
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
	point write(const point& pt, char ch);
	point write(const point& pt, const std::string& str);
	point write(const point& pt, const std::string& str, int n);
//Overrides
	void clear()                               override;
	void refresh()                             override;
	void set_position(const point& position)   override;
	void set_dimension(const point& dimension) override;
	point get_position()                 const override;
	point get_dimension()                const override;
//Colour
	void set_background(short bg);
	void set_foreground(short fg);
	short get_background() const;
	short get_foreground() const;

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
