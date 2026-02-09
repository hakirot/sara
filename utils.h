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

int is_char_in_search(wchar_t wc, wchar_t * search_str);
void print_clear_terminal();
void init_window();
void error(char * err);
void get_helped();
void ensure_cache_dir();
void ensure_path_perm(char * file_path, char perm);

#endif
