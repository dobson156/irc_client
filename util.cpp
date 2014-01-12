
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "util.hpp"

#include <boost/algorithm/string.hpp>

#if defined(__unix__) || defined(__APPLE__)
	#include <pwd.h>
#endif

#include <cstdlib>
#include <string>
#include <vector>

namespace util {
	
boost::optional<std::string> try_get_user_name() {
	const char *usr=std::getenv("USER");
	if(usr!=nullptr)
		return std::string{ usr };
	return { };
}

boost::optional<std::string> try_get_full_name() {
	auto name=try_get_user_name();
	if(!name) return { };

	long bs=::sysconf(_SC_GETPW_R_SIZE_MAX);
	if(bs==-1) bs=4096;

	std::vector<char> buff ( bs );
	passwd pwd, *res;

	if(::getpwnam_r(name->c_str(), &pwd, buff.data(), buff.size(), &res)==0) {
		std::string s=pwd.pw_gecos;
		s.erase(std::find(begin(s), end(s), ','), end(s));
		if(s.empty()) return { };
		return s;
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
