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

#define partix_00000 partix
#define partix_10000 partix_h
#define partix_01000 partix_m
#define partix_11000 partix_m_h
#define partix_00100 partix_t
#define partix_10100 partix_h_t
#define partix_01100 partix_m_t
#define partix_11100 partix_m_h_t
#define partix_20000 partix_n
#define partix_21000 partix_m_n
#define partix_20100 partix_t_n
#define partix_21100 partix_m_t_n

#define partix_00010 partix_l
#define partix_10010 partix_h_l
#define partix_01010 partix_m_l
#define partix_11010 partix_m_h_l
#define partix_00110 partix_t_l
#define partix_10110 partix_h_t_l
#define partix_01110 partix_m_t_l
#define partix_11110 partix_m_h_t_l
#define partix_20010 partix_n_l
#define partix_21010 partix_m_n_l
#define partix_20110 partix_t_n_l
#define partix_21110 partix_m_t_n_l

#define partix_30000 partix_s
#define partix_31000 partix_m_s
#define partix_30100 partix_t_s
#define partix_31100 partix_m_t_s
#define partix_30010 partix_l_s
#define partix_31010 partix_m_l_s
#define partix_30110 partix_t_l_s
#define partix_31110 partix_m_t_l_s

#define partix_00001 partix_r
#define partix_10001 partix_h_r
#define partix_01001 partix_m_r
#define partix_11001 partix_m_h_r
#define partix_00101 partix_t_r
#define partix_10101 partix_h_t_r
#define partix_01101 partix_m_t_r
#define partix_11101 partix_m_h_t_r
#define partix_20001 partix_n_r
#define partix_21001 partix_m_n_r
#define partix_20101 partix_t_n_r
#define partix_21101 partix_m_t_n_r
#define partix_00011 partix_l_r
#define partix_10011 partix_h_l_r
#define partix_01011 partix_m_l_r
#define partix_11011 partix_m_h_l_r
#define partix_00111 partix_t_l_r
#define partix_10111 partix_h_t_l_r
#define partix_01111 partix_m_t_l_r
#define partix_11111 partix_m_h_t_l_r
#define partix_20011 partix_n_l_r
#define partix_21011 partix_m_n_l_r
#define partix_20111 partix_t_n_l_r
#define partix_21111 partix_m_t_n_l_r
#define partix_30001 partix_s_r
#define partix_31001 partix_m_s_r
#define partix_30101 partix_t_s_r
#define partix_31101 partix_m_t_s_r
#define partix_30011 partix_l_s_r
#define partix_31011 partix_m_l_s_r
#define partix_30111 partix_t_l_s_r
#define partix_31111 partix_m_t_l_s_r

#define partix(a, b, c, d, e) partix_expand(a, b, c, d, e)
#define partix_expand(a, b, c, d, e) partix_##a##b##c##d##e



#define partfx_000 partfx
#define partfx_100 partfx_m
#define partfx_010 partfx_t
#define partfx_110 partfx_m_t
#define partfx_001 partfx_r
#define partfx_101 partfx_m_r
#define partfx_011 partfx_t_r
#define partfx_111 partfx_m_t_r

// todo make all these basic ints to decode and scrap the bloat
//#define partix(a, b, c) partix_expand(a, b, c)
//#define partix_expand(a, b, c) a ## b ## c) // returns abcde as int [untested]

#define part_fx(a, b, c) part_fx_expand(a, b, c)
#define part_fx_expand(a, b, c) partfx_##a##b##c

#define YES           1
#define Y             1
#define NO            0
#define N             0

#define BG            1
#define FG            2
#define ALL           3

#define OMIT          2
#define O             OMIT
#define NO_OMIT       0
#define NO_TRAVEL     3
#define NT            NO_TRAVEL
#define GO_AWAY       0
#define GO_HOME       1
#define NO_MINI_MODE  1
#define MINI_MODE     0
#define MORE_PARTX    1
#define LESS_PARTX    0
#define LINGER        1
#define NO_LINGER     0
#define RISE          1
#define FALL          0
#define NO_OFFSET     0

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
void holo_if_set();
void _affect_holo();
void partfx_if_set();
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
void if_partix(animation option);
void _shift_arr(int arr[], int size);
void _affect(int offset_y, int offset_x);
void _print_confirm_selection(int selection);
void _fps_sleep(struct timespec * next_frame);
void glint_if_set(int offset_y, int offset_x);
void _overlay(const char * glyph[], char fill);
void _glint(fx mode, int offset_y, int offset_x);
void _bg_offset(char ch, int offset_y, int offset_x);
void _fg_offset(const char * glyph[], int offset_y, int offset_x);
void _inc_nanosecs(struct timespec *timespec, long long nanosecs);
void _clear_menu(int dim_y, int dim_x, int offset_y, int offset_x);
void _partix(int home, int mini, int more, int lingering, int rising);
void _print_menu_borders(int dim_y, int dim_x, int offset_y, int offset_x, int c);
int _print_fetch(char * fetch_path, int entries, int wait, int fg_c, int bg_c, int hd_c);
int _inc_partix_frame(int rows, int cols, int lazarr[rows][cols], int rising, wchar_t wc[], char ch, int count, int* finish_flag);
#endif

