#ifndef MESSAGE_STENCIL_HPP
#define MESSAGE_STENCIL_HPP

#include "ui/basic.hpp"

#include "message_visitor.hpp"

class message_stencil : public message_vistor {
	cons::frame *frame_;
	cons::point last;
public:	
	cons::point write_to(message&, cons::frame&);
	void operator()(message&)      override;
	void operator()(chan_message&) override;
}; //message_stencil

#endif //MESSAGE_STENCIL_HPP
