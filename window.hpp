#include "irc/types.hpp"

#include <memory>
#include <vector>

class message;

class window {
protected:
	std::vector<std::shared_ptr<message>> messages;	
public:
	virtual ~window(){}
};



class channel_window : public window {
	irc::channel&                      chan;
	std::vector<irc::bsig::connection> connections;

	channel_window(channel_window&&)                =delete;
	channel_window(const channel_window&)           =delete;
	channel_window& operator=(channel_window&&)     =delete;
	channel_window& operator=(const channel_window&)=delete;
public:
	channel_window(irc::channel& chan);
	~channel_window();
};
