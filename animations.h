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

void quickprint(int fg, int bg, int printColorbar);
void printstandard();
void print_bg();
void print_fg(const char * glyph[]);
void print_header();
void print_overlay(const char * glyph[], char fill);
void glitch(int numFrames, int bigmode);
void neon();
void neon_reverse();
void shutter_slide();
// cycle_length, roll_chance, and usleep_time can all be parameters to change
// behavior TODO: move this description to config
void pixel_fill(double cycle_length, int num_sides, int usleep_time);
void tv_static(double cycle_length);
void xray();
void mega_glitch(int duration);

#endif
