
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "util.hpp"
#include "message.hpp"
#include "message_stencil.hpp"

#include <sstream>
#include <cassert>
#include <chrono>

cons::point message_stencil::write_to(cons::output_pane& frame, const value_type& msg) {
	assert(msg);
	frame_=&frame;
	msg->visit(*this);
	return last;
}

void message_stencil::operator()(message& msg) {
	assert(false);
}

void message_stencil::operator()(motd_message& msg) {
	assert(frame_ && "frame was not valid");
	cons::output_pane& frame=*frame_;
	frame_=nullptr;

	cons::point pos {0,0}, dim=frame.get_dimension();
	std::istringstream iss { msg.get_motd() };
	std::string line;
	for(int i=0; i!=dim.y && std::getline(iss, line); ++i) {
		pos=frame.write({0,i}, line);
	}
	last=cons::point{dim.x, pos.y+1};
}

void message_stencil::operator()(error_message& msg) {
	assert(frame_ && "frame was not valid");
	cons::output_pane& frame=*frame_;
	frame_=nullptr;

	cons::point pos {0,0}, dim=frame.get_dimension();
	std::istringstream iss { msg.get_error() };
	std::string line;
	for(int i=0; i!=dim.y && std::getline(iss, line); ++i) {
		pos=frame.write({0,i}, line);
	}
	last=cons::point{dim.x, pos.y+1};
}

void message_stencil::operator()(list_message& msg) {
	assert(frame_ && "frame was not valid");
	cons::output_pane& frame=*frame_;
	frame_=nullptr;

	int max_wid=msg.max_element_size() + 2;
	auto dim=frame.get_dimension();
	auto time=util::time_to_string(msg.get_time_stamp());
	cons::point pos{0,0};

	if(dim.y > 0) {
		pos=frame.write(pos, time);
		pos=frame.write(pos, msg.get_header());

		int indent=pos.x;
		int divisions=(dim.x-indent) / max_wid;
		int i=0;

		for(auto& e : msg) {
			if(i==divisions) {
				i=0;
				++pos.y;
				pos.x=indent;
			}
			frame.write(frame.write(pos, e.first), ", ");
			pos.x+=max_wid;
			++i;
		}
	}
	last=cons::point{dim.x, pos.y+1};
}

void message_stencil::operator()(text_message& msg) {
	assert(frame_ && "frame was not valid");

	//the frame to write to
	cons::output_pane& frame=*frame_;
	frame_=nullptr; 

	auto dim=frame.get_dimension();
	cons::point pos{0,0};

	if(dim.y > 0) {
		frame.set_colour(msg.get_header_colour());
		//Write time and header
		pos=frame.write(pos, util::time_to_string(msg.get_time_stamp()));
		pos=frame.write(pos, " ");
		frame.set_bold(true);
		pos=frame.write(pos, msg.get_header());
		pos=frame.write(pos, " ");
		//TODO: make more complex
		frame.set_bold(false);
		frame.set_colour(msg.get_body_colour());
		pos=frame.write(pos, msg.get_body());
	}
	last=cons::point{dim.x, pos.y+1};
}
