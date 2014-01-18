channels= [ "#ircpp", "##news" ]

def on_new_session(session):
	for chan in channels:
		session.join(chan)

#set your full name if not set this will consult you OS
#irc.default_fullname("your full name")

#set your nick here
irc.default_nick("yournick")

## list servers to connect to here
irc.connect("irc.freenode.net")
irc.connect_on_new_session(on_new_session)
