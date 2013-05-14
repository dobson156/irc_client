#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <vector>
#include <string>

class window {
	std::vector<std::string> messages;
	std::size_t              selected_idx;
public:
	std::size_t get_selected_idx() const;
	std::size_t size()	const;
	const std::string& msg_at(std::size_t i) const;

	window();
}; //class window

#endif //WINDOW_HPP
