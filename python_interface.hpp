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
	               on_change_default_username;

	py::object     main_module,
	               main_namespace;
public:
	python_interface(std::string file_name="t.py"); 
	void reload_conf();
	void p_connect_new_session(py::object obj);
	void p_start_connection(const std::string& con);
	void p_set_default_username(const std::string& con);
	void p_set_default_nick(const std::string& con);

	void accept_new_session(irc::session& connection);
	//boost python doesn't like references without extra leg work
	std::string get_python_file() const;

	template<typename F> void connect_on_connect(F&& f);
	template<typename F> void connect_on_change_default_nick(F&& f);
	template<typename F> void connect_on_change_default_username(F&& f);
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

#endif //PYTHON_INTERFACE
