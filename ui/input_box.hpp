#ifndef INPUT_BOX_HPP
#define INPUT_BOX_HPP

#include <fstream>

#include "basic.hpp"
#include "string_stencil.hpp"

namespace cons {

enum class { no_input, new_line, new_char, control_char };

class input_box : public base {
public:
	using grow_cb =std::function<bool(point)>;
private:
	grow_cb        on_grow;
	frame          frame_;
	std::string    value;
	string_stencil stencil;
public:
	input_box(unique_window_ptr ptr)
	:	frame_ { std::move(ptr) }
	{
		frame_.set_background(COLOR_RED);
		scrollok(frame_.get_handle(), TRUE);
	}

	int get_char() {
		noecho();
		int c=mvwgetch(frame_.get_handle(),0,0);
		//int c=wgetch(frame_.get_handle());
		value+=c;
		return c;
		/*
		int i=wgetch(frame_.get_handle());

		while(std::isprint(i)) {
			value+=i;
			refresh();
			i=wgetch(frame_.get_handle());
		}
		*/
	}

	bool grow(point pt) {
		return (on_grow ? on_grow(pt) : false);
	}

	void refresh() {
		/*
		auto dim=get_dimension();
		int y=stencil.required_y(dim.x, value.size());
		point required { dim.x, y };
		frame_.clear();

		if(y <= dim.y) {
		*/
			//CONS_ASSERT(frame_.get_dimension() == required, "predicate failed");

			std::ofstream dbg { "xx", std::ios_base::app };
			dbg << "rf" << std::endl;

			std::ostringstream oss;
			oss << frame_.get_dimension();

			//TODO: may remove clear
			frame_.clear();
			frame_.write({0,0}, "hi");
			//stencil.write_to(frame_, value + oss.str());
		/*	
		}
		
		else if(!grow(required)) {	//the simple case
		*/
		/*


		}

			CONS_ASSERT(frame_.get_dimension() == required, "predicate failed");
			frame_.clear();
			frame_.write(point{1,0}, "hello");
		}
		else { //the compromise
		*/
		/*
			//std::ofstream dbg { "debug", std::ios_base::app };
			//dbg << "source topleft" <<  point{0, dim.y-y} 
			//	<<"dest size" << frame_.get_dimension() << std::endl;
			CONS_ASSERT(false, "i should not get here");


			frame pad { make_pad(required) };
			stencil.write_to(pad, value);
			frame_.clear();
			copy(pad, 
				 frame_, 
				 {0, required.y-dim.y}, 
				 {0,0}, 
				 frame_.get_dimension()
			);
		}
		*/
		//frame_.clear();
		//frame_.write(point{0,0}, "hello world");
		frame_.refresh();
	}

	void clear() {
		value.clear();
		frame_.clear();
		//TODO remove dynamic sizing from refresh put some where else
		//refresh();
	}

	const std::string& get_value() const {
		return value;
	}

	//watch thread safety
	void reg_on_grow(grow_cb gcb) {
		on_grow=std::move(gcb);
	}


//Overrides
	void set_position(const point& position) override { 
		auto dim=frame_.get_dimension();
		WINDOW *p=wgetparent(frame_.get_handle());

		CONS_ASSERT(p, "parent is null");

		frame_=frame { make_window(p, position, dim) };
		touchwin(frame_.get_handle());
		//frame_.set_position(position); 
	}
	void set_dimension(const point& dimension) override { 
		/*
		auto pos=frame_.get_position();

		WINDOW *p=wgetparent(frame_.get_handle());

		frame_=frame { make_window(p, pos, dimension) };
		touchwin(frame_.get_handle());
		*/
		frame_.set_dimension(dimension); 
		touchwin(frame_.get_handle());
	}
	point get_position()  const override { return frame_.get_position();  }
	point get_dimension() const override { return frame_.get_dimension(); }
//Colour
	void set_background(short bg) { frame_.set_background(bg); }
	void set_foreground(short fg) { frame_.set_foreground(fg); }
	short get_background() const  { return frame_.get_background(); }
	short get_foreground() const  { return frame_.get_foreground(); }
}; //class input_box

} //namespace cons

#endif //INPUT_BOX_HPP
