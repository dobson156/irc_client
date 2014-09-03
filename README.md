irc_client
==========

About
-----
irc_client (pending new name) is a terminal irc client written in C++11 using ncurses and boost heavily.
irc_client exposes a python interface to allow the user to configure and extend the client using python.

Prerequisites
------------
 + A modern C++11 compiler (tested with g++ version: 4.7.2, 4.82 and clang: 3.2)
 + A modern boost installation (tested with boost 1.50 and 1.54)
 + ncurses libs
 + python2.7 libs (should also work with other python versions ify ou change the Makefile

You may need ot alter the `$(LIB)` and `$(INC)` in the `Makefile` at the root directory if your libraries are not in the normal places.

Retrieving the source code
------------------
This project uses submodules which means you need to use a recursive clone to retreive all of the source, like so:

```bash
git clone https://github.com/dobson156/irc_client.git --recursive
```
###Updating
To update the submodules 

git submodule update --init --recursive

However this shouldn't be necessary if cloned correctly

Building
--------
Ensuring that the prerequisites are met, we can simply build using:

```bash
make
```
    
Note you maybe speed up compilation by building in parallel (using `make -jN`) however note that building can be very memory hungry.

Basic Usage
-----------

###Connecting

    /connect <server name>
    /connect irc.freenode.net

###Joining a channel

From the session or associated channel buffers you may issue the following to join a channel:

    /join <channel name>
    /join ##irc_client
    
###Leaving a channel

To leave a channel, on the associated channel buffer issue:

    /leave <your exit message>
    /leave "goodbye cruel world" 

Command list
------------


Command | Description | Context | Example
--
/join <channel name> | Joins a new IRC channel | This command can be run from any window relating to a server | /join ##ircpp

Python (and config)
------
By default irc_client will look in your current directory for an `irc_config.py`, a specific config file may be specified with following flag:

```bash
irc_client --config-script=<your_script>
irc_client --config-script=my_annoying_irc_bot.py
```

###Basic configuration
The basic configuration settings used by the irc_client include

```python
irc.default_username("myname145")
irc.default_nick("mynick145")
irc.default_port("6667")
```

of course, with this just being python you may do:

```python
import getpass
#...
irc.default_username(getpass.getuser())
```

Or whatever else you wish.

###Connecting to a server
You maybe initiate connections to a server from your config file.

```python
irc.connect("<your_host_name>")
irc.connect("irc.freenode.net")
```

###IO
You can print to the status buffer using python build in `print` statement.

Please notify all error using stderr with `sys.stderr.write`, note you need to terminate with a `\n` to flush to the status buffer.

```python
if(success):
	print "success!"
else:
	sys.stderr.write("failed!\n")
```

###Running python from the GUI

You may run any valid python code from the GUI using:

    /python <code>

For example:

	/python print "hello world"
	/python execfile("my_py_file.py")
	/python my_func() #assuming my_func is defined

###Advice

 * It is strongly recommend that you do not store any persistent references to sessions, channels, or users; instead get them from the API.
 * It is recommended that you filter by registering handlers only the objects you are interested in. For example:

If you are only interested in certain users messages, prefer:

```python
if(user.get_nick() == interesting_nick):
	user.connect_on_channel_message(
		lambda ch, us, m:
			foo(ch, us, m)
	)
```
to:

```python
channel.connect_on_message(
	lambda chan, u, m:
		if(u.get_nick()==interesting_nick):
			foo(ch, u, m)
)
```

However if you wish to capture every users messages in a channel use the channel 

###Thread safety

* Do not call into any of the exposed interface from any threads you may have spawned yourself
* Do not block the main thread of execution (writing to a file is okay, waiting for a condition is not)

This maybe relaxed later at a later date.

License
-------
This project is licensed under the Boost license, for more imformation:

 * http://www.boost.org/users/license.html
 * http://en.wikipedia.org/wiki/Boost_Software_License
 * http://www.boost.org/LICENSE_1_0.txt
