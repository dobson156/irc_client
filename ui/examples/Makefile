export CPP = g++
export OPTS ?=-O0 -ggdb
INC   =-I ./../include/ui

CFLAGS?= $(OPTS) -std=c++11 -pedantic -Wall -Wextra -D_GLIBCXX_DEBUG
LFLAGS?= $(OPTS) 

LIB   =./../src/console.o -lncurses -lpthread -lboost_system -lboost_signals

PROGS=brightness
all: $(PROGS)

%: %.cpp 
	$(CPP) $(CFLAGS) $(INC) $< -o $@ $(LIB) 

async_term: async_term.cpp
	$(CPP) $(CFLAGS) $(INC) $< -o $@ $(LIB) 

clean:
	rm -rf $(PROGS)
