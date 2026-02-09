/*

--   ██████╗ ██╗      ██████╗ ██████╗  █████╗ ██╗     ███████╗  --
--  ██╔════╝ ██║     ██╔═══██╗██╔══██╗██╔══██╗██║     ██╔════╝  --
--  ██║  ███╗██║     ██║   ██║██████╔╝███████║██║     ███████╗  --
--  ██║   ██║██║     ██║   ██║██╔══██╗██╔══██║██║     ╚════██║  --
--  ╚██████╔╝███████╗╚██████╔╝██████╔╝██║  ██║███████╗███████║  --
--   ╚═════╝ ╚══════╝ ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝.h--
                                                                       */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <time.h>
#include <ncurses.h>

typedef enum {
  SMALL,
  NORMAL,
  BIG
} screen_size;

typedef enum {
  QUICK,
  STANDARD,
  NEON,
  EMPTY
} start_animation;

extern wchar_t SEARCH_STR[64];

extern int GLYPH_LENGTH;
extern int GLYPH_HEIGHT;

extern int FOREGROUND;
extern int BACKGROUND;
extern int HOLOGRAPHIC;
extern int FOLLOW;

extern int ROW;
extern int COL;
extern int CACHE;


extern clock_t LAST_INPUT_TIME;
extern screen_size WIN_SIZE;
extern clock_t WAIT_START;

#endif
