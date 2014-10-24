
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CONS_MINIMAL_FRAME_WRAPPER_HPP
#define CONS_MINIMAL_FRAME_WRAPPER_HPP

#include "basic.hpp"

namespace cons {

/**
The aim of this class to aid decorators of frame by exposing and forwarding
the cons::base interface onto frame, whilst allowing for it's children 
to access to whole interface of frame via protected member functions

This is useful because it means that the decentants do not have to implement the 
boiler plate code for the majority of bases interface. 

An example of this use case is cons::progress_bar
*/
class minimal_frame_wrapper : public base {
private:
	frame frame_;
protected:

	/**
	 * This function allow the children of this class to access the full 
	 * interface of the frame
	 *
	 * @return a reference to the frame
	 */
	      frame& get_frame();
	/**
	 * This function allow the children of this class to access the full 
	 * interface of the frame
	 *
	 * @return a const reference to the frame
	 */
	const frame& get_frame() const;
public:
	/**
	 * Constructor
	 *
	 * @param takes the handle to the raw window
	 */
	minimal_frame_wrapper(unique_window_ptr handle_);
	virtual ~minimal_frame_wrapper()=default;

	virtual void clear() override;
	virtual void refresh() override;
	virtual void set_position(const point& position) override;
	virtual void set_dimension(const point& dimension) override;
	virtual point get_position() const override;
	virtual point get_dimension() const override;

	virtual unique_window_ptr reset(unique_window_ptr) override;

	virtual short get_background() const;
	virtual short get_foreground() const;

	virtual void set_background(short bg);
	virtual void set_foreground(short fg);

}; //class minimal_frame_wrapper

} //namespace cons

#endif //CONS_MINIMAL_FRAME_WRAPPER_HPP
