#include <boost/signals/connection.hpp>

class unique_connection {
	boost::signals::connection connection;
public:
	unique_connection()                        =default;
	unique_connection(const unique_connection&)=delete;
	unique_connection(unique_connection&&);
	unique_connection(boost::signals::connection sig);
	~unique_connection();

	unique_connection& operator=(const unique_connection&)=delete;
	unique_connection& operator=(unique_connection&&);
	unique_connection& operator=(boost::signals::connection sig);
}; //class unique_connection
