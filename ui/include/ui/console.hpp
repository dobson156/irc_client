
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CONSOLE_HPP
#define CONSOLE_HPP

//error reporting
#include "assert.hpp"
#include "exception.hpp"

//Frame and primitive functions
#include "basic.hpp"

//view
#include "anchor_view.hpp"

//default stencils
#include "string_stencil.hpp"
//stencil containers
#include "stenciled_list.hpp"
#include "stenciled_frame.hpp"

//input
//#include "input_box.hpp"
#include "async_input_box.hpp"
#include "input_box.hpp"

//decorartors
#include "bordered.hpp"

#ifdef CONS_FAST_COMPILE
	extern template class cons::stenciled_frame<cons::string_stencil>;
	extern template class cons::stenciled_list<cons::string_stencil>;
	extern template class cons::anchor_view<cons::anchors::top>;
	extern template class cons::anchor_view<cons::anchors::bottom>;
	extern template class cons::anchor_view<cons::anchors::left>;
	extern template class cons::anchor_view<cons::anchors::right>;
#endif 

#endif // CONSOLE_HPP
