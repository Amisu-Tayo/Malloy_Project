CC = gcc
CFLAGS = -Wall -g -Iinclude
LDFLAGS = 

# default target to build test program
all: test_malloy malloc_vs_malloy

# Build object file from malloy.c
malloy.o: srcs/malloy.c include/malloy.h
	$(CC) $(CFLAGS) -c srcs/malloy.c -o malloy.o

# Build test program
test_malloy: malloy.o tests/test_malloy.c
	$(CC) $(CFLAGS) tests/test_malloy.c malloy.o -o test_malloy $(LDFLAGS)

malloc_vs_malloy: malloy.o tests/malloc_vs_malloy.c
	$(CC) $(CFLAGS) tests/malloc_vs_malloy.c malloy.o -o malloc_vs_malloy $(LDFLAGS)


#clean up compiled fiiles
clean:
	rm -f *.o test_malloy malloc_vs_malloy
