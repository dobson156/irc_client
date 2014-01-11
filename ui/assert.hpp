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
