#include "controller.hpp"
#include "util.hpp"

#include "irc/session.hpp"
#include "irc/connection.hpp"
#include "irc/channel.hpp"
#include "irc/user.hpp"

#include <boost/iterator/transform_iterator.hpp>

#include <exception>
#include <iostream>
#include <fstream>

void controller::set_channel(buffer& buff) {
	auto& win=view.get_selected_window();
	view.set_selected_channel(buff.get_name());
	win.set_target_buffer(buff);
}

void controller::set_channels() {
    auto get_name=[](const std::unique_ptr<buffer>& bp) { 
		return bp->get_name(); 
	};
	view.assign_channels( //iterate over channels as strings
		boost::make_transform_iterator(buffers.begin(), get_name),
		boost::make_transform_iterator(buffers.end(),   get_name)
	);
}

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

	buffers.push_back(std::move(sess_win));
	set_channels();
	//if auto change
	set_channel(*buffers.back());
	
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
			status_buf.push_back_msg("successfully resolved host " + server);
		}
	);
	ic->connect_on_connect(
		[=] {
			auto& status_buf=get_status_buffer();
			status_buf.push_back_msg("successfully connected to host " + server);
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
	auto& window=view.get_selected_window();
	const auto& buff=window.get_buffer();
	auto first=begin(buffers), last=end(buffers);
		
	auto it=std::find_if(first, last, 
		[&](const std::unique_ptr<buffer>& b) { return b.get()==&buff; }
	);
	assert(it!=last); //is an actual existing buffer

	switch(ch) {
	default: 
		break;

	case cons::ctrl_char::ctrl_arrow_left:
		if(it==first) it=last;
		--it;
		set_channel(**it);
		break;
	case cons::ctrl_char::ctrl_arrow_right:
		++it;
		if(it==last) it=first;

		set_channel(**it);
		break;
	}
}

void controller::handle_join(const std::vector<std::string>& chans) {
	auto& win=view.get_selected_window();
	auto& buf=win.get_buffer();

	if(has_session hs { buf } ) {
		auto& sess=hs.get_session();
		for(const auto& ch : chans) {
			sess.async_join(ch);
		}
	}
}

void controller::handle_part(const std::string& chan, const std::string& msg) {
	auto& win=view.get_selected_window();
	auto& buf=win.get_buffer();
	
	if(has_channel hc { buf } ) {
		auto& chan=hc.get_channel();
		chan.async_part();
	}
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
	auto& win=view.get_selected_window();
	auto& buf=win.get_buffer();
	
	if(has_channel hc { buf } ) {
		auto& chan=hc.get_channel();
		chan.async_send_message(text);
	}
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

void controller::handle_names() {
	auto& win=view.get_selected_window();
	auto& buf=win.get_buffer();
	
	if(has_channel hs { buf } ) {
		auto& chan=hs.get_channel();
		auto& status=get_status_buffer();
		
		std::for_each( chan.user_begin(), chan.user_end(),
			[&](const irc::user& u) { status.push_back_msg(u.get_nick()); }
		);

	}
}

void controller::handle_session_join_channel(irc::channel& chan) {
	chan.connect_on_channel_part(
		[&](irc::channel& chand) {
			auto it=std::find_if(begin(buffers), end(buffers),
				[&](const std::unique_ptr<buffer>& b) {
					assert(b);
					has_channel hc { *b };
					return &hc.get_channel() == &chan;
				}
			);
			if(it!=end(buffers)) {
				buffers.erase(it);
				set_channels();
				set_channel(get_status_buffer());
			}
		}
	);

	buffers.push_back(util::make_unique<channel_buffer>(chan));
	set_channels();
	set_channel(*buffers.back());	
}

log_buffer& controller::get_status_buffer() {
	for(const auto& p : buffers) {
		if(p->get_name() == "status") 
			return static_cast<log_buffer&>(*p);
	}
	assert(false && "status buffer missing");
	throw std::runtime_error("status buffer missing");
}

const log_buffer& controller::get_status_buffer() const {
	for(const auto& p : buffers) {
		if(p->get_name() == "status") 
			return static_cast<const log_buffer&>(*p);
	}
	assert(false && "status buffer missing");
	throw std::runtime_error("status buffer missing");
}

log_buffer& controller::get_or_make_error_buffer() {
	for(const auto& p : buffers) {
		if(p->get_name() == "error") 
			return static_cast<log_buffer&>(*p);
	}

	auto p=util::make_unique<log_buffer>("error");
	auto& r=*p;
	buffers.push_back(std::move(p));
	return r; 
}


controller::controller() 
// can not init init list form r vals, hence lmbd hack
:	buffers       {	[]{	std::vector<std::unique_ptr<buffer>> b; 
	                  	b.push_back(util::make_unique<buffer>("status"));
						return b;
	                }()
                  }
,	view          { io_service, *buffers[0]           }
{
	view.connect_on_text_input(
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
				auto& ebuff=get_or_make_error_buffer();
				ebuff.push_back_msg(e.what());
			}
		}
	}
}

// This is required so that the implementation of 
// irc::session isn't required in the header
controller::~controller() { }
