#ifndef CONSOLE_UI_HPP
#define CONSOLE_UI_HPP
#define BOOST_RESULT_OF_USE_DECLTYPE

#include "message.hpp"
#include "message_stencil.hpp"

#include "ui/console.hpp"

#include <boost/asio/io_service.hpp>

#include <string>
#include <memory>
#include <functional>


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
//GUI elements
	anchor_top          parent;
	anchor_bottom&      input_anchor;
	anchor_bottom&      status_anchor;
	anchor_left&        channel_anchor;
	bordered&           channel_border;
	text_list&          channel_list;
	msg_list&           message_list;
	text_box&           title;
	text_box&           status;
	async_input_box&    input;
//Callbacks
	boost::asio::io_service *io_service;
	action_str               on_text_input;
	action_int               on_special_char;

	void refresh();
public:
	ui(boost::asio::io_service& io_service);
	void stop();
//event handlers	
	void reg_on_text_input(std::function<void(std::string)> action);
	void reg_on_special_char(std::function<void(int)> action);
//setters
	void set_input(const std::string& str);

	void set_title(const std::string& title);

	void append_message(const message_p& msg);

	template<typename Iterator> //*Iterator==unique_ptr<message>
	void assign_messages(Iterator first, Iterator last);

	template<typename Iterator> //*Iterator==std::string
	void set_channels(Iterator first, Iterator last);

	template<typename Iterator> //*Iterator==std::string
	void set_users(Iterator first, Iterator last);
};


//IMPL
template<typename Iterator> //*Iterator==shared_ptr<message>
void ui::assign_messages(Iterator first, Iterator last) {
	message_list.clear();
	std::copy(first, last, std::inserter(message_list, message_list.end()));
	message_list.refresh();
}

template<typename Iterator>
void ui::set_users(Iterator first, Iterator last) {
	assert(false); //depricated
	std::copy(first, last, std::inserter(message_list, message_list.end()));
}
template<typename Iterator>
void ui::set_channels(Iterator first, Iterator last) {
	//TODO make an iterate inserter
	channel_list.clear();
	std::copy(first, last, std::inserter(channel_list, channel_list.begin()));
	channel_list.refresh();
}

} //namespace ui_impl

using ui_impl::ui;

#endif //CONSOLE_UI_HPP
