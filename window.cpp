#include "window.hpp"

window::window(std::string name_) 
:	name { std::move(name_) }
{	}

const std::string& window::get_name() const {
	return name;
}
