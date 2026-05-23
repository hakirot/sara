/*
--  ███████╗ █████╗ ██████╗  █████╗   --
--  ██╔════╝██╔══██╗██╔══██╗██╔══██╗  --
--  ███████╗███████║██████╔╝███████║  --
--  ╚════██║██╔══██║██╔══██╗██╔══██║  --
--  ███████║██║  ██║██║  ██║██║  ██║  --
--  ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝.h--

   . Active branches
     | main
        - tackle-loose-todos 

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

#include "globals.h"

struct file_node {
  char name[256];
  struct file_node * next;
};

int check_size();
screen_size get_win_size();
void printstandard();
int input_color(char * arg);
void _pshd();
void _reprint_pshd(int dim_y, int dim_x, int offset_y, int offset_x, int selection, FILE* file, int count);
void ensure_path_perm(char * file_path, char perm);
void __interrupt__(int signum);

#endif
