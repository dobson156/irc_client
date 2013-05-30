#include "message.hpp"
#include "message_stencil.hpp"

#include <sstream>
#include <cassert>
#include <chrono>

std::string time_to_string(const std::chrono::system_clock::time_point& pt) {
	std::time_t t=std::chrono::system_clock::to_time_t(pt);
	std::string ts(6, '\0');
	std::strftime(&ts[0], ts.size(), "%R", std::localtime(&t));
	return ts; //TODO: shrink str
}

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
		auto ts=time_to_string(msg.get_time_stamp());
		pos=frame.write({0,0}, ts);
		pos=frame.write(pos, msg.get_sender());
		pos=frame.write(pos, msg.get_content());
	}
	last=cons::point{dim.x, pos.y+1};
}



void message_stencil::operator()(join_message& msg) {
	assert(frame_ && "frame was not valid");
	cons::frame& frame=*frame_;

	frame_=nullptr;

	cons::point pos {0,0};
	auto dim=frame.get_dimension();

	if(dim.y > 0) {
		const auto& pfx=msg.get_prefix();
		auto ts=time_to_string(msg.get_time_stamp());
		pos=frame.write(pos, ts);

		assert(pfx.nick);
		std::ostringstream oss;
		oss << pfx;

		pos=frame.write(pos, *pfx.nick);
		pos=frame.write(pos, " has joined ");
		pos=frame.write(pos, oss.str());
	}
	last=cons::point{dim.x, pos.y+1};
}

void message_stencil::operator()(part_message& msg) {
	assert(frame_ && "frame was not valid");
	cons::frame& frame=*frame_;

	frame_=nullptr;

	cons::point pos {0,0};
	auto dim=frame.get_dimension();

	if(dim.y > 0) {
		const auto& pfx=msg.get_prefix();
		auto ts=time_to_string(msg.get_time_stamp());
		pos=frame.write(pos, ts);

		assert(pfx.nick);
		std::ostringstream oss;
		oss << pfx;

		pos=frame.write(pos, *pfx.nick);
		pos=frame.write(pos, " has parted ");
		if(msg.get_message()) {
			pos=frame.write(pos, "with ");
			pos=frame.write(pos, *msg.get_message());
		}
		pos=frame.write(pos, oss.str());
	}
	last=cons::point{dim.x, pos.y+1};
}
