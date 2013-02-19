CPP   =g++
INC   =-I../.

CFLAGS?= -c -O0 -g -std=c++11 -pedantic -Wall -Wextra 
LFLAGS?= 

LIB   =-lncurses ../console.o 

all: frames anchors ui_test input_box_test simple


simple: simple.o 
	$(CPP) $(LFLAGS) $^ $(LIB) -o $@

ui_test: ui_test.o 
	$(CPP) $(LFLAGS) $^ $(LIB) -o $@

input_box_test: input_box_test.o 
	$(CPP) $(LFLAGS) $^ $(LIB) -o $@

frames: frames.o 
	$(CPP) $(LFLAGS) $^ $(LIB) -o $@

anchors: anchors.o 
	$(CPP) $(LFLAGS) $^ $(LIB) -o $@

%.o: %.cpp 
	$(CPP) $(CFLAGS) $(INC) $< -o $@ 

clean:
	rm -rf frames anchors *.o
