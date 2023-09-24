# Makefile #

CC=gcc
CFLAGS=-I -Wall -g -lncurses

sara: sara.c
	$(CC) sara.c -o sara $(CFLAGS)

clean:
	rm -f sara
