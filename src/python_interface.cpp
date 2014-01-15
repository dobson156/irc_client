
//          Copyright Joseph Dobson 2014
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "python_interface.hpp"

#include "irc/user.hpp"
#include "irc/session.hpp"
#include "irc/channel.hpp"

#include <Python.h>
#include <boost/python.hpp>


//try catch, py catch, get it?
template<typename F>
class py_catch {
	F func;
public:
	template<
		typename F2,
		typename=typename std::enable_if<std::is_same<F2, F>::value>::type
	> 
	py_catch(F2&& f)
	:	func ( std::forward<F2>(f) )
	{	}

	py_catch(py_catch&&)=default;
	py_catch(const py_catch&)=default;
	py_catch& operator=(const py_catch&)=default;
	py_catch& operator=(py_catch&&)=default;

	template<typename... Args> auto operator()(Args&&... args) 
	-> decltype(func(std::forward<Args>(args)...)) {
		try {
			return func(std::forward<Args>(args)...);
		}
		catch(const py::error_already_set&) {
			PyErr_Print();
		}
	}
};
template<typename F>
py_catch<typename std::remove_reference<F>::type> make_py_catch(F&& f) {
	return { std::forward<F>(f) };
}

static void session_connect_on_join_channel(irc::session& sess, const py::object& func) {
	sess.connect_on_join_channel(make_py_catch(
		[&,func](irc::channel& chan) { func(boost::ref(chan)); }));
}
static void session_connect_on_user_notice(irc::session& sess, const py::object& func) {
	sess.connect_on_user_notice(make_py_catch(
		[&,func](irc::user& user, const std::string& msg) { 
			func(boost::ref(user), msg); })); 
}
static void session_connect_on_new_user(irc::session& sess, const py::object& func) {
	sess.connect_on_new_user(make_py_catch(
		[&,func](irc::user& user) { func(boost::ref(user)); }));
}
static void channel_connect_on_message(irc::channel& chan, const py::object& func) {
	chan.connect_on_message(make_py_catch(
		[&,func](irc::channel& chan, irc::user& user, const std::string& str) {
				func(boost::ref(chan), boost::ref(user), str); }));
}
static void channel_connect_on_user_join(irc::channel& chan, const py::object& func) {
	chan.connect_on_user_join(make_py_catch(
		[&,func](irc::channel& chan, irc::user& user) {
				func(boost::ref(chan), boost::ref(user)); }));
}
static void user_connect_on_channel_message(irc::user& user, const py::object& func) {
	user.connect_on_channel_message(make_py_catch(
		[&,func](irc::channel& chan, irc::user& user, const std::string& msg) {
				func(boost::ref(chan), boost::ref(user), msg); }));
}

BOOST_PYTHON_MODULE(irc_client) {
//OBJECT MAPS
	py::object p_chan=py::class_<irc::channel, boost::noncopyable>("channel_t", py::no_init)
		.def("get_name",                &irc::channel::get_name, 
			py::return_value_policy<py::copy_const_reference>())
		.def("send_message",            &irc::channel::async_send_message)
		.def("connect_on_message",      &channel_connect_on_message)
		.def("connect_on_user_join",    &channel_connect_on_user_join)
		;

	py::object p_sess=py::class_<irc::session, boost::noncopyable>("session_t", py::no_init)
		.def("get_nick",                &irc::session::get_nick, 
			py::return_value_policy<py::copy_const_reference>())
		.def("change_nick",             &irc::session::async_change_nick)
		.def("privmsg",                 &irc::session::async_privmsg)
		.def("join",                    &irc::session::async_join)
		.def("connect_on_join_channel", &session_connect_on_join_channel)
		.def("connect_on_user_notice",  &session_connect_on_user_notice)
		.def("connect_on_new_user",     &session_connect_on_new_user)
		;

	py::object p_user=py::class_<irc::user, boost::noncopyable>("user_t", py::no_init)
		.def("get_nick",                   &irc::user::get_nick, 
			py::return_value_policy<py::copy_const_reference>())
		.def("connect_on_channel_message", &user_connect_on_channel_message)
		;

	py::object p_irc=py::class_<python_interface, boost::noncopyable>("irc_t", py::no_init)
		.def("get_python_file",         &python_interface::get_python_file)
		.def("connect",                 &python_interface::p_start_connection)
		.def("connect_on_new_session",  &python_interface::p_connect_new_session)
		.def("default_nick",            &python_interface::p_set_default_nick)
		.def("default_username",        &python_interface::p_set_default_username)
		.def("default_fullname",        &python_interface::p_set_default_fullname)
		.def("write_error",             &python_interface::p_write_error)
		.def("write_output",            &python_interface::p_write_output)
		;
}



python_interface::python_interface(std::string python_file_) 
:	python_file { std::move(python_file_) }
{

	try {
		Py_Initialize();
		initirc_client();

		std::string error_redirect=//is it worth doing this is C++ instead?
			"import sys\n"
			"class redirect_io:\n"
			"	def __init__(self, func_):\n"
			"		self.value=''\n"
			"		self.func=func_\n"
			"	def write(self, txt):\n"
			"		self.value+=txt\n"
			"		i=self.value.find('\\n')\n"
			"		while(i != -1):\n"
			"			v2=self.value[:i]\n"
			"			self.func(v2)\n"
			"			self.value=self.value[i+1:]\n"
			"			i=self.value.find('\\n')\n\n"
			"sys.stderr=redirect_io(irc.write_error)\n"
			"sys.stdout=redirect_io(irc.write_output)\n"
			;

		main_module    =py::import("__main__");
		main_namespace =main_module.attr("__dict__");
		py::object icm =py::import("irc_client");
		auto icn       =icm.attr("__dict__");

		main_namespace["irc"]=py::ptr(this);
		py::exec(error_redirect.c_str(), main_namespace);
	}
	catch(const py::error_already_set&) {
		PyErr_Print();
	}

}

void python_interface::exec(const std::string& py_code) {
	try {
		py::exec(py_code.c_str(), main_namespace);
	}
	catch(const py::error_already_set&) {
		PyErr_Print();
	}
}
void python_interface::reload_conf() {
	try {
		auto result=py::exec_file(python_file.c_str(), main_namespace);
	}
	catch(const py::error_already_set&) {
		PyErr_Print();
	}
}

void python_interface::p_connect_new_session(py::object func) {
	p_on_new_session.connect(
		[&,func](irc::session& sess) {
			try {
				func(boost::ref(sess));
			}
			catch(const py::error_already_set&) {
				PyErr_Print();
			}
		}
	);
}
void python_interface::p_set_default_nick(const std::string& nick) {
	on_change_default_nick(nick);
}
void python_interface::p_write_error(const std::string& str) {
	on_python_error(str);
}
void python_interface::p_write_output(const std::string& str) {
	on_python_output(str);
}
void python_interface::p_set_default_username(const std::string& name) {
	on_change_default_username(name);
}
void python_interface::p_set_default_fullname(const std::string& name) {
	on_change_default_fullname(name);
}
void python_interface::accept_new_session(irc::session& sess) {
	p_on_new_session(sess); //try catch is set in the handlers
}

std::string python_interface::get_python_file() const { 
	return python_file; 
}

void python_interface::p_start_connection(const std::string& con) {
	on_connect(con);
}

