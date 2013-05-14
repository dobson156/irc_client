#include "window.hpp"

std::size_t window::get_selected_idx() const {
	return selected_idx;
}

std::size_t window::size() const {
	return messages.size();
}

const std::string& window::msg_at(std::size_t i) const {
	return messages[i];
}

window::window() 
:	messages { "quick", "brown", "fox", "josh", "bin", "acrim" }
,	selected_idx { 1 }
{
}
