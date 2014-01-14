
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef UTIL_HPP
#define UTIL_HPP

#include <boost/optional.hpp>

#include <memory>
#include <chrono>
#include <string>
#include <limits>
#include <stdexcept>

//this is for my own sanity
namespace ph=std::placeholders;

//HELPERS
namespace util {

template<typename T, typename ...Args>
std::unique_ptr<T> make_unique( Args&& ...args ) {
	return std::unique_ptr<T>(new T(std::forward<Args>(args)... ));
}

template<typename Unsigned=unsigned>
Unsigned stou(const std::string& str, std::size_t *idx=nullptr, int base=10) {
	unsigned long result=std::stoul(str, idx, base);

	if (result > std::numeric_limits<Unsigned>::max())
		throw std::out_of_range("stou: ");

	return result;
}

boost::optional<std::string> try_get_user_name();

boost::optional<std::string> try_get_full_name();

std::string time_to_string(const std::chrono::system_clock::time_point& pt=
                                 std::chrono::system_clock::now());

std::chrono::system_clock::time_point get_next_min();

template<typename T>
class assign_on_call {
	T& val;
public:
	assign_on_call(T& val_)
	:	val ( val_ )
	{	}
	
	template<typename R>
	void operator()(R&& r) { val=std::forward<R>(r); }
}; //assign_on_call

template<typename T>
assign_on_call<T> make_assign_on_call(T& val) {
	return { val };
}


} //namespace util
	
#endif //UTIL_HPP
