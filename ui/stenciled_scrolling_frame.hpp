
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SCROLLING_BUFFER_HPP
#define SCROLLING_BUFFER_HPP

#include "scrolling_frame.hpp"
#include "basic.hpp"

#include <iterator>

namespace cons {

template<typename StencilType>
class stenciled_scrolling_frame : public base {
public:
	using stencil_type   =StencilType;
	using value_type     =typename stencil_type::value_type;
	using const_reference=const value_type&;

	stenciled_scrolling_frame(unique_window_ptr handle)
	:	frame_ { std::move(handle) }
	{	}

	template<typename Iter>
	void assign(Iter first, Iter last) {
		std::copy(first, last, std::back_inserter(*this));
	}

	void push_back(const_reference v) {
		scrolling_frame sf{ frame_, y_usage };
	 	auto pos=stencil.write_to(sf, v);

		get_underlying_frame().set_underlined(false);
		get_underlying_frame().set_blinking(false);
		get_underlying_frame().set_dim(false);
		get_underlying_frame().set_bold(false);
		//TODO: is this equivelant to set_colour(frame.get_colour_pair())
		get_underlying_frame().set_colour({ -1, -1 });

		y_usage=std::min(y_usage+pos.y, frame_.get_dimension().y);
	}

	void refresh() override { frame_.refresh(); }

	void clear() override { 
		frame_.clear();
		y_usage=0;
	}

	int lines_used() const { return y_usage; }

	void set_position(const point& position) override { 
		frame_.set_position(position); 
	}
	void set_dimension(const point& dimension) override { 
		frame_.set_dimension(dimension); 
	}
	point get_position()  const override { return frame_.get_position();  }
	point get_dimension() const override { return frame_.get_dimension(); }
	unique_window_ptr reset(unique_window_ptr handle) override {
		return frame_.reset(std::move(handle));
	}
	frame&       get_underlying_frame()       { return frame_; }
	const frame& get_underlying_frame() const { return frame_; }
private:
	frame        frame_;
	stencil_type stencil;
	int          y_usage { 0 };
}; //class stenciled_scrolling_frame

} //namespace cons

#endif //SCROLLING_BUFFER_HPP
