# Makefile #

CC=gcc
CFLAGS=-I -Wall -g -lncursesw -DNCURSES_WIDECHAR=1 -Wimplicit-function-declaration

all: sara

sara: sara.c sara.h
	$(CC) sara.c -o sara $(CFLAGS)

clean:
	rm -f sara
