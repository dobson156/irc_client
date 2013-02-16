#ifndef BOARDERED_HPP
#define BOARDERED_HPP

#include "basic.hpp"

#include <utility> //move
#include <algorithm> //max

namespace cons {

enum class boarders { top, bottom, left, right };
		  
class boardered : public base {
	frame           frame_;
	unique_base_ptr element;
	bool top  { false }, bottom { false }, 
	     left { false }, right  { false };

	point calc_element_dimension() const;
	point calc_element_position()  const;

	void set_flags() { }
	template<typename T, typename... Args>
	void set_flags(T val, Args... args) {
		switch(val)	{
		case boarders::top:    top   =true; break;
		case boarders::bottom: bottom=true; break;
		case boarders::left:   left  =true; break;
		case boarders::right:  right =true; break;
		}
		set_flags(args...);
	}
public:
	template<typename... Boarders>
	boardered(unique_window_ptr handle_, Boarders... b)
	:	frame_ { std::move(handle_) }
	{
		set_flags(b...);
	}

	template<typename ElementType, typename... Args>
	ElementType& emplace_element(Args&&...additional_args) {
		auto element_fill=util::make_unique<ElementType>(
			//TODO make safe for very small sizes
			make_window(
				frame_.get_handle(),
				calc_element_position(),
				calc_element_dimension()
			),
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
//Colours
	void set_background(short bg);
	void set_foreground(short fg);
	short get_background() const;
	short get_foreground() const;
}; //class boarder

} //namespace cons

#endif //BOARDERED_HPP
