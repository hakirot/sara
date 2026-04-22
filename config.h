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


/* Toggle if you use chdir */
bool FOLLOW = true;

/* Toggle if you want to remove bg glyph when not fitting */
int dynamic_resize = true;

/* COLORS */
int FOREGROUND = RED;
int BACKGROUND = GREEN;

bool use_bold_color_for_bg = true;
bool use_bold_color_for_fg = false;

// why do I not need to define these in the header file
bool use_fg_color_for_hd_as_bg = true;  // ensure both are not true
bool use_bg_color_for_hd_as_bg = false; // ensure both are not true

int GLITCH_FRAME_TIME = 3500;
float HOLD_CHAR_TIME = 0.00001;

bool HOLOGRAPHIC = false;


/* CUSTOM GLYPHS */ 

        /* RESTRICTIONS 

           Glyphs must contain distinct characters
           from other glyphs

           Glyphs must be rectangular             */

/* Foreground glyph */
const char * fg[] = {
  "███████╗    █████╗    ██████╗     █████╗    ",
  "██╔════╝   ██╔══██╗   ██╔══██╗   ██╔══██╗   ",
  "███████╗   ███████║   ██████╔╝   ███████║   ",
  "╚════██║   ██╔══██║   ██╔══██╗   ██╔══██║   ",
  "███████║██╗██║  ██║██╗██║  ██║██╗██║  ██║██╗",
  "╚══════╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝",
};


/* Background glyph */
const char * bg[] = {
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
  "        :osssssss/        osssso+++.        ",
  "       /ossssssss/        +ssssooo/-        ",
  "     `/ossssso+/:-        -:/+osssso+-      ",
  "    `+sso+:-`                 `.-/+oso:     ",
  "   `++:.                           `-/+/    ",
  "   .`                                 `/    ",
};


/* Header */              // May be empty if desired
const char hd[] = "SPECIAL APPLICATION RANGING AREA";

/* POSITION */

/* Reposition glyphs here
   - By default, all glyphs are printed in center of screen
   - Positive x values will reposition glyphs DOWN
   - Negative x values will reposition glyphs UP
*/
int bg_offset_x = 0;
int bg_offset_y = 0;
int fg_offset_x = 0;
int fg_offset_y = 2;
int hd_offset_x = 2;
int hd_offset_y = -4;


/* automate this and perform checks [make a perform check cmd-line option]
     - ensure no duplicate chars */
    
wchar_t FG_STR[] = L"█╔╗╚╝║═";
wchar_t BG_STR[] = L"`+so:-./";

#endif
