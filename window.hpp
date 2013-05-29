#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "irc/types.hpp"

#include <boost/signals.hpp>

#include <memory>
#include <vector>

class message;

class window {
protected:
	std::vector<std::shared_ptr<message>> messages;	

	boost::signal<void(window&, const std::string& topic)> on_topic_change;
	boost::signal<void(window&, const std::shared_ptr<message>&)> on_new_msg;
public:
	window()=default;
	virtual ~window(){}

	template<typename F>
	boost::signals::connection connect_on_new_message(F&& f);
	template<typename F>
	boost::signals::connection connect_on_topic_change(F&& f);
}; //window


template<typename F>
boost::signals::connection window::connect_on_new_message(F&& f) {
		return on_new_msg.connect(std::forward<F>(f));
}
template<typename F>
boost::signals::connection window::connect_on_topic_change(F&& f) {
		return on_topic_change.connect(std::forward<F>(f));
}



class channel_window : public window {
	irc::channel&                      chan;
	std::vector<irc::bsig::connection> connections;

	channel_window(channel_window&&)                =delete;
	channel_window(const channel_window&)           =delete;
	channel_window& operator=(channel_window&&)     =delete;
	channel_window& operator=(const channel_window&)=delete;
public:
	channel_window(irc::channel& chan);
	~channel_window();
}; //channel_window


class session_window : public window {
	irc::session&                      session;
	std::vector<irc::bsig::connection> connections;

	session_window(session_window&&)                =delete;
	session_window(const session_window&)           =delete;
	session_window& operator=(session_window&&)     =delete;
	session_window& operator=(const session_window&)=delete;
public:
	session_window(irc::session& session_);
	~session_window();
}; //session_window

#endif //WINDOW_HPP
