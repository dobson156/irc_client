#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "types.hpp"

#include <set>
#include <string>

namespace irc {

class channel {
	std::string           name;
	std::string           topic;
	std::set<shared_user> users;
//signals
	sig_ch_usr_s  on_message;
	sig_ch_usr    on_user_join;
	sig_ch_usr_os on_user_part;
	sig_ch_s      on_topic_change;
//helpers
	void add_user(const shared_user& user);
//deleted functions
	channel(const channel&)           =delete;
	channel(channel&&)                =delete;
	channel& operator=(const channel&)=delete;
	channel& operator=(channel&&)     =delete;
public:
	channel(std::string name_);

	//USER INTERFACE
	const std::string& get_name()  const;
	const std::string& get_topic() const;

	//SYSTEM INTERFACE
	void message(const shared_user& user, const std::string message);
	void user_part(const shared_user& user, const optional_string& msg);
	void user_join(const shared_user& user);
	void user_quit(const shared_user& user, const std::string& msg);
	void set_topic(std::string str); 
}; //class irc

} //namespace irc

#endif //CHANNEL_HPP
