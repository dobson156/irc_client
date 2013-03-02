#include "controller.hpp"
#include "util.hpp"

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
}
void controller::handle_exec(const std::string& exec) {
}

controller::controller() {
	view.reg_on_text_input(
		std::bind(&controller::handle_text_input, this, ph::_1));
	view.reg_on_special_char(
		std::bind(&controller::handle_special_char, this, ph::_1));
}

void controller::run() {
	view.run();
}
