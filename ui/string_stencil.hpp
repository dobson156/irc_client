#ifndef STRING_STENCIL_HPP
#define STRING_STENCIL_HPP

#include "basic.hpp"

#include <string>

namespace cons {

class string_stencil {
	int calculate_y(int max_width, std::string::size_type chars) const;
	int calculate_x(int max_width, int chars) const;
public:
//types
	using value_type=std::string;
//members
	string_stencil()                     =default;
	string_stencil(const string_stencil&)=default;
//STENCIL INTERFACE
	int required_y(int max_width, std::string::size_type chars) const;
	
	point write_to(frame& frame_, const std::string& str) const;

/*
	template<typename FrameType>
	point write_to(FrameType& frame_, const std::string& str) const {
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
	*/

}; //class string_stencil

} //namespace cons

#endif //STRING_STENCIL_HPP
