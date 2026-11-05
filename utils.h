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

void  _print_menu_selection(const Menu * menu, int selection, int len, int dim_y, int dim_x, int offset_y, int offset_x);
int   is_char_in_search(wchar_t wc, const wchar_t * search_str);
int   _deduce_offset_y(int dim_y, int requested_offset_y);
int   _deduce_offset_x(int dim_x, int requested_offset_x);
void  ensure_path_perm(char * file_path, char perm);
int   _get_confirm(const Command * command);
void  _free_range(const Command * command);
int   __execute__(const Command * command);
void  _menuselect(const Menu * menu);
void  get_helped(int exit_code);
void  _chdir(char * target_dir);
int   key(char optional_input);
void  __command__(char input);
void  __builtin__(char input);
void  __topmenu__(char input);
void  load_command_config();
void  ensure_config_dir();
int   __key(key_arg arg);
void  _preflight_check();
int   ensure_cache_dir();
void  warn(char * err);
void  slap(char * err);
void  crit(char * err);
void  launch_window();
int   _slap_timer();
void  _run_menu();
int   _confirm();
void  version();
void  _check();
void  _get_pwd();
void  _path_run();
void _init_state();
void  _toggle_rave();
void  _show_colors();
void  special_chars();
void  _invert_colors();
void  _write_exit_dir();
void reset_glyph_state();
void  _deflect_signals();
int   roll(int num_sides);
void  _randomize_colors();
void  _quit(int exit_code);
void  set_glyph_dimensions();
void  __interrupt__(int signum);
void  generate_path_run_file();
int   _run_exec(char * selection);
int   _check_menu(const Menu * menu_ptr, clock_t start_time);
void  _basic_args(int argc, char* argv[]);
int   _is_binary_in_path(const char * binary);
Command * ranger_command(const Command * command);
int   _run_args(int offset_y, int offset_x, char * selection);
int   _execute_run_args(char * selection, char * args_buffer);
void  _run_args_refresh(int offset_y, int offset_x, char *selection, char * args_buffer);
void  _populate_run_body(int dim_y, int dim_x, int offset_y, int offset_x, char * filter_buffer, char * selection);

#endif
