#include "message.hpp"
#include "buffer.hpp"

#include "irc/session.hpp"

#include <string>


session_buffer::session_buffer(irc::session& session_) 
:	buffer        { "channel" }
,	session_bound ( session_  )
,	session       ( session_  )
{
	connections.push_back(
		session.connect_on_notice(
			[&](const std::string& notice) {

			}
		)
	);
	connections.push_back(
		session.connect_on_motd(
			[&](const std::string& motd) {
				messages.push_back(std::make_shared<motd_message>(motd));
				on_new_msg(*this, messages.back());
			}
		)
	);

	connections.push_back(
		session.connect_on_new_user(
			[&](irc::user& u) {
				u.connect_on_notice(
					[&](irc::user& u, const std::string& msg) {
						//auto& status_buf=get_status_buffer();
						//std::ostringstream oss;
						//oss << "NOTICE: " << u.get_nick() << ": " << msg;
						messages.push_back(std::make_shared<chan_message>(u.get_nick(), msg));
						on_new_msg(*this, messages.back());
					}
				);
			}
		)
	);
}


session_buffer::~session_buffer() {
	//should auto-destroy with unique_connection
}

