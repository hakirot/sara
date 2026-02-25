/*
--   ██████╗ ██╗      ██████╗ ██████╗  █████╗ ██╗     ███████╗  --
--  ██╔════╝ ██║     ██╔═══██╗██╔══██╗██╔══██╗██║     ██╔════╝  --
--  ██║  ███╗██║     ██║   ██║██████╔╝███████║██║     ███████╗  --
--  ██║   ██║██║     ██║   ██║██╔══██╗██╔══██║██║     ╚════██║  --
--  ╚██████╔╝███████╗╚██████╔╝██████╔╝██║  ██║███████╗███████║  --
--   ╚═════╝ ╚══════╝ ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝.c--
                                                                     */

#include <wchar.h>
#include <time.h>
#include "globals.h"

int GLYPH_LENGTH;
int GLYPH_HEIGHT;
const int BIG_GLYPH_HEIGHT = 19;
const int BIG_GLYPH_LENGTH = 44;
const int NORMAL_GLYPH_HEIGHT = 7;
const int NORMAL_GLYPH_LENGTH = 44;

int FOREGROUND = 3;
int BACKGROUND = 2;
int HOLOGRAPHIC = 0;
int FOLLOW = 0;;

int ROW;
int COL;
int CACHE;

clock_t LAST_INPUT_TIME;
screen_size WIN_SIZE;
clock_t WAIT_START;

start_animation START_ANIMATION = EMPTY;
char HOLD_CHAR;

//int GLITCH_FRAME_TIME = 7000;
//int STANDARD_GLITCH_TIME = 46;
//int QUICK_GLITCH_TIME = 20;
int GLITCH_FRAME_TIME = 3500;
int STANDARD_GLITCH_TIME = 92;
int QUICK_GLITCH_TIME = 40;

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
const int WHITE_BLACKD  = 17;
const int RED_BLACK     = 18;
const int GREEN_BLACK   = 19;
const int YELLOW_BLACK  = 20;
const int BLUE_BLACK    = 21;
const int MAGENTA_BLACK = 22;
const int CYAN_BLACK    = 23;
const int BLACK_WHITED  = 24;
