
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "irc/channel.hpp"
#include "irc/user.hpp"
#include "irc/prefix.hpp"

#include "ui/colour_pair.hpp"

#include <chrono>
#include <string>

class message_vistor;

class message {
	using time_point=std::chrono::system_clock::time_point;
	time_point time_stamp;
public:
	message(time_point time_stamp_);
	message(); //time_stamp == now()

	message(message&&)                =default;
	message(const message&)           =default;
	message& operator=(message&&)     =default;
	message& operator=(const message&)=default;
	virtual ~message();

	const time_point& get_time_stamp() const;

	virtual void visit(message_vistor&)=0;
}; //class message


class motd_message : public message {
	std::string motd;
public:
	motd_message(std::string motd_);
	motd_message(motd_message&&)                =default;
	motd_message(const motd_message&)           =default;
	motd_message& operator=(motd_message&&)     =default;
	motd_message& operator=(const motd_message&)=default;

	void visit(message_vistor& visitor) override;

	const std::string& get_motd() const;
}; //motd_message


//[time] [header] | <e1> <e2> <e3>  |
//                | <e3> <e4>...    |
class list_message : public message {
public:
	//text, colour
	using value_type     =std::pair<std::string, short>;
	using const_reference=const value_type&;
	using container_type =std::vector<value_type>;
	using const_iterator =container_type::const_iterator;
	
	template<typename Iter>
	list_message(std::string header_, Iter first, Iter last) 
	:	header { header_   }
	,	store (first, last)
	{   }

	void visit(message_vistor& visitor) override;
	std::size_t max_element_size() const;

	const std::string& get_header() const;

	const_iterator begin() const;
	const_iterator end() const;
private:
	std::string       header;
	container_type    store;
	cons::colour_pair header_colour { -1, -1 };
}; //list_message


//[time] [header] | body test goes  |
//                | here            |
class text_message : public message {
	std::string       header, 
	                  body;

	cons::colour_pair header_colour { -1, -1 },  //using default colour 
	                  body_colour   { -1, -1 };
public:
	text_message(std::string header_, std::string body_);
	text_message(std::string header_, cons::colour_pair header_colour_, 
	             std::string body_,   cons::colour_pair body_colour_);

	text_message(text_message&&)                =default;
	text_message(const text_message&)           =default;
	text_message& operator=(text_message&&)     =default;
	text_message& operator=(const text_message&)=default;

	const std::string& get_header() const;
	const std::string& get_body() const;

	const cons::colour_pair& get_header_colour() const;
	const cons::colour_pair& get_body_colour() const;

	void visit(message_vistor& visitor) override;
}; //text_message

#endif //MESSAGE_HPP
