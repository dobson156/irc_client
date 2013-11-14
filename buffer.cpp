#include "buffer.hpp"

buffer::buffer(std::string name_) 
:	name { std::move(name_) }
{	}

buffer::~buffer(){ }

const std::string& buffer::get_name() const {
	return name;
}

const std::string& buffer::get_topic() const {
	return name;
}

buffer::const_iterator buffer::messages_begin() const {
	return messages.cbegin();
}
buffer::const_iterator buffer::messages_end() const {
	return messages.cend();
}

bool buffer::has_session() const { return false; }
bool buffer::has_channel() const { return false; }
