CPP   =g++
INC   =-I../.

CFLAGS?= -O0 -g -std=c++11 -pedantic -Wall -Wextra -D_GLIBCXX_DEBUG
LFLAGS?= 

LIB   =../console.o -lncurses -lpthread -lboost_system -lboost_signals

PROGS=frames anchors ui_test input_box_test simple typing_game async_input_box_test input_manager_test scroll_test #async_term
all: $(PROGS)

%: %.cpp 
	$(CPP) $(CFLAGS) $(INC) $< -o $@ $(LIB) 

async_term: async_term.cpp
	$(CPP) $(CFLAGS) $(INC) $< -o $@ $(LIB) 

clean:
	rm -rf $(PROGS)
