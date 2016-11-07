#file := main.cpp
CC := gcc
CXX := g++
#CFLAGS := "-std=c++14"
CXXFLAGS := -ggdb -g -std=c++14 -Wall

objs := main.o

all: repl
	./repl

repl: $(objs)
	$(CXX) $(CXXFLAGS) $(objs) -o repl

clean: 
	rm *.o
	rm repl
