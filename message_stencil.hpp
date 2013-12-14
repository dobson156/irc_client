#ifndef MESSAGE_STENCIL_HPP
#define MESSAGE_STENCIL_HPP

#include "ui/basic.hpp"

#include "message_visitor.hpp"

class message_stencil : public message_vistor {
	cons::frame *frame_;
	cons::point last;
public:	
	using value_type=std::shared_ptr<message>;
	cons::point write_to(cons::frame&, const value_type&);
	void operator()(message&)      override;
	void operator()(chan_message&) override;
	void operator()(join_message&) override;
	void operator()(part_message&) override;
	void operator()(motd_message&) override;
	void operator()(error_message&) override;
	void operator()(list_message&) override;
}; //message_stencil

#endif //MESSAGE_STENCIL_HPP
