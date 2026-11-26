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

#define asc_00000 ascend
#define asc_10000 ascend_h
#define asc_01000 ascend_m
#define asc_11000 ascend_m_h
#define asc_00100 ascend_t
#define asc_10100 ascend_h_t
#define asc_01100 ascend_m_t
#define asc_11100 ascend_m_h_t
#define asc_20000 ascend_n
#define asc_21000 ascend_m_n
#define asc_20100 ascend_t_n
#define asc_21100 ascend_m_t_n

#define asc_00010 ascend_l
#define asc_10010 ascend_h_l
#define asc_01010 ascend_m_l
#define asc_11010 ascend_m_h_l
#define asc_00110 ascend_t_l
#define asc_10110 ascend_h_t_l
#define asc_01110 ascend_m_t_l
#define asc_11110 ascend_m_h_t_l
#define asc_20010 ascend_n_l
#define asc_21010 ascend_m_n_l
#define asc_20110 ascend_t_n_l
#define asc_21110 ascend_m_t_n_l

#define asc_30000 ascend_s
#define asc_31000 ascend_m_s
#define asc_30100 ascend_t_s
#define asc_31100 ascend_m_t_s
#define asc_30010 ascend_l_s
#define asc_31010 ascend_m_l_s
#define asc_30110 ascend_t_l_s
#define asc_31110 ascend_m_t_l_s

#define asc_00001 ascend_r
#define asc_10001 ascend_h_r
#define asc_01001 ascend_m_r
#define asc_11001 ascend_m_h_r
#define asc_00101 ascend_t_r
#define asc_10101 ascend_h_t_r
#define asc_01101 ascend_m_t_r
#define asc_11101 ascend_m_h_t_r
#define asc_20001 ascend_n_r
#define asc_21001 ascend_m_n_r
#define asc_20101 ascend_t_n_r
#define asc_21101 ascend_m_t_n_r
#define asc_00011 ascend_l_r
#define asc_10011 ascend_h_l_r
#define asc_01011 ascend_m_l_r
#define asc_11011 ascend_m_h_l_r
#define asc_00111 ascend_t_l_r
#define asc_10111 ascend_h_t_l_r
#define asc_01111 ascend_m_t_l_r
#define asc_11111 ascend_m_h_t_l_r
#define asc_20011 ascend_n_l_r
#define asc_21011 ascend_m_n_l_r
#define asc_20111 ascend_t_n_l_r
#define asc_21111 ascend_m_t_n_l_r
#define asc_30001 ascend_s_r
#define asc_31001 ascend_m_s_r
#define asc_30101 ascend_t_s_r
#define asc_31101 ascend_m_t_s_r
#define asc_30011 ascend_l_s_r
#define asc_31011 ascend_m_l_s_r
#define asc_30111 ascend_t_l_s_r
#define asc_31111 ascend_m_t_l_s_r

#define ascend(a, b, c, d, e) ascend_expand(a, b, c, d, e)
#define ascend_expand(a, b, c, d, e) asc_##a##b##c##d##e

#define yes           1
#define YES           1
#define Y             1
#define no            0
#define NO            0
#define N             0

#define BG            1
#define FG            2
#define ALL           3

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
#define rise          1
#define fall          0
#define O             omit
#define NT            no_travel

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
void if_ascend(animation option);
void _shift_arr(int arr[], int size);
void _print_confirm_selection(int selection);
void holo_if_set(int offset_y, int offset_x);
void _fps_sleep(struct timespec * next_frame);
void glint_if_set(int offset_y, int offset_x);
void _overlay(const char * glyph[], char fill);
void _affect(int fx, int offset_y, int offset_x);
void _glint(fx mode, int offset_y, int offset_x);
void _bg_offset(char ch, int offset_y, int offset_x);
void _fg_offset(const char * glyph[], int offset_y, int offset_x);
void _inc_nanosecs(struct timespec *timespec, long long nanosecs);
void _clear_menu(int dim_y, int dim_x, int offset_y, int offset_x);
void _ascend(int home, int mini, int more, int lingering, int rising);
void _print_menu_borders(int dim_y, int dim_x, int offset_y, int offset_x, int c);
int _print_fetch(char * fetch_path, int entries, int wait, int fg_c, int bg_c, int hd_c);

#endif
