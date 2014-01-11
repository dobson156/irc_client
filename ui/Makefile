export CPP  =g++
export OPTS ?=-O0 -ggdb

#-ggdb -DCONS_FAST_COMPILE
#-D_GLIBCXX_DEBUG

INC          =
LIB          =

export CFLAGS=$(OPTS) -std=c++11 -pedantic -Wall -Wextra -DBOOST_SIGNALS_NO_DEPRECATION_WARNING -DBOOST_RESULT_OF_USE_DECLTYPE

export LFLAGS=$(OPTS)

OBJS=basic.o string_stencil.o async_input_box.o input_manager.o colour_pair.o scrolling_frame.o

all: console.o console.a tests

tests: console.o
	cd tests && $(MAKE) 

console.o: $(OBJS)
	ld -r $^ $(LFLAGS) -o $@

console.a: console.o
	ar r console.a console.o

%.o: %.cpp
	$(CPP) -c $(CFLAGS) $(INC) $< -o $@ 

clean:
	rm -rf *.o
	cd tests && $(MAKE) clean

stat:
	wc -l *.hpp *.cpp
