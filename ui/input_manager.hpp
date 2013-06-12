#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include "util.hpp"

#include <boost/asio/io_service.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>

//for open
#include <sys/stat.h> 
#include <fcntl.h>

//for tcgetattr
#include <termios.h>

#include <unistd.h>

#include <string>
#include <functional> //std::function

namespace cons {

//RAII buffer on off selector
class buffer_off {
public:
	buffer_off(buffer_off&& rhs);
	buffer_off();
	~buffer_off();
private:
	bool    owning  { false };
	termios old_tio;
}; //class buffer_off


class input_manager {
public:
	input_manager(input_manager&& rhs);
	input_manager(boost::asio::io_service& io_service_, int fd_);
	~input_manager();
	void async_read(std::function<void(std::string)> f_);
	void stop();
private:
	void handle_read_complete(const boost::system::error_code& ec, std::size_t n);
	void set();
//member vars
	int                                   fd;
	std::function<void(std::string)>      f;
	std::vector<char>                     buffer;
	buffer_off                            buff_off;
	boost::asio::posix::stream_descriptor file;
	boost::asio::io_service              *io_service;
}; //class input_manager

input_manager make_tty_manager(boost::asio::io_service& io_service); 

} //cons

#endif //INPUT_MANAGER_HPP
