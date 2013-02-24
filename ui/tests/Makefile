CPP   =g++
INC   =-I../.

CFLAGS?= -O0 -g -std=c++11 -pedantic -Wall -Wextra #-DCONS_FAST_COMPILE
LFLAGS?= 

LIB   =-lncurses ../console.o

PROGS=frames anchors ui_test input_box_test simple typing_game
all: $(PROGS)

%: %.cpp 
	$(CPP) $(CFLAGS) $(INC) $< -o $@ $(LIB) 

#simple: simple.o 
#	$(CPP) $(LFLAGS) $^ $(LIB) -o $@
#
#ui_test: ui_test.o 
#	$(CPP) $(LFLAGS) $^ $(LIB) -o $@
#
#input_box_test: input_box_test.o 
#	$(CPP) $(LFLAGS) $^ $(LIB) -o $@
#
#frames: frames.o 
#	$(CPP) $(LFLAGS) $^ $(LIB) -o $@
#
#anchors: anchors.o 
#	$(CPP) $(LFLAGS) $^ $(LIB) -o $@
#
#%.o: %.cpp 
#	$(CPP) $(CFLAGS) $(INC) $< -o $@ 
#
clean:
	rm -rf $(PROGS)
