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

static const char *test_cmd[] = { "mkdir", "TEST_DIR_PLS_DELETE", NULL};
static const char *make_cmd[] = { "make", NULL};

/* TODO: issue a warning if duplicates or crash if necessary */
/* TODO: implement pshd, option_windows */
static const Command commandkeys[] = {
/*  triggerkey   command      wait_option      pre_animation,   post animation  */
  { 'a',         test_cmd,           WAIT,     neon_reverse,    neon },
  { 'm',         make_cmd,    WAIT_ON_ERR,     none,            shutter_slide },
};


/* BUILTIN FUNCTIONS */

static const Builtin builtins[] = {
  { 'I', randomize_colors },
  { 'i', invert_colors },
  { 'H', rave },
  { 'p', pshd },
  { 'q', quit },
};


/* MENUS AND SUBMENUS */

static const char *connect_xm5_cmd[] = { "bluetoothctl", "connect", "AC:80:0A:19:89:A8", NULL };
static const MenuCommand connect_xm5 = { connect_xm5_cmd, WAIT_ON_ERR, none, shutter_slide };

static const char *connect_acoustic_cmd[] = { "bluetoothctl", "connect", "FC:58:FA:9B:D7:3D", NULL };
static const MenuCommand connect_acoustic = { connect_acoustic_cmd, WAIT_ON_ERR, none, shutter_slide };

static const char *connect_onforu_cmd[] = { "bluetoothctl", "connect", "31:51:27:F9:1D:62", NULL };
static const MenuCommand connect_onforu = { connect_onforu_cmd, WAIT_ON_ERR, none, shutter_slide };

static const char *disconnect_xm5_cmd[] = { "bluetoothctl", "disconnect", "AC:80:0A:19:89:A8", NULL };
static const MenuCommand disconnect_xm5 = { disconnect_xm5_cmd, WAIT_ON_ERR, none, shutter_slide };

static const char *disconnect_acoustic_cmd[] = { "bluetoothctl", "disconnect", "FC:58:FA:9B:D7:3D", NULL };
static const MenuCommand disconnect_acoustic = { disconnect_acoustic_cmd, WAIT_ON_ERR, none, shutter_slide };

static const char *disconnect_onforu_cmd[] = { "bluetoothctl", "disconnect", "31:51:27:F9:1D:62", NULL };
static const MenuCommand disconnect_onforu = { disconnect_onforu_cmd, WAIT_ON_ERR, none, shutter_slide };

static const char *fake_backlight_cmd[] = { "mkdir", "FAKE_BACKLIGHT_CMD", NULL };
static const MenuCommand fake_backlight = { fake_backlight_cmd, WAIT_ON_ERR, none, shutter_slide };

static const Menu connect_devices[] = {
  { "XM5",      COMMAND, connect_xm5           },
  { "ACOUSTIC", COMMAND, connect_acoustic      },
  { "ONFORU",   COMMAND, connect_onforu        },
};

static const Menu disconnect_devices[] = {
  { "XM5",      COMMAND, disconnect_xm5        },
  { "ACOUSTIC", COMMAND, disconnect_acoustic   },
  { "ONFORU",   COMMAND, disconnect_onforu     },
};

static const Menu connect_options[] = {
  { "connect",    SUBMENU, connect_devices     },
  { "disconnect", SUBMENU, disconnect_devices  },
};

static const Menu b_menu[] = {
  { "bluetooth", SUBMENU, connect_options      },
  { "backlight", COMMAND, fake_backlight       },
};

static const TopMenu menukeys[] = {
  {'b', b_menu },
};

#endif
