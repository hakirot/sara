/* TODO
 *  Boot animations
 *  Interval animations
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

// length 44 per line
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


typedef enum {
  SMALL,
  NORMAL
} screen_size;

clock_t LAST_INPUT_TIME;
screen_size CURRENT_WINDOW_SIZE;
double WAIT_START;
double WAIT_BUFFER = 0.00010;

void checkchar(int row, int col, screen_size WIN_SIZE) {

  char ch;
  char input = getch();

  if (input != ERR && input != '\n' && input != EOF && input > 19 && input < 127) {
    if(input == 'q'){
      endwin();
      exit(0);
    } else if (input == 'v') {
      endwin();
      execv("/usr/bin/nvim", NULL);
    } else if (WIN_SIZE == NORMAL) {
      LAST_INPUT_TIME = clock();
      WAIT_START = clock();
      ch = input;
      mvprintw(row/2, col/2, "%c", ch);
      refresh();
    }
  } 


  // Clear center row if 1 second has elapsed
  double elapsed_time = (double)(clock() - LAST_INPUT_TIME) / CLOCKS_PER_SEC;
  if(elapsed_time >= 0.001 && WIN_SIZE == NORMAL){
    mvprintw(row/2, (col-44)/2, "%s", title[3][0]);
    refresh();
  }
}

// Check if screen is too small
void checksize(int row, int col){

  while (col < 44 || row < 7){
    CURRENT_WINDOW_SIZE = SMALL;
    mvprintw(row/2, (col-10)/2, "%s", ".. kill me.");
    refresh();

    usleep(10000);
    checkchar(row, col, CURRENT_WINDOW_SIZE);

    getmaxyx(stdscr,row,col); // Get total screen dimensions again
  }

  CURRENT_WINDOW_SIZE = NORMAL;
}

void quickprint(int row, int col){
  clear();
  for(int i = 0; i < 7; i++){
    mvprintw(row/2 - 3 + i, (col-44)/2, "%s", title[i][0]);
    refresh();
  }
}

void glitch(int row, int col){

  double interval = (double)(clock() - WAIT_START) / CLOCKS_PER_SEC;

  if(interval >= 0.040){

    time_t t = clock();
    srand((unsigned) time(&t));

    int rng_row, rng_shift, rng_backdrop = 0;

    for( int i = 0 ; i < 28; i++ ) {
      // RNG between 0 and 6 (inclusive)
      rng_row   = rand() % 7;
      // RNG between 0 and 1 (inclusive)
      rng_shift = (rand() % 3) - 1;
      // RNG between 0 and 1 (inclusive)
      rng_backdrop = rand() % 3;

      if (rng_backdrop == 0){
        mvprintw(row/2 - 3 + rng_row, (col - 44)/2 - rng_shift, "%s", title[rng_row][0]);
      } else if (rng_backdrop == 1){
        mvprintw(row/2 - 3 + rng_row, (col - 44)/2 - rng_shift, "%s", backdrop[rng_row][0]);
      } else {
        mvprintw(row/2 - 3 + rng_row, (col - 44)/2 - rng_shift, "%s", foreground[rng_row][0]);
      }
      refresh();
      usleep(23000);
    }
    quickprint(row, col);
    WAIT_START = (double)clock();
  }
}

int main(int argc, char* argv[]) {

  LAST_INPUT_TIME = clock();
  WAIT_START = clock();
  enum { DEFAULT, ANIMATED } mode = DEFAULT;
  int DELAY                       = 2000000;
  int opt;

  while ((opt = getopt(argc, argv, "a")) != -1){
    switch (opt) {
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
  while(1){

    getmaxyx(stdscr,row,col); // Get total screen dimensions

    if (cache != row + col){
//  cache = checksize(row, col, cache);                                       << refactor
//    if (cache != row + col){
//      clear()
//      while (col < 44 || row < 7){
//        ...
//      }
//      
//    }

      clear();
      checksize(row, col);
      cache = row + col;

      if(mode == ANIMATED){

        while(1){
          usleep(DELAY);
          clear();
          for(int i = 0; i < 7; i++){
            //printw("%s\n", title[i][0]);
            mvprintw(row/2 - 4 + i, (col-44)/2, "%s", title[i][0]);
            refresh();
          }

          usleep(DELAY);
          clear();
          for(int i = 0; i < 7; i++){
            //printw("%s\n", title[i][0]);
            mvprintw(row/2 - 3 + i, (col-44)/2, "%s", title[i][0]);
            refresh();
          }
        }
      } 

      if(mode == DEFAULT){
        // Print standard rows
        for(int i = 0; i < 7; i++){
          checkchar(row, col, CURRENT_WINDOW_SIZE);
          mvprintw(row/2 - 3 + i, (col-44)/2, "%s", title[i][0]);
          refresh();
          usleep(20000);          // Add some sexy timing
        }
      }
    }

    usleep(10000); // Simple wait to reduce overhead
    checkchar(row, col, CURRENT_WINDOW_SIZE); // check input for this cycle

    double time_idle = (double)(clock() - WAIT_START) / CLOCKS_PER_SEC;
    if(time_idle >= WAIT_BUFFER){
      glitch(row, col);
    }
  }

  // TODO: Add stars w/ . and +
  // Should the main title and animations be handled independently?
  //   how would that work..?
  //   never render over the center box..?
  //   cut out the dimensions of the title itself within in the terminal
  //
  // Will probably need to work with time instead of sleep, update
  //   and print the screen with different objects after a certain 
  //   amount of time.

  // ANIMATION IDEAS
  //  STARS: . and + twinkles
  //  lIGHTSPEED: ----- lines moving right to left
  //  FIREWORKS: Up and boom

  refresh();
  endwin();

  return 0;
}

