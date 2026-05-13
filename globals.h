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

#define KEY_ARRAY_SIZE  128

typedef enum {
  SMALL,
  NORMAL,
  BIG
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
} animation_option;

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
} confirm;

typedef enum {
  NO_OUT,
  OUTS,
} out_option;

typedef struct {
  char* chdir;
  confirm confirmtion;
  out_option output_option;
} ExtraArgs;

typedef struct {
  char smashkey;
  const void * cmd;
  wait_option option;
  animation_option pre_animation;
  animation_option post_animation;
  ExtraArgs extra_args;
} Command;

typedef enum {
  pshd,
  quit,
  randomize_colors,
  invert_colors,
  rave,
  check,         // TODO: implement show all config (run preflight_check)
} internal;

typedef enum {
  COMMAND,
  SUBMENU,
} ItemType;

typedef struct {
  char smashkey;
  internal option;
} Builtin;

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

extern const Menu * RegisteredMenus[];

extern int BG_GLYPH_HEIGHT;
extern int BG_GLYPH_LENGTH;
extern int FG_GLYPH_HEIGHT;
extern int FG_GLYPH_LENGTH;

extern int option_window_height;
extern int option_window_length;

extern int IM_SET;

extern int FOREGROUND;
extern int BACKGROUND;
extern int HEADER;
extern int HOLOGRAPHIC;

extern int ROW;
extern int COL;
extern int CACHE;

extern clock_t LAST_INPUT_TIME;
extern screen_size WIN_SIZE;
extern clock_t WAIT_START;

//extern const animation_option start_animation;
extern char HOLD_CHAR;

extern int GLITCH_TIME;

extern char global_chars[];
extern char commandkeys_chars[];
extern char builtins_chars[];
extern char menukeys_chars[];

extern int commandkeys_len;
extern int builtins_len;
extern int menukeys_len;

extern int KEY_LOCK;

static const Menu END_OF_MENU = {"END_OF_MENU", COMMAND, {.command={0,"eww_dont_look_at_me",WAIT,none,none,{0} }}};

#endif
