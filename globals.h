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

#define BLACK           1
#define RED             2
#define GREEN           3
#define YELLOW          4
#define BLUE            5
#define MAGENTA         6
#define CYAN            7
#define WHITE           8
#define WHITE_BLACK     9
#define BLACK_RED       10
#define BLACK_GREEN     11
#define BLACK_YELLOW    12
#define BLACK_BLUE      13
#define BLACK_MAGENTA   14
#define BLACK_CYAN      15
#define BLACK_WHITE     16
#define WHITE_BLACKD    17
#define RED_BLACK       18
#define GREEN_BLACK     19
#define YELLOW_BLACK    20
#define BLUE_BLACK      21
#define MAGENTA_BLACK   22
#define CYAN_BLACK      23
#define BLACK_WHITED    24

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

extern wchar_t BG_STR[64];
extern wchar_t FG_STR[16];

extern int GLYPH_LENGTH;
extern int GLYPH_HEIGHT;
extern const int BIG_GLYPH_HEIGHT;
extern const int BIG_GLYPH_LENGTH;
extern const int NORMAL_GLYPH_HEIGHT;
extern const int NORMAL_GLYPH_LENGTH;

extern int FOREGROUND;
extern int BACKGROUND;
extern bool HOLOGRAPHIC;
extern bool FOLLOW;

extern int ROW;
extern int COL;
extern int CACHE;

extern clock_t LAST_INPUT_TIME;
extern screen_size WIN_SIZE;
extern clock_t WAIT_START;

extern start_animation START_ANIMATION;
extern char HOLD_CHAR;
extern float HOLD_CHAR_TIME;

extern int STANDARD_GLITCH_TIME;
extern int QUICK_GLITCH_TIME;

#endif
