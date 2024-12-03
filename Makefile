CC = gcc
CFLAGS = -Wall -g -Iinclude
LDFLAGS = 

all: test_malloy

malloy.o: srcs/malloy.c include/malloy.h
	$(CC) $(CFLAGS) -c srcs/malloy.c -o malloy.o

test_malloy: malloy.o tests/test_malloy.c
	$(CC) $(CFLAGS) tests/test_malloy.c malloy.o -o test_malloy $(LDFLAGS)

clean:
	rm -f *.o test_malloy
