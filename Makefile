# Makefile #

CC=gcc
CFLAGS=-I -Wall -g -lncurses

all: sara test

sara: sara.c
	$(CC) sara.c -o sara $(CFLAGS)

test: test.c
	$(CC) test.c -o test $(CFLAGS)

clean:
	rm -f sara
	rm -f test
