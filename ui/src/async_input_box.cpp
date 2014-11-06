
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "assert.hpp"
#include "async_input_box.hpp"
#include "ctrl_char_parser.hpp"

#include <algorithm>

namespace cons {

async_input_box::async_input_box(unique_window_ptr ptr,
                                 boost::asio::io_service& io_service)
:	frame_        { std::move(ptr)               }
,	in_manager    { make_tty_manager(io_service) }
,	io_service    { &io_service                  }
{
	set();
}

void async_input_box::stop() {
	in_manager.stop();
}

point async_input_box::calc_cursor_position() const {
	const auto& value=history.get_current();

	auto dim=get_dimension();
	return {
		int(value.size()) % dim.x,
		int(value.size()) / dim.x,
	};
}

void async_input_box::handle_read_error(const boost::system::error_code&) {

}

void async_input_box::handle_read_complete(std::string str) {
	bool do_refresh=false;
	auto value=history.get_current();
/*
	std::stringstream os;
	for(char c : str) {
		os << "0x" << std::hex << int(c);
	}
	str =os.str();
*/

	for(auto it=str.cbegin(); it < str.cend(); ++it) {

		char c = *it;

		ctrl_char cht;
		std::tie(cht, it)=parse_ctrl_char(it, str.cend());

		switch(cht) {
		case ctrl_char::none:
			if(std::isprint(c)) {
				value.insert(value.begin()+pos, *it);
				++pos;
				do_refresh=true;
			}
			break;
		case ctrl_char::newline: //fall through
		case ctrl_char::carriage_ret:
			history.commit_current();
			set_position();
			on_input(value);
			refresh();
		//	do_refresh=true;
			break;
		case ctrl_char::arrow_right:
			if(pos < value.size())
				++pos;
			do_refresh=true;
			break;
		case ctrl_char::arrow_left:
			if(pos > 0)
				--pos;
			do_refresh=true;
			break;
		case ctrl_char::backspace:
			if(!value.empty() && pos != 0) {
				value.erase(pos-1, 1);
				--pos;
				do_refresh=true;
			}
			break;
		case ctrl_char::del:
			if(pos < value.size()) {
				value.erase(pos, 1);
				do_refresh=true;
			}
			break;
		case cons::ctrl_char::arrow_up:
			history.dec_idx();
			set_position();
			refresh();
			break;
		case cons::ctrl_char::arrow_down:
			history.inc_idx();
			set_position();
			refresh();
			break;
		default:
			//All other cases are passed on to the user
			on_ctrl_char(cht);
			break;
		}
	}
	if(do_refresh) {
		history.set_current(std::move(value));
		refresh();
	}
	set();
}

void async_input_box::set() {
	auto pos=calc_cursor_position();

	wmove(frame_.get_handle(), pos.y, pos.x);
	frame_.refresh();

	//TODO: handle  error
	in_manager.async_read(
		[this](std::string str) {
			handle_read_complete(std::move(str));
		}
	);
		//std::bind(&async_input_box::handle_read_complete, this, ph::_1));
}

bool async_input_box::grow(point pt) {
	//TODO ensure this works as expected in the
	//true, false and unconnected cases
	auto res=on_grow(pt);
	return res ? *res : false;
}

void async_input_box::refresh() {
	auto dim=get_dimension();
	const auto& value=history.get_current();

	int y=stencil.required_y(dim.x, value.size());
	point required { dim.x, y };

	frame_.clear();

	if(value.empty()) {
		if(dim.y!=1 && grow({dim.x, 1})) {
			//In theory this should invoke a recursive refresh
			//we should just guard the function so that can't happen
			//and explicitly reresh on the first iteration
			return;
		}

		frame_.set_colour(cursor_colour);
		frame_.write({0,0}, ' ');
		frame_.set_colour(cursor_colour, false);
		frame_.refresh();
		return;
	}
	if(y <= dim.y) {
		auto last=frame_.write({0,0}, value, pos);
		frame_.set_colour(cursor_colour);
		if(pos == value.size()) {
			last=frame_.write(last, ' ');
			frame_.set_colour(cursor_colour, false);
		}
		else {
			last=frame_.write(last, value[pos]);
			frame_.set_colour(cursor_colour, false);
			frame_.write(last, value.begin() + pos + 1, value.end());
		}
	}
	else if(!grow(required)) {
		//TODO: a pad based draw
	}
	frame_.refresh();
}

void async_input_box::clear() {
	pos=0;
	history.set_current({});
	frame_.clear();
}

void async_input_box::set_position() {
	pos=history.get_current().size();
}

void async_input_box::set_value(const std::string& str) {
	history.set_current(str);
	set_position();
}

const std::string& async_input_box::get_value() const {
	return history.get_current();
}

//Overrides
void async_input_box::set_position(const point& position) {
	auto dim=frame_.get_dimension();
	WINDOW *p=wgetparent(frame_.get_handle());

	CONS_ASSERT(p, "parent is null");

	frame_=frame { make_window(p, position, dim) };
	touchwin(frame_.get_handle());
	//frame_.set_position(position);
}
void async_input_box::set_dimension(const point& dimension) {
	frame_.set_dimension(dimension);
	touchwin(frame_.get_handle());
}
point async_input_box::get_position()  const {
	return frame_.get_position();
}
point async_input_box::get_dimension() const {
	return frame_.get_dimension();
}
unique_window_ptr async_input_box::reset(unique_window_ptr handle) {
	return frame_.reset(std::move(handle));
}
//Colour
void async_input_box::set_background(short bg) {
	frame_.set_background(bg);
}
void async_input_box::set_foreground(short fg) {
	frame_.set_foreground(fg);
}
short async_input_box::get_background() const {
	return frame_.get_background();
}
short async_input_box::get_foreground() const {
	return frame_.get_foreground();
}

} //namespace cons
