/*
--   ██████╗ ██╗      ██████╗ ██████╗  █████╗ ██╗     ███████╗  --
--  ██╔════╝ ██║     ██╔═══██╗██╔══██╗██╔══██╗██║     ██╔════╝  --
--  ██║  ███╗██║     ██║   ██║██████╔╝███████║██║     ███████╗  --
--  ██║   ██║██║     ██║   ██║██╔══██╗██╔══██║██║     ╚════██║  --
--  ╚██████╔╝███████╗╚██████╔╝██████╔╝██║  ██║███████╗███████║  --
--   ╚═════╝ ╚══════╝ ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝.c--
                                                                     */

#include "globals.h"
#include "config.h"
#include <time.h>

int GLYPH_LENGTH;
int GLYPH_HEIGHT;

int FOREGROUND = fg_c;
int BACKGROUND = bg_c;

// Pull this from config.h
// how does sucklesss get away with this
const int BIG_GLYPH_HEIGHT = 19;
const int BIG_GLYPH_LENGTH = 44;
const int NORMAL_GLYPH_HEIGHT = 7;
const int NORMAL_GLYPH_LENGTH = 44;

int ROW;
int COL;
int CACHE;

clock_t LAST_INPUT_TIME;
screen_size WIN_SIZE;
clock_t WAIT_START;

int BG_GLYPH_HEIGHT;
int FG_GLYPH_HEIGHT;
int BG_GLYPH_LENGTH;
int FG_GLYPH_LENGTH;

start_animation START_ANIMATION = EMPTY;
char HOLD_CHAR;

int HOLOGRAPHIC = 0;

//int GLITCH_FRAME_TIME = 7000;
//int STANDARD_GLITCH_TIME = 46;
//int QUICK_GLITCH_TIME = 20;
int STANDARD_GLITCH_TIME = 92;
int QUICK_GLITCH_TIME = 40;
