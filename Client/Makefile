#Sample Makefile. You can make changes to this file according to your need
# The executable must be named proxy

CC = gcc
CFLAGS = -Wall -g 
LDFLAGS = -lpthread

OBJS = client.o csapp.o

all: client

client: $(OBJS)

csapp.o: csapp.c
	$(CC) $(CFLAGS) -c csapp.c

client.o: client.c
	$(CC) $(CFLAGS) -c client.c
clean:
	rm -f *~ *.o client
