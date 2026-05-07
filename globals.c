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

int FOREGROUND = fg_c;
int BACKGROUND = bg_c;

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

int option_window_height = 7;
int option_window_length = 44;

int IM_SET = true;

start_animation START_ANIMATION = EMPTY;
char HOLD_CHAR;

int HOLOGRAPHIC = 0;

int GLITCH_TIME = 52;
