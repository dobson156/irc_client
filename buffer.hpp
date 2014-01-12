
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "unique_connection.hpp"

#include "irc/types.hpp"

#include <memory>
#include <vector>

class message;


class session_bound {
	irc::session& sess;
public:
	session_bound(irc::session& sess_)
	:	sess ( sess_ )
	{	}
	irc::session&       get_session()       { return sess; }
	const irc::session& get_session() const { return sess; }
};

class channel_bound {
	irc::channel& chan;
public:
	channel_bound(irc::channel& chan_)
	:	chan ( chan_ )
	{	}
	irc::channel&       get_channel()       { return chan; }
	const irc::channel& get_channel() const { return chan; }
};

class has_session {
	irc::session* sess { nullptr };
public:
	template<typename T>
	has_session(T& t) {
		auto tp=dynamic_cast<session_bound*>(&t);
		if(tp) sess=&tp->get_session();
	}
	operator bool()                   const { return sess;  }
	irc::session       &get_session()       { return *sess; }
	const irc::session &get_session() const { return *sess; }
};

class has_channel {
	irc::channel* chan { nullptr };
public:
	template<typename T>
	has_channel(T& t) {
		auto tp=dynamic_cast<channel_bound*>(&t);
		if(tp) chan=&tp->get_channel();
	}
	operator bool()                   const { return chan;  }
	irc::channel       &get_channel()       { return *chan; }
	const irc::channel &get_channel() const { return *chan; }
};


class buffer {
	using container_type=std::vector<std::shared_ptr<message>>;
	using const_iterator=container_type::const_iterator;
protected:
	std::string    name;
	container_type messages;	

	bsig::signal<void(buffer&, const std::string& topic)> on_topic_change;
	bsig::signal<void(buffer&, const std::shared_ptr<message>&)> on_new_msg;
public:
	buffer(std::string str);
	virtual ~buffer();

	bool                       has_session() const;
	bool                       has_channel() const;

	const std::string&         get_name()       const;
	virtual const std::string& get_topic()      const;
	const_iterator             messages_begin() const;
	const_iterator             messages_end()   const;

	template<typename F>
	bsig::connection connect_on_new_message(F&& f);
	template<typename F>
	bsig::connection connect_on_topic_change(F&& f);
}; //buffer


template<typename F>
bsig::connection buffer::connect_on_new_message(F&& f) {
		return on_new_msg.connect(std::forward<F>(f));
}
template<typename F>
bsig::connection buffer::connect_on_topic_change(F&& f) {
		return on_topic_change.connect(std::forward<F>(f));
}


class channel_buffer : public buffer, public channel_bound, public session_bound {
	irc::channel&                      chan;
	std::vector<irc::bsig::connection> connections;

	channel_buffer(channel_buffer&&)                =delete;
	channel_buffer(const channel_buffer&)           =delete;
	channel_buffer& operator=(channel_buffer&&)     =delete;
	channel_buffer& operator=(const channel_buffer&)=delete;
public:
	channel_buffer(irc::channel& chan);
	~channel_buffer();
	const std::string& get_topic() const override;
	void list_names();
}; //channel_buffer


class session_buffer : public buffer, public session_bound {
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

class log_buffer : public buffer {
	log_buffer(log_buffer&&)                =delete;
	log_buffer(const log_buffer&)           =delete;
	log_buffer& operator=(log_buffer&&)     =delete;
	log_buffer& operator=(const log_buffer&)=delete;

public:
	log_buffer(std::string name);
	~log_buffer() {}
	void push_back_msg(std::string msg);
}; //log_buffer

#endif //BUFFER_HPP
