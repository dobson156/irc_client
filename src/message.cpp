
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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


motd_message::motd_message(std::string motd_) 
:	motd { std::move(motd_) }
{	}

void motd_message::visit(message_vistor& visitor) {
	visitor(*this);
}

const std::string& motd_message::get_motd() const {
	return motd;
}


void list_message::visit(message_vistor& visitor) {
	visitor(*this);
}

list_message::const_iterator list_message::begin() const {
	return store.begin();
}

list_message::const_iterator list_message::end() const {
	return store.end();
}

std::size_t list_message::max_element_size() const {
	auto it=std::max_element(store.begin(), store.end(), 
		[](const_reference a, const_reference b) {
			return a.first.size() < b.first.size();
		}
	);
	if(it==store.end()) return 0;
	return it->first.size();
}

const std::string& list_message::get_header() const {
	return header;
}



text_message::text_message(std::string header_, std::string body_)
:	header { std::move(header_) }
,	body   { std::move(body_)   }
{	}

text_message::text_message(std::string header_, 
                           cons::colour_pair header_colour_, 
                           std::string body_,   
                           cons::colour_pair body_colour_)
:	header        { std::move(header_)        }
,	body          { std::move(body_)          }
,	header_colour { std::move(header_colour_) }
,	body_colour   { std::move(body_colour_)   }
{	

}

const std::string& text_message::get_header() const { return header; }
const std::string& text_message::get_body() const   { return body;   }

const cons::colour_pair& text_message::get_header_colour() const { 
	return header_colour; 
}
const cons::colour_pair& text_message::get_body_colour() const { 
	return body_colour; 
}
void text_message::visit(message_vistor& visitor) {
	visitor(*this);
}


rich_message::rich_message(std::string header_, std::vector<rich_word> body_)
:	header { std::move(header_) }
,	body   { std::move(body_)   }
{	}

rich_message::rich_message(std::string header_, 
                           cons::colour_pair header_colour_, 
                           std::vector<rich_word> body_,   
                           cons::colour_pair body_colour_)
:	header        { std::move(header_)        }
,	body          { std::move(body_)          }
,	header_colour { std::move(header_colour_) }
,	body_colour   { std::move(body_colour_)   }
{	

}

const std::string& rich_message::get_header() const { return header; }
const std::vector<rich_word>& rich_message::get_body() const   { return body;   }

const cons::colour_pair& rich_message::get_header_colour() const { 
	return header_colour; 
}
const cons::colour_pair& rich_message::get_body_colour() const { 
	return body_colour; 
}
void rich_message::visit(message_vistor& visitor) {
	visitor(*this);
}

