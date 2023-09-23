# Makefile #

CC=gcc
CFLAGS=-I -Wall -g

sara: sara.c
	$(CC) sara.c -o sara

clean:
	rm -f sara
