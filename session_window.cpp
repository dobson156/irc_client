#include "message.hpp"
#include "window.hpp"

#include "irc/session.hpp"

#include <string>


session_window::session_window(irc::session& session_) 
:	session ( session )
{
	auto ntc_sig=session.connect_on_notice(
		[&](const std::string& notice) {

		}
	);

	auto motd_sig=session.connect_on_motd(
		[&](const std::string& motd) {
			
		}
	);

	//Didn't really want any more nested functions
	connections.assign( { 
		std::move(ntc_sig), 
		std::move(motd_sig)
	} );
}


session_window::~session_window() {
	for(auto& con : connections) con.disconnect();
}

