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
#include "wchar.h"

#define black           1
#define red             2
#define green           3
#define yellow          4
#define blue            5
#define magenta         6
#define cyan            7
#define white           8
#define white_black     9
#define black_red       10
#define black_green     11
#define black_yellow    12
#define black_blue      13
#define black_magenta   14
#define black_cyan      15
#define black_white     16
#define white_blackd    17
#define red_black       18
#define green_black     19
#define yellow_black    20
#define blue_black      21
#define magenta_black   22
#define cyan_black      23
#define black_whited    24

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

extern int GLYPH_LENGTH;
extern int GLYPH_HEIGHT;
extern const int BIG_GLYPH_HEIGHT;
extern const int BIG_GLYPH_LENGTH;

extern int BG_GLYPH_HEIGHT;
extern int BG_GLYPH_LENGTH;
extern int FG_GLYPH_HEIGHT;
extern int FG_GLYPH_LENGTH;

extern int option_window_height;
extern int option_window_length;

extern int IM_SET;

extern int FOREGROUND;
extern int BACKGROUND;
extern int HOLOGRAPHIC;

extern int ROW;
extern int COL;
extern int CACHE;

extern clock_t LAST_INPUT_TIME;
extern screen_size WIN_SIZE;
extern clock_t WAIT_START;

extern start_animation START_ANIMATION;
extern char HOLD_CHAR;

extern int STANDARD_GLITCH_TIME;
extern int QUICK_GLITCH_TIME;

#endif
