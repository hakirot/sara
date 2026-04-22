/*
--   ██████╗ ██████╗ ███╗   ██╗███████╗██╗ ██████╗   --
--  ██╔════╝██╔═══██╗████╗  ██║██╔════╝██║██╔════╝   --
--  ██║     ██║   ██║██╔██╗ ██║█████╗  ██║██║  ███╗  --
--  ██║     ██║   ██║██║╚██╗██║██╔══╝  ██║██║   ██║  --
--  ╚██████╗╚██████╔╝██║ ╚████║██║     ██║╚██████╔╝  --
--   ╚═════╝ ╚═════╝ ╚═╝  ╚═══╝╚═╝     ╚═╝ ╚═════╝.h --
                                                             */

#include "globals.h"
#include <wchar.h>

#ifndef CONFIG_H
#define CONFIG_H

/* COLORS */

int FOREGROUND = RED;
int BACKGROUND = GREEN;

bool use_bold_color_for_bg = true;
bool use_bold_color_for_fg = false;

bool HOLOGRAPHIC = false;
bool FOLLOW = true;

int GLITCH_FRAME_TIME = 3500;
float HOLD_CHAR_TIME = 0.00001;


/* CUSTOM GLYPHS */ 

        /* RESTRICTIONS 

           Glyphs must contain distinct characters
           from each other glyph

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


/* OFFSETS */

/* Reposition glyphs here
   - By default, all glyphs are printed in center of screen
   - Positive x values will reposition glyphs DOWN
   - Negative x values will reposition glyphs UP
*/
int bg_offset_x = 0;
int bg_offset_y = 0;
int fg_offset_x = 0;
int fg_offset_y = 0;
int hd_offset_x = 2;
int hd_offset_y = -4;


/* automate this and perform checks [make a perform check cmd-line option]
     - ensure no duplicate chars */
    
wchar_t FG_STR[] = L"█╔╗╚╝║═";
wchar_t BG_STR[] = L"`+so:-./";

#endif
