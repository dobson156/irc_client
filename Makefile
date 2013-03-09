#export CPP   =clang++
export CPP  =g++
export LNK =$(CPP)

export OPTS         =-O3 -DNDEBUG
#OPTS         =-O0 -ggdb -DCONS_FAST_COMPILE -D_GLIBCXX_DEBUG

INC          =
LIB          =-lncurses -lboost_system -lpthread

export CFLAGS=$(OPTS) -std=c++11 -pedantic -Wall -Wextra -Wno-unused-parameter

export LFLAGS=$(OPTS)

#slow objects are library elements and spirit parsers 
SLOW_OBJS=ui/console.o controller_parse_text.o
FAST_OBJS=controller.o program.o console_ui.o
OBJS=$(SLOW_OBJS) $(FAST_OBJS)

all: irc_client

irc_client: $(OBJS)
	$(LNK) $^ -o $@ $(LIB)

ui/console.o:
	cd ui && $(MAKE)

%.o: %.cpp 
	$(CPP) -c $(CFLAGS) $(INC) $< -o $@ 

clean:
	rm -rf $(OBJS) irc_client
	cd ui && $(MAKE) clean

#quick clean for development
qclean:
	rm -rf $(FAST_OBJS) irc_client

stat:
	wc -l *.hpp *.cpp
