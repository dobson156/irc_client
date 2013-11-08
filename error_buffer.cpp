#include "buffer.hpp"
#include "message.hpp"

void error_buffer::push_back_error(const std::string& error_msg) {
	messages.push_back(std::make_shared<error_message>(error_msg));
}
