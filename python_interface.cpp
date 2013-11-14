#include <Python.h>
#include <boost/python.hpp>

namespace py=boost::python;

class settings {
	std::string nickname, username;
public:
	void set_nickname(const std::string& s);
	void set_username(const std::string& s);
	std::string get_nickname() const;
	std::string get_username() const;
};


BOOST_PYTHON_MODULE(irc_client) {

	py::object settings_class=py::class_<settings>("settings")
		.add_static_property("username", &settings::get_username, &settings::set_username)
		.add_static_property("nickname", &settings::get_nickname, &settings::set_nickname)
		;
}

int main() {
	Py_Initialize();

	object main_module = import("__main__");
	object main_namespace = main_module.attr("__dict__");

	auto result=exec_file("config.py", main_namespace);
	
}
