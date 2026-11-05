/*
--   █████╗ ███╗   ██╗██╗███╗   ███╗ █████╗ ████████╗██╗ ██████╗ ███╗   ██╗███████╗  --
--  ██╔══██╗████╗  ██║██║████╗ ████║██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║██╔════╝  --
--  ███████║██╔██╗ ██║██║██╔████╔██║███████║   ██║   ██║██║   ██║██╔██╗ ██║███████╗  --
--  ██╔══██║██║╚██╗██║██║██║╚██╔╝██║██╔══██║   ██║   ██║██║   ██║██║╚██╗██║╚════██║  --
--  ██║  ██║██║ ╚████║██║██║ ╚═╝ ██║██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║███████║  --
--  ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝     ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝.h--
                                                                                       */
#include "globals.h"

#ifndef ANIMATIONS_H
#define ANIMATIONS_H

void animate(animation option);
void _none(Arg printColorbar);
void _glitch(Arg bigmode);
void _shutter_slide();
void _neon_reverse();
void _down_wipes();
void _pixel_fill();
void _tv_static();
void _neon();
void _bg();
void _hd();
void _blink();
void _glint();
void _colorbar();
void tinyprint();
void _overlay_bg(char ch);
void _fg(const char * glyph[]);
void _print_confirm_selection(int selection);
void _print_confirm_window(char * command_str);
void _overlay(const char * glyph[], char fill);
void _clear_menu(int dim_y, int dim_x, int offset_y, int offset_x);
void _print_menu_borders(int dim_y, int dim_x, int offset_y, int offset_x, int c);

#endif
