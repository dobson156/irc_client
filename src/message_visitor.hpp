
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MESSAGE_VISTOR_HPP
#define MESSAGE_VISTOR_HPP

class message;
class chan_message;// : public message;

class message_vistor {
public:
	virtual void operator()(message&)     =0;
	virtual void operator()(motd_message&)=0;
	virtual void operator()(error_message&)=0;

	virtual void operator()(list_message&)=0;
	virtual void operator()(text_message&)=0;
};

#endif //MESSAGE_HPP
