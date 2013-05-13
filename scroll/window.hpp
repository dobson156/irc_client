#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <vector>
#include <string>

class window {
	std::size_t              selected_idx;
	std::vector<std::string> messages;
public:
	std::size_t get_selected_idx() const;
	std::size_t size()	const;


}; //class window

#endif //WINDOW_HPP
