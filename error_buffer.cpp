#include "buffer.hpp"
#include "message.hpp"

log_buffer::log_buffer(std::string name) 
:	buffer(std::move(name)) 
{	}

void log_buffer::push_back_msg(std::string msg) {
	messages.push_back(std::make_shared<error_message>(std::move(msg)));
	on_new_msg(*this, messages.back());		
}
