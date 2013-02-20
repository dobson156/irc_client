export CPP  =g++
#OPTS         =-O3 -DNDEBUG
OPTS         =-O0 -ggdb
INC          =
LIB          =-lncurses
#export CPP   =clang++
export CFLAGS=$(OPTS) -std=c++11 -pedantic -Wall -Wextra 
#export CFLAGS=-c $(OPTS) -std=c++11 -pedantic -Wall -Wextra -Weffc++
export LFLAGS=$(OPTS)

all: console.o tests

tests: console.o
	cd tests && $(MAKE) -j

%.o: %.cpp
	$(CPP) -c $(CFLAGS) $(INC) $< -o $@ 

clean:
	rm -rf *.o
	cd tests && $(MAKE) clean
