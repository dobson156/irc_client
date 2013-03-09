#include "controller.hpp"
#include "util.hpp"

#include <exception>
#include <iostream>

//see controller_parse_text.hpp for impl
void controller::parse_text(const std::string& text) {
	parse_text(text.cbegin(), text.cend());
}

void controller::handle_text_input(const std::string& str) {
	parse_text(str);
}
void controller::handle_special_char(int) {
}

void controller::handle_join(const std::vector<std::string>& input) {
	view.set_channels(input.cbegin(), input.cend());
}
void controller::handle_part(const std::string& chan, const std::string msg) {
}
void controller::handle_connect(const std::string& chan) {
}
void controller::handle_nick(const std::string& nick) {
}
void controller::handle_msg(const std::string& target, const std::string& msg) {
}
void controller::handle_text(const std::string& text) {
	view.append_message(text);
}
void controller::handle_exec(const std::string& exec) {
}
void controller::handle_quit() {
	view.stop();
}

controller::controller() 
:	view { io_service }
{
	view.reg_on_text_input(
		std::bind(&controller::handle_text_input, this, ph::_1));
	view.reg_on_special_char(
		std::bind(&controller::handle_special_char, this, ph::_1));
}

void controller::run() {
	for(;;) {
		try {
			io_service.run();
			break; //io service has run to completion
		}
		catch(const std::exception& e) {
			std::cerr << "Exception" << e.what() << std::endl;
		}
	}
}
