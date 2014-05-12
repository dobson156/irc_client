
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "message.hpp"
#include "buffer.hpp"
#include "pallet.hpp"

#include "irc/parse_coloured_string.hpp"
#include "irc/channel.hpp"
#include "irc/user.hpp"

#include <sstream>
#include <string>



channel_buffer::channel_buffer(irc::channel& chan_) 
:	buffer        { chan_.get_name()     }
,	channel_bound ( chan_                )
,	session_bound ( chan_.get_session()  )
,	chan          ( chan_                )
{
	//TODO: make the signal handlers exception safe
	auto usr_sig=chan.connect_on_user_join(
		[&](const irc::channel& chan_, const irc::user& user) {
			assert(&chan_==&chan);

			std::ostringstream oss;
			oss << "has joined " << user.get_prefix();

			messages.push_back(
				std::make_shared<text_message>(
					user.get_nick(),
					get_pallet().get_colour_pair(pallet_idx::join_msg),
					oss.str(),
					get_pallet().get_colour_pair(pallet_idx::join_msg)
				)
			);
			on_new_msg(*this, messages.back());
		}
	);

	auto usr_prt_sig=chan.connect_on_user_part(
		[&](const irc::channel& chan_, const irc::user& user, 
		                               const irc::optional_string& str) {
			assert(&chan_==&chan);

			std::ostringstream oss;
			oss << "has parted";
			if(str) oss << " with: " << *str;

			messages.push_back(
				std::make_shared<text_message>(
					user.get_nick(),
					get_pallet().get_colour_pair(pallet_idx::part_msg),
					oss.str(),
					get_pallet().get_colour_pair(pallet_idx::part_msg)
				)
			);
			on_new_msg(*this, messages.back());
		}
	);

	auto msg_sig=chan.connect_on_privmsg(
		[&](const irc::channel& chan_, const irc::user& user, 
			                           const std::string& str) {
			assert(&chan_==&chan);

			auto rl=irc_split_string_to_rich_list(
				irc::parse_coloured_string(str));

			messages.push_back(
				std::make_shared<rich_message>(user.get_nick(), rl)
			);
			on_new_msg(*this, messages.back());
		}
	);

	auto topic_sig=chan.connect_on_topic_change(
		[&](const irc::channel& chan_, const std::string& topic) {
			assert(&chan_==&chan);
			on_topic_change(*this, topic);
		}
	);

	auto mode_sig=chan.connect_on_mode_change(
		[&](const irc::channel& chan_, const irc::prefix& user, 
			                           const irc::mode_diff& md) {
			std::ostringstream oss;
			oss << "the following modes have been set: " << md << " by " << user;
			//TODO: check whether "user" is an actual user

			messages.push_back(std::make_shared<text_message>(
					std::string{},  
					get_pallet().get_colour_pair(pallet_idx::default_colour),
					oss.str(), 
					get_pallet().get_colour_pair(pallet_idx::set_mode_msg)
				)
			);
			on_new_msg(*this, messages.back());
		}
	);


	//Didn't really want any more nested functions
	connections.assign( { 
		std::move(usr_sig), std::move(usr_prt_sig), 
		std::move(msg_sig), std::move(topic_sig) 
	} );

	connections.push_back(
		chan.connect_on_list_users(
			std::bind(&channel_buffer::list_names, this))
	);
}

void channel_buffer::list_names() {
	auto first=chan.begin_users(),
	     last=chan.end_users();
	
	auto to_list_pair=[](const irc::user& u) {
		return std::make_pair(u.get_nick(), -1);
	};

	messages.push_back(std::make_shared<list_message>(" in channel: ",
			boost::make_transform_iterator(first, to_list_pair),
			boost::make_transform_iterator(last, to_list_pair)
		)
	);
	on_new_msg(*this, messages.back());
}

channel_buffer::~channel_buffer() {
	for(auto& con : connections) con.disconnect();
}

const std::string& channel_buffer::get_topic() const {
	return chan.get_topic();
}
