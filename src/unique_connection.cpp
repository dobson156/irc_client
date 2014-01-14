
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "unique_connection.hpp"
#include <algorithm>

unique_connection::unique_connection(unique_connection&& other) {
	using std::swap;
	swap(other.connection, connection);
}

unique_connection::unique_connection(bsig::connection sig) 
:	 connection { std::move(sig) }
{	}

unique_connection::~unique_connection() {
	connection.disconnect(); //no-op if not set
}

unique_connection& unique_connection::operator=(unique_connection&& other) {
	using std::swap;
	swap(other.connection, connection);
	return *this;
}

unique_connection& unique_connection::operator=(bsig::connection sig) {
	connection.disconnect();
	connection=std::move(sig);
	return *this;
}
