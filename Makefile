# Makefile #

VERSION = 0.9.2

SRC = sara.c animations.c globals.c utils.c
OBJ = $(SRC:.c=.o)

CC			= gcc
LINK    = gcc
LFLAGS  = -lncursesw -DNCURSES_WIDECHAR=1
TARGET  = sara

all: sara config.h

config.h:
	cp config.def config.h
	sed -i "s|PATH_ME_PLS|${PWD}/sara|g" config.h

sara: $(OBJ) config.h
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJ)
	./sara -C

dist: clean
	mkdir -p sara-$(VERSION)
	cp -R FAQ TODO LICENSE Makefile README.md config.def \
		sara.h utils.h animations.h globals.h $(SRC)\
		sara-$(VERSION)
	tar -cf - sara-$(VERSION) | gzip > sara-$(VERSION).tar.gz
	rm -rf sara-$(VERSION)
	sha256sum sara-$(VERSION).tar.gz > SHA256SUM
	gpg --detach-sign sara-$(VERSION).tar.gz

clean:
	rm -f sara $(OBJ) sara-$(VERSION).tar.gz sara-$(VERSION).tar.gz.sig SHA256SUM

animations.o: config.h animations.c animations.h
	$(CC) -c -o animations.o animations.c

globals.o: config.h globals.c globals.h
	$(CC) -c -o globals.o globals.c

sara.o: config.h sara.c sara.h
	$(CC) -c -o sara.o sara.c

utils.o: config.h utils.c utils.h
	$(CC) -c -o utils.o utils.c

