#include "window.hpp"

std::size_t window::get_selected_idx() const {
	return selected_idx;
}

std::size_t window::size() const {
	return messages.size();
}
