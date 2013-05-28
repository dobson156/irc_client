#include "message.hpp"
#include "window.hpp"

#include "irc/channel.hpp"
#include "irc/user.hpp"

#include <string>


channel_window::channel_window(irc::channel& chan_) 
:	chan ( chan_ )
{
	//TODO: make the signal handlers exception safe
	auto usr_sig=chan.connect_on_user_join(
		[&](const irc::channel& chan_, const irc::user& user) {
			assert(&chan_==&chan);
			messages.push_back(
				std::make_shared<join_message>(user.get_prefix())
			);
		}
	);

	auto msg_sig=chan.connect_on_message(
		[&](const irc::channel& chan_, const irc::user& user, 
			                           const std::string& str) {
			assert(&chan_==&chan);
			messages.push_back(
				std::make_shared<chan_message>(user.get_nick(), str)
			);
		}
	);

	//Didn't really want any more nested functions
	connections.assign( { std::move(msg_sig), std::move(usr_sig) } );
}


channel_window::~channel_window() {
	for(auto& con : connections) con.disconnect();
}

