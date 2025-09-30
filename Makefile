# Makefile #

CC=gcc
CFLAGS=-I -Wall -g -lncursesw -DNCURSES_WIDECHAR=1 -Wimplicit-function-declaration
DEPS = sara.h glyphs.h
OBJ = sara.o glyphs.o

all: sara clean

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

sara: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
