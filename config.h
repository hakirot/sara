#ifndef CONFIG_H
#define CONFIG_H
#include "globals.h"

/*   ██████╗ ██████╗ ███╗   ██╗███████╗██╗ ██████╗   --
--  ██╔════╝██╔═══██╗████╗  ██║██╔════╝██║██╔════╝   --
--  ██║     ██║   ██║██╔██╗ ██║█████╗  ██║██║  ███╗  --
--  ██║     ██║   ██║██║╚██╗██║██╔══╝  ██║██║   ██║  --
--  ╚██████╗╚██████╔╝██║ ╚████║██║     ██║╚██████╔╝  --
--   ╚═════╝ ╚═════╝ ╚═╝  ╚═══╝╚═╝     ╚═╝ ╚═════╝.h --
                                                                         */

/* Toggle if you chdir */
static int FOLLOW = true;

/* COLOR SETTINGS */
static const int fg_c = white;
static const int bg_c = black;

static const int use_bold_color_for_bg = true;
static const int use_bold_color_for_fg = false;

/* CUSTOM GLYPHS */ 

    /* RESTRICTIONS

       Glyphs must contain distinct characters
       from other glyphs                        TODO is this even true anymore
                                                     (I want it to not be true)
       Ensure strings in character array are equal length 

       im[], if defined, must have identical dimensions to fg[]                       */


/* background glyph */
//static const char * bg[] = {
//  "                     -`                     ",
//  "                    .o+`                    ",
//  "                   `ooo/                    ",
//  "                  `+oooo:                   ",
//  "                 `+oooooo:                  ",
//  "                 -+oooooo+:                 ",
//  "               `/:-:++oooo+:                ",
//  "              `/++++/+++++++:               ",
//  "             `/++++++++++++++:              ",
//  "            `/+++ooooooooooooo/`            ",
//  "           ./ooosssso++osssssso+`           ",
//  "          .oossssso-````/ossssss+`          ",
//  "         -osssssso.      :ssssssso.         ",
//  "        : sssssss/        osssso+++.        ",
//  "       /ossssssss/        +ssssooo/-        ",
//  "     `/ossssso+/:-        -:/+osssso+-      ",
//  "    `+sso+:-`                 `.-/+oso:     ",
//  "   `++:.                           `-/+/    ",
//  "   .`                                 `/    ",
//};

static const char * bg[] = {
  " ████    ████    ████          ",
  "  ████    ████    ████         ",
  "    ███     ███     ███        ",
  "     ███     ███     ███       ",
  "     █ ██    █ ██    █ ██      ",
  "        ██      ██      ██     ",
  "         ██      ██      ██    ",
  "          ██      ██      ██   ",
  "           ██      ██      ██  ",
  "            █       █       █  ",
  "             █       █       █ ",
};

//static const char * bg[] = {
//  "                                  *                   *  ",
//  "                         ✦                     ,         ",
//  "                                ,      *                 ",
//  "                                                        ✦",
//  "    .             ✦           ,           *    .         ",
//  "        .             ✦             ✦                    ",
//  "   *           .                          ✦              ",
//  "           .   ✦        ,         *                      ",
//  "          ,                                              ",
//  "        .                                                ",
//  "  ✦        ,              .                              ",
//  "   *           .                                         ",
//  "         .             .                                 ",
//  "˚          ,      .                                      ",
//  "          .                                              ",
//  "   *            ✦                                        ",
//  "        .                                                ",
//};


/* foreground glyph */
static const char * fg[] = {
  "███████╗    █████╗    ██████╗     █████╗    ",
  "██╔════╝   ██╔══██╗   ██╔══██╗   ██╔══██╗   ",
  "███████╗   ███████║   ██████╔╝   ███████║   ",
  "╚════██║   ██╔══██║   ██╔══██╗   ██╔══██║   ",
  "███████║██╗██║  ██║██╗██║  ██║██╗██║  ██║██╗",
  "╚══════╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝",
};


/* optional intermediate glyph - an effect layer for the following animations:
   - glitch
   - neon
*/
static const char * im[] = {
  "╔══════╗    ╔════╗    ╔═════╗     ╔════╗    ",
  "║ ╔════╝   ╔╝╔══╗╚╗   ║ ╔══╗╚╗   ╔╝╔══╗╚╗   ",
  "║ ╚════╗   ║ ╚══╝ ║   ║ ╚══╝╔╝   ║ ╚══╝ ║   ",
  "╚════╗ ║   ║ ╔══╗ ║   ║ ╔══╗╚╗   ║ ╔══╗ ║   ",
  "╔════╝ ║╔═╗║ ║  ║ ║╔═╗║ ║  ║ ║╔═╗║ ║  ║ ║╔═╗",
  "╚══════╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝",
};
/* Set empty if desired */
//static const char * im[] = {NULL};


/* optional header */
static const char hd[] = "SPECIAL APPLICATION RANGING AREA";
/* Set empty if desired */
//static const char hd[] = {'\0'};


/* POSITION */
/* Reposition glyphs here
   - By default, all glyphs are printed in center of screen
   - Positive x values will position glyphs DOWNWARD
   - Negative x values will position glyphs UPWARD
*/
static const int bg_offset_y = -1;
static const int bg_offset_x = -2;
static const int fg_offset_y = 0;
static const int fg_offset_x = 0;

/* header position when fullsize mode */
static const int hd_offset_y = 0;
static const int hd_offset_x = -9;

/* header position when small window mode */
static const int hd_offset_y_min = 0;
static const int hd_offset_x_min = -9;

/* header highlight options*/
static const int highlight_hd_in_full_mode  = 1;
static const int highlight_hd_in_small_mode = 0;

// TODO make this work (conflicts with highlight options) (maybe just set a color) (or make this a toggle)
static const int use_fg_c_for_hd_as_bg = false;  // ensure both are not true
static const int use_bg_c_for_hd_as_bg = true;   // ensure both are not true

/* Set dynamic_resize to skip printing bg at defined constraints */
static const int dynamic_resize = true; // TODO ensure this value is greater than tiny mode
static const int resize_x = 44;
static const int resize_y = 7;

/* Set tiny_mode to */
static const int tiny_mode = true;
static const char tn[] = "S.A.R.A";
static const int tiny_mode_y = 7;
static const int tiny_mode_x = 44;


/* automate this and perform checks [make a perform check cmd-line option]
     - ensure no duplicate chars */

/* ANIMATION SETTINGS */

/* ANIMATION OPTIONS:

  print
  down_wipes
  glitch
  neon
  neon_reverse
  shutter_slide
  pixel_fill
  tv_static

*/

static const int GLITCH_FRAME_TIME = 3500;
static const float HOLD_CHAR_TIME = 0.00001;

/* CUSTOM COMMANDS */

static const char *test_cmd[] = { "mkdir", "TEST_DIR", NULL};
static const char *make_cmd[] = { "make", NULL};

/* TODO: issue a warning if duplicates or crash if necessary */
static Command commands[] = {
/*  triggerkey   command             wait_option         pre_animation, post animation */
  { 'a',         test_cmd,           WAIT,               neon_reverse,  neon },
  { 'm',         make_cmd,    WAIT_ON_ERR,               neon_reverse,  shutter_slide },
};

#endif
