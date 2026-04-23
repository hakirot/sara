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
int FOLLOW = true;

/* COLORS */
int FOREGROUND = RED;
int BACKGROUND = GREEN;

int use_bold_color_for_bg = true;
int use_bold_color_for_fg = false;

// why do I not need to define these in the header file
int use_fg_color_for_hd_as_bg = true;  // ensure both are not true
int use_bg_color_for_hd_as_bg = false; // ensure both are not true

int GLITCH_FRAME_TIME = 3500;
float HOLD_CHAR_TIME = 0.00001;

int HOLOGRAPHIC = false;


/* CUSTOM GLYPHS */ 

        /* RESTRICTIONS 

           Glyphs must contain distinct characters
           from other glyphs

           Ensure strings in character array are equal length */


/* background glyph */
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


/* foreground glyph */
const char * fg[] = {
  "███████╗    █████╗    ██████╗     █████╗    ",
  "██╔════╝   ██╔══██╗   ██╔══██╗   ██╔══██╗   ",
  "███████╗   ███████║   ██████╔╝   ███████║   ",
  "╚════██║   ██╔══██║   ██╔══██╗   ██╔══██║   ",
  "███████║██╗██║  ██║██╗██║  ██║██╗██║  ██║██╗",
  "╚══════╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝",
};


/* optional intermediate glyph */
const char * im[7] = {
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
const char hd[] = "SPECIAL APPLICATION RANGING AREA";
/* Set empty if desired */
// const char hd[] = '\0';


/* POSITION */
/* Reposition glyphs here
   - By default, all glyphs are printed in center of screen
   - Positive x values will position glyphs DOWNWARD
   - Negative x values will position glyphs UPWARD
*/
int bg_offset_x = 0;
int bg_offset_y = 0;
int fg_offset_x = 0;
int fg_offset_y = 2;
int hd_offset_x = 2;
int hd_offset_y = -4;

/* Toggle if you want to remove bg glyph when not fitting */
int dynamic_resize = true;
int resize_width = 44;
int resize_height = 19;


/* automate this and perform checks [make a perform check cmd-line option]
     - ensure no duplicate chars */
    
wchar_t FG_STR[] = L"█╔╗╚╝║═";
wchar_t BG_STR[] = L"`+so:-./";

#endif
