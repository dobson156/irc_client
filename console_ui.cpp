#include "console_ui.hpp"

#include "ui/console.hpp"

namespace ui_impl {

ui::ui()  
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
	channel_list.insert(channel_list.begin(), "channel");
	message_list.insert(message_list.begin(), "message");
	parent.refresh();
}

void ui::run() {
	event_loop();
}

//TODO: note to self, what happens if you change this on THIS callback?
//perhaps use the event system
void ui::reg_on_text_input(std::function<void(std::string)> action){
	on_text_input=std::move(action);
}

void ui::reg_on_special_char(std::function<void(int)> action){ 
	on_special_char=std::move(action);
}

//setters
void ui::set_title(const std::string& text){ 
	title.set_content(text);
	title.refresh();
}

/*
** this handles the user input
*/
void ui::event_loop() {
	char t;
	while(true) { //TODO stopping condition
		t=input.get_char();
		if(t=='\n') { //new text input
			if(on_text_input) {
				on_text_input(input.get_value());
			}
		}
		else if(false) { //TODO: is special 
			if(on_special_char) {
				on_special_char(t);
			}
		}
		//TODO: do a non blocking get_char 
		//so that reading a big blocks of text is fast
		io_service.run();
	}
}


} //namespace ui_impl
