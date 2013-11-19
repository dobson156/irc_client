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

boost::optional<std::string> get_user_name();

std::string time_to_string(const std::chrono::system_clock::time_point& pt=
                                 std::chrono::system_clock::now());

std::chrono::system_clock::time_point get_next_min();

} //namespace util
	
#endif //UTIL_HPP
