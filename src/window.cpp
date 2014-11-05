
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "util.hpp"
#include "window.hpp"
#include "buffer.hpp"

#include <limits>

namespace ui_impl {

window::window(unique_window_ptr        handle,
               boost::asio::io_service& io_service,
               buffer&                  buf)

:	base          ( /*TODO: gcc 4.8 allows {} braces here */         )
,	buf_          { buf                                              }
,	timer         { io_service                                       }
,	title_anchor  { std::move(handle), 1                             }
,	status_anchor ( title_anchor.emplace_fill<anchor_bottom>(1)      ) //check
,	title         ( title_anchor.emplace_anchor<text_box>("title")   )
,	status        ( status_anchor.emplace_anchor<text_box>("status") )
,	message_list  ( status_anchor.emplace_fill<msg_list>()           )
{
	title.set_background(COLOR_BLUE);
	title.set_foreground(COLOR_WHITE);
	status.set_background(COLOR_BLUE);
	status.set_foreground(COLOR_WHITE);

	//message_list.selected_idx(std::numeric_limits<msg_list::size_type>::max());

	retarget_buffer();

	timer.expires_at(util::get_next_min());
	timer.async_wait(std::bind(&window::timer_set_status, this, ph::_1));
}

void window::set_status() {
	auto& b=get_buffer();
	status.set_content(util::time_to_string() + " " + b.get_name());
	status.refresh();
}

void window::timer_set_status(const boost::system::error_code& ec) {
	if(!ec) {
		set_status();
		timer.expires_at(util::get_next_min()); //resubscribe
		timer.async_wait(std::bind(&window::timer_set_status, this, ph::_1));
	}
}

void window::retarget_buffer() {
	auto& buff=get_buffer();

	message_list.clear();
	message_list.assign(buff.messages_begin(), buff.messages_end());

	set_status();

	title.set_content(get_buffer().get_topic());

	con_topic_change=buff.connect_on_topic_change(
		[&](buffer& b, const std::string& new_title) {
			assert(&b==&get_buffer());

			title.set_content(new_title);
			title.refresh();
		}
	);

	con_new_msg=buff.connect_on_new_message(
		[&](buffer& b, const std::shared_ptr<message>& msg) {
			assert(&b==&get_buffer());

			message_list.push_back(msg);
			message_list.refresh();
		}
	);
	refresh();
}

void window::set_target_buffer(buffer& buf) {
	buf_=buf;
	retarget_buffer();
}

buffer&       window::get_buffer()       { return buf_; }
const buffer& window::get_buffer() const { return buf_; }

void window::scroll_up() { message_list.scroll_up(); message_list.refresh(); }
void window::scroll_down() { message_list.scroll_down(); message_list.refresh(); }

void window::stop() { timer.cancel(); }

//Overrides :- all of these will be forwarded on to the top level anchor
void window::clear()   { title_anchor.clear(); }
void window::refresh() { title_anchor.refresh(); }
void window::set_position(const point& position) {
	title_anchor.set_position(position);
}
void window::set_dimension(const point& dimension) {
	title_anchor.set_dimension(dimension);
}
point window::get_position()  const { return title_anchor.get_position(); }
point window::get_dimension() const { return title_anchor.get_dimension(); }
unique_window_ptr window::reset(unique_window_ptr handle) {
	return title_anchor.reset(std::move(handle));
}

} //namespace ui_impl
