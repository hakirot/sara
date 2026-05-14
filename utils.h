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
#include <globals.h>

int  __key__();
void __command__(char input);
void __execute__(const Command * command);
void __builtin__(char input);
void __topmenu__(char input);
void _menuselect(const Menu * menu, int dim_y, int dim_x);
void _print_menu_selection(const Menu * menu, int selection, int len, int dim_y, int dim_x);
int _get_confirm(const Command * command);
Command * ranger_command(const Command * command);
void _free_range(const Command * command);
void _chdir(char * target_dir);
int _confirm();
void preflight_check();
void load_command_config();
void warning(char * err);
void crit(char * err);
int  is_char_in_search(wchar_t wc, const wchar_t * search_str);
void print_clear_terminal();
void launch_window();
void get_helped();
void ensure_cache_dir();
void ensure_path_perm(char * file_path, char perm);
int  roll(int num_sides);
void set_glyph_dimensions();
void special_chars();
void _toggle_rave();
void _invert_colors();
void _randomize_colors();
void _check();
void _quit();

#endif
