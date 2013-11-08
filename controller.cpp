#include "controller.hpp"
#include "util.hpp"

#include "irc/session.hpp"
#include "irc/channel.hpp"
#include "irc/user.hpp"

#include <boost/iterator/transform_iterator.hpp>

#include <exception>
#include <iostream>

void controller::set_channel(buffer& buff) {
	auto& win=view.get_selected_window();
	win.set_target_buffer(buff);

	//TODO: reverse on throw exceptionsafe?
	/*
	view.set_title(win.get_topic());	

	view.assign_messages(
		win.messages_begin(),
		win.messages_end()
	);

	//This unset callbacks for the old buffer and resets then for the new
	//unsetting is done via RAII with unique_connection
	win_msg=win.connect_on_new_message(
		[&](const buffer&, std::shared_ptr<message> msg_ptr) {
			view.append_message(std::move(msg_ptr));
		}
	);
	win_tpc=win.connect_on_topic_change(
		[&](const buffer&, const std::string& topic) {
			view.set_title(topic);
		}
	);
	*/
}

void controller::set_channels() {
    auto get_name=[](const std::unique_ptr<buffer>& win) { 
		return win->get_name(); 
	};
	view.assign_channels( //iterate over channels as strings
		boost::make_transform_iterator(buffers.begin(), get_name),
		boost::make_transform_iterator(buffers.end(),   get_name)
	);
}


//TODO implement as a search on the correct name, with assertions
/*
error_buffer& controller::get_status_buffer() { 
	  	return dynamic_cast<error_buffer&>(*buffers[0]); 
}
const error_buffer& controller::get_status_buffer() const { 
	  	return dynamic_cast<const error_buffer&>(*buffers[0]); 
}
*/
void controller::handle_connection_connect(
                  std::shared_ptr<irc::connection> connection) {
	assert(connection && "can not craete sesion with invalid connection");
	sessions.push_back(
		util::make_unique<irc::session>(
			std::move(connection), 
			"test156", "test156"
		)
	);
	auto& session=sessions.back();

	auto sess_win=util::make_unique<session_buffer>(*session);
	set_channel(*sess_win);

	buffers.push_back(std::move(sess_win));
	set_channels();
	
	//this will create a new buffer view
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
		[=] {
			auto& status_buf=get_status_buffer();
			status_buf.push_back_error("successfully resolved host " + server);
		}
	);
	ic->connect_on_connect(
		[=]{ 
			auto& status_buf=get_status_buffer();
			status_buf.push_back_error("successfully connected to host " + server);
			//TODO: ic has an sp to itself? probably not good!!!
			handle_connection_connect(ic); 
		}
	);
	connections.push_back(std::move(ic));
}


//see controller_parse_text.hpp for impl
void controller::parse_text(const std::string& text) {
	parse_text(text.cbegin(), text.cend());
}

void controller::handle_text_input(const std::string& str) {
	parse_text(str);
	view.set_input({});
}
void controller::handle_ctrl_char(cons::ctrl_char ch) {
	switch(ch) {
	default: break;
	//retarget current window

	case cons::ctrl_char::ctrl_arrow_left:
		break;
	case cons::ctrl_char::ctrl_arrow_right:
		auto& window=view.get_selected_window();
		const auto& buffer=window.get_buffer();

		auto first=begin(buffers), last=end(buffers);
		//urghh different pointer types :(
		auto it=std::find(first, last, buffer);

		assert(it!=last); //is an actual existing buffer

		++it;
		if(it==last) it=first;

		window.set_target_buffer(**it);
		break;
	}
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
	/*
	if(selected_channel != nullptr) {
		selected_channel->async_send_message(text);
		auto msg_ptr=std::make_shared<chan_message>(sessions[0]->get_nick(), text);
		view.append_message(msg_ptr);
	}
	*/
}
void controller::handle_exec(const std::string& exec) {
}
void controller::handle_quit() {
	view.stop();
	for(auto& sess : sessions) {
		sess->stop();
	}
	for(auto& conn : connections) {
		conn->stop();
	}
}

void controller::handle_session_join_channel(irc::channel& chan) {
	//TODO: if on auto change
	selected_channel=&chan;	

	auto ch_win=util::make_unique<channel_buffer>(chan);
	set_channel(*ch_win);	
	buffers.push_back(std::move(ch_win));
	set_channels();
}

controller::controller() 
// can not init init list form r vals, hence lmbd hack
:	buffers       {	[]{	std::vector<std::unique_ptr<buffer>> b; 
	                  	b.push_back(util::make_unique<error_buffer>());
						return b;
	                }()
                  }
,	view          { io_service, *buffers[0]           }
{
	view.reg_on_text_input(
		std::bind(&controller::handle_text_input, this, ph::_1));

	view.connect_on_ctrl_char(
		std::bind(&controller::handle_ctrl_char, this, ph::_1));
	
	set_channels();
}

void controller::run() {
	for(;;) {
		try {
			io_service.run();
			break; //io service has run to completion
		}
		catch(const std::exception& e) {
			if(show_errors) {

				auto it=std::find_if(
					begin(buffers), end(buffers), [](const std::unique_ptr<buffer>& win) {
						return win->get_name() == "error";
					}
				);

				if(it!=end(buffers)) {
					//OK better 
					auto error_win=dynamic_cast<error_buffer*>(it->get());
					assert(error_win && "error buffer not of type error_buffer");
					//TODO: significantly improve error reporting
					error_win->push_back_error(e.what());
				}
				else { //we'll just dump it to stdout
					auto ebuff=util::make_unique<error_buffer>();				
					ebuff->push_back_error(e.what());
					set_channel(*ebuff);
					buffers.push_back(std::move(ebuff));
					set_channels();
				}
			}
		}
	}
}
