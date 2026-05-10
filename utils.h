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

int  __key__();
void __command__(char input);
void __builtin__(char input);
void __menu__(char input);
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
void _quit();

#endif
