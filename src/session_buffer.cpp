
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "message.hpp"
#include "buffer.hpp"
#include "pallet.hpp"

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
						messages.push_back(std::make_shared<text_message>(
								u.get_nick(), 
								get_pallet().get_colour_pair(pallet_idx::notice_msg),
								msg,
								get_pallet().get_colour_pair(pallet_idx::default_colour)
							)
						);
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

