/*
--  ██╗   ██╗████████╗██╗██╗     ███████╗  --
--  ██║   ██║╚══██╔══╝██║██║     ██╔════╝  --
--  ██║   ██║   ██║   ██║██║     ███████╗  --
--  ██║   ██║   ██║   ██║██║     ╚════██║  --
--  ╚██████╔╝   ██║   ██║███████╗███████║  --
--   ╚═════╝    ╚═╝   ╚═╝╚══════╝╚══════╝.h--
                                               */

#ifndef UTILS_H
#define UTILS_H

#include <wchar.h>
#include "globals.h"

typedef struct {
  char i;
} key_arg ;

void _check();
void version();
int  _confirm();
void _get_pwd();
int  _resized();
void _get_dims();
void _run_menu();
void _path_run();
void _so_random();
char* _run_fetch();
void _init_state();
int  _slap_timer();
void _toggle_rave();
void _show_colors();
void special_chars();
void launch_window();
void _invert_colors();
void crit(char * err);
void warn(char * err);
void slap(char * err);
void _write_exit_dir();
void _deflect_signals();
int  __key(key_arg arg);
void _preflight_check();
int  ensure_cache_dir();
void _reset_glyph_state();
void _randomize_colors();
int  roll(int num_sides);
void ensure_config_dir();
void _quit(int exit_code);
void load_command_config();
void set_glyph_dimensions();
void __command__(char input);
void __builtin__(char input);
void __topmenu__(char input);
void generate_path_run_file();
int  key(char optional_input);
void __interrupt__(int signum);
void get_helped(int exit_code);
void _chdir(char * target_dir);
int  _run_exec(char * selection);
void _args(int argc, char* argv[]);
void _menuselect(const Menu * menu);
int  _deduce_dim_x(int requested_dim_x);
void _free_range(const Command * command);
int  __execute__(const Command * command);
int  _get_confirm(const Command * command);
int  _get_num_fetch_lines(char* fetch_path);
int  _is_binary_in_path(const char * binary);
int  _deduce_dim_y(int requested_dim_y, int len);
Command * ranger_command(const Command * command);
void ensure_path_perm(char * file_path, char perm);
int  _deduce_offset_y(int dim_y, int requested_offset_y);
int  _deduce_offset_x(int dim_x, int requested_offset_x);
int  _check_menu(const Menu * menu_ptr, clock_t start_time);
int  _run_args(int offset_y, int offset_x, char * selection);
int  _execute_run_args(char * selection, char * args_buffer);
int  is_char_in_search(wchar_t wc, const wchar_t * search_str);
void _run_args_refresh(int offset_y, int offset_x, char *selection, char * args_buffer);
void _populate_run_body(int dim_y, int dim_x, int offset_y, int offset_x, char * filter_buffer, char * selection);
void _print_menu_selection(const Menu * menu, int selection, int len, int dim_y, int dim_x, int offset_y, int offset_x);

#endif
