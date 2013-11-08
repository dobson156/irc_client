#include "buffer.hpp"
#include "console_ui.hpp"

#include "ui/console.hpp"

namespace ui_impl {

ui::ui(boost::asio::io_service& io_service_, buffer& buffer           )  
:	parent         { make_window(), 10                                }	
,	channel_border (parent.emplace_anchor<bordered>(borders::right)   )
,	channel_list   (channel_border.emplace_element<text_list>()       ) 
,	window1        ( parent.emplace_fill<window>(io_service_, buffer) )
,	io_service     { &io_service_                                     }
{	
	channel_border.set_background(COLOR_CYAN);
	channel_border.set_foreground(COLOR_WHITE);
	channel_list.highlight_selected(true);
	refresh();
	//title.set_background(COLOR_CYAN);
	//status.set_background(COLOR_CYAN);

	/*
	input.connect_on_grow(
		[&](const point& pt) {
			input_anchor.set_partition(pt.y);
			input_anchor.refresh();
			return true;
		}
	);

	nput.connect_on_ctrl_char(
		[&](cons::ctrl_char ch) {
			on_ctrl_char(ch);
		}
	);

	message_list.selected_idx(1000);
	*/
	refresh();
	//input.refresh();
}

void ui::refresh() {
	parent.refresh();
	//input.refresh();
} 

void ui::stop() {
	//input.stop();
}

//TODO: note to self, what happens if you change this on THIS callback?
//perhaps use the event system
void ui::reg_on_text_input(std::function<void(std::string)> action){
	/*
	on_text_input=std::move(action);
	input.connect_on_input(on_text_input);
	*/
}

      window& ui::get_selected_window()       { return window1; }
const window& ui::get_selected_window() const { return window1; }

void ui::set_input(const std::string& str) {
	/*
	input.set_value(str);
	input.refresh();
	*/
}

} //namespace ui_impl
