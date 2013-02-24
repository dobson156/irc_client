#ifndef STENCILED_FRAME_HPP
#define STENCILED_FRAME_HPP

#include "basic.hpp"

#include <utility> //move

namespace cons {

template<typename StencilType>
class stenciled_frame : public base {
public:
	using stencil_type=StencilType;
	using value_type  =typename stencil_type::value_type;
private:
	frame        frame_;
	value_type   value;
	stencil_type stencil;
public:
	stenciled_frame(unique_window_ptr handle, value_type value_)
	:	frame_ { std::move(handle) }
	,	value  ( std::move(value_) )
	//,	value  ( value_ )
	{	}
	void set_content(value_type value_) {
		value=std::move(value_);
	}
//Overrides
	void clear() override { 
		value=value_type();
		frame_.clear(); 
	}
	void refresh() override { 
		frame_.clear();
		stencil.write_to(frame_, value);
		frame_.refresh(); 
	}
	void set_position(const point& position) override { 
		frame_.set_position(position); 
		//touchwin(frame_.get_handle());
	}
	void set_dimension(const point& dimension) override { 
		frame_.set_dimension(dimension); 
	}
	point get_position()  const override { return frame_.get_position();  }
	point get_dimension() const override { return frame_.get_dimension(); }
	unique_window_ptr reset(unique_window_ptr handle) override {
		return frame_.reset(std::move(handle));
	}
//Colour
	void set_background(short bg) { frame_.set_background(bg); }
	void set_foreground(short fg) { frame_.set_foreground(fg); }
	short get_background() const  { return frame_.get_background(); }
	short get_foreground() const  { return frame_.get_foreground(); }
}; //class stenciled_frame

} //namespace cons

#endif // STENCILED_FRAME_HPP
