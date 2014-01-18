
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CTRL_CHAR_HPP
#define CTRL_CHAR_HPP

namespace cons {

enum class ctrl_char {
	none, 
	arrow_left, arrow_right, 
	ctrl_arrow_left, ctrl_arrow_right,
	backspace,
	carriage_ret, newline
};

} //namespace cons

#endif //CTRL_CHAR_HPP
