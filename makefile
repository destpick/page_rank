CC=g++
CFLAGS=-std=c++11 -fopenmp

TARGET= page_rank
LIBS=Page.h Graph.h

make:
        $(CC) -o $(TARGET) $(CFLAGS) $(LIBS) Main.cpp

clean:
        rm $(TARGET) *.out