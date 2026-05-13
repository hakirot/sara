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

int ROW = 0;
int COL = 0;
int CACHE = 10;

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

char HOLD_CHAR;

int HOLOGRAPHIC = 0;

int GLITCH_TIME = 52;

char global_chars[KEY_ARRAY_SIZE];
char commandkeys_chars[KEY_ARRAY_SIZE];
char builtins_chars[KEY_ARRAY_SIZE];
char menukeys_chars[KEY_ARRAY_SIZE];

int commandkeys_len;
int builtins_len;
int menukeys_len;

int KEY_LOCK = 0;
