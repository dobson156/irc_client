#include "unique_connection.hpp"
#include <algorithm>

unique_connection::unique_connection(unique_connection&& other) {
	using std::swap;
	swap(other.connection, connection);
}

unique_connection::unique_connection(boost::signals::connection sig) 
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

unique_connection& unique_connection::operator=(
				boost::signals::connection sig) {
	connection.disconnect();
	connection=std::move(sig);
	return *this;
}
