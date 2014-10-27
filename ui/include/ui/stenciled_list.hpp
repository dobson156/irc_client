
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef STENCILED_LIST_HPP
#define STENCILED_LIST_HPP

#include "basic.hpp"
#include "minimal_frame_wrapper.hpp"

#include <boost/optional.hpp>

#include <utility> //std::move 
#include <vector> 
#include <deque>

namespace cons {

//TODO pass in stencil type
template<typename StencilType>
frame message_to_pad(StencilType ss, const typename StencilType::value_type& str, point max) {
	frame frm { make_pad(max) };
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
class stenciled_list : public minimal_frame_wrapper {
public:
	using stencil_type  =StencilType;
	using container_type=ContainerType;
	using value_type    =typename container_type::value_type;
	using size_type     =typename container_type::size_type;
	//Const is easier to reason about
	using iterator      =typename container_type::iterator;
	using const_iterator=typename container_type::const_iterator;
private:
	container_type               values;
	stencil_type                 stencil;
	size_type                    selected { 0 };

	//if present then the hilighted colour is that, else not highlighted
	boost::optional<colour_pair> selected_colour;
public:
	stenciled_list(unique_window_ptr handle)
	:	minimal_frame_wrapper { std::move(handle) }
	{	}
//Mutuate
	iterator insert(iterator it, value_type value) {
		return values.insert(it, std::move(value));
	}

	template<typename Iter>
	iterator insert(iterator pos, Iter first, Iter last) {
		//TODO: when GCC has fixed insert just directly return it's value
		auto dist=std::distance(begin(), pos);
		values.insert(pos, first, last);
		return values.begin() + dist;
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

	void selected_idx(size_type n) { 
		//currently selected_idx can be largerr than size() 
		//to indicate "stay at end" of set
		//CONS_ASSERT(n < values.size(), "new value is out of range");
		selected=n;
	} 

	const value_type& selected_value() const {
		using std::begin;
		auto it=begin(values);
		std::advance(it, selected_idx());
		return *it;
	}
//Overrides
	void clear() override {
		get_frame().clear();
		values.clear();
	}

	/*
	** Okay, this function is hideous, but it works
	*/
	void refresh() override {
		get_frame().clear();
		if(!empty()) {
			auto selected=selected_idx();
			const auto dime=get_dimension();


			//ensure inrange
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

			//start with the selected message
			message_frames.push_back(message_to_pad<stencil_type>(stencil, *selected_it, dime));
			if(selected_colour) {
				//TODO: this must be optimised, by just passing the colour_piar
				message_frames.back().set_background(selected_colour->get_background());
				message_frames.back().set_foreground(selected_colour->get_foreground());
			}

			y_used=message_frames.back().get_dimension().y;
			


			//true whilst there are still more upper & lower values
			//when both are false loop breaks
			bool tdo=true, bdo=true;

			
			//constructs the vector of frames
			while(y_used < dime.y && (tdo || bdo)) {
				//produce a forerunning message
				if(top!=cbegin()) {
					--top;
					message_frames.push_front(message_to_pad<stencil_type>(stencil, *top, dime));
					y_used+=message_frames.front().get_dimension().y;
				}
				else tdo=false;

				//produce a trailng message
				if(bdo) { //stops incrementing ptr once obsolete
					++bottom;
					if(bottom!=cend() && y_used < dime.y) {
						message_frames.push_back(message_to_pad<stencil_type>(stencil, *bottom, dime));
						y_used+=message_frames.back().get_dimension().y;
					}
					else bdo=false;
				}
			}

			//compile vector into single frame
			int y=0;
			for(auto& mfrm : message_frames) {
				auto dime=mfrm.get_dimension();
				copy(mfrm, get_frame(), {0,0}, {0,y}, dime);
				y+=dime.y;
			}
		}
		get_frame().refresh();
	}
	bool highlight_selected() const { return selected_colour; }
	void highlight_selected(bool v) { 
		if(!selected_colour) {
			//this is a hack, because I don't know how to get what the default
			//actually represents whilst on an areoplane.
			//TODO: fix
			selected_colour=colour_pair { COLOR_BLACK, COLOR_WHITE };
		}
	}
}; //class stenciled_list

} //namespace cons

#endif // STENCILED_LIST_HPP
