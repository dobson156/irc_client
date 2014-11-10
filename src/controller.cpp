
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "controller.hpp"
#include "log_messages.hpp"
#include "util.hpp"

#include "irc/session.hpp"
#include "irc/persistant_connection.hpp"
#include "irc/channel.hpp"
#include "irc/user.hpp"

#include <boost/iterator/transform_iterator.hpp>

#include <exception>

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

void controller::handle_connection_connect(irc::session& session) {
	buffers.push_back(util::make_unique<session_buffer>(session));
	set_channels();
	//if auto change
	set_channel(*buffers.back());
	//this will create a new buffer view
	session.connect_on_join_channel(
		std::bind(
			&controller::handle_session_join_channel,
			this,
			ph::_1
		)
	);

	session.connect_on_irc_error([=](const std::string& err) {
			auto& status_buf=get_status_buffer();
			status_buf.push_back_msg(make_irc_error_msg(err));
		}
	);

	// ne need to register all users so we can hook
	// any priv_msgs
	session.connect_on_new_user(
		[&](irc::user& u) {
			u.connect_on_direct_message(
				[](irc::user& u, const std::string&) {
					//TODO implement privmsg here
				}
			);
		}
	);
#ifdef USING_PYTHON
	session.connect_on_connection_established([this, &session] {
			python_controller.accept_new_session(session);
		}
	);
#endif //USING_PYTHON
}

void controller::start_connection(const std::string& hostname) {
	start_connection(hostname, get_default_nick(), get_default_username(),
		get_default_fullname(), get_default_port());
}
void controller::start_connection(const std::string& hostname,
                                  const std::string& nickname,
                                  const std::string& username,
                                  const std::string& realname,
                                  const std::string& port) {

	auto ic=util::make_unique<irc::persistant_connection>(
		io_service, hostname, port);

	auto& icr=*ic; //for simplicities sake
	icr.connect_on_resolve([=](const std::string& str) {
			get_status_buffer().push_back_msg(str);
		}
	);
	//icr.connect_on_network_error(
	icr.connect_on_disconnect(
		[this](const std::string& e) {
			get_status_buffer().push_back_msg(make_network_error_msg(e));
		}
	);

	sessions.push_back(util::make_unique<irc::session>(
		std::move(ic), nickname, username, realname));
	irc::session& sess=*sessions.back();

	icr.connect_on_connect([=, &sess](const std::string& str) {
			auto& status_buf=get_status_buffer();
			status_buf.push_back_msg("successfully connected to host " + hostname);
			handle_connection_connect(sess);
		}
	);
}

//see controller_parse_text.hpp for impl
void controller::parse_text(const std::string& text) {
	parse_text(text.cbegin(), text.cend());
}

void controller::handle_text_input(const std::string& str) {
	parse_text(str);
	//view.set_input({});
}


void controller::handle_invite(const std::string& nick) {
	auto& win=view.get_selected_window();
	auto& buf=win.get_buffer();

	if(has_channel hc { buf } ) {
		auto& chan=hc.get_channel();
		chan.send_invite(nick);
	}
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
			if(!ch.empty()) { //shouldn't be necessary but still
				if(ch.front() == '#')
					sess.async_join(ch);
				else
					sess.async_join('#' + ch);
			}
		}
	}
}

void controller::handle_part(const std::string& chan, const std::string& msg) {
	auto& win=view.get_selected_window();
	auto& buf=win.get_buffer();

	if(has_channel hc { buf } ) {
		auto& chan=hc.get_channel();
		chan.send_part(msg);
	}
}

void controller::handle_leave(const std::string& msg) {
	auto& win=view.get_selected_window();
	auto& buf=win.get_buffer();

	if(has_channel hc { buf } ) {
		auto& chan=hc.get_channel();
		chan.send_part(msg);
	}

}
void controller::handle_connect(const std::string& chan) {
	start_connection(chan);
}


void controller::handle_nick(const std::string& nick) {
	auto& win=view.get_selected_window();
	auto& buf=win.get_buffer();

	if(has_session hs { buf } ) {
		auto& sess=hs.get_session();
		sess.async_change_nick(nick);
	}
}

void controller::handle_msg(const std::string& target, const std::string& msg) {
	auto& win=view.get_selected_window();
	auto& buf=win.get_buffer();

	if(has_session hs { buf }) {
		auto& sess=hs.get_session();
		sess.async_privmsg(target, msg);
	}
}

void controller::handle_text(const std::string& text) {
	auto& win=view.get_selected_window();
	auto& buf=win.get_buffer();
	
	if(has_channel hc { buf } ) {
		auto& chan=hc.get_channel();
		chan.send_privmsg(text);
	}
}

void controller::handle_python(const std::string& py_code) {
#ifdef USING_PYTHON
	python_controller.exec(py_code);
#endif //USING_PYTHON
}

void controller::handle_exec(const std::string& exec) {

}

void controller::handle_quit() {
	view.stop();
	for(auto& sess : sessions) {
		sess->stop();
	}
	//io_service.stop();
}

void controller::handle_names() {
	auto& win=view.get_selected_window();
	auto& buf=win.get_buffer();
	
	if(has_channel hs { buf } ) {
		auto& chan=hs.get_channel();
		auto& status=get_status_buffer();
		
		std::for_each( chan.begin_users(), chan.end_users(),
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

const std::string controller::get_default_nick() const {
	return default_nick;
}
const std::string controller::get_default_username() const {
	return default_username;
}
const std::string controller::get_default_fullname() const {
	return default_fullname;
}
const std::string controller::get_default_port() const {
	return default_port;
}

controller::controller(std::string pyton_config_file_)
// can not init init list form r vals, hence lmbd hack
:	buffers       {	[]{	std::vector<std::unique_ptr<buffer>> b; 
	                 		b.push_back(util::make_unique<log_buffer>("status"));
							return b;
	                 	}()
                      }
,	view              { io_service, *buffers[0]           }
#ifdef USING_PYTHON
,	python_controller { std::move(pyton_config_file_)  }
#endif //USING_PYTHON
{
	auto username=util::try_get_user_name();
	if(username) {
		default_username=*username;
		default_nick=std::move(*username);
	}
	auto fullname=util::try_get_full_name();
	if(fullname) default_fullname=std::move(*fullname);

	view.connect_on_text_input(
		std::bind(&controller::handle_text_input, this, ph::_1));

	view.connect_on_ctrl_char(
		std::bind(&controller::handle_ctrl_char, this, ph::_1));

#ifdef USING_PYTHON
	python_controller.connect_on_connect(
		[this](const std::string& server) { start_connection(server); });
	python_controller.connect_on_change_default_nick(
		util::make_assign_on_call(default_nick));
	python_controller.connect_on_change_default_username(
		util::make_assign_on_call(default_username));
	python_controller.connect_on_change_default_fullname(
		util::make_assign_on_call(default_fullname));


	python_controller.connect_on_python_error(
		[this](const std::string s) {
			if(show_errors) {
				auto& status_buf=get_status_buffer();
				status_buf.push_back_msg(make_python_error_msg(s));
			}
		}
	);
	python_controller.connect_on_python_output(
		[this](const std::string s) {
			auto& status_buf=get_status_buffer();
			status_buf.push_back_msg(make_python_stdout_msg(s));
		}
	);
	python_controller.reload_conf();
#endif //USING_PYTHON

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
				//auto& ebuff=get_or_make_error_buffer();
				auto& ebuff=get_status_buffer();
				ebuff.push_back_msg(e.what());
			}
		}
	}
}

// This is required so that the implementation of 
// irc::session isn't required in the header
controller::~controller() { }
