#ifndef CONSOLE_UI_HPP
#define CONSOLE_UI_HPP

#include "window.hpp"
#include "message.hpp"
#include "message_stencil.hpp"

#include "ui/signals.hpp"
#include "ui/console.hpp"

#include <boost/asio/io_service.hpp>

#include <string>
#include <memory>
#include <functional>

class buffer;

/*
** the detail namespace allows us to use the namespace cons
** without muddying ns irc
*/
namespace ui_impl {
using namespace cons;

class ui {
	using message_p    =std::shared_ptr<message>;
//Member types
	using text_list    =stenciled_list<string_stencil>;
	using msg_list     =stenciled_list<message_stencil>;
	using text_box     =stenciled_frame<string_stencil>;
	using anchor_top   =anchor_view<anchors::top>;
	using anchor_bottom=anchor_view<anchors::bottom>;
	using anchor_left  =anchor_view<anchors::left>;


	using action_str   =std::function<void(std::string)>;
	using action_int   =std::function<void(int)>;


//Member variables
//UI elements
	anchor_left         parent;
	bordered&           channel_border;
	text_list&          channel_list;
	window&             window1; //todo vector of windows

	/*
	anchor_bottom&      input_anchor;
	anchor_bottom&      status_anchor;
	anchor_left&        channel_anchor;
	bordered&           channel_border;
	text_list&          channel_list;
	msg_list&           message_list;
	text_box&           title;
	text_box&           status;
	async_input_box&    input;
	*/


//Callbacks
	boost::asio::io_service *io_service;
	action_str               on_text_input;
	cons::sig_ctrl_ch        on_ctrl_char;

	void refresh();
public:
	ui(boost::asio::io_service& io_service, buffer& buffer);
	void stop();
//event handlers	
	void reg_on_text_input(std::function<void(std::string)> action);

//accessors
	window& get_selected_window();
	const window& get_selected_window() const;
//setters
	void set_input(const std::string& str);

	//void set_title(const std::string& title);

	//void append_message(const message_p& msg);

	//template<typename Iterator> //*Iterator==unique_ptr<message>
	//void assign_messages(Iterator first, Iterator last);

	template<typename Iterator>
	void assign_channels(Iterator first, Iterator last);

	template<typename F>
	cons::bsig::connection connect_on_ctrl_char(F&& f);
};


//IMPL
	/*
template<typename Iterator> // *Iterator==shared_ptr<message>
void ui::assign_messages(Iterator first, Iterator last) {
	message_list.clear();
	std::copy(first, last, std::inserter(message_list, message_list.end()));
	message_list.refresh();
}
	*/

template<typename Iterator>
void ui::assign_channels(Iterator first, Iterator last) {
	channel_list.clear();
	//std::copy(first, last, std::inserter(channel_list, channel_list.begin()));
	channel_list.insert(channel_list.begin(), first, last);
	channel_list.refresh();
}

template<typename F>
cons::bsig::connection ui::connect_on_ctrl_char(F&& f) {
	return on_ctrl_char.connect(std::forward<F>(f));
}

} //namespace ui_impl

using ui_impl::ui;

#endif //CONSOLE_UI_HPP
