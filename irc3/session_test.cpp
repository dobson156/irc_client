#include "session.hpp"
#include "connection.hpp"

#include "util.hpp"

#include <iostream>

/*
** Main program state 
*/
class program {
public:
	program(boost::asio::io_service& io_service) {
		auto ic=irc::connection::make_shared(io_service, "irc.freenode.net", "6667");

		ic->connect_on_resolve([]{ std::cout << "resolved"  << std::endl; });

		ic->connect_on_connect(
			std::bind(
				&program::handle_connect,
				this,
				ic
			)
		);
	}

	void handle_connect(std::shared_ptr<irc::connection> connection) {
		assert(connection);
		sessions.push_back(
			util::make_unique<irc::session>(
				std::move(connection), 
				"test156", "test156"
			)
		);

		sessions.back()->async_join("#linux");
		sessions.back()->async_join("#bown_fox");

		sessions.back()->connect_on_motd(
			[](const std::string& motd) {
				std::cout << "MOTD\n" << motd << std::endl;
			}
		);

		sessions.back()->connect_on_join_channel(
			[](irc::channel& chan) {
				std::cout << "JOINED" << chan.get_name() << std::endl;
				chan.connect_on_user_join(
					[&](const irc::prefix& pfx, const std::string& str) {
						std::cout << str << " at " <<  pfx << " has joined: " << chan.get_name() << std::endl;
					}
				);
				chan.connect_on_user_leave(
					[&](const irc::prefix& pfx, const std::string& chn, irc::optional_string& msg) {
						std::cout << *pfx.nick << " has left from: " << chan.get_name();
						if(msg) std::cout << " reason: " << *msg;
						std::cout << std::endl;
					}
				);
				chan.connect_on_message(
					[&](const std::string& nick, const std::string& msg) {
						if(nick=="|pipe|") {
							chan.async_send_message("hi pipe");
						}
						std::cout << nick << " said " << msg << std::endl;
					}
				);
			}
		);
			
	}
private:
	std::vector<std::unique_ptr<irc::session>> sessions;
};


int main() {
	boost::asio::io_service io_service;
	program prog { io_service };
	io_service.run();
}
