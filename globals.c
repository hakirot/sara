/*
--   ██████╗ ██╗      ██████╗ ██████╗  █████╗ ██╗     ███████╗  --
--  ██╔════╝ ██║     ██╔═══██╗██╔══██╗██╔══██╗██║     ██╔════╝  --
--  ██║  ███╗██║     ██║   ██║██████╔╝███████║██║     ███████╗  --
--  ██║   ██║██║     ██║   ██║██╔══██╗██╔══██║██║     ╚════██║  --
--  ╚██████╔╝███████╗╚██████╔╝██████╔╝██║  ██║███████╗███████║  --
--   ╚═════╝ ╚══════╝ ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝.c--
                                                                     */

#include <sys/time.h>
#include "globals.h"
#include "config.h"

int BG_GLYPH_HEIGHT;
int FG_GLYPH_HEIGHT;
int BG_GLYPH_LENGTH;
int FG_GLYPH_LENGTH;
int HD_LENGTH;

int HOLOGRAPHIC = 0;

int FRAME_RATE = 60;
int FRAME_TIME_NS = 16666667LL;

char      global_chars[KEY_ARRAY_SIZE];
char commandkeys_chars[KEY_ARRAY_SIZE];
char builtinkeys_chars[KEY_ARRAY_SIZE];
char    menukeys_chars[KEY_ARRAY_SIZE];

int commandkeys_len;
int builtinkeys_len;
int menukeys_len;
int DEBUG = 0;

int slap_frames = 0;

state * _state = NULL;
