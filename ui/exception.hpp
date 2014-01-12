
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef UI_EXCEPT_HPP
#define UI_EXCEPT_HPP

#include <exception>
#include <sstream>
#include <string>

namespace cons {

class exception : public std::exception {
	std::string msg, 
	            file,
	            func;
	int         line;
	std::string what_;	
public:
	exception(std::string msg_,  std::string file_, 
	             std::string func_, int line_)
	:	std::exception()
	,	msg  { std::move(msg_)  }
	,	file { std::move(file_) }
	,	func { std::move(func_) }
	,	line { line_            }
	{	
		std::ostringstream oss;
		oss << msg << "  from: " << file << ':' << line << "__" << func;
		what_=oss.str();
	}
	const std::string& get_msg()  const { return msg;  }
	const std::string& get_file() const { return file; }
	int get_line()                const { return line; }
	virtual const char *what() const noexcept override {
		return what_.c_str();
	}
}; //class exception

} //namespace cons

#define CONS_MAKE_EXCEPTION(msg) \
	::cons::exception (msg, __FILE__, __func__, __LINE__)

#endif //UI_EXCEPT_HPP
