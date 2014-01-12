
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "stenciled_scrolling_frame.hpp"
#include "basic.hpp"

#include <vector>

namespace cons {

template<typename StencilType>
class stenciled_logger : public base {
public:
	using stencil_type   =StencilType;
	using value_type     =typename stencil_type::value_type;
	using const_reference=const value_type&;
	using container_type =std::vector<value_type>;
	using size_type      =typename container_type::size_type;
	//Const is easier to reason about
	using iterator       =typename container_type::iterator;
	using const_iterator =typename container_type::const_iterator;

	stenciled_logger(unique_window_ptr handle)
	:	frame_ { std::move(handle)                              }
	,	buffer { make_pad( { frame_.get_dimension().x, 1024 } ) }
	{	}

//Mutuate
	template<typename Iter>
	void assign(Iter first, Iter last) {
		values.assign(first, last);
		buffer.assign(first, last);
	}

	void push_back(value_type v) {
		buffer.push_back(v); //append onto the master buffer
		values.push_back(std::move(v));
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

//Overrides
	void clear() override {
		frame_.clear();
		buffer.clear();
		values.clear();
	}

	void refresh() override {
		auto fdim=frame_.get_dimension();
		int src_y=buffer.lines_used();
		int y_pos=std::max(src_y-fdim.y, 0);

		copy(
			buffer.get_underlying_frame(),  //src
			frame_, //dst
			{ 0, y_pos }, //src upper left
			{ 0, 0 },  //dst upper left
			{ fdim.x, std::min(src_y, fdim.y) } //size
		);

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
		auto p=frame_.reset(std::move(handle));
		buffer.reset(make_pad( { frame_.get_dimension().x, 1024 }));
		buffer.assign(values.begin(), values.end());
		return p;
	}
//Colour
	void set_background();
	void set_foreground();
	short get_background() const { return frame_.get_background(); }
	short get_foreground() const { return frame_.get_foreground(); }
private:
	frame                                   frame_;
	stenciled_scrolling_frame<stencil_type> buffer;
	container_type                          values;
	stencil_type                            stencil;
}; //class stenciled_logger

} //namespace cons
