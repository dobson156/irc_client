#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include "util.hpp"

#include <boost/asio.hpp>

//for open
#include <sys/stat.h> 
#include <fcntl.h>

//for tcgetattr
#include <termios.h>

#include <unistd.h>

#include <cassert>

#include <ctime>
#include <chrono>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <functional>

namespace cons {


//RAII buffer on off selector
class buffer_off {
public:
	buffer_off(buffer_off&& rhs) 
	:	owning  { true        }
	,	old_tio ( rhs.old_tio )
	{
		rhs.owning=false;
	}

	buffer_off() 
	:	owning { true }
	{
		termios new_tio;
		tcgetattr(STDIN_FILENO, &old_tio);
		new_tio=old_tio;
		new_tio.c_lflag &=(~ICANON & ~ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
	}
	~buffer_off() {
		if(owning) tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
	}
private:
	bool    owning  { false };
	termios old_tio;
};




class input_manager {
public:
	input_manager(input_manager&& rhs)
	:	buff_off   { std::move(rhs.buff_off) }
	,	file       { std::move(rhs.file)     }
	,	io_service ( rhs.io_service          )
	{	}

	input_manager(boost::asio::io_service& io_service_, int fd)
	:	buffer     ( 512, '\0'       )
	,	file       { io_service_, fd }
	,	io_service { &io_service_    } 
	{	}

	void async_read(std::function<void(std::string)> f_) {
		f=std::move(f_);	
		set();
	}
private:
	void handle_read_complete(const boost::system::error_code& , std::size_t n) {
		if(f) f(std::string(buffer.begin(), buffer.begin() + n));
	}
	void set() {
		file.async_read_some(
			boost::asio::buffer(buffer, buffer.size()),
			std::bind(&input_manager::handle_read_complete, this, ph::_1, ph::_2)
		);
	}

	std::function<void(std::string)>      f;
	std::vector<char>                     buffer;
	buffer_off                            buff_off;
	//boost::asio::streambuf              sbuf;
	boost::asio::posix::stream_descriptor file;
	boost::asio::io_service              *io_service;
};

inline 
input_manager make_tty_manager(boost::asio::io_service& io_service) {
	const char *ttyn=ttyname(0);
	int fd=::open(ttyn, O_RDONLY);
	if(fd==-1) {
		throw std::runtime_error("Unable to read from tty");
	}
	return { io_service, fd };
}

} //cons

#endif //INPUT_MANAGER_HPP
