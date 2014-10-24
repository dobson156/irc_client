
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef STENCILED_FRAME_HPP
#define STENCILED_FRAME_HPP

#include "basic.hpp"
#include "minimal_frame_wrapper.hpp"

#include <utility> //move

namespace cons {

template<typename StencilType>
class stenciled_frame : public minimal_frame_wrapper {
/**
 * This class is used to provide an easy way of providing formatting to data
 * 
 * unlike cons::frame which just deals in raw text buffers and characters,
 * this frame is specialised with a class, this template parameter defines a 
 * a value_type, the type of data which is stenciled.
 *
 * the data that is you wish to have displayed is set with set_content, note
 * that this will have version which it owns, you may move the value in 
 * or it will copy
 *
 * the stencil is the responsible translating the abstract data onto a 
 * raw output_pane (the interface to cons::frame)
 * the stencil also provides a layer of separation between the data and
 * the way in which it is displayed
 *
 * Most non interactive UI elements (ie outputs) are best modelled as stencils
 *
 * for an example of stencil see cons::string_stencil
 */
public:
	using stencil_type=StencilType;
	using value_type  =typename stencil_type::value_type;

	stenciled_frame(unique_window_ptr handle, value_type value_)
	:	minimal_frame_wrapper { std::move(handle) }
	,	value                 ( std::move(value_) )
	{	}

	/**
	 * This sets the value which is stenciled onto the UI via the stencil_type
	 * with which the stencil_frame is specialised
	 *
	 * This function takes the value by value, and the moves it internally.
	 */
	void set_content(value_type value_) {
		value=std::move(value_);
	}
//Overrides
	void clear() override { 
		value=value_type();
		get_frame().clear(); 
	}
	/**
	 * This function implements refresh, 
	 * in this step we delegate the formatting of the data to the 
	 * stencils write_to function
	 */
	void refresh() override { 
		get_frame().clear();
		stencil.write_to(get_frame(), value);
		get_frame().refresh(); 
	}
private:
	value_type   value;
	stencil_type stencil;
}; //class stenciled_frame

} //namespace cons

#endif // STENCILED_FRAME_HPP
