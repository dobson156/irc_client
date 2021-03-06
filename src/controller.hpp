
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "buffer.hpp"
#include "console_ui.hpp"
#ifdef USING_PYTHON
	#include "python_interface.hpp"
#endif //USING_PYTHON

#include <boost/asio/io_service.hpp>

#include <string>
#include <vector>
#include <memory>

namespace irc { class connection; class session; }

class controller {
	void start_connection(const std::string& hostname);
	void start_connection(const std::string& hostname, 
                          const std::string& nickname, 
                          const std::string& username,
                          const std::string& fullname,
                          const std::string& port);

	void parse_text(std::string::const_iterator first, 
	                std::string::const_iterator last);
	void parse_text(const std::string& text);
//view callbacks
	void handle_text_input(const std::string& str);
	void handle_ctrl_char(cons::ctrl_char);
//parser handlers
	void handle_join   (const std::vector<std::string>& input);
	void handle_part   (const std::string& chan, const std::string& msg);
	void handle_connect(const std::string& chan);
	void handle_nick   (const std::string& nick);
	void handle_msg    (const std::string& target, const std::string& msg);
	void handle_text   (const std::string& text);
	void handle_invite (const std::string& nick);
	void handle_exec   (const std::string& exec);
	void handle_python (const std::string& py_code);
	void handle_quit   ();
	void handle_names  ();
	
	void handle_leave  (const std::string& exec);
//connection handlers
	void handle_connection_connect(irc::session& sess);
//session handlers
	void handle_session_join_channel(irc::channel& chan);

//internals
	//TODO: more descriptive names
	void set_channel(buffer& win);
	void set_channels();
	
	      log_buffer& get_status_buffer();
	const log_buffer& get_status_buffer() const;
	
	log_buffer& get_or_make_error_buffer();
	/**
	 * Gets the nick that is used when establishing new sessions 
	 * @returns the default nick
	 */
	const std::string get_default_nick() const;
	/**
	 * Sets the nick used when establishing new session 
	 * Note that setting this value will not alter current sessions 
	 * @param nick the new value
	 */
	void  set_default_nick(const std::string& nick);
	/**
	 * Gets the username that is used when establishing new session 
	 * @returns the default usersname 
	 */
	const std::string get_default_username() const;
	/**
	 * Sets the user used when establishing new session 
	 * Note that setting this value will not alter current sessions 
	 * @param username the new value
	 */	
	 void  set_default_username(const std::string& username);
	/**
	 * Gets the fullname that is used when establishing new sessions 
	 * @returns the default fullname
	 */
	const std::string get_default_fullname() const;
	/**
	 * Sets the nick used when establishing new session 
	 * Note that setting this value will not alter current sessions 
	 * @param fullname the new value
	 */
	void  set_default_fullname(const std::string& fullname);
	/**
	 * Gets the port or service used when establishing new connection
	 * @returns the default port
	 */
	const std::string get_default_port() const;
	/**
	 * Sets the port used when establishing new connection
	 * Note that setting this value will not alter current connection
	 * @param port the new value
	 */
	void  set_default_port(const std::string& str);
//varaibles
	std::string                                   default_nick,
	                                              default_username,
	                                              default_fullname,
	                                              default_port     { "6667" };
	std::vector<std::unique_ptr<buffer>>          buffers;
	boost::asio::io_service                       io_service       { 1      };
	ui                                            view;
	std::vector<std::unique_ptr<irc::session>>    sessions;
	bool                                          show_errors      { true  };

#ifdef USING_PYTHON
	python_interface python_controller;
#endif //USING_PYTHON

public:
	controller(std::string pyton_config_file);
	~controller();
	controller(const controller&)=delete;
	void run();
}; //class controller


#endif //CONTROLLER_HPP
