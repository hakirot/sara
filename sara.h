/*
--  ███████╗ █████╗ ██████╗  █████╗   --
--  ██╔════╝██╔══██╗██╔══██╗██╔══██╗  --
--  ███████╗███████║██████╔╝███████║  --
--  ╚════██║██╔══██║██╔══██╗██╔══██║  --
--  ███████║██║  ██║██║  ██║██║  ██║  --
--  ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝.h--

   . Active branches
     | main
       - ueberzug-integration
         . Replacement for fuzzy newlook option
         . Measure screen and generate thumbnails based on config wall_dir
       - allow-custom-glyphs
         . Use doopstr test function to create the third search_str for title
         . Abstract search_str vars to config
         . All animations derivable from fg_chars, bg_chars, title
         . change BIG_GLYPH* to FULL_GLYPH*
         . abstract GLYPH_HEIGHT variables to measure glyph sizes on startup
         . create options and toggles for header colors

   . sleep() and usleep() functions affect measured program time T_T
   . sara -c
   . DNS server
   . More animations
       > Sparkle
       > Simple Blink (backdrop -> sarafill -> backdrop -> sarafill)
       > Scan (xray but single direction)
   . Fix WAIT_BUFFER inconsistency
   . mega_glitch()
   . Break relevant functions into new files (utils, animations, pshd)
   . input 'c' to show term colors and enter COMMAND MODE
   . Add -s --small options to force small size
   . More boot animations
   . mega_glitch()
   . Use archsarazap
   . Create input for alternate acronyms from headers (low priority)
*/

#ifndef SARA_H
#define SARA_H

struct file_node {
  char name[256];
  struct file_node * next;
};

int check_char();
int check_size();
int get_confirmation();
void print_start_animation();
void printstandard();
const char * select_option_window(char** choices, int len);
void patch_backlight();
void patch_border();
void prompt_newlook();
int input_color(char * arg);
void pshd();
void ensure_path_perm(char * file_path, char perm);
int generate_pw_file();
char * prompt_fuzzy();
char * theme_select();
void fork_newlook(char * file);
struct file_node * init_dir_list(char * dir);
void free_dir_list(struct file_node * list);

#endif
