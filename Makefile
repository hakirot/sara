# Makefile #

VERSION = 0.9.1-1

SRC = sara.c animations.c globals.c utils.c
OBJ = $(SRC:.c=.o)

#CFLAGS        = -pipe -O2 -flto -fno-fat-lto-objects -Wall -Wextra -D_REENTRANT -fPIC $(DEFINES)
CFLAGS        = -pipe -O2 -flto -fno-fat-lto-objects -D_REENTRANT -fPIC $(DEFINES)

LINK    = gcc
LFLAGS  = -Wl,-O1 -pipe -O2 -flto=16 -fno-fat-lto-objects -fuse-linker-plugin -fPIC -lncursesw -DNCURSES_WIDECHAR=1
LIBS    = -lGL -lpthread   
TARGET  = sara

all: sara config.h

config.h:
	cp config.def config.h

sara: $(OBJ) config.h
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJ) $(OBJCOMP) $(LIBS)

dist: clean
	mkdir -p sara-$(VERSION)
	cp -R FAQ TODO LICENSE Makefile README.md config.def\
		sara.h utils.h animations.h globals.h $(SRC)\
		sara-$(VERSION)
	tar -cf - sara-$(VERSION) | gzip > sara-$(VERSION).tar.gz
	rm -rf sara-$(VERSION)

clean:
	rm -f sara $(OBJ) sara-$(VERSION).tar.gz

animations.o: animations.c animations.h \
		globals.h \
		config.h \
		utils.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o animations.o animations.c

globals.o: globals.c globals.h \
		config.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o globals.o globals.c

sara.o: sara.c sara.h \
		globals.h \
		animations.h \
		config.h \
		utils.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o sara.o sara.c

utils.o: utils.c utils.h \
		globals.h \
		config.h \
		sara.h \
		animations.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o utils.o utils.c

