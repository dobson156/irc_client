#ifndef CONSOLE_UI_HPP
#define CONSOLE_UI_HPP

#include "window.hpp"
#include "message.hpp"

#include "ui/signals.hpp"
#include "ui/console.hpp"

#include <string>
#include <memory>

class buffer;
namespace boost { namespace asio { class io_service; } } 

/*
** the detail namespace allows us to use the namespace cons
** without muddying ns irc
*/
namespace ui_impl {
using namespace cons;

class ui {
	using message_p    =std::shared_ptr<message>;
//Member types
	using anchor_left  =anchor_view<anchors::left>;
	using text_list    =stenciled_list<string_stencil>;

//Member variables
//UI elements
	anchor_left         parent;
	bordered&           channel_border;
	text_list&          channel_list;
	window&             window1; //todo vector of windows

	//async_input_box&    input;

//Callbacks
	boost::asio::io_service *io_service;
	cons::sig_s              on_text_input;
	cons::sig_ctrl_ch        on_ctrl_char;

	void refresh();
public:
	ui(boost::asio::io_service& io_service, buffer& buffer);
	void stop();
//event handlers	

//accessors
	window& get_selected_window();
	const window& get_selected_window() const;
//setters

	void set_selected_channel(const std::string& channel_name);

	void set_input(const std::string& str);

	template<typename Iterator>
	void assign_channels(Iterator first, Iterator last);

	template<typename F> cons::bsig::connection connect_on_ctrl_char(F&& f);
	template<typename F> cons::bsig::connection connect_on_text_input(F&& f);
};


//IMPL
template<typename Iterator>
void ui::assign_channels(Iterator first, Iterator last) {
	channel_list.clear();
	channel_list.insert(channel_list.begin(), first, last);
	channel_list.refresh();
}

template<typename F>
cons::bsig::connection ui::connect_on_ctrl_char(F&& f) {
	return on_ctrl_char.connect(std::forward<F>(f));
}

template<typename F>
cons::bsig::connection ui::connect_on_text_input(F&& f) {
	return window1.connect_on_text_input(std::forward<F>(f));
}

} //namespace ui_impl

using ui_impl::ui;

#endif //CONSOLE_UI_HPP
