#Sample Makefile. You can make changes to this file according to your need
# The executable must be named proxy

CC = gcc
CFLAGS = -Wall -g 
LDFLAGS = -lpthread

OBJS = server.o csapp.o

all: server

server: $(OBJS)

csapp.o: csapp.c
	$(CC) $(CFLAGS) -c csapp.c

server.o: server.c
	$(CC) $(CFLAGS) -c server.c
clean:
	rm -f *~ *.o server
