OPENCV=/opt/opencv/
ROOT=.

CC = g++
CFLAGS  = -Wall -Wextra -I$(ROOT) -I$(OPENCV)/include
LDFLAGS = -L$(OPENCV)/lib -lopencv_core -lopencv_imgproc -lopencv_highgui 

OBJ = morphology.o
BIN = \
	make-se\
	dilation

all: $(OBJ) $(BIN)

make-se: 
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $@.cpp
morphology.o: morphology.cpp morphology.hpp
	$(CC) -c $(CFLAGS) -o $@ morphology.cpp
%: %.cpp morphology.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
clean:
	rm -rf $(BIN) *.o *~ *.png

.PHONY: all clean 
