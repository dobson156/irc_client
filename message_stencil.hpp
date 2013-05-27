#ifndef MESSAGE_STENCIL_HPP
#define MESSAGE_STENCIL_HPP

#include "ui/basic.hpp"

#include "message_visitor.hpp"

std::string time_to_string(const std::chrono::system_clock::time_point& pt);

class message_stencil : public message_vistor {
	cons::frame *frame_;
	cons::point last;
public:	
	using value_type=std::shared_ptr<message>;
	cons::point write_to(cons::frame&, const value_type&);
	void operator()(message&)      override;
	void operator()(chan_message&) override;
	void operator()(join_message&) override;
}; //message_stencil

#endif //MESSAGE_STENCIL_HPP
