#ifndef UI_IRC_WINDOW_HPP
#define UI_IRC_WINDOW_HPP

#include <console.hpp>

#include <csignal>

#include <sstream>
#include <thread>
#include <chrono>
#include <iostream>

#include <sys/ioctl.h>
#include <ncurses.h>

class irc_ui_window;
irc_ui_window *current;
void resize_handler(int sig);

using namespace cons;
class irc_ui_window {
//Member types
	using text_list    =stenciled_list<string_stencil>;
	using text_box     =stenciled_frame<string_stencil>;
	using anchor_top   =anchor_view<anchors::top>;
	using anchor_bottom=anchor_view<anchors::bottom>;
	using anchor_left  =anchor_view<anchors::left>;
//Member variables
	anchor_top          parent;
	anchor_bottom&      input_anchor;
	anchor_bottom&      status_anchor;
	anchor_left&        channel_anchor;
	bordered&           channel_border;
	text_list&          channel_list;
	text_list&          message_list;
	text_box&           title;
	text_box&           status;
	input_box&          input;
public: 
	irc_ui_window() 
	:	parent         { make_window(), 1 }	
	,	input_anchor   (parent.emplace_fill<anchor_bottom>(1))
	,	status_anchor  (input_anchor.emplace_fill<anchor_bottom>(1))
	,	channel_anchor (status_anchor.emplace_fill<anchor_left>(20))
	,	channel_border (channel_anchor.emplace_anchor<bordered>(borders::right))
	,	channel_list   (channel_border.emplace_element<text_list>()) 
	,	message_list   (channel_anchor.emplace_fill<text_list>())
	,	title          (parent.emplace_anchor<text_box>("my irc client") )
	,	status         (status_anchor.emplace_anchor<text_box>("channel's status"))
	,	input          (input_anchor.emplace_anchor<input_box>())
	{

		input.reg_on_grow(
			[&](const point& pt) {
				input_anchor.set_partition(pt.y);
				input_anchor.refresh();
				return true;
			}
		);

		current=this;
		std::signal(SIGWINCH, &resize_handler);

		message_list.insert(message_list.begin(), "msg1");

		channel_list.insert(channel_list.begin(), "1");
		channel_border.set_foreground(COLOR_CYAN);

		status.set_background(COLOR_CYAN);
		status.set_foreground(COLOR_BLACK);
		status.set_content("Status Bar");

		title.set_background(COLOR_CYAN);

		parent.refresh();
	}

	void refresh() {
		parent.refresh();
	}

	void rejig() {
		//FROM POSIX
		struct winsize w;
		ioctl(0, TIOCGWINSZ, &w);
		//FROM curses

		resizeterm(w.ws_row,w.ws_col);

	   	endwin();
		parent.reset(make_window());

		std::ostringstream oss;
		oss << parent.get_dimension() << "  COLS  " << COLS << "  LINES   " << LINES;
		message_list.insert(message_list.end(), oss.str());

		message_list.refresh();
		input.refresh();
	}

	void run() {
		int c;
		std::string s;
		while(true) {
			c=input.get_char();
			if(c=='\n') {
				s.clear();
				std::swap(s, input.get_value());
				message_list.insert(message_list.end(), s);
				message_list.refresh();
				input.refresh();
			}
		}
	}
}; //class irc_ui_window

void resize_handler(int sig) {
	CONS_ASSERT(sig==SIGWINCH, "signal is not SIGWINCH");
	current->rejig();
	current->refresh();
}

#endif //UI_IRC_WINDOW_HPP
