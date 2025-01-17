
/*
--  ███████╗ █████╗ ██████╗  █████╗   --
--  ██╔════╝██╔══██╗██╔══██╗██╔══██╗  --
--  ███████╗███████║██████╔╝███████║  --
--  ╚════██║██╔══██║██╔══██╗██╔══██║  --
--  ███████║██║  ██║██║  ██║██║  ██║  --
--  ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝  --
        .. with love .. 2025 PUNKMONK -- */

/* FEATLIST
 *  Boot animations
 *  . Different Interval animations
 *  X Glitch animation
 *  X Display chars
 *  X Dynamically resize with window
 *  X Press q to exit
*/
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

const int MAX_HEIGHT = 19;
const int MAX_LENGTH = 44;
const int MID_HEIGHT = 7;
const int MID_LENGTH = 44;

int LENGTH = 44;
int HEIGHT = 7;

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

clock_t LAST_INPUT_TIME;
screen_size WIN_SIZE;
clock_t WAIT_START;
const double WAIT_BUFFER = 0.04000;
char HOLD_CHAR;
start_animation START_ANIMATION = EMPTY;


char * arch[19][99] = {
  { "                    -`                      " },
  { "                   .o+`                     " },
  { "                  `ooo/                     " },
  { "                 `+oooo:                    " },
  { "                `+oooooo:                   " },
  { "                -+oooooo+:                  " },
  { "              `/:-:++oooo+:                 " },
  { "             `/++++/+++++++:                " },
  { "            `/++++++++++++++:               " },
  { "           `/+++ooooooooooooo/`             " },
  { "          ./ooosssso++osssssso+`            " },
  { "         .oossssso-````/ossssss+`           " },
  { "        -osssssso.      :ssssssso.          " },
  { "       :osssssss/        osssso+++.         " },
  { "      /ossssssss/        +ssssooo/-         " },
  { "    `/ossssso+/:-        -:/+osssso+-       " },
  { "   `+sso+:-`                 `.-/+oso:      " },
  { "  `++:.                           `-/+/     " },
  { "  .`                                 `/     " },
};

char * archsarafull[19][99] = {
  { "                    -`                      " },
  { "                   .o+`                     " },
  { "                  `ooo/                     " },
  { "                 `+oooo:                    " },
  { "                `+oooooo:                   " },
  { "                -+oooooo+:                  " },
  { "              `/:-:++oooo+:                 " },
  { "███████╗    █████╗ /++██████╗     █████╗    " },
  { "██╔════╝   ██╔══██╗+++██╔══██╗   ██╔══██╗   " },
  { "███████╗   ███████║ooo██████╔╝`  ███████║   " },
  { "╚════██║  .██╔══██║o++██╔══██╗+  ██╔══██║   " },
  { "███████║██╗██║ss██║██╗██║  ██║██╗██║  ██║██╗" },
  { "╚══════╝╚═╝╚═╝ss╚═╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝" },
  { "       : SOFTWARE ARCHITECTED RANGING AREA  " },
  { "      /ossssssss/        +ssssooo/-         " },
  { "    `/ossssso+/:-        -:/+osssso+-       " },
  { "   `+sso+:-`                 `.-/+oso:      " },
  { "  `++:.                           `-/+/     " },
  { "  .`                                 `/     " },
};

char * titlefill[7][99] = {
  { "███████╗    █████╗ /++██████╗     █████╗    " },
  { "██╔════╝   ██╔══██╗+++██╔══██╗   ██╔══██╗   " },
  { "███████╗   ███████║ooo██████╔╝`  ███████║   " },
  { "╚════██║  .██╔══██║o++██╔══██╗+  ██╔══██║   " },
  { "███████║██╗██║ss██║██╗██║  ██║██╗██║  ██║██╗" },
  { "╚══════╝╚═╝╚═╝ss╚═╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝" },
  { "         SOFTWARE ARCHITECTED RANGING AREA  " },
};

// length LENGTH
// height HEIGHT
char * title[7][99] = {
  { "███████╗    █████╗    ██████╗     █████╗    " },
  { "██╔════╝   ██╔══██╗   ██╔══██╗   ██╔══██╗   " },
  { "███████╗   ███████║   ██████╔╝   ███████║   " },
  { "╚════██║   ██╔══██║   ██╔══██╗   ██╔══██║   " },
  { "███████║██╗██║  ██║██╗██║  ██║██╗██║  ██║██╗" },
  { "╚══════╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝" },
  { "         SOFTWARE ARCHITECTED RANGING AREA  " },
};

char * backdrop[7][99] = {
  { "╔══════╗    ╔════╗    ╔═════╗     ╔════╗    " },
  { "║ ╔════╝   ╔╝╔══╗╚╗   ║ ╔══╗╚╗   ╔╝╔══╗╚╗   " },
  { "║ ╚════╗   ║ ╚══╝ ║   ║ ╚══╝╔╝   ║ ╚══╝ ║   " },
  { "╚════╗ ║   ║ ╔══╗ ║   ║ ╔══╗╚╗   ║ ╔══╗ ║   " },
  { "╔════╝ ║╔═╗║ ║  ║ ║╔═╗║ ║  ║ ║╔═╗║ ║  ║ ║╔═╗" },
  { "╚══════╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝" },
  { "         SOFTWARE ARCHITECTED RANGING AREA  " },
};

char * foreground[7][99] = {
  { "███████     █████     ██████      █████     " },
  { "██         ██   ██    ██   ██    ██   ██    " },
  { "███████    ███████    ██████     ███████    " },
  { "     ██    ██   ██    ██   ██    ██   ██    " },
  { "███████ ██ ██   ██ ██ ██   ██ ██ ██   ██ ██ " },
  { "                                            " },
  { "         SOFTWARE ARCHITECTED RANGING AREA  " },
};

void checkchar(int row, int col) {

  char input = getch();

  if (input != ERR && input != '\n' && input != EOF && input > 19 && input < 127) {
    if(input == 'q'){
      endwin();
      exit(0);
    } else if (input == 'v') {
      endwin();
      execv("/usr/bin/nvim", NULL);
    } else if (WIN_SIZE != SMALL) {
      LAST_INPUT_TIME = clock();
      HOLD_CHAR = input;
      mvprintw(row/2, col/2, "%c", HOLD_CHAR);
      refresh();
    }
  } 

  double time_since_input = (double)(clock() - LAST_INPUT_TIME) / CLOCKS_PER_SEC;
  if(time_since_input >= 0.00005 && WIN_SIZE != SMALL){
    HOLD_CHAR = '\0';
  }

}

void printstandard(int row, int col){
  for(int i = 0; i < HEIGHT; i++){
    mvprintw(row/2 - 3 + i, (col-LENGTH)/2, "%s", title[i][0]);
    checkchar(row, col);
    if(HOLD_CHAR) mvprintw(row/2, col/2, "%c", HOLD_CHAR);
    refresh();
    usleep(20000);          // Add some sexy timing
  }
}

void quickprint(int row, int col){
  clear();
  if (WIN_SIZE == NORMAL){
    for(int i = 0; i < HEIGHT; i++){
      mvprintw(row/2 - 3 + i, (col-LENGTH)/2, "%s", title[i][0]);
    }
  }
  if (WIN_SIZE == BIG){
    for(int i = 0; i < HEIGHT; i++){
      mvprintw(row/2 - 9 + i, (col-LENGTH)/2, "%s", archsarafull[i][0]);
    }
  }
  if(HOLD_CHAR) mvprintw(row/2, col/2, "%c", HOLD_CHAR);
  refresh();
}

void neon(int row, int col) {

  clock_t cycle_start = clock();
  double cycle_length = 1.2;
  double elapsed_time = (double)(clock() - cycle_start) / CLOCKS_PER_SEC;

  int first_frame = 0;
  int second_frame = 0;

  clear();
  refresh();

  while(cycle_length > elapsed_time){

    elapsed_time = (double)(clock() - cycle_start) / CLOCKS_PER_SEC;

    if(elapsed_time > 0.2 && first_frame == 0){
      if (WIN_SIZE == NORMAL){
        for(int i = 0; i < 6; i++){
          mvprintw(row/2 - 3 + i, (col-LENGTH)/2, "%s", backdrop[i][0]);
        }
      } else { // screen is BIG
        for(int i = 0; i < 19; i++){
          mvprintw(row/2 - 9 + i, (col-LENGTH)/2, "%s", arch[i][0]);
        }
      }
      first_frame = 1;
    }

    if(elapsed_time > 0.7 && second_frame == 0){
      if (WIN_SIZE == NORMAL){
        for(int i = 0; i < 6; i++){
          mvprintw(row/2 - 3 + i, (col-LENGTH)/2, "%s", title[i][0]);
        }
      } else { // screen is big
        for(int i = 0; i < 6; i++){
          mvprintw(row/2 - 2 + i, (col-LENGTH)/2, "%s", titlefill[i][0]);
        }
      }
      second_frame = 1;
    }

    checkchar(row, col);
    if (HOLD_CHAR != '\0') mvprintw(row/2, col/2, "%c", HOLD_CHAR);
    refresh();
  }

  mvprintw(row/2 + 3, (col-LENGTH)/2, "%s", title[6][0]);
  refresh();
}


void print_start_animation(int row, int col) {

  srand((unsigned)time(0));

  if (START_ANIMATION == EMPTY){
    int start_roll = rand() % 3;
//  if (start_roll == 0){
//    START_ANIMATION = QUICK;
//  } else if (start_roll == 1){
//    START_ANIMATION = STANDARD;
//  } else {
//    START_ANIMATION = NEON;
//  }
    START_ANIMATION = NEON;
  }

  if (START_ANIMATION == NEON){
    neon(row, col);
  } else if (START_ANIMATION == QUICK){
    quickprint(row, col);
  } else {
    printstandard(row, col);
  }
}

// Check if screen is too small, returns new cache
int checksize(int row, int col, int cache){

  if (cache != row + col){
    START_ANIMATION = EMPTY;
    clear();
    while (col < MID_LENGTH || row < MID_HEIGHT){
      WIN_SIZE = SMALL;
      clear();
      mvprintw(row/2, (col-10)/2, "%s", ".. kill me.");
      refresh();

      usleep(10000);
      checkchar(row, col);

      getmaxyx(stdscr,row,col); // Get total screen dimensions again
    }
  }

  if (row > MAX_HEIGHT && col > MAX_LENGTH){
    WIN_SIZE = BIG;
    LENGTH = MAX_LENGTH;
    HEIGHT = MAX_HEIGHT;
  } else {
    WIN_SIZE = NORMAL;
    LENGTH = MID_LENGTH;
    HEIGHT = MID_HEIGHT;
  }

  return row + col;
}

void glitch(int row, int col){

  int rng_row, rng_shift, rng_backdrop = 0;

  for( int i = 0 ; i < 28; i++ ) {
    rng_row   = rand() % MID_HEIGHT;    // RNG 0 and 6
    rng_shift = (rand() % 3) - 1;       // RNG -1 and 1
    rng_backdrop = rand() % 3;          // RNG 0 and 2


    if (WIN_SIZE == NORMAL) {
      if (rng_backdrop == 0){
        mvprintw(row/2 - 3 + rng_row, (col - LENGTH)/2 - rng_shift, "%s", title[rng_row][0]);
      } else if (rng_backdrop == 1){
        mvprintw(row/2 - 3 + rng_row, (col - LENGTH)/2 - rng_shift, "%s", backdrop[rng_row][0]);
      } else {
        mvprintw(row/2 - 3 + rng_row, (col - LENGTH)/2 - rng_shift, "%s", foreground[rng_row][0]);
      }
    }

    if (WIN_SIZE == BIG) {
      if (rng_backdrop == 0){
        mvprintw(row/2 - 2 + rng_row, (col - LENGTH)/2 - rng_shift, "%s", title[rng_row][0]);
      } else if (rng_backdrop == 1){
        mvprintw(row/2 - 2 + rng_row, (col - LENGTH)/2 - rng_shift, "%s", backdrop[rng_row][0]);
      } else {
        mvprintw(row/2 - 2 + rng_row, (col - LENGTH)/2 - rng_shift, "%s", foreground[rng_row][0]);
      }
    }


    checkchar(row, col);
    if (rng_row == 3 && HOLD_CHAR != '\0') mvprintw(row/2, col/2, "%c", HOLD_CHAR);
    refresh();
    usleep(23000);

  }

  quickprint(row, col);
}

int main(int argc, char* argv[]) {

  LAST_INPUT_TIME = clock();
  double time_idle;

  enum { DEFAULT, ANIMATED } mode = DEFAULT;
  int DELAY                       = 2000000;
  int opt;

  while ((opt = getopt(argc, argv, "a")) != -1){
    switch (opt) {
			// Removed implementation
      case 'a': mode = ANIMATED; break;
    }
  }

  setlocale(LC_ALL, "");    // Needed to print special characters
  initscr();                // Initialize screen
//raw();                    // Pass F1, ^C to program w/o signals, needed for ANIMATED
                            // Also disables line buffering like cbreak()
  cbreak();                 // Disables line buffering
  noecho();                 // Don't print input to screen when using getch()
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);     // Enable reading of F1/2, arrow keys, etc
  curs_set(FALSE);          // No cursor

  int cache = 10000;
  int row, col;             // For storing the number of rows/cols

  refresh();                // clear screen

  WAIT_START = clock();
  while(1){

    getmaxyx(stdscr, row, col); // Get total screen dimensions

    cache = checksize(row, col, cache);

    if (START_ANIMATION == EMPTY) print_start_animation(row, col);

    usleep(10000); // Simple wait to reduce some overhead
    checkchar(row, col); // check input for this cycle

    time_idle = (double)(clock() - WAIT_START) / CLOCKS_PER_SEC;

    if(time_idle >= WAIT_BUFFER){
      glitch(row, col);
      WAIT_START = clock();
    }

    if (HOLD_CHAR == '\0'){
      if (WIN_SIZE == NORMAL) {
        mvprintw(row/2, (col-LENGTH)/2, "%s", title[3][0]);
      } else {
        mvprintw(row/2, (col-LENGTH)/2, "%s", titlefill[2][0]);
      }

    }
  }

  // ANIMATION IDEAS
  //  STARS: . and + twinkles
  //  lIGHTSPEED: ----- lines moving right to left
  //  FIREWORKS: Up and boom

  refresh();
  endwin();

  return 0;
}

