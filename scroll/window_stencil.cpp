#include "window_stencil.hpp"
#include "../ui/console.hpp"

#include "window.hpp"

void  window_stencil::write_to(cons::frame frm, window& window) const {
	// Turn scrolling on, we are going to be sloppy
	auto selected=window.get_selected_idx();
	auto size=window.size();
	auto dime=frm.get_dimension();
	cons::frame pad { cons::make_pad(dime) };
}
