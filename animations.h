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

#define asc_0000 ascend
#define asc_1000 ascend_h
#define asc_0100 ascend_m
#define asc_1100 ascend_m_h
#define asc_0010 ascend_t
#define asc_1010 ascend_h_t
#define asc_0110 ascend_m_t
#define asc_1110 ascend_m_h_t
#define asc_2000 ascend_n
#define asc_2100 ascend_m_n
#define asc_2010 ascend_t_n
#define asc_2110 ascend_m_t_n

#define asc_0001 ascend_l
#define asc_1001 ascend_h_l
#define asc_0101 ascend_m_l
#define asc_1101 ascend_m_h_l
#define asc_0011 ascend_t_l
#define asc_1011 ascend_h_t_l
#define asc_0111 ascend_m_t_l
#define asc_1111 ascend_m_h_t_l
#define asc_2001 ascend_n_l
#define asc_2101 ascend_m_n_l
#define asc_2011 ascend_t_n_l
#define asc_2111 ascend_m_t_n_l

//#define asc_000 ascend_r
//#define asc_100 ascend_h_r
//#define asc_010 ascend_m_r
//#define asc_110 ascend_m_h_r
//#define asc_001 ascend_t_r
//#define asc_101 ascend_h_t_r
//#define asc_011 ascend_m_t_r
//#define asc_111 ascend_m_h_t_r

#define ascend(a, b, c, d) ascend_expand(a, b, c, d)
#define ascend_expand(a, b, c, d) asc_##a##b##c##d

#define no            0
#define yes           1

#define omit          2
#define no_omit       0
#define no_travel     3
#define go_away       0
#define go_home       1
#define no_mini_mode  1
#define mini_mode     0
#define more_partx    1
#define less_partx    0
#define linger        1
#define no_linger     0

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
void _print_confirm_selection(int selection);
void holo_if_set(int offset_y, int offset_x);
void _fps_sleep(struct timespec * next_frame);
void glint_if_set(int offset_y, int offset_x);
void _overlay(const char * glyph[], char fill);
void _affect(int fx, int offset_y, int offset_x);
void _glint(fx mode, int offset_y, int offset_x);
void _bg_offset(char ch, int offset_y, int offset_x);
void _ascend(Arg home, Arg mini, Arg more, Arg lingering);
void _fg_offset(const char * glyph[], int offset_y, int offset_x);
void _inc_nanosecs(struct timespec *timespec, long long nanosecs);
void _clear_menu(int dim_y, int dim_x, int offset_y, int offset_x);
void _print_menu_borders(int dim_y, int dim_x, int offset_y, int offset_x, int c);

#endif
