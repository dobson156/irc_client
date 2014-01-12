
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "buffer.hpp"
#include "console_ui.hpp"

#include "ui/console.hpp"

#include <csignal>
#include <sys/ioctl.h>
#include <ncurses.h>

ui_impl::ui *term { nullptr };

//It's UB for a handler to not have C linkage
extern "C" {
	void handle_sigwinch(int sig) {
		assert(sig==SIGWINCH);
		if(term) term->async_redraw();
	}
}

namespace ui_impl {

ui::ui(boost::asio::io_service& io_service_, buffer& buffer                    )  
:	parent         { make_window(), 16                                         }	
,	input_anchor   ( parent.emplace_fill<anchor_bottom>(1)                     )
,	channel_border ( parent.emplace_anchor<bordered>(borders::right)           )
,	channel_list   ( channel_border.emplace_element<text_list>()               ) 
,	input          ( input_anchor.emplace_anchor<async_input_box>(io_service_) )
,	window1        ( input_anchor.emplace_fill<window>(io_service_, buffer)    )
,	io_service     { &io_service_                                              }
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
		[&](cons::ctrl_char ch) {
			on_ctrl_char(ch);
		}
	);

	::term=this;
	std::signal(SIGWINCH, handle_sigwinch);

	refresh();
	input.refresh();
}

void ui::redraw() {
	endwin();
	winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	resizeterm(w.ws_row,w.ws_col);
	parent.reset(make_window());
	refresh();
}

void ui::async_redraw() { io_service->post([this]{ redraw(); }); }

void ui::refresh() {
	parent.refresh();
	//input.refresh();
} 

void ui::stop() {
	input.stop();
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
	input.set_value(str);
	input.refresh();
}

} //namespace ui_impl
