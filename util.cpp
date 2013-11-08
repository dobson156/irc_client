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

}
