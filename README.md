irc_client
==========

About
-----
irc_client (pending new name) is a terminal irc client written in C++11 using ncurses and boost heavily.
irc_client exposes a python interface to allow the user to configure and extend the client using python.

Prerequisites
------------
 + A modern C++11 compiler (tested with g++ version: 4.7.2 and clang 3.2)
 + A modern boost installation (tested with boost 1.50 and newer)
 + ncurses libs
 + python2.7 libs (should also work with other python versions ify ou change the 

You may need ot alter the $(LIB) and $(INC) in the `Makefile` at the root directory if your libraries are not in the normal places.

Building
--------
Ensuring that the prerequisites are met, we can simply build using:

    make
    
Note you maybe speed up compilation by building in parallel (using `make -jN`) however note that building can be very memory hungry.

Basic Usage
-----------

###Connecting

    /connect <server name>
    /connect irc.freenode.net

####TODO
extend to allow nick and port numbers in this command
     
###Joining a channel

From the session or associated channel buffers you may issue the following to join a channel:

    /join <channel name>
    /join ##irc_client
    
###Leaving a channel

To leave a channel, on the associated channel buffer issue:

    /leave <your exit message>
    /leave "goodbye cruel world" 

Python (and config)
------
By default irc_client will look in your current directory for an `irc_config.py`, a specific config file may be specified with following flag:

	irc_client --config-script=<your_script>
	irc_client --config-script=my_annoying_irc_bot.py

###Basic configuration
The basic configuration settings used by the irc_client include

    irc.default_username("myname145")
    irc.default_nick("mynick145")
    irc.default_port("6667")

of course, with this just being python you may do:

	import getpass
	#...
	irc.default_username(getpass.getuser())

Or whatever else you wish.

###Connecting to a server
You maybe initiate connections to a server from your config file.

	irc.connect("<your_host_name>")
	irc.connect("irc.freenode.net")

###IO
You can print to the status buffer using python build in `print` statement.

Please notify all error using stderr with `sys.stderr.write`, note you need to terminate with a `\n` to flush to the status buffer.

	if(success):
		print("success!")
	else
		sys.stderr.write("failed!\n")

###Running python from the GUI

You may run any valid python code from the GUI using:

    /python <code>

For example:

	/python print "hello world"
	/python execfile("my_py_file.py")
	/python my_func() #assuming my_func is defined

###Thread safety

* Do not call into any of the exposed interface from any threads you may have spawned yourself
* Do not block the main thread of execution (writing to a file is okay, waiting for a condition is not)

This maybe relaxed later at a later date.

License
-------
