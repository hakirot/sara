/*
--  ███████╗ █████╗ ██████╗  █████╗   --
--  ██╔════╝██╔══██╗██╔══██╗██╔══██╗  --
--  ███████╗███████║██████╔╝███████║  --
--  ╚════██║██╔══██║██╔══██╗██╔══██║  --
--  ███████║██║  ██║██║  ██║██║  ██║  --
--  ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝.h--

   . ueberzugpp newlook
   . sara -c
   . DNS server
   . More animations
       > Sparkle
       > Simple Blink (backdrop -> sarafill -> backdrop -> sarafill)
       > Scan (xray but single direction)
   . Fix WAIT_BUFFER inconsistency
   . mega_glitch()
   . Break relevant functions into new files (utils, animations, pshd)
   . Load colors/randomness from config file
   . input 'c' to show term colors and enter COMMAND MODE
   . Add -s --small options to force small size
   . More boot animations
   . mega_glitch()
   . Use archsarazap
   . Create input for alternate acronyms from headers (low priority)
   . Abstracted functions, config option selection, and variable compilation
*/

#include <wchar.h>

#ifndef SARA_H
#define SARA_H

struct file_node {
  char name[256];
  struct file_node * next;
};

void check_char();
int check_size();
int get_confirmation();
void print_start_animation();
void printstandard();
void quickprint(int fg, int bg, int printColorbar);
void neon();
void neon_reverse();
void glitch(int numFrames, int bigmode);
void mega_glitch(int duration);
void xray();
const char * select_option_window(char** choices, int len);
void patch_backlight();
void patch_border();
void prompt_newlook();
int input_color(char * arg);
void pshd();
void ensure_path_perm(char * file_path, char perm);
int generate_pw_file();
char * prompt_fuzzy();
void fork_newlook(char * file);
struct file_node * init_dir_list(char * dir);
void free_dir_list(struct file_node * list);

#endif
