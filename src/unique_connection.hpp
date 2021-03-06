
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef UNIQUE_CONNECTION_HPP
#define UNIQUE_CONNECTION_HPP

#include <boost/signals2/connection.hpp>

namespace bsig=boost::signals2;

class unique_connection {
	bsig::connection connection;
public:
	unique_connection()                        =default;
	unique_connection(const unique_connection&)=delete;
	unique_connection(unique_connection&&);
	unique_connection(bsig::connection sig);
	~unique_connection();

	unique_connection& operator=(const unique_connection&)=delete;
	unique_connection& operator=(unique_connection&&);
	unique_connection& operator=(bsig::connection sig);
}; //class unique_connection

#endif //UNIQUE_CONNECTION_HPP
