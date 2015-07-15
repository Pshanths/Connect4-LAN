#Sample Makefile. You can make changes to this file according to your need
# The executable must be named proxy

CC = gcc
CFLAGS = -Wall -g 
LDFLAGS = -lpthread

OBJS = board.o csapp.o

all: board

board: $(OBJS)

csapp.o: csapp.c
	$(CC) $(CFLAGS) -c csapp.c

board.o: board.c
	$(CC) $(CFLAGS) -c board.c
clean:
	rm -f *~ *.o board
