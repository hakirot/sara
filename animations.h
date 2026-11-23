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

#define asc_000 ascend
#define asc_100 ascend_h
#define asc_010 ascend_m
#define asc_110 ascend_m_h
#define asc_001 ascend_t
#define asc_101 ascend_h_t
#define asc_011 ascend_m_t
#define asc_111 ascend_m_h_t

#define ascend(a, b, c) ascend_expand(a, b, c)
#define ascend_expand(a, b, c) asc_##a##b##c

void _bg();
void _neon();
void _holo();
void _blink();
void _fetch();
void _a_all();
void _splode();
void _multiply();
void _fx_waves();
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
void _waves(Arg full);
void _glimmer(Arg full);
void _overlay_bg(char ch);
void _glitch(Arg bigmode);
void _print_confirm_window();
void _none(Arg printColorbar);
void _hd(int off_y, int off_x);
void _mega_static(Arg reverse);
void _fg(const char * glyph[]);
void animate(animation option);
void _shift_arr(int arr[], int size);
void _ascend(Arg home, Arg mini, Arg more);
void _print_confirm_selection(int selection);
void _fps_sleep(struct timespec * next_frame);
void _overlay(const char * glyph[], char fill);
void _bg_offset(char ch, int offset_y, int offset_x);
void _fg_offset(const char * glyph[], int offset_y, int offset_x);
void _inc_nanosecs(struct timespec *timespec, long long nanosecs);
void _clear_menu(int dim_y, int dim_x, int offset_y, int offset_x);
void _print_menu_borders(int dim_y, int dim_x, int offset_y, int offset_x, int c);

#endif
