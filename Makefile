#LIB          =-Wl,-Bstatic -lncurses -lboost_python -lboost_system -lboost_program_options -lboost_signals -Wl,-Bdynamic -lpython2.7 -lpthread-lboost_signals 
#-DCONS_FAST_COMPILE -D_GLIBCXX_DEBUG
#export CPP   =clang++
export CPP  =g++
export LNK =$(CPP)


INC          =-I /usr/include/python2.7 -I . -I ./libircpp/include -I ./ui/include
LIB          =-lncurses -lboost_python -lboost_system -lboost_program_options -lboost_filesystem -lpython2.7 -lpthread

export CFLAGS=-std=c++11 -pedantic -Wall -Wextra -Wno-unused-parameter -DBOOST_RESULT_OF_USE_DECLTYPE -DUSING_PYTHON 

BUILD_TYPE=dbg
ifeq ($(BUILD_TYPE), dbg)
	COMPILE_FLAGS=-ggdb -O0 $(CFLAGS) $(INC) 
	LFLAGS=
else
	COMPILE_FLAGS=-O3 -DNDEBUG $(CFLAGS) $(INC) 
	LFLAGS=-flto
endif


#slow objects are library elements and spirit parsers 
SLOW_OBJS=ui/src/console.o src/controller_parse_text.o libircpp/src/irc.o src/python_interface.o
FAST_OBJS=src/controller.o src/program.o src/console_ui.o src/message.o src/message_stencil.o src/channel_buffer.o src/session_buffer.o src/error_buffer.o src/buffer.o src/unique_connection.o src/window.o src/util.o src/pallet.o src/log_messages.o
OBJS=$(SLOW_OBJS) $(FAST_OBJS)

all: irc_client

irc_client: $(OBJS)
	cd ui && $(MAKE)
	cd libircpp && $(MAKE)
	$(LNK) $(LFLAGS) $^ -o $@ $(LIB)

ui/src/console.o:
	cd ui && $(MAKE)

libircpp/src/irc.o:
	cd libircpp && $(MAKE)

src/%.o: src/%.cpp
	$(CPP) -c $(COMPILE_FLAGS) $< -o $@ 

clean:
	rm -rf $(OBJS) irc_client
	cd ui && $(MAKE) clean
	cd libircpp && $(MAKE) clean

#quick clean for development
qclean:
	rm -rf $(FAST_OBJS) irc_client

stat:
	wc -l *.hpp *.cpp
