#include "window.hpp"

window::window(std::string name_) 
:	name { std::move(name_) }
{	}

window::~window(){ }

const std::string& window::get_name() const {
	return name;
}

const std::string& window::get_topic() const {
	return name;
}

window::const_iterator window::messages_begin() const {
	return messages.cbegin();
}
window::const_iterator window::messages_end() const {
	return messages.cend();
}

