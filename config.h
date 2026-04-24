#ifndef CONFIG_H
#define CONFIG_H
#include "globals.h"
/*   ██████╗ ██████╗ ███╗   ██╗███████╗██╗ ██████╗   -- */
/*  ██╔════╝██╔═══██╗████╗  ██║██╔════╝██║██╔════╝   --
--  ██║     ██║   ██║██╔██╗ ██║█████╗  ██║██║  ███╗  --
--  ██║     ██║   ██║██║╚██╗██║██╔══╝  ██║██║   ██║  --
--  ╚██████╗╚██████╔╝██║ ╚████║██║     ██║╚██████╔╝  --
--   ╚═════╝ ╚═════╝ ╚═╝  ╚═══╝╚═╝     ╚═╝ ╚═════╝.h --
                                                             */


/* Toggle if you chdir */
static int FOLLOW = true;

/* COLORS */
static const int fg_c = RED;
static const int bg_c = GREEN;

static const int use_bold_color_for_bg = true;
static const int use_bold_color_for_fg = false;

static const int use_fg_c_for_hd_as_bg = true;  // ensure both are not true
static const int use_bg_c_for_hd_as_bg = false; // ensure both are not true

static const int GLITCH_FRAME_TIME = 3500;
static const float HOLD_CHAR_TIME = 0.00001;


/* CUSTOM GLYPHS */ 

        /* RESTRICTIONS 

           Glyphs must contain distinct characters
           from other glyphs

           Ensure strings in character array are equal length */


/* background glyph */
static const char * bg[] = {
  "                     -`                     ",
  "                    .o+`                    ",
  "                   `ooo/                    ",
  "                  `+oooo:                   ",
  "                 `+oooooo:                  ",
  "                 -+oooooo+:                 ",
  "               `/:-:++oooo+:                ",
  "              `/++++/+++++++:               ",
  "             `/++++++++++++++:              ",
  "            `/+++ooooooooooooo/`            ",
  "           ./ooosssso++osssssso+`           ",
  "          .oossssso-````/ossssss+`          ",
  "         -osssssso.      :ssssssso.         ",
  "        : sssssss/        osssso+++.        ",
  "       /ossssssss/        +ssssooo/-        ",
  "     `/ossssso+/:-        -:/+osssso+-      ",
  "    `+sso+:-`                 `.-/+oso:     ",
  "   `++:.                           `-/+/    ",
  "   .`                                 `/    ",
};


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
static const char * im[7] = {
  "╔══════╗    ╔════╗    ╔═════╗     ╔════╗    ",
  "║ ╔════╝   ╔╝╔══╗╚╗   ║ ╔══╗╚╗   ╔╝╔══╗╚╗   ",
  "║ ╚════╗   ║ ╚══╝ ║   ║ ╚══╝╔╝   ║ ╚══╝ ║   ",
  "╚════╗ ║   ║ ╔══╗ ║   ║ ╔══╗╚╗   ║ ╔══╗ ║   ",
  "╔════╝ ║╔═╗║ ║  ║ ║╔═╗║ ║  ║ ║╔═╗║ ║  ║ ║╔═╗",
  "╚══════╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝",
};
/* Set empty if desired */
//const char * im[] = '\0';


/* optional header */
static const char hd[] = "SPECIAL APPLICATION RANGING AREA";
/* Set empty if desired */
// const char hd[] = '\0';


/* POSITION */
/* Reposition glyphs here
   - By default, all glyphs are printed in center of screen
   - Positive x values will position glyphs DOWNWARD
   - Negative x values will position glyphs UPWARD
*/
static const int bg_offset_x = 0;
static const int bg_offset_y = 0;
static const int fg_offset_x = 0;
static const int fg_offset_y = 1;
static const int hd_offset_x = -9;
static const int hd_offset_y = 1;

/* Toggle if you want to only print fg[] glyph at a certain size */
static const int dynamic_resize = true; // ensure this value is greater than tiny mode
static const int resize_width = 44;
static const int resize_height = 19;

/* Tiny mode options */
static const char tn[] = "S.A.R.A";
static const int tiny_mode = true;
static const int tiny_mode_y = 7;
static const int tiny_mode_x = 44;


/* automate this and perform checks [make a perform check cmd-line option]
     - ensure no duplicate chars */

static const wchar_t FG_STR[] = L"█╔╗╚╝║═";
static const wchar_t BG_STR[] = L"`+so:-./";

#endif
