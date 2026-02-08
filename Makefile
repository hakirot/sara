# Makefile #

CC=gcc
CFLAGS=-I -Wall -g -lncursesw -DNCURSES_WIDECHAR=1 -Wimplicit-function-declaration
DEPS = sara.h glyphs.h animations.h globals.h
OBJ = sara.o glyphs.o animations.o globals.h

all: sara clean

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

sara: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o
