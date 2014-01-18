
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "input_manager.hpp"

#include <cassert>
#include <stdexcept> //std::runtime_error

namespace cons {

buffer_off::buffer_off(buffer_off&& rhs) 
:	owning  { true        }
,	old_tio ( rhs.old_tio )
{
	rhs.owning=false;
}

buffer_off::buffer_off() 
:	owning { true }
{
	termios new_tio;
	tcgetattr(STDIN_FILENO, &old_tio);
	new_tio=old_tio;
	new_tio.c_lflag &=(~ICANON & ~ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

buffer_off::~buffer_off() {
	if(owning) tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}


input_manager::input_manager(input_manager&& rhs)
:	buffer     { std::move(rhs.buffer)   }
,	buff_off   { std::move(rhs.buff_off) }
,	file       { std::move(rhs.file)     }
,	io_service ( rhs.io_service          )
{	}

input_manager::input_manager(boost::asio::io_service& io_service_, int fd_)
:	fd         { fd_              }
,	buffer     ( 512, '\0'        )
,	file       { io_service_, fd_ }
,	io_service { &io_service_     } 
{	}

input_manager::~input_manager() {
	::close(fd);
}

void input_manager::async_read(std::function<void(std::string)> f_) {
	f=std::move(f_);	
	set();
}

void input_manager::stop() {
	file.close();
}

void input_manager::handle_read_complete(const boost::system::error_code& ec, std::size_t n) {
	if(!ec && f) {
		f(std::string(buffer.begin(), buffer.begin() + n));
	} //else this is the end of program
}
void input_manager::set() {
	file.async_read_some(
		boost::asio::buffer(buffer, buffer.size()),
		std::bind(&input_manager::handle_read_complete, this, ph::_1, ph::_2)
	);
}

input_manager make_tty_manager(boost::asio::io_service& io_service) {
	const char *ttyn=ttyname(0);
	int fd=::open(ttyn, O_RDONLY);
	if(fd==-1) {
		throw std::runtime_error("Unable to read from tty");
	}
	return { io_service, fd };
}



} //cons
