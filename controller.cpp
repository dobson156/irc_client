#include "controller.hpp"
#include "util.hpp"

#include "irc/session.hpp"
#include "irc/channel.hpp"
#include "irc/user.hpp"

#include <exception>
#include <iostream>



void controller::handle_connection_connect(
                  std::shared_ptr<irc::connection> connection) {
	view.append_message("connected");

	sessions.push_back(
		util::make_unique<irc::session>(
			std::move(connection), 
			"test156", "test156"
		)
	);
	auto& session=sessions.back();

	session->connect_on_motd(
		std::bind(
			&controller::handle_session_motd,
			this,
			ph::_1
		)
	);
	session->connect_on_join_channel(
		std::bind(
			&controller::handle_session_join_channel,
			this,
			ph::_1
		)
	);

	session->connect_on_notice(
		[&](const std::string& msg) {
			view.append_message("notice: "+msg);
		}
	);
}

void controller::start_connection(const std::string& server) {
	auto ic=irc::connection::make_shared(io_service, server, "6667");
	ic->connect_on_resolve(
		[&]{ view.append_message("connection resolved"); });
	ic->connect_on_connect(
		std::bind(
			&controller::handle_connection_connect,
			this,
			ic
		)
	);
}


//see controller_parse_text.hpp for impl
void controller::parse_text(const std::string& text) {
	parse_text(text.cbegin(), text.cend());
}

void controller::handle_text_input(const std::string& str) {
	parse_text(str);
	view.set_input({});
}
void controller::handle_special_char(int) {
}

void controller::handle_join(const std::vector<std::string>& chans) {
	if(!sessions.empty()) {
		for(const auto& chan : chans) {
			sessions[0]->async_join(chan);			
		}
	}
	//view.set_channels(input.cbegin(), input.cend());
}

void controller::handle_part(const std::string& chan, const std::string msg) {

}

void controller::handle_connect(const std::string& chan) {
	 view.append_message("attempting to connect to: " + chan);
	 start_connection(chan);
}



void controller::handle_nick(const std::string& nick) {
	if(!sessions.empty()) {
		sessions[0]->async_change_nick(nick);	
	}
}
void controller::handle_msg(const std::string& target, const std::string& msg) {
}
void controller::handle_text(const std::string& text) {
	if(selected_channel != nullptr) {
		selected_channel->async_send_message(text);
	}
	view.append_message(text);
}
void controller::handle_exec(const std::string& exec) {
}
void controller::handle_quit() {
	view.stop();
}

void controller::handle_session_motd(const std::string& motd) {
	std::istringstream iss { motd };
	std::string t;
	while(std::getline(iss, t)) {
		view.append_message(t);
	}
}

void controller::handle_session_join_channel(irc::channel& chan) {
	//TODO: if on auto change
	selected_channel=&chan;	

	chan.connect_on_message(
		std::bind(
			&controller::handle_channel_message,
			this, ph::_1, ph::_2, ph::_3
		)
	);
	chan.connect_on_topic_change(
		std::bind(
			&controller::handle_channel_topic_change,
			this, ph::_1, ph::_2
		)
	);

	view.append_message("JOINED " + chan.get_name());
}

void controller::handle_channel_message(irc::channel& chan, 
                                        const irc::user& user, 
                                        const std::string& msg) {
	if(selected_channel != nullptr
	&& &chan == selected_channel) {
		std::ostringstream oss;
		oss << user.get_nick() << " said: " << msg;

		auto msg_ptr=std::make_shared<chan_message>(user.get_nick(), msg);
		view.append_message(msg_ptr);
		messages.push_back(std::move(msg_ptr));
	}
}

void controller::handle_channel_topic_change(irc::channel& chan, 
                                             const std::string& msg) {
	if(selected_channel==&chan) {
		view.set_title(msg);
	}
}

controller::controller() 
:	view { io_service }
{
	view.reg_on_text_input(
		std::bind(&controller::handle_text_input, this, ph::_1));

	view.reg_on_special_char(
		std::bind(&controller::handle_special_char, this, ph::_1));
}

void controller::run() {
	for(;;) {
		try {
			io_service.run();
			break; //io service has run to completion
		}
		catch(const std::exception& e) {
			std::cerr << "Exception" << e.what() << std::endl;
		}
	}
}
