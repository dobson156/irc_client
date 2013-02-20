#ifndef BOARDERED_HPP
#define BOARDERED_HPP

#include "basic.hpp"

#include <utility> //move
#include <algorithm> //max

namespace cons {

enum class borders { top, bottom, left, right };
		  
class bordered : public base {
	frame           frame_;
	unique_base_ptr element;
	bool top  { false }, bottom { false }, 
	     left { false }, right  { false };
	//TODO make safe for very small sizes
	point calc_element_dimension() const;
	point calc_element_position()  const;
	unique_window_ptr make_element_window() {
		return make_window(
			frame_.get_handle(),
			calc_element_position(),
			calc_element_dimension()
		);
	}

	void set_flags() { }
	template<typename T, typename... Args>
	void set_flags(T val, Args... args) {
		switch(val)	{
		case borders::top:    top   =true; break;
		case borders::bottom: bottom=true; break;
		case borders::left:   left  =true; break;
		case borders::right:  right =true; break;
		}
		set_flags(args...);
	}
public:
	template<typename... Boarders>
	bordered(unique_window_ptr handle_, Boarders... b)
	:	frame_ { std::move(handle_) }
	{
		set_flags(b...);
	}

	template<typename ElementType, typename... Args>
	ElementType& emplace_element(Args&&...additional_args) {
		auto element_fill=util::make_unique<ElementType>(
			make_element_window(),
			std::forward<Args>(additional_args)...
		);
		auto raw_ptr=element_fill.get();
		element=std::move(element_fill);
		return *raw_ptr;
	}
//Overrides
	void clear()                               override;
	void refresh()                             override;
	void set_position(const point& position)   override;
	void set_dimension(const point& dimension) override;
	point get_position()                 const override;
	point get_dimension()                const override;

	unique_window_ptr reset(unique_window_ptr handle) override;
//Colours
	void set_background(short bg);
	void set_foreground(short fg);
	short get_background() const;
	short get_foreground() const;
}; //class border

} //namespace cons

#endif //BOARDERED_HPP
