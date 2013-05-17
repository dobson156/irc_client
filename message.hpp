#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <chrono>

class message_vistor {
public:
	virtual void operator(message&)     =0;
	virtual void operator(chan_message&)=0;
};

class message {
	using time_point=std::chrono::system_clock::time_point;
	time_point time_stamp;
public:
	message(time_point time_stamp_)
	:	time_stamp { std::move(time_stamp) }
	{	}

	message() 
	:	time_stamp { std::chrono::system_clock::now() }
	{	}

	virtual void visit(message_vistor&)=0;

	message(message&&)                =default;
	message(const message&)           =default;
	message& operator=(message&&)     =default;
	message& operator=(const message&)=default;

	const time_point get_time_stamp() const;
}; //class message


class chan_message : public message {
	std::string sender;
	std::string content;
public:
	chan_message(std::string sender_, std::string content_) 
	:	sender  { std::move(sender_)  }
	,	content { std::move(content_) } 
	{	}

	void visit(message_vistor& visitor) override {
		visitor(*this);
	}
}; //chan_message


#endif //MESSAGE_HPP
