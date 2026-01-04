
/*
--  ███████╗ █████╗ ██████╗  █████╗   --
--  ██╔════╝██╔══██╗██╔══██╗██╔══██╗  --
--  ███████╗███████║██████╔╝███████║  --
--  ╚════██║██╔══██║██╔══██╗██╔══██║  --
--  ███████║██║  ██║██║  ██║██║  ██║  --
--  ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝.h--
                                           */

/* FEATLIST

    . Do a perms check on the backlight file, make writeable if needed
    . More animations
        > Sparkle
        > Simple Blink (backdrop -> sarafill -> backdrop -> sarafill)
        > Scan (xray but single direction)
    . Fix the inaccurate WAIT_BUFFER time muckery
    . Fuzzy find menu for newlook
        . Build directory list
    . mega_glitch()
    . pshd()
    . Break functions into new files
    . Make row/col global
    . Load colors/randomness from config file
    . input 'c' to show term colors and enter COMMAND MODE
    . 'w' creates a menu

    . Add -s --small options to force small size
    . More boot animations
    . mega_glitch()
    . Use archsarazap
    . Create input for alternate acronyms from headers (low priority)
    . Replace char input 'c' (with what ..? .. there was something)
    . Eventually reengineer to allow for abstracted hookups, single config, etc

   BESTIARY
    X rtorrent command (launch with cd function refactor)
    X Shutdown command
    X Recheck size before neon() call
    X Follow ranger through the directories
    X add char input 'i' -> invert)
    X Holographic arch (background arch colors cycle 1-7)
    X Random startup colors
    X More animations (xray)
    X Add command-line args for fg and bg
    X Add menues for 'b' prompt (bluetooth and backlight)
    X Create a cooler newlook prompt
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

const int BIG_GLYPH_HEIGHT = 19;
const int BIG_GLYPH_LENGTH = 44;
const int NORMAL_GLYPH_HEIGHT = 7;
const int NORMAL_GLYPH_LENGTH = 44;

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
int is_char_in_search(wchar_t wc, wchar_t * search_str);
int get_confirmation(int row, int col);
void print_start_animation(int row, int col);
void printstandard(int row, int col);
void quickprint(int row, int col, int fg, int bg, int printColorbar);
void neon(int row, int col);
void glitch(int row, int col);
void mega_glitch(int row, int col, int duration);
void xray(int row, int col);
const char * select_option_window(int row, int col, char** choices, int len);
void patch_backlight(int row, int col);
void patch_border(int row, int col);
void prompt_newlook(int row, int col);
int input_color(char * arg);
void error(char * err);
void pshd(int row, int col);
void init_window();
void ensure_path_perm(char * file_path, char perm);
void set_askpass();

#endif
