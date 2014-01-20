
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "buffer.hpp"
#include "message.hpp"
#include "pallet.hpp"

log_buffer::log_buffer(std::string name) 
:	buffer(std::move(name)) 
{	}

void log_buffer::push_back_msg(std::string msg) {
	messages.push_back(std::make_shared<text_message>(
			"ERROR",
			get_pallet().get_colour_pair(pallet_idx::system_error),
			std::move(msg),
			get_pallet().get_colour_pair(pallet_idx::system_error)
		)
	);
	on_new_msg(*this, messages.back());		
}
