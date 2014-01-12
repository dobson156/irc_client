
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef UI_ASSERT_HPP
#define UI_ASSERT_HPP


#ifdef NDEBUG 
	#define CONS_ASSERT(expr, msg) static_cast<void>(0);
#else
	#include <iostream> //for cerr
	#define CONS_ASSERT(expr, msg)\
		if(!(expr)) {\
			endwin();	\
			std::cerr << #expr << "   " \
			          << msg << __FILE__ << ':'<< __LINE__ << "__" \
					  << __func__ << std::endl;\
			std::abort();\
		}
#endif //NDEBUG

#endif //UI_ASSERT_HPP
