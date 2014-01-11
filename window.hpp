#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <boost/asio/io_service.hpp>
#include <boost/asio/system_timer.hpp>

#include "ui/console.hpp"
#include "ui/stenciled_logger.hpp"

#include "message.hpp"
#include "message_stencil.hpp"
#include "unique_connection.hpp"

class buffer;

/*
** This a user defined UI element, it will in essence be a view onto a single
** buffer, there maybe multiple view
*/
namespace ui_impl {

using namespace cons;

class window : public base {
//Member types
	using anchor_top              =anchor_view<anchors::top>;
	using anchor_bottom           =anchor_view<anchors::bottom>;
	using text_box                =stenciled_frame<string_stencil>;
	using msg_list                =stenciled_logger<message_stencil>;
//Member variables
	unique_connection              con_topic_change, 
	                               con_new_msg;
	std::reference_wrapper<buffer> buf_;
	boost::asio::system_timer      timer; //for updating the clock
//UI elements
	anchor_top                     title_anchor;
	anchor_bottom&                 status_anchor;
	text_box&                      title;
	text_box&                      status;
	msg_list&                      message_list;

//helpers
	//resets the internal routing for the buffer stored in buf_
	void retarget_buffer();
	void set_status();
	void timer_set_status(const boost::system::error_code&);
public:
	window(unique_window_ptr        handle, 
	       boost::asio::io_service& io_service, //for async_input_box
	       buffer&                  buf);
	buffer&       get_buffer();
	const buffer& get_buffer() const;
	void set_target_buffer(buffer& buf);

//Overrides :- all of these will be forwarded on to the top level anchor
	void clear()                                      override;
	void refresh()                                    override;
	void set_position(const point& position)          override;
	void set_dimension(const point& dimension)        override;
	point get_position()  const                       override;
	point get_dimension() const                       override;
	unique_window_ptr reset(unique_window_ptr handle) override;
}; //class window

} //namespace ui_impl

using ui_impl::window;

#endif //WINDOW_HPP
