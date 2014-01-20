
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef MESSAGE_STENCIL_HPP
#define MESSAGE_STENCIL_HPP

#include "ui/basic.hpp"

#include "message_visitor.hpp"

class message_stencil : public message_vistor {
	cons::output_pane *frame_;
	cons::point last;
public:	
	using value_type=std::shared_ptr<message>;
	cons::point write_to(cons::output_pane&, const value_type&);
	void operator()(message&)      override;
	void operator()(motd_message&) override;

	void operator()(list_message&) override;
	void operator()(text_message&) override;
}; //message_stencil

#endif //MESSAGE_STENCIL_HPP
