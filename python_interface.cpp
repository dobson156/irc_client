#include "python_interface.hpp"

#include "irc/user.hpp"
#include "irc/session.hpp"
#include "irc/channel.hpp"

#include <ncurses.h> //endwin

#include <Python.h>
#include <boost/python.hpp>

//helper
static void session_connect_on_join_channel(irc::session& sess, const py::object& func) {
	sess.connect_on_join_channel(
		[&,func](irc::channel& chan) {
			func(boost::ref(chan));
		}
	);
}

static void session_connect_on_user_notice(irc::session& sess, const py::object& func) {
	sess.connect_on_user_notice(
		[&,func](irc::user& user, const std::string& msg) {
			func(boost::ref(user), msg);
		}
	);
}

static void session_connect_on_new_user(irc::session& sess, const py::object& func) {
	sess.connect_on_new_user(
		[&,func](irc::user& user) {
			func(boost::ref(user));
		}
	);
}


static void channel_connect_on_message(irc::channel& chan, const py::object& func) {
	chan.connect_on_message(
		[&,func](irc::channel& chan, irc::user& user, const std::string& str) {
			func(boost::ref(chan), boost::ref(user), str);
		}
	);
}

static void channel_connect_on_join(irc::channel& chan, const py::object& func) {
	chan.connect_on_user_join(
		[&,func](irc::channel& chan, irc::user& user) {
			func(boost::ref(chan), boost::ref(user));
		}
	);
}

BOOST_PYTHON_MODULE(irc_client) {
//OBJECT MAPS
	py::object p_chan=py::class_<irc::channel, boost::noncopyable>("channel_t", py::no_init)
		.def("get_name",                &irc::channel::get_name, 
			py::return_value_policy<py::copy_const_reference>())
		.def("connect_on_message",      &channel_connect_on_message)
		.def("connect_on_join",         &channel_connect_on_join)
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
		.def("get_nick",                &irc::user::get_nick, 
			py::return_value_policy<py::copy_const_reference>())
		;

	py::object p_irc=py::class_<python_interface, boost::noncopyable>("irc_t", py::no_init)
		.def("get_python_file",         &python_interface::get_python_file)
		.def("connect",                 &python_interface::p_start_connection)
		.def("connect_on_new_session",  &python_interface::p_connect_new_session)
		.def("default_nick",            &python_interface::p_set_default_nick)
		.def("default_username",        &python_interface::p_set_default_username)
		;
}



python_interface::python_interface(std::string python_file_) 
:	python_file { std::move(python_file_) }
{
	Py_Initialize();
	initirc_client();

	main_module    =py::import("__main__");
	main_namespace =main_module.attr("__dict__");
	py::object icm =py::import("irc_client");
	auto icn       =icm.attr("__dict__");

	main_namespace["irc"]=py::ptr(this);
}

void python_interface::reload_conf() {
	try {
		auto result=py::exec_file(python_file.c_str(), main_namespace);
	}
	catch(const py::error_already_set&) {
		endwin();	
		PyErr_Print();
		abort();
	}
}

void python_interface::p_connect_new_session(py::object func) {
	p_on_new_session.connect(
		[&,func](irc::session& sess) {
			func(boost::ref(sess));
		}
	);
}
void python_interface::p_set_default_nick(const std::string& nick) {
	on_change_default_nick(nick);
}
void python_interface::p_set_default_username(const std::string& name) {
	on_change_default_username(name);
}
void python_interface::accept_new_session(irc::session& sess) {
	try {
		p_on_new_session(sess);
	}
	catch(const py::error_already_set&) {
		endwin();	\
		PyErr_Print();
		abort();
	}
}

std::string python_interface::get_python_file() const { 
	return python_file; 
}

void python_interface::p_start_connection(const std::string& con) {
	on_connect(con);
}

