
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef PYTHON_INTERFACE
#define PYTHON_INTERFACE

#include <boost/python/object.hpp>
#include <boost/signals2/signal.hpp>

#include <string>

namespace irc { class session; }

namespace py=boost::python;

class python_interface {
	using sig_sess=boost::signals2::signal<void(irc::session& sess)>;
	using sig_s   =boost::signals2::signal<void(const std::string& s)>;
	std::string    python_file;

	sig_sess       p_on_new_session;

	sig_s          on_connect,
	               on_change_default_nick,
	               on_change_default_username,
	               on_change_default_fullname,
				   on_python_error,
				   on_python_output;

	py::object     main_module,
	               main_namespace;
public:
	python_interface(std::string file_name="t.py"); 
	void exec(const std::string& py_code);
	void reload_conf();
	void p_connect_new_session(py::object obj);
	void p_start_connection(const std::string& con);
	void p_set_default_nick(const std::string& con);
	void p_set_default_username(const std::string& con);
	void p_set_default_fullname(const std::string& con);
	void p_write_error(const std::string& str);
	void p_write_output(const std::string& str);

	void accept_new_session(irc::session& connection);
	//boost python doesn't like references without extra leg work
	std::string get_python_file() const;

	template<typename F> void connect_on_connect(F&& f);
	template<typename F> void connect_on_change_default_nick(F&& f);
	template<typename F> void connect_on_change_default_username(F&& f);
	template<typename F> void connect_on_change_default_fullname(F&& f);
	template<typename F> void connect_on_python_error(F&& f);
	template<typename F> void connect_on_python_output(F&& f);
}; //class python_interface

template<typename F> 
void python_interface::connect_on_connect(F&& f) {
	on_connect.connect(std::forward<F>(f));
}
template<typename F> 
void python_interface::connect_on_change_default_nick(F&& f) {
	on_change_default_nick.connect(std::forward<F>(f));
}
template<typename F> 
void python_interface::connect_on_change_default_username(F&& f) {
	on_change_default_username.connect(std::forward<F>(f));
}
template<typename F> 
void python_interface::connect_on_change_default_fullname(F&& f) {
	on_change_default_fullname.connect(std::forward<F>(f));
}
template<typename F> 
void python_interface::connect_on_python_error(F&& f) {
	on_python_error.connect(std::forward<F>(f));
}
template<typename F> 
void python_interface::connect_on_python_output(F&& f) {
	on_python_output.connect(std::forward<F>(f));
}

#endif //PYTHON_INTERFACE
