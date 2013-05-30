#include "controller.hpp"
#include "util.hpp"

#include "irc/session.hpp"
#include "irc/channel.hpp"
#include "irc/user.hpp"

#include <boost/iterator/transform_iterator.hpp>

#include <exception>
#include <iostream>

const std::string& win_get_name::operator()(
                      const std::unique_ptr<window>& win) const {
	return win->get_name();
}


void controller::set_channels() {
	view.set_channels( //iterate over channels as strings
		boost::make_transform_iterator(windows.begin(), win_get_name()),
		boost::make_transform_iterator(windows.end(),   win_get_name())
	);
}

void controller::handle_connection_connect(
                  std::shared_ptr<irc::connection> connection) {
	sessions.push_back(
		util::make_unique<irc::session>(
			std::move(connection), 
			"test156", "test156"
		)
	);
	auto& session=sessions.back();

	auto sess_win=util::make_unique<session_window>(*session);
	windows.push_back(std::move(sess_win));
	set_channels();
	
	//this will create a new window view
	session->connect_on_join_channel(
		std::bind(
			&controller::handle_session_join_channel,
			this,
			ph::_1
		)
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
		auto msg_ptr=std::make_shared<chan_message>(sessions[0]->get_nick(), text);
		view.append_message(msg_ptr);
		messages.push_back(std::move(msg_ptr));
	}
}
void controller::handle_exec(const std::string& exec) {
}
void controller::handle_quit() {
	view.stop();
}

void controller::handle_session_join_channel(irc::channel& chan) {
	//TODO: if on auto change
	selected_channel=&chan;	

	auto ch_win=util::make_unique<channel_window>(chan);
	ch_win->connect_on_new_message(
		[&](const window&, std::shared_ptr<message> msg_ptr) {
			view.append_message(std::move(msg_ptr));
		}
	);
	ch_win->connect_on_topic_change(
		[&](const window&, const std::string& topic) {
			view.set_title(topic);
		}
	);
	windows.push_back(std::move(ch_win));
	set_channels();
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
//			auto it=std::find_if(
//				windows.begin(), windows.end(), [](const std::unique_ptr<window>& win) {
//					return win.get_name() == "error";
//				}
//			);
//			if(it!=windows.end()) {
//				auto error_win=dynamic_cast<error_window>(it->get());
//				if(error_win) {
//					error_win->add_error(e);
//				}
//			}
//			else { //we'll just dump it to stdout
				std::cerr << "Exception" << e.what() << std::endl;
//			}
		}
	}
}
