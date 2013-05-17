#include "message.hpp"
#include "message_stencil.hpp"

#include <cassert>

cons::point message_stencil::write_to(message& msg, cons::frame& frame) {
	frame_=&frame;
	msg.visit(*this);
	return last;
}

void message_stencil::operator()(message& msg) {
	assert(false);
}

void message_stencil::operator()(chan_message& msg) {
	assert(frame_);
	cons::frame& frame=*frame_;
	frame_=nullptr;

	int y=0;
	auto dim=frame.get_dimension();
	cons::point pos;
	if(dim.y > y) { //at least on line
		pos=frame.write({0,0}, msg.get_sender());
		frame.write(pos, msg.get_content());
	}
	last=cons::point{dim.x, pos.y};
}


