#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "unique_connection.hpp"

#include "irc/types.hpp"

#include <boost/signals.hpp>

#include <memory>
#include <vector>

class message;

class buffer {
	using container_type=std::vector<std::shared_ptr<message>>;
	using const_iterator=container_type::const_iterator;
protected:
	std::string    name;
	container_type messages;	

	boost::signal<void(buffer&, const std::string& topic)> on_topic_change;
	boost::signal<void(buffer&, const std::shared_ptr<message>&)> on_new_msg;
public:
	buffer(std::string str);
	virtual ~buffer();

	const std::string&         get_name()       const;
	virtual const std::string& get_topic()      const;
	const_iterator             messages_begin() const;
	const_iterator             messages_end()   const;

	template<typename F>
	boost::signals::connection connect_on_new_message(F&& f);
	template<typename F>
	boost::signals::connection connect_on_topic_change(F&& f);
}; //buffer


template<typename F>
boost::signals::connection buffer::connect_on_new_message(F&& f) {
		return on_new_msg.connect(std::forward<F>(f));
}
template<typename F>
boost::signals::connection buffer::connect_on_topic_change(F&& f) {
		return on_topic_change.connect(std::forward<F>(f));
}


class channel_buffer : public buffer {
	irc::channel&                      chan;
	std::vector<irc::bsig::connection> connections;

	channel_buffer(channel_buffer&&)                =delete;
	channel_buffer(const channel_buffer&)           =delete;
	channel_buffer& operator=(channel_buffer&&)     =delete;
	channel_buffer& operator=(const channel_buffer&)=delete;
public:
	channel_buffer(irc::channel& chan);
	~channel_buffer();
}; //channel_buffer


class session_buffer : public buffer {
	irc::session&                  session;
	std::vector<unique_connection> connections;

	session_buffer(session_buffer&&)                =delete;
	session_buffer(const session_buffer&)           =delete;
	session_buffer& operator=(session_buffer&&)     =delete;
	session_buffer& operator=(const session_buffer&)=delete;
public:
	session_buffer(irc::session& session_);
	~session_buffer();
}; //session_buffer

class error_buffer : public buffer {
	error_buffer(error_buffer&&)                =delete;
	error_buffer(const error_buffer&)           =delete;
	error_buffer& operator=(error_buffer&&)     =delete;
	error_buffer& operator=(const error_buffer&)=delete;

public:
	error_buffer() : buffer("error") {};
	~error_buffer() {}
	void push_back_error(const std::string& error_msg);
}; //error_buffer


#endif //BUFFER_HPP
