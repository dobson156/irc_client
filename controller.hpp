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
	void handle_exec   (const std::string& exec);
	void handle_python (const std::string& py_code);
	void handle_quit   ();
	void handle_names  ();
//connection handlers
	void handle_connection_connect(std::shared_ptr<irc::connection>,
				  std::string nick, std::string username, std::string fullname);
//session handlers
	void handle_session_join_channel(irc::channel& chan);

//internals
	//TODO: more descriptive names
	void set_channel(buffer& win);
	void set_channels();
	
	      log_buffer& get_status_buffer();
	const log_buffer& get_status_buffer() const;
	
	log_buffer& get_or_make_error_buffer();

	const std::string get_default_nick() const;
	const std::string get_default_username() const;
	const std::string get_default_fullname() const;
	const std::string get_default_port() const;

//varaibles
	std::string                                   default_nick,
	                                              default_username,
	                                              default_fullname,
	                                              default_port     { "6667" };
	std::vector<std::unique_ptr<buffer>>          buffers;
	boost::asio::io_service                       io_service       { 1      };
	ui                                            view;
	std::vector<std::shared_ptr<irc::connection>> connections;
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
