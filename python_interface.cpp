
#include <Python.h>

#include "irc/connection.hpp"

#include <boost/python.hpp>
#include <boost/signals2/signal.hpp>

#include <iostream>

namespace py=boost::python;

struct settings {
public:
	std::string nickname, username;

	void set_nickname(const std::string& s) { nickname=s; }
	void set_username(const std::string& s) { username=s; }
	std::string get_nickname() const { return nickname; }
	std::string get_username() const { return username; }
};

std::vector<std::function<void(void)>> vf;


void f(py::object &callback) {
	vf.push_back(callback);
}


BOOST_PYTHON_MODULE(test_mod) {
	py::def("f", f);
}

class python_interface {
	using sig_con=boost::signals2::signal<void(irc::connection& con)>;
	std::string             python_file         { "t.py" };
	sig_con                 p_on_new_connection;
	boost::python::object   main_module,
	                        main_namespace;

public:
	python_interface(); 
	void reload_conf();
	void p_connect_new_connection(const py::object& obj);
	void accept_new_connection(irc::connection& connection);
	const std::string& get_python_file() const;
};

BOOST_PYTHON_MODULE(irc_client) {
	py::object p_irc=py::class_<python_interface, boost::noncopyable>("irc_t", py::no_init)
		.def("connect_on_new_connection", &python_interface::p_connect_new_connection)
		.def("get_python_file",           &python_interface::get_python_file)
		;
}

python_interface::python_interface() {
	Py_Initialize();
	initirc_client();

	main_module    =py::import("__main__");
	main_namespace =main_module.attr("__dict__");
	py::object icm =py::import("irc_client");
	auto icn       =icm.attr("__dict__");

	main_namespace["irc"]=py::ptr(this);

	reload_conf();
}
void python_interface:reload_conf() {
	auto result=py::exec_file(python_file.c_str(), main_namespace);
}
void python_interface::p_connect_new_connection(const py::object& obj) {
	p_on_new_connection.connect(obj);
}
void python_interface::accept_new_connection(irc::connection& connection) {
	p_on_new_connection(connection);
}
const python_interface::std::string& get_python_file() const { 
	return python_file; 
}

int main() {
	try {
		python_interface pi;
		/*
		Py_Initialize();

		py::object settings_class=py::class_<settings>("settings")
			.add_property("nickname", &settings::get_nickname, &settings::set_nickname)
			.add_property("username", &settings::get_username, &settings::set_username)
			;

		inittest_mod();


		auto main_module    =py::import("__main__");
		auto main_namespace =main_module.attr("__dict__");

		main_namespace["settings"]=settings_class;
		main_namespace["default_settings"]=settings_class();


		auto result=py::exec_file("t.py", main_namespace);
		
		settings& returned_settings=py::extract<settings&>(
			main_namespace["default_settings"]);

		for(const auto& a : vf) a();

		std::cout << "username: " << returned_settings.get_username() << '\n'
		          << "nickname: " << returned_settings.get_nickname() << '\n';
		*/
	}
	catch(const py::error_already_set&) {
		PyErr_Print();
		std::cerr << "python error\n";
		return EXIT_FAILURE;
	}
}


