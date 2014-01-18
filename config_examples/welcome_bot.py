#A really annoying welcoming bot to deomostrate bot writing
server_name="irc.freenode.net"
channel_list=[ "##welcome_bot" ]
nick="welcome_bot1"
user="welcome_bot1"
fullname="Welcome Bot"

def on_user_join(chan, user):
	print "user"
	chan.send_message("Welcome to " + chan.get_name() + " " + user.get_nick())

def on_join_channel(chan):
	print "chan"
	chan.connect_on_user_join(on_user_join)

def on_new_session(sess):
	print "sess"
	sess.connect_on_join_channel(on_join_channel)
	for chan in channel_list:
		sess.join(chan)

irc.default_nick(nick)
irc.default_username(user)
irc.default_fullname(fullname)

irc.connect(server_name)
irc.connect_on_new_session(on_new_session)
