#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "irc/session.hpp"
#include "irc/connection.hpp"

#include "console_ui.hpp"

#include <boost/asio/io_service.hpp>

#include <string>
#include <vector>
#include <functional>

class controller {
	void start_connection(const std::string& server_name);

	void parse_text(std::string::const_iterator first, 
	                std::string::const_iterator last);
	void parse_text(const std::string& text);
//view callbacks
	void handle_text_input(const std::string& str);
	void handle_special_char(int);
//parser handlers
	void handle_join   (const std::vector<std::string>& input);
	void handle_part   (const std::string& chan, const std::string msg);
	void handle_connect(const std::string& chan);
	void handle_nick   (const std::string& nick);
	void handle_msg    (const std::string& target, const std::string& msg);
	void handle_text   (const std::string& text);
	void handle_exec   (const std::string& exec);
	void handle_quit   ();
//connection handlers
	void handle_connection_connect(std::shared_ptr<irc::connection>);
//session handlers
	void handle_session_motd(const std::string& motd);
	void handle_session_join_channel(irc::channel& chan);
//channel handlers 
	void handle_channel_message(irc::channel& chan, 
	                            const irc::user& user, 
	                            const std::string& msg);

//varaibles
	boost::asio::io_service                       io_service;
	ui                                            view;

	irc::channel                                 *selected_channel { nullptr };

	std::vector<std::shared_ptr<irc::connection>> connections;
	std::vector<std::unique_ptr<irc::session>>    sessions;
public:
	controller();
	void run();
}; //class controller


#endif //CONTROLLER_HPP
