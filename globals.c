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

char HOLD_CHAR;
int HOLOGRAPHIC = 0;
int GLITCH_TIME = 52;

char      global_chars[KEY_ARRAY_SIZE];
char commandkeys_chars[KEY_ARRAY_SIZE];
char builtinkeys_chars[KEY_ARRAY_SIZE];
char    menukeys_chars[KEY_ARRAY_SIZE];

int commandkeys_len;
int builtinkeys_len;
int menukeys_len;

int SHOW_COLORS = false;
int INTERRUPT = false;

const int GLITCH_FRAME_TIME = 3500;
const float HOLD_CHAR_TIME = 0.00001;
char SLAP_STR[256];
struct timeval slap_time = {0};

state * _state = NULL;
