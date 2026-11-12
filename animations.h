#ifndef ANIMATIONS_H
#define ANIMATIONS_H

/*
--   █████╗ ███╗   ██╗██╗███╗   ███╗ █████╗ ████████╗██╗ ██████╗ ███╗   ██╗███████╗  --
--  ██╔══██╗████╗  ██║██║████╗ ████║██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║██╔════╝  --
--  ███████║██╔██╗ ██║██║██╔████╔██║███████║   ██║   ██║██║   ██║██╔██╗ ██║███████╗  --
--  ██╔══██║██║╚██╗██║██║██║╚██╔╝██║██╔══██║   ██║   ██║██║   ██║██║╚██╗██║╚════██║  --
--  ██║  ██║██║ ╚████║██║██║ ╚═╝ ██║██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║███████║  --
--  ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝     ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝.h--
                                                                                       */
#include "globals.h"

#define glint_f   1
#define glint_b   2
#define glint_a   3

void _bg();
void _hd();
void _neon();
void _blink();
void _fetch();
void _colorbar();
void tinyprint();
void _tv_static();
void _down_wipes();
void _pixel_fill();
void _affect_holo();
void _glint(fx mode);
void _affect(int fx);
void _neon_reverse();
void _shutter_slide();
void _overlay_bg(char ch);
void _glitch(Arg bigmode);
void _none(Arg printColorbar);
void _fg(const char * glyph[]);
void animate(animation option);
void _print_confirm_selection(int selection);
void _print_confirm_window(char * command_str);
void _overlay(const char * glyph[], char fill);
void _bg_offset(char ch, int offset_y, int offset_x);
void _fg_offset(const char * glyph[], int offset_y, int offset_x);
void _inc_nanosecs(struct timespec *timespec, long long nanosecs);
void _clear_menu(int dim_y, int dim_x, int offset_y, int offset_x);
void _print_menu_borders(int dim_y, int dim_x, int offset_y, int offset_x, int c);

#endif
