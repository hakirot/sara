# Makefile #

CC=gcc
CFLAGS=-I -Wall -g -lncursesw -DNCURSES_WIDECHAR=1 -Wimplicit-function-declaration
DEPS = sara.h glyphs.h animations.h globals.h utils.h
OBJ = sara.o glyphs.o animations.o globals.o utils.o

sara: $(OBJ)
	@echo Checking wallpaper directories
	@echo ERROR: newlook requested in config.h
	@echo ERROR: wallpaper directory not defined
	@echo ERROR: theme script not defined
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

all: sara clean

clean:
	rm -f *.o
