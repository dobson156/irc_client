#include "window.hpp"

#include "buffer.hpp"

namespace ui_impl {

window::window(unique_window_ptr        handle, 
               boost::asio::io_service& io_service, 
			   buffer&                  buf) 

:	base          ( /*TODO: gcc 4.8 allows {} braces here */                 ) 

,	buf_          { buf                                                      }

,	title_anchor  { std::move(handle), 1                                     }
,	input_anchor  ( title_anchor.emplace_fill<anchor_bottom>(1)              )
,	status_anchor ( input_anchor.emplace_fill<anchor_bottom>(1)             )

,	title         ( title_anchor.emplace_anchor<text_box>("title")           )
,	input         ( input_anchor.emplace_anchor<async_input_box>(io_service) )
,	status        ( status_anchor.emplace_anchor<text_box>("status")         )
,	message_list  ( status_anchor.emplace_fill<msg_list>()                   )
{
	retarget_buffer();
	title.set_background(COLOR_CYAN);
	status.set_background(COLOR_CYAN);
}


void window::retarget_buffer() {
	auto& buff=buf_.get();
	message_list.clear(); //TODO implement assign
	message_list.insert(
		message_list.begin(), 
		buff.messages_begin(), 
		buff.messages_end()
	);

	con_topic_change=buff.connect_on_topic_change(
		[&](buffer& b, const std::string& new_title) {
			assert(&b==&buf_.get());

			title.set_content(new_title);
			title.refresh();
		}
	);

	con_new_msg=buff.connect_on_new_message(
		[&](buffer& b, const std::shared_ptr<message>& msg) {
			assert(&b==&buf_.get());

			message_list.insert(message_list.end(), msg);
			message_list.refresh();
		}
	);
}

void window::set_target_buffer(buffer& buf) {
	buf_=buf;
	retarget_buffer();
}

buffer&       window::get_buffer()       { return buf_; }
const buffer& window::get_buffer() const { return buf_; }

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
