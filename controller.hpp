#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "console_ui.hpp"

#include <string>
#include <vector>
#include <functional>

class controller {
	void parse_text(std::string::const_iterator first, 
	                std::string::const_iterator last);
	void parse_text(const std::string& text);
//view callbacks
	void handle_text_input(const std::string& str);
	void handle_special_char(int);
//parser handlers
	void handle_join(const std::vector<std::string>& input);
	void handle_part(const std::string& chan, const std::string msg);
	void handle_connect(const std::string& chan);
	void handle_nick(const std::string& nick);
	void handle_msg(const std::string& target, const std::string& msg);
	void handle_text(const std::string& text);
	void handle_exec(const std::string& exec);
//varaibles
	ui view;
public:
	controller();
	void run();
}; //class controller


#endif //CONTROLLER_HPP
