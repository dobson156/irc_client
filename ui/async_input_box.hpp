#ifndef ASYNC_INPUT_BOX_HPP
#define ASYNC_INPUT_BOX_HPP

#include "basic.hpp"
#include "ctrl_char.hpp"
#include "input_manager.hpp"
#include "string_stencil.hpp"

#include <functional>
#include <string>

namespace cons {

class async_input_box : public base {
public:
	//TODO use boost::signals
	using grow_cb     =std::function<bool(point)>;
	using input_cb    =std::function<void(std::string)>;
	using ctrl_char_cb=std::function<void(ctrl_char)>;
private:
	grow_cb                  on_grow;
	input_cb                 on_input;
	ctrl_char_ch             on_ctrl_char;	

	
	frame                    frame_;
	std::string              value;
	unsigned                 pos = 0;
	string_stencil           stencil;
	input_manager            in_manager;
	boost::asio::io_service *io_service;
public:
	async_input_box(unique_window_ptr ptr, 
	                boost::asio::io_service& io_service);
	void stop();
	point calc_cursor_position() const;

	void handle_read_error(const boost::system::error_code&);
	arrow_key is_arrow_key(std::string::const_iterator it, 
	                       std::string::const_iterator last);
	void handle_read_complete(std::string str);
	void set();
	bool grow(point pt);
	void refresh();
	void clear();
	void set_value(const std::string& str);
	const std::string& get_value() const;
	//watch thread safety
	//what happens if a thread on this callback modifies this value?
	void reg_on_grow(grow_cb gcb);
	void reg_on_input(input_cb cb);
//Overrides
	void set_position(const point& position) override;
	void set_dimension(const point& dimension) override;
	point get_position()  const override;
	point get_dimension() const override;

	unique_window_ptr reset(unique_window_ptr handle);
//Colour
	void set_background(short bg);
	void set_foreground(short fg);
	short get_background() const;
	short get_foreground() const;
}; //class async_input_box

} //namespace cons

#endif //ASYNC_INPUT_BOX_HPP
