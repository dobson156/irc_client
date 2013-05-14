#ifndef WINDOW_STENCIL_HPP
#define WINDOW_STENCIL_HPP

//FORWARD DECL
class window;
namespace cons { 
	class frame;
	class point; 
} //namespace cons


class window_stencil {
public:
	using value_type=window;
	cons::point write_to(cons::frame frm, window& window) const;
}; //class window_stencil


#endif //WINDOW_STENCIL_HPP
