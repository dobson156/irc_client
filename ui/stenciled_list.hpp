#ifndef STENCILED_LIST_HPP
#define STENCILED_LIST_HPP

#include "basic.hpp"

#include <vector>
#include <utility> //std::move

namespace cons {

template
<
	typename StencilType, 
	typename ContainerType=std::vector
	<
		typename StencilType::value_type
	>
>
class stenciled_list : public base {
public:
	using stencil_type  =StencilType;
	using container_type=ContainerType;
	using value_type    =typename container_type::value_type;
	using size_type     =typename container_type::size_type;
	//Const is easier to reason about
	using iterator      =typename container_type::iterator;
	using const_iterator=typename container_type::const_iterator;
private:
	frame          frame_;
	container_type values;
	stencil_type   stencil;
public:
	stenciled_list(unique_window_ptr handle)
	:	frame_ { std::move(handle) }
	{	}
//Mutuate
	iterator insert(iterator it, value_type value) {
		//return values.insert(it, std::move(value));
		return values.insert(it, value);
	}
	iterator erase(iterator it) {
		return values.erase(it);
	}
//Observe
	size_type      size()  const { return values.size();   }
	const_iterator begin() const { return values.cbegin(); }
	const_iterator end()   const { return values.cend();   }
	iterator       begin()       { return values.begin();  }
	iterator       end()         { return values.end();    }
//Scroll
	/*
	void up() {
	}
	void down() {

	}
	void focus_on(const_iterator) {

	}
	*/
//Overrides
	void clear()   override {
		frame_.clear();
		values.clear();
	}
	void refresh() override {
		int y_max=0,
		    y_sum=0;

		auto dim=frame_.get_dimension();
		frame_.clear();
		if(!values.empty()) {
			//find the largest entry
			for(const auto& val : values) {
				int y_cur=stencil.required_y(dim.x, val.size());
				y_sum+=y_cur;
				y_max=std::max(y_max, y_cur);

				if(y_sum > dim.y) break; 
			}

			//construct a temporary buffer with size of the large entry
			frame pad { make_pad({ dim.x, y_max }) };

			//point at which to write the next entry
			point last_position { 0, 0 };

			for(const auto& val : values) {
				auto dim_used=stencil.write_to(pad, val);

				if(last_position.y+dim_used.y > dim.y) {
					int i=dim.y-last_position.y;
					copy(pad, frame_, {0,0}, last_position, { dim_used.x, i });
					break;
				}
				else {
					copy(pad, frame_, {0,0}, last_position, dim_used);
					last_position.y+=dim_used.y;
					pad.clear();
				}
			}
		}
		frame_.refresh();
	}
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
//Colour
	void set_background();
	void get_background() const;
	short set_foreground();
	short get_foreground() const;
}; //class stenciled_list

} //namespace cons

#endif // STENCILED_LIST_HPP
