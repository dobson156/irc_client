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
