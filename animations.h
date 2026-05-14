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

void animate(animation_option option);
void _none(Arg printColorbar);
void _down_wipes();
void _glitch(Arg bigmode);
void _neon();
void _neon_reverse();
void _shutter_slide();
void _pixel_fill();
void _tv_static();
void _bg();
void _fg(const char * glyph[]);
void _hd();
void _overlay(const char * glyph[], char fill);
void _print_menu_borders();
void _print_confirm_window(char * header);
void _print_confirm_selection(int selection);
void _clear_menu();
void tinyprint();

#endif
