CC = gcc
CFLAGS = -Wall -g
LDFLAGS = 

all: test_malloy

malloy.o: src/malloy.c include/malloy.h
	$(CC) $(CFLAGS) -c src/malloy.c -o malloy.o

test_malloy: malloy.o tests/test_malloy.c
	$(CC) $(CFLAGS) tests/tests_malloy.c malloy.o -o test_malloy $(LDFLAGS)

clean:
	rm -f *.o test_malloy