/*

--  ███████╗ █████╗ ██████╗  █████╗   --
--  ██╔════╝██╔══██╗██╔══██╗██╔══██╗  --
--  ███████╗███████║██████╔╝███████║  --
--  ╚════██║██╔══██║██╔══██╗██╔══██║  --
--  ███████║██║  ██║██║  ██║██║  ██║  --
--  ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝.h--
                                           */

/* FEATS

   . Does 'sara -c' still work?
   . Can I host my own local DNS server?
   . Fuzzy newlook
   . More animations
       > Sparkle
       > Simple Blink (backdrop -> sarafill -> backdrop -> sarafill)
       > Scan (xray but single direction)
   . Fix the inaccurate WAIT_BUFFER time muckery
   . newlook ueberzug selector
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

  BESTIARY
   X Add key input to pshd for quick/fuzzy find
     X When 'char' -> filter lines
     X When 'int' -> start selecting from options
   X 'Q' -> terms sara and current tmux pane
   X 'X' -> kill tmux session
   X Glitch whole glyph before nvim when bigmode
   X pshd()
   X wait on mzk
   X sara prints after nvim exit, also not sure if I'm waiting on that thread
   X Do a perms check on the backlight file, make writeable if needed
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

struct file_node {
  char name[256];
  struct file_node * next;
};


extern int GLYPH_LENGTH;
extern int GLYPH_HEIGHT;
extern int FOREGROUND;
extern int BACKGROUND;
extern int HOLOGRAPHIC;
extern int FOLLOW;
extern int ROW;
extern int COL;
extern int CACHE;

void get_helped();
void check_char();
int check_size();
int is_char_in_search(wchar_t wc, wchar_t * search_str);
int get_confirmation();
void print_start_animation();
void printstandard();
void quickprint(int fg, int bg, int printColorbar);
void neon();
void glitch(int numFrames, int bigmode);
void mega_glitch(int duration);
void xray();
const char * select_option_window(char** choices, int len);
void patch_backlight();
void patch_border();
void prompt_newlook();
int input_color(char * arg);
void error(char * err);
void pshd();
void init_window();
void ensure_path_perm(char * file_path, char perm);
void ensure_cache_dir();
int generate_pw_file();
char * prompt_fuzzy();
void fork_newlook(char * file);
struct file_node * init_dir_list(char * dir);
void free_dir_list(struct file_node * list);

#endif
