# Makefile #

CC=gcc
CFLAGS=-I -Wall -g -lncursesw -DNCURSES_WIDECHAR=1 -Wimplicit-function-declaration
DEPS = sara.h glyphs.h animations.h globals.h
OBJ = sara.o glyphs.o animations.o globals.o

sara: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: sara clean

.PHONY: clean

clean:
	rm -f *.o
