#include "util.hpp"

#include <cstdlib>

namespace util {
	
boost::optional<std::string> get_user_name() {
	const char *usr=std::getenv("USER");
	if(usr!=nullptr)
		//return { usr };
		return std::string{ usr };
	return { };
}

std::string time_to_string(const std::chrono::system_clock::time_point& pt) {
	std::time_t t=std::chrono::system_clock::to_time_t(pt);
	std::string ts(6, '\0');
	std::strftime(&ts[0], ts.size(), "%R", std::localtime(&t));
	ts.pop_back();
	return ts; 
}

std::chrono::system_clock::time_point get_next_min() {
	return std::chrono::time_point_cast<std::chrono::minutes>(
		std::chrono::system_clock::now() + std::chrono::minutes(1));
}

}
