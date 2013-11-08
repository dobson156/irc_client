#include "buffer.hpp"
#include "message.hpp"

void error_buffer::push_back_error(const std::string& error_msg) {
	messages.push_back(std::make_shared<error_message>(error_msg));
<<<<<<< HEAD
}
=======
	on_new_msg(*this, messages.back());		
}

>>>>>>> e7a3394efe2ed4a4010cf13880ec21f0d9d02a86
