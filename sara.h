
/*
--  ███████╗ █████╗ ██████╗  █████╗   --
--  ██╔════╝██╔══██╗██╔══██╗██╔══██╗  --
--  ███████╗███████║██████╔╝███████║  --
--  ╚════██║██╔══██║██╔══██╗██╔══██║  --
--  ███████║██║  ██║██║  ██║██║  ██║  --
--  ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝.h --
                       2025 PUNKMONK  -- */

/* FEATLIST

    . Add options to change the colors (and a random option maybe?)
    . Add -s --small options to force small size
    . More boot animations
    . Add char input 'b' -> prompt for laptop brightness
                         -> and/or bluetooth
    . mega_glitch()
    . More Interval animations
        > Double SARA + clear() + Arch + SARA + SPECIAL APPLICATION RANGING AREA
        > Wide (fullscreen) glitch
        > Simple Blink (backdrop -> sarafill -> backdrop -> sarafill)
    . Use archsarazap
    . Create input for alternate acronyms from headers (low priority)
    . Replace char input 'c' (with what ..? .. there was something)

   BESTIARY
    X Add char input 'n' -> prompt for newlook
    X FIX neon animation pause 
    X Add char input 'p' -> fork polybar restart
    X Add char input 'g' -> git status
    X Add char input 't' -> nvim open tasks
    X Display colors
    X Add pattern of color characters
    X Add char input 'p' -> run polybar as forked process
    X Add char input 'g' -> git status
    X Add char input 'w' -> exec ranger $HOME/pix/wall/
    X Add char input 'r' -> exec ranger
    X Colored
    X Add Arch logo
    X Glitch animation
    X Display chars
    X Dynamically resize with window
    X Press q to exit
*/

#include <wchar.h>

#ifndef SARA_H
#define SARA_H

typedef enum {
  SMALL,
  NORMAL,
  BIG
} screen_size;

typedef enum {
  QUICK,
  STANDARD,
  NEON,
  EMPTY
} start_animation;

void get_helped();
void check_char(int row, int col);
int check_size(int row, int col, int cache);
int is_char_in_search(wchar_t wc);
int get_confirmation(int row, int col);
void print_start_animation(int row, int col);
void printstandard(int row, int col);
void quickprint(int row, int col, int printColorbar);
void neon(int row, int col);
void glitch(int row, int col);
void mega_glitch(int row, int col);

const int BIG_GLYPH_HEIGHT = 19;
const int BIG_GLYPH_LENGTH = 44;
const int NORMAL_GLYPH_HEIGHT = 7;
const int NORMAL_GLYPH_LENGTH = 44;

#endif
