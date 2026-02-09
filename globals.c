/*

--   ██████╗ ██╗      ██████╗ ██████╗  █████╗ ██╗     ███████╗  --
--  ██╔════╝ ██║     ██╔═══██╗██╔══██╗██╔══██╗██║     ██╔════╝  --
--  ██║  ███╗██║     ██║   ██║██████╔╝███████║██║     ███████╗  --
--  ██║   ██║██║     ██║   ██║██╔══██╗██╔══██║██║     ╚════██║  --
--  ╚██████╔╝███████╗╚██████╔╝██████╔╝██║  ██║███████╗███████║  --
--   ╚═════╝ ╚══════╝ ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝.c--
                                                                       */

#include <wchar.h>
#include "globals.h"
#include <time.h>

wchar_t SEARCH_STR[] = L"`+so:-./";

int GLYPH_LENGTH;
int GLYPH_HEIGHT;

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
