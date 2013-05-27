#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "irc/prefix.hpp"

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


class chan_message : public message {
	std::string sender;
	std::string content;
public:
	chan_message(std::string sender_, std::string content_);
	chan_message(chan_message&&)                =default;
	chan_message(const chan_message&)           =default;
	chan_message& operator=(chan_message&&)     =default;
	chan_message& operator=(const chan_message&)=default;

	void visit(message_vistor& visitor) override;

	const std::string& get_sender() const;
	const std::string& get_content() const;
}; //chan_message


class join_message : public message {
	irc::prefix prefix;
public:
	join_message(irc::prefix prefix_);
	join_message(join_message&&)                =default;
	join_message(const join_message&)           =default;
	join_message& operator=(join_message&&)     =default;
	join_message& operator=(const join_message&)=default;

	void visit(message_vistor& visitor) override;

	const irc::prefix& get_prefix() const;
}; //chan_message

#endif //MESSAGE_HPP
