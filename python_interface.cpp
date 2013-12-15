#include "python_interface.hpp"

#include "irc/user.hpp"
#include "irc/session.hpp"
#include "irc/channel.hpp"

#include <Python.h>
#include <boost/python.hpp>

//TODO: code reduction, maybe a MACRO maybe not.
static void session_connect_on_join_channel(irc::session& sess, const py::object& func) {
	sess.connect_on_join_channel(
		[&,func](irc::channel& chan) {
			try {
				func(boost::ref(chan));
			}
			catch(const py::error_already_set&) {
				PyErr_Print();
			}
		}
	);
}
static void session_connect_on_user_notice(irc::session& sess, const py::object& func) {
	sess.connect_on_user_notice(
		[&,func](irc::user& user, const std::string& msg) {
			try {
				func(boost::ref(user), msg);
			}
			catch(const py::error_already_set&) {
				PyErr_Print();
			}
		}
	);
}
static void session_connect_on_new_user(irc::session& sess, const py::object& func) {
	sess.connect_on_new_user(
		[&,func](irc::user& user) {
			try {
				func(boost::ref(user));
			}
			catch(const py::error_already_set&) {
				PyErr_Print();
			}
		}
	);
}
static void channel_connect_on_message(irc::channel& chan, const py::object& func) {
	chan.connect_on_message(
		[&,func](irc::channel& chan, irc::user& user, const std::string& str) {
			try {
				func(boost::ref(chan), boost::ref(user), str);
			}
			catch(const py::error_already_set&) {
				PyErr_Print();
			}
		}
	);
}
static void channel_connect_on_join(irc::channel& chan, const py::object& func) {
	chan.connect_on_user_join(
		[&,func](irc::channel& chan, irc::user& user) {
			try {
				func(boost::ref(chan), boost::ref(user));
			}
			catch(const py::error_already_set&) {
				PyErr_Print();
			}
		}
	);
}

BOOST_PYTHON_MODULE(irc_client) {
//OBJECT MAPS
	py::object p_chan=py::class_<irc::channel, boost::noncopyable>("channel_t", py::no_init)
		.def("get_name",                &irc::channel::get_name, 
			py::return_value_policy<py::copy_const_reference>())
		.def("send_message",            &irc::channel::async_send_message)
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
		.def("write_error",             &python_interface::p_write_error)
		.def("write_output",            &python_interface::p_write_output)
		;
}



python_interface::python_interface(std::string python_file_) 
:	python_file { std::move(python_file_) }
{
	Py_Initialize();
	initirc_client();

	std::string error_redirect=//is it worth doing this is C++ instead?
		"import sys\n"
		"class stderr_redirect:\n"
		"	def __init__(self):\n"
		"		self.value=''\n"
		"	def write(self, txt):\n"
		"		self.value+=txt\n"
		"		i=self.value.find('\\n')\n"
		"		while(i != -1):\n"
		"			v2=self.value[:i]\n"
		"			irc.write_error(v2)\n"
		"			self.value=self.value[i+1:]\n"
		"			i=self.value.find('\\n')\n\n"
		"class stdout_redirect:\n"
		"	def __init__(self):\n"
		"		self.value=''\n"
		"	def write(self, txt):\n"
		"		self.value+=txt\n"
		"		i=self.value.find('\\n')\n"
		"		while(i != -1):\n"
		"			v2=self.value[:i]\n"
		"			irc.write_output(v2)\n"
		"			self.value=self.value[i+1:]\n"
		"			i=self.value.find('\\n')\n\n"		
		"sys.stderr=stderr_redirect()\n"
		"sys.stdout=stdout_redirect()\n"
		;

	main_module    =py::import("__main__");
	main_namespace =main_module.attr("__dict__");
	py::object icm =py::import("irc_client");
	auto icn       =icm.attr("__dict__");

	py::exec(error_redirect.c_str(), main_namespace);

	main_namespace["irc"]=py::ptr(this);
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
void python_interface::accept_new_session(irc::session& sess) {
	p_on_new_session(sess); //try catch is set in the handlers
}

std::string python_interface::get_python_file() const { 
	return python_file; 
}

void python_interface::p_start_connection(const std::string& con) {
	on_connect(con);
}

