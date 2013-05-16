#ifndef STENCILED_LIST_HPP
#define STENCILED_LIST_HPP

#include "basic.hpp"

#include <cassert>
#include <utility> //std::move
#include <vector>
#include <deque>

namespace cons {


template<typename StencilType>
frame message_to_pad(const std::string& str, point max) {
	frame frm { make_pad(max) };
	StencilType ss;
	int y_used=ss.write_to(frm, str).y;
	frame cut { make_pad({ max.x, y_used }) };
	copy(frm, cut, {0,0}, {0,0}, cut.get_dimension());
	return cut;
}

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
	size_type      selected { 0 };
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
	bool           empty()  const { return values.empty();  }
	size_type      size()   const { return values.size();   }
	const_iterator begin()  const { return values.cbegin(); }
	const_iterator end()    const { return values.cend();   }
	const_iterator cbegin() const { return values.cbegin(); }
	const_iterator cend()   const { return values.cend();   }
	iterator       begin()        { return values.begin();  }
	iterator       end()          { return values.end();    }
	size_type      selected_idx() const { return selected;  } 
	void           selected_idx(size_type n){ selected=n;   } 
//Overrides
	void clear()   override {
		frame_.clear();
		values.clear();
	}
	void refresh() override {
		frame_.clear();
		if(!empty()) {
			auto selected=selected_idx();
			const auto dime=get_dimension();


			//without this, it is bugged
			auto max_idx=size()-1;
			if(selected > max_idx) {
				selected=max_idx;
			}


			//iterator to selected position
			const_iterator top, bottom, selected_it=cbegin();
			std::advance(selected_it, selected);
			top=bottom=selected_it;


			//This holds all the compoent pads, deque so we can add to front and back
			std::deque<cons::frame> message_frames;
			
			//the amount of verticle space used by the pads so far
			int y_used=0;

			//idx of current top and bottom elements
			//TODO: use iterators


			//start with the selected message
			message_frames.push_back(message_to_pad<stencil_type>(*selected_it, dime));
			y_used=message_frames.back().get_dimension().y;




			//true whilst there are still more upper & lower values
			//when both are false loop breaks
			bool tdo=true, bdo=true;

			
			//constructs the vector of frames
			while(y_used < dime.y && (tdo || bdo)) {
				//produce a forerunning message
				if(top!=cbegin()) {
					--top;
					message_frames.push_front(message_to_pad<stencil_type>(*top, dime));
					y_used+=message_frames.front().get_dimension().y;
				}
				else tdo=false;

				//produce a trailng message
				if(bdo) { //stops incrementing ptr once obsolete
					++bottom;
					if(bottom!=cend() && y_used < dime.y) {
						message_frames.push_back(message_to_pad<stencil_type>(*bottom, dime));
						y_used+=message_frames.back().get_dimension().y;
					}
					else bdo=false;
				}
			}

			//compile vector into single frame
			int y=0;
			for(auto& mfrm : message_frames) {
				auto dime=mfrm.get_dimension();
				copy(mfrm, frame_, {0,0}, {0,y}, dime);
				y+=dime.y;
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
