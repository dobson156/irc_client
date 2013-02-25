CPP   =clang++
INC   =-I../.

CFLAGS?= -O0 -g -std=c++11 -pedantic -Wall -Wextra -D_GLIBCXX_DEBUG
LFLAGS?= 

LIB   =-lncurses ../console.o

PROGS=frames anchors ui_test input_box_test simple typing_game
all: $(PROGS)

%: %.cpp 
	$(CPP) $(CFLAGS) $(INC) $< -o $@ $(LIB) 

clean:
	rm -rf $(PROGS)
