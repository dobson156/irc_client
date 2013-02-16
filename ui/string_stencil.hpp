#ifndef STRING_STENCIL_HPP
#define STRING_STENCIL_HPP

#include "basic.hpp"

#include <fstream>

#include <string>
#include <algorithm> // min

namespace cons {

class string_stencil {
	int calculate_y(int max_width, std::string::size_type chars) const {
		return chars/max_width+1;
	}
	int calculate_x(int max_width, int chars) const {
		return std::min(max_width, chars);
	}
public:
//types
	using value_type=std::string;
//members
	string_stencil()                     =default;
	string_stencil(const string_stencil&)=default;

//STENCIL INTERFACE
	int required_y(int max_width, std::string::size_type chars) const {
		return chars/max_width+1;
	}
	/*
	point required_dimensions(const frame& frame_, 
	                          const std::string& str) const {
		int max_x=frame_.get_dimension().x;
		return { 
			calculate_x(max_x, str.size()),
			calculate_y(max_x, str.size())
		};
	}
	*/

	point write_to(frame& frame_, const std::string& str) const {
		auto dim  =frame_.get_dimension();
		auto chars=str.size();
		auto rows =required_y(dim.x, chars);


		std::ofstream dbg { "hhh", std::ios_base::app };
		for(int i=0; i!=rows; ++i) {
			frame_.write(
				{0,i}, 
				str.c_str() + i*dim.x,
				std::min<int>(dim.x, chars)
			);
			chars-=dim.x;
			dbg << i << "  ";
		}
		dbg << std::endl;
		return { dim.x, rows };
	}
}; //class string_stencil

}

#endif //STRING_STENCIL_HPP
