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
#include <sys/time.h>
#include <ncurses.h>

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

#define KEY_ARRAY_SIZE  128

typedef enum {
  t_tiny,
  t_small,
  t_full
} screen_size;

typedef union {
  int x;
  double y;
} Arg;

typedef enum {
  none,
  down_wipes,
  glitch,
  glitch_full,
  neon,
  neon_reverse,
  shutter_slide,
  shutter_slide_neon,
  pixel_fill,
  tv_static,
  print_b,
  print_f,
  blink,
} animation;

typedef enum {
  WAIT,
  NOWAIT,
  STOP,
  STOP_ON_ERR,
  EXEC,
} wait_option;

typedef enum {
  CONFIRM,
  NOCONFIRM,
} confirm_option;

typedef enum {
  NO_OUT,
  OUTS,
} out_option;

typedef struct {
  char* chdir;
  confirm_option confirmation;
  out_option output_option;
} ExtraArgs;

typedef struct {
  char smashkey;
  const void * cmd;
  wait_option option;
  animation pre_animation;
  animation post_animation;
  ExtraArgs extra_args;
} Command;

typedef enum {
  pshd,
  quit,
  randomize_colors,
  invert_colors,
  rave,
  colors,
  path_run,
  pwd,
  glint_fg,
  glint_bg,
  glint_full,
} internal;

typedef enum {
  COMMAND,
  SUBMENU,
} ItemType;

typedef struct {
  char smashkey;
  internal option;
} Builtin;

typedef struct {

  struct timeval slap_time;

  int bg_glyph_height;
  int fg_glyph_height;
  int bg_glyph_length;
  int fg_glyph_length;
  int hd_length;

  char global_chars       [KEY_ARRAY_SIZE];
  char commandkeys_chars  [KEY_ARRAY_SIZE];
  char builtinkeys_chars  [KEY_ARRAY_SIZE];
  char menukeys_chars     [KEY_ARRAY_SIZE];

  int commandkeys_len;
  int builtinkeys_len;
  int menukeys_len;

} config;


typedef struct {

  int input;

  int t_rows;
  int t_cols;
  int t_cache;

  int fg_c;
  int bg_c;
  int hd_c;

  int show_colors;
  int interrupt;
  bool preflight_check;
  clock_t last_input_time;
  screen_size win_size;
  int glitch_frame_time;
  float hold_char_time;
  char slap_str[256];
  int key_lock;
  int fx;
  int reprint;
  char ** fg_state;
  char ** bg_state;
} state;

typedef struct Menu Menu;

struct Menu{
  char * name;
  ItemType type;
  union {
    const Command command;
    const Menu * submenu;
  } next;
};

typedef struct {
  char smashkey;
  const Menu * submenu;
} MenuKey;

extern int BG_GLYPH_HEIGHT;
extern int BG_GLYPH_LENGTH;
extern int FG_GLYPH_HEIGHT;
extern int FG_GLYPH_LENGTH;
extern int HD_LENGTH;

extern int FOREGROUND;
extern int BACKGROUND;
extern int HEADER;
extern int HOLOGRAPHIC;

extern int ROW;
extern int COL;
extern int CACHE;

extern clock_t LAST_INPUT_TIME;
extern screen_size WIN_SIZE;

extern char HOLD_CHAR;
extern int GLITCH_TIME;

extern char global_chars[];
extern char commandkeys_chars[];
extern char builtinkeys_chars[];
extern char menukeys_chars[];

extern int commandkeys_len;
extern int builtinkeys_len;
extern int menukeys_len;

extern char SLAP_STR[256];
extern struct timeval slap_time;

extern int SHOW_COLORS;
extern int KEY_LOCK;
extern int INTERRUPT;

static const char *FORCE_ECHO_COMMAND[] = { "echo", NULL};
static const Command FORCE_ECHO_STRUCT = {0, FORCE_ECHO_COMMAND, WAIT,none,none, {NULL, NOCONFIRM, NO_OUT} };
static const Command * FORCE_ECHO = &FORCE_ECHO_STRUCT;

extern const int GLITCH_FRAME_TIME;
extern const float HOLD_CHAR_TIME;

extern state * _state;

#endif
