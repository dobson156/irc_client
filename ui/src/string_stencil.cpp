
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "string_stencil.hpp"

#include <algorithm>

namespace cons {

int string_stencil::calculate_y(int max_width, std::string::size_type chars) const {
	return chars/max_width+1;
}
int string_stencil::calculate_x(int max_width, int chars) const {
	return std::min(max_width, chars);
}

//STENCIL INTERFACE
int string_stencil::required_y(int max_width, std::string::size_type chars) const {
	return chars/max_width+1;
}

point string_stencil::write_to(output_pane& frame_, const std::string& str) const {
	auto dim  =frame_.get_dimension();
	auto rows =required_y(dim.x, str.size());

	if(rows <= dim.y) {
		frame_.write({0, 0}, str);
		return { dim.x, rows };
	}
	else {
		frame_.write({0, 0}, str.cbegin(), str.cbegin()+dim.y*dim.x-1);
		return dim;
	}
}

} //namespace cons
