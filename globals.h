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
extern const int BIG_GLYPH_HEIGHT;
extern const int BIG_GLYPH_LENGTH;
extern const int NORMAL_GLYPH_HEIGHT;
extern const int NORMAL_GLYPH_LENGTH;

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

extern start_animation START_ANIMATION;
extern char HOLD_CHAR;
extern int GLITCH_FRAME_TIME;

extern const int BLACK;
extern const int RED;
extern const int GREEN;
extern const int YELLOW;
extern const int BLUE;
extern const int MAGENTA;
extern const int CYAN;
extern const int WHITE;
extern const int WHITE_BLACK;
extern const int BLACK_RED;
extern const int BLACK_GREEN;
extern const int BLACK_YELLOW;
extern const int BLACK_BLUE;
extern const int BLACK_MAGENTA;
extern const int BLACK_CYAN;
extern const int BLACK_WHITE;
extern const int BLACK_WHITED;
extern const int RED_BLACK;
extern const int GREEN_BLACK;
extern const int YELLOW_BLACK;
extern const int BLUE_BLACK;
extern const int MAGENTA_BLACK;
extern const int CYAN_BLACK;
extern const int WHITE_BLACKD;

#endif
