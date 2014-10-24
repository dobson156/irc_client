#include "minimal_frame_wrapper.hpp"

namespace cons {
minimal_frame_wrapper::minimal_frame_wrapper(unique_window_ptr handle_)
:	frame_ ( std::move(handle_) )
{	}

frame& minimal_frame_wrapper::get_frame() { return frame_; }
const frame& minimal_frame_wrapper::get_frame() const { return frame_; }

void minimal_frame_wrapper::clear() { get_frame().clear(); }
void minimal_frame_wrapper::refresh() { get_frame().refresh(); }
void minimal_frame_wrapper::set_position(const point& position) { get_frame().set_position(position); }
void minimal_frame_wrapper::set_dimension(const point& dimension) { get_frame().set_dimension(dimension); }
point minimal_frame_wrapper::get_dimension()  const { return get_frame().get_dimension();   }
point minimal_frame_wrapper::get_position()   const { return get_frame().get_position();    }

unique_window_ptr minimal_frame_wrapper::reset(unique_window_ptr nwin) { return get_frame().reset(std::move(nwin)); }

short minimal_frame_wrapper::get_background() const  { return get_frame().get_background(); }
short minimal_frame_wrapper::get_foreground() const  { return get_frame().get_foreground(); }
void minimal_frame_wrapper::set_background(short bg) { get_frame().set_background(bg);      }
void minimal_frame_wrapper::set_foreground(short fg) { get_frame().set_foreground(fg);      }


} //namespace cons
