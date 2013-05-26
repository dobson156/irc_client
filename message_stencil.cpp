#include "message.hpp"
#include "message_stencil.hpp"

#include <cassert>
#include <chrono>

cons::point message_stencil::write_to(cons::frame& frame, const value_type& msg) {
	assert(msg);
	frame_=&frame;
	msg->visit(*this);
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
		std::time_t t=std::chrono::system_clock::to_time_t(msg.get_time_stamp());
		std::string ts(6, '\0');
		std::strftime(&ts[0], ts.size(), "%R", std::localtime(&t));
		pos=frame.write({0,0}, ts);
		pos=frame.write(pos, msg.get_sender());
		pos=frame.write(pos, msg.get_content());
	}
	last=cons::point{dim.x, pos.y+1};
}


