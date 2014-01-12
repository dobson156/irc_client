
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "buffer.hpp"
#include "message.hpp"

log_buffer::log_buffer(std::string name) 
:	buffer(std::move(name)) 
{	}

void log_buffer::push_back_msg(std::string msg) {
	messages.push_back(std::make_shared<error_message>(std::move(msg)));
	on_new_msg(*this, messages.back());		
}
