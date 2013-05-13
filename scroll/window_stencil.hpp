#ifndef WINDOW_STENCIL_HPP
#define WINDOW_STENCIL_HPP

//FORWARD DEC
namespace cons { class frame; }
class window;


class window_stencil {
public:
	using value_type=window;
	void write_to(cons::frame frm, window& window) const;
}; //class window_stencil


#endif //WINDOW_STENCIL_HPP
