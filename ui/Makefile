#export CPP   =clang++
export CPP  =g++

#OPTS         =-O3 -DNDEBUG
OPTS         =-O0 -ggdb -DCONS_FAST_COMPILE -D_GLIBCXX_DEBUG

INC          =
LIB          =-lncurses

export CFLAGS=$(OPTS) -std=c++11 -pedantic -Wall -Wextra 

export LFLAGS=$(OPTS)

all: console.o console.a tests

tests: console.o
	cd tests && $(MAKE) -j

console.a: console.o
	ar r console.a console.o

%.o: %.cpp
	$(CPP) -c $(CFLAGS) $(INC) $< -o $@ 

clean:
	rm -rf *.o
	cd tests && $(MAKE) clean

stat:
	wc -l *.hpp *.cpp
