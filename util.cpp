#include "util.hpp"

#include <boost/algorithm/string.hpp>

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

namespace util {
	
boost::optional<std::string> try_get_user_name() {
	const char *usr=std::getenv("USER");
	if(usr!=nullptr)
		//return { usr };
		return std::string{ usr };
	return { };
}



boost::optional<std::string> try_get_full_name() {
	//TODO: use regex
	auto name=try_get_user_name();
	if(!name) return { };
	std::ifstream file { "/etc/passwd" };
	std::string line, fn;
	while(std::getline(file, line)) {
		if(boost::starts_with(line, *name)) {
			std::istringstream iss { line };
			for(unsigned i=0; i!=4; ++i)
				std::getline(iss, fn, ':');
			if(std::getline(iss, fn, ','))
				return fn;
			return { };
		}
	}
	return { };
}

std::string time_to_string(const std::chrono::system_clock::time_point& pt) {
	std::time_t t=std::chrono::system_clock::to_time_t(pt);
	std::string ts(6, '\0');
	auto n=std::strftime(&ts[0], ts.size(), "%R", std::localtime(&t));
	ts.resize(n);
	return ts; 
}

std::chrono::system_clock::time_point get_next_min() {
	return std::chrono::time_point_cast<std::chrono::minutes>(
		std::chrono::system_clock::now() + std::chrono::minutes(1));
}

}
