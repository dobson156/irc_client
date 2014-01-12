
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef ASSERT_CAST_HPP
#define ASSERT_CAST_HPP

#include "assert.hpp"

namespace cons {

template<typename T, typename F>
T assert_cast(F& v) {
	CONS_ASSERT(dynamic_cast<T*>(&v) != nullptr,
		"requested type does not meet type requirements");
	return static_cast<T>(v);
}

} //namespace cons

#endif //ASSERT_CAST_HPP
