#include "util.hpp"

#include <boost/optional.hpp>
#include <cstdlib>
#include <string>

namespace util {

boost::optional<std::string> get_user_name() {
	const char *name=std::getenv("$USER");
	return boost::make_optional(std::string{name});
}

}
