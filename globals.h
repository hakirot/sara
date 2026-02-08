/*

--   ██████╗ ██╗      ██████╗ ██████╗  █████╗ ██╗     ███████╗  --
--  ██╔════╝ ██║     ██╔═══██╗██╔══██╗██╔══██╗██║     ██╔════╝  --
--  ██║  ███╗██║     ██║   ██║██████╔╝███████║██║     ███████╗  --
--  ██║   ██║██║     ██║   ██║██╔══██╗██╔══██║██║     ╚════██║  --
--  ╚██████╔╝███████╗╚██████╔╝██████╔╝██║  ██║███████╗███████║  --
--   ╚═════╝ ╚══════╝ ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝  --

*/

#include <time.h>
#include <ncurses.h>

#ifndef GLOBALS_H
#define GLOBALS_H

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

clock_t LAST_INPUT_TIME;
screen_size WIN_SIZE;
clock_t WAIT_START;

wchar_t SEARCH_STR[] = L"`+so:-./";

int GLYPH_LENGTH;
int GLYPH_HEIGHT;

int FOREGROUND;
int BACKGROUND;
int HOLOGRAPHIC;
int FOLLOW;

int ROW;
int COL;
int CACHE;


const int BLACK         = 1;
const int RED           = 2;
const int GREEN         = 3;
const int YELLOW        = 4;
const int BLUE          = 5;
const int MAGENTA       = 6;
const int CYAN          = 7;
const int WHITE         = 8;
const int WHITE_BLACK   = 9;
const int BLACK_RED     = 10;
const int BLACK_GREEN   = 11;
const int BLACK_YELLOW  = 12;
const int BLACK_BLUE    = 13;
const int BLACK_MAGENTA = 14;
const int BLACK_CYAN    = 15;
const int BLACK_WHITE   = 16;
const int BLACK_WHITED  = 17;
const int RED_BLACK     = 18;
const int GREEN_BLACK   = 19;
const int YELLOW_BLACK  = 20;
const int BLUE_BLACK    = 21;
const int MAGENTA_BLACK = 22;
const int CYAN_BLACK    = 23;
const int WHITE_BLACKD  = 24;

//const int EFFECT_MUTE   = 0;

start_animation START_ANIMATION = EMPTY;
char HOLD_CHAR;
int GLITCH_FRAME_TIME = 7000;

void slide_in();

#endif
