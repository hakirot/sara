/*
--   █████╗ ███╗   ██╗██╗███╗   ███╗ █████╗ ████████╗██╗ ██████╗ ███╗   ██╗███████╗  --
--  ██╔══██╗████╗  ██║██║████╗ ████║██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║██╔════╝  --
--  ███████║██╔██╗ ██║██║██╔████╔██║███████║   ██║   ██║██║   ██║██╔██╗ ██║███████╗  --
--  ██╔══██║██║╚██╗██║██║██║╚██╔╝██║██╔══██║   ██║   ██║██║   ██║██║╚██╗██║╚════██║  --
--  ██║  ██║██║ ╚████║██║██║ ╚═╝ ██║██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║███████║  --
--  ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝     ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝.h--
                                                                                       */

#ifndef ANIMATIONS_H
#define ANIMATIONS_H

void print_none(int fg, int bg, int printColorbar);
void print_down_wipes();
void print_glitch(int numFrames, int bigmode);
void print_neon();
void print_neon_reverse();
void print_shutter_slide();
// cycle_length, roll_chance, and usleep_time can all be parameters to change
// behavior TODO: move this description to config
void print_pixel_fill(int usleep_time);
void print_tv_static(double cycle_length);
void print_bg();
void print_fg(const char * glyph[]);
void print_hd();
void print_overlay(const char * glyph[], char fill);


#endif
