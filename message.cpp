#include "message.hpp"
#include "message_visitor.hpp"

message::message(message::time_point time_stamp_)
:	time_stamp { std::move(time_stamp) }
{	}

message::message() 
:	time_stamp { std::chrono::system_clock::now() }
{	}

message::~message() {

}

const message::time_point& message::get_time_stamp() const {
	return time_stamp;
}







chan_message::chan_message(std::string sender_, std::string content_) 
:	sender  { std::move(sender_)  }
,	content { std::move(content_) } 
{	}

void chan_message::visit(message_vistor& visitor) {
	visitor(*this);
}

const std::string& chan_message::get_sender() const {
	return sender;
}

const std::string& chan_message::get_content() const {
	return content;
}
