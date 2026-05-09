/*
--  ███████╗ █████╗ ██████╗  █████╗   --
--  ██╔════╝██╔══██╗██╔══██╗██╔══██╗  --
--  ███████╗███████║██████╔╝███████║  --
--  ╚════██║██╔══██║██╔══██╗██╔══██║  --
--  ███████║██║  ██║██║  ██║██║  ██║  --
--  ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝.h--

   . Active branches
     | main
        - allow-custom-commands
          . Build commands from a struct array

   . sleep() and usleep() functions halt timings using clock()
       https://stackoverflow.com/questions/22661300/calculate-execution-time-when-sleep-is-used
   . sara -c
   . DNS server
   . More animations
       > Sparkle
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
void printstandard();
const char * select_option_window(char* choices[], int len);
void patch_backlight();
void patch_border();
void prompt_newlook();
int input_color(char * arg);
void _pshd();
void ensure_path_perm(char * file_path, char perm);
int generate_pw_file();
char * prompt_fuzzy();
char * theme_select();
void fork_newlook(char * file);
struct file_node * init_dir_list(char * dir);
void free_dir_list(struct file_node * list);

#endif
