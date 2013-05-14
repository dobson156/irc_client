#include "window_stencil.hpp"
#include "window.hpp"

#include "../ui/console.hpp"

#include <deque>

cons::frame message_to_pad(const std::string& str, cons::point max) {
	cons::frame frm { cons::make_pad(max) };
	cons::string_stencil ss;
	int y_used=ss.write_to(frm, str).y;
	cons::frame cut { cons::make_pad({ max.x, y_used }) };
	copy(frm, cut, {0,0}, {0,0}, cut.get_dimension());
	return cut;
}

//OK this is an fairly expensive algorithm, will try and cut it down at a later date
cons::point window_stencil::write_to(cons::frame& frm, window& win) const {
	auto selected=win.get_selected_idx();

	const auto size=win.size();
	const auto dime=frm.get_dimension();

	std::deque<cons::frame> message_frames;

	//this is going to be an expensive implementation,  which'll do for now
	assert(selected < size);


	int y_used=0;

	std::size_t top, bottom;
	top=bottom=selected;

	message_frames.push_back(message_to_pad(win.msg_at(selected), dime));

	y_used=message_frames.back().get_dimension().y;
	
	//constructs the vector of frames
	bool tdo=true, bdo=true;
	while(y_used < dime.y && (tdo || bdo)) {
		if(top!=0) {
			--top;
			message_frames.push_front(message_to_pad(win.msg_at(top), dime));
			y_used+=message_frames.front().get_dimension().y;
		}
		else tdo=false;

		if(bottom<(size-1) && y_used < dime.y) {
			++bottom;
			message_frames.push_back(message_to_pad(win.msg_at(bottom), dime));
			y_used+=message_frames.back().get_dimension().y;
		}
		else bdo=false;
	}
	//compile vector into single frame
	
	int y=0;
	for(auto& mfrm : message_frames) {
		auto dime=mfrm.get_dimension();
		copy(mfrm, frm, {0,0}, {0,y}, dime);
		y+=dime.y;
	}
}
