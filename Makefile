# Makefile #

CC=gcc
CFLAGS=-I -Wall -g -lncursesw -DNCURSES_WIDECHAR=1

all: sara

sara: sara.c
	$(CC) sara.c -o sara $(CFLAGS)

clean:
	rm -f sara
