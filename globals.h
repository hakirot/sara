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
  pixel_fill,
  tv_static,
  print_b,
  print_f,
} animation_option;

// TODO: Consider options to confirm
typedef enum {
  WAIT,
  WAIT_ON_ERR,
  NOWAIT,
  EXEC,
} wait_option;

typedef struct {
  char smashkey;
  const void * cmd;
  wait_option option;
  animation_option pre_animation;
  animation_option post_animation;
} Command;

typedef enum {
  pshd,
  quit,
  randomize_colors,
  invert_colors,
  rave,
} internal;

typedef struct {
  char smashkey;
  internal option;
} Builtin;

extern int GLYPH_LENGTH;
extern int GLYPH_HEIGHT;
// TODO: Delete me
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

extern int GLITCH_TIME;

#endif
