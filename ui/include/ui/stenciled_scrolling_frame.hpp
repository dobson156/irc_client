
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SCROLLING_BUFFER_HPP
#define SCROLLING_BUFFER_HPP

#include "window.hpp"
#include "scrolling_frame.hpp"
#include "minimal_frame_wrapper.hpp"

#include <iterator>

namespace cons {

template<typename StencilType>
class stenciled_scrolling_frame : public minimal_frame_wrapper {
public:
	using stencil_type   =StencilType;
	using value_type     =typename stencil_type::value_type;
	using const_reference=const value_type&;

	stenciled_scrolling_frame(unique_window_ptr handle)
	:	minimal_frame_wrapper { std::move(handle) }
	{	}

	template<typename Iter>
	void assign(Iter first, Iter last) {
		std::copy(first, last, std::back_inserter(*this));
	}

	void push_back(const_reference v) {
		frame& frame_=get_underlying_frame();
		scrolling_frame sf{ frame_, y_usage };
	 	auto pos=stencil.write_to(sf, v);

		frame_.set_underlined(false);
		frame_.set_blinking(false);
		frame_.set_dim(false);
		frame_.set_bold(false);
		//TODO: is this equivelant to set_colour(frame.get_colour_pair())
		frame_.set_colour({ -1, -1 });

		y_usage=std::min(y_usage+pos.y, get_frame().get_dimension().y);
	}

	void clear() override { 
		get_frame().clear();
		y_usage=0;
	}

	int lines_used() const { return y_usage; }
	frame&       get_underlying_frame()       { return get_frame(); }
	const frame& get_underlying_frame() const { return get_frame(); }
private:
	stencil_type stencil;
	int          y_usage { 0 };
}; //class stenciled_scrolling_frame

} //namespace cons

#endif //SCROLLING_BUFFER_HPP
