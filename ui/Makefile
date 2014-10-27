export CPP  =g++
export OPTS ?=-O0 

#-ggdb -DCONS_FAST_COMPILE
#-D_GLIBCXX_DEBUG

INC          = -I ./include/ui
LIB          =

export CFLAGS=$(OPTS) -ggdb -std=c++11 -pedantic -Wall -Wextra -DBOOST_SIGNALS_NO_DEPRECATION_WARNING -DBOOST_RESULT_OF_USE_DECLTYPE

export LFLAGS=$(OPTS)

OBJS=src/basic.o src/string_stencil.o src/async_input_box.o src/input_manager.o src/colour_pair.o src/scrolling_frame.o src/rlwrapper.o src/minimal_frame_wrapper.o src/progress_bar.o

all: src/console.o src/console.a tests

tests:
	cd tests && $(MAKE)

src/console.o: $(OBJS)
	ld -r $^ $(LFLAGS) -o $@

src/console.a: src/console.o
	ar r src/console.a src/console.o

src/%.o: src/%.cpp
	$(CPP) -c $(CFLAGS) $(INC) $< -o $@ 

clean:
	rm -rf src/*.o
	cd tests && $(MAKE) clean

stat:
	wc -l include/*.hpp src/*.cpp
