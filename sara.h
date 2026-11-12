#ifndef SARA_H
#define SARA_H

/*
--  ███████╗ █████╗ ██████╗  █████╗   --
--  ██╔════╝██╔══██╗██╔══██╗██╔══██╗  --
--  ███████╗███████║██████╔╝███████║  --
--  ╚════██║██╔══██║██╔══██╗██╔══██║  --
--  ███████║██║  ██║██║  ██║██║  ██║  --
--  ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝.h--

   . sleep() and usleep() functions halt timings using clock()
       https://stackoverflow.com/questions/22661300/calculate-execution-time-when-sleep-is-used
*/
#include "globals.h"

#define PROGRAM_NAME "SARA"
#define PROGRAM_VERSION "0.9.5"

int check_size();
screen_size get_win_size();
int input_color(char * arg);
void _pshd();
void _reprint_pshd(int dim_y, int dim_x, int offset_y, int offset_x, int selection, FILE* file, int count);
void __interrupt__(int signum);

#endif
