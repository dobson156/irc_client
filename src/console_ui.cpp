
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "buffer.hpp"
#include "console_ui.hpp"

#include <csignal>
#include <sys/ioctl.h>
#include <ncurses.h>

namespace ui_impl {

ui::ui(boost::asio::io_service& io_service_, buffer& buffer                    )
:	parent         { make_window(), 16                                         }
,	input_anchor   ( parent.emplace_fill<anchor_bottom>(1)                     )
,	channel_border ( parent.emplace_anchor<bordered>(borders::right)           )
,	channel_list   ( channel_border.emplace_element<text_list>()               )
,	input          ( input_anchor.emplace_anchor<async_input_box>(io_service_) )
,	window1        ( input_anchor.emplace_fill<window>(io_service_, buffer)    )
,	io_service     { &io_service_                                              }
,	signal_set     { io_service_, SIGWINCH, SIGINT                             }
{
	channel_border.set_background(COLOR_BLUE);
	channel_border.set_foreground(COLOR_WHITE);
	channel_list.highlight_selected(true);

	input.connect_on_grow(
		[&](const point& pt) {
			input_anchor.set_partition(pt.y);
			input_anchor.refresh();
			return true;
		}
	);

	input.connect_on_ctrl_char(
		[&, this](cons::ctrl_char ch) {
			switch(ch) {
			case cons::ctrl_char::page_up:
				get_selected_window().scroll_up();
				break;
			case cons::ctrl_char::page_down:
				get_selected_window().scroll_down();
				break;
			default:
				on_ctrl_char(ch);
			}
		}
	);

	signal_set.async_wait(
		std::bind(&ui::handle_sigwinch, this, ph::_1, ph::_2));

	refresh();
	input.refresh();
}

void ui::redraw() {
	//TODO: this code should be in the UI lib
	endwin();
	winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	resizeterm(w.ws_row,w.ws_col);
	parent.reset(make_window());
	refresh();
}

void ui::handle_sigwinch(const boost::system::error_code& e, int sig) {
	//sig==SIGWINCH this should be true, but there are other signals that are equivelant and
	//also get passed in here
	if(sig == SIGINT) return;

	if(!e) {
		redraw();
		signal_set.async_wait(
			std::bind(&ui::handle_sigwinch, this, ph::_1, ph::_2));
	}
	//TODO what if e is set?
}

void ui::refresh() {
	parent.refresh();
	//input.refresh();
}

void ui::stop() {
	input.stop();
	signal_set.cancel();
	window1.stop();
}

      window& ui::get_selected_window()       { return window1; }
const window& ui::get_selected_window() const { return window1; }

void ui::set_selected_channel(const std::string& channel_name) {
	const auto first=begin(channel_list), last=end(channel_list);

	auto it=std::find(first, last, channel_name);

	if(it==last) {
		throw std::runtime_error("channel is missing from channel list!");
	}

	channel_list.selected_idx(std::distance(first, it));
	channel_list.refresh();
}

void ui::set_input(const std::string& str) {
	//input.set_value(str);
	//input.refresh();
}

} //namespace ui_impl
