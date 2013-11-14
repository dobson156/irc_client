

class setting:
	def __init__(self, username_, nickname_):
		username=username_
		nickname=nickname_

default = settings("guest", "guest")



connect_to("irc.freenode.net").on_connect(
	lambda s:
		s.privmsg("nickserv", "indentify mypassword")
)



