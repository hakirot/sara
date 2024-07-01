/* TODO
 *    Press q to exit
 *    Dynamically resize with window
*/
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// length 44 per line
const char * title[7][99] = {
  { "███████╗    █████╗    ██████╗     █████╗    " },
  { "██╔════╝   ██╔══██╗   ██╔══██╗   ██╔══██╗   " },
  { "███████╗   ███████║   ██████╔╝   ███████║   " },
  { "╚════██║   ██╔══██║   ██╔══██╗   ██╔══██║   " },
  { "███████║██╗██║  ██║██╗██║  ██║██╗██║  ██║██╗" },
  { "╚══════╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝" },
  { "         SOFTWARE ARCHITECTED RANGING AREA  " },
};

typedef enum {
  SMALL,
  NORMAL
} screen_size;

clock_t LAST_INPUT_TIME;
screen_size CURRENT_WINDOW_SIZE;

void checkchar(int row, int col, screen_size WIN_SIZE) {

  char ch;
  char input = getch();

  if (input != ERR && input != '\n' && input != EOF && input > 19 && input < 127) {
    if(input == 'q'){
      endwin();
      exit(0);
    } else if (WIN_SIZE == NORMAL) {
      LAST_INPUT_TIME = clock();
      ch = input;
      mvprintw(row/2, col/2, "%c", ch);
      refresh();
    }
  }

  // clear center row if 1 second has elapsed
  double elapsed_time = (double)(clock() - LAST_INPUT_TIME) / CLOCKS_PER_SEC;
  if(elapsed_time >= 0.001 && WIN_SIZE == NORMAL){
    mvprintw(row/2, (col-44)/2, "%s", title[3][0]);
    refresh();
  }
}

// Check if screen is too small
void checksize(int row, int col){
  while (col < 44 || row < 7){
    mvprintw(row/2, (col-10)/2, "%s", ".. kill me.");
    refresh();

    usleep(10000);
    CURRENT_WINDOW_SIZE = SMALL;
    checkchar(row, col, CURRENT_WINDOW_SIZE);

    getmaxyx(stdscr,row,col); // Get total screen dimensions again
  }

  CURRENT_WINDOW_SIZE = NORMAL;
}

void glitch(){

};

int main(int argc, char* argv[]) {

  enum { DEFAULT, ANIMATED } mode = DEFAULT;
  LAST_INPUT_TIME = clock();
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

  while(1){

    refresh();

    getmaxyx(stdscr,row,col); // Get total screen dimensions

    if (cache != row + col){
      clear();
      checksize(row, col);

      cache = row + col;
      clear();

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
          mvprintw(row/2 - 3 + i, (col-44)/2, "%s", title[i][0]);
          refresh();
          usleep(50000);          // Add some sexy timing
        }
      }
    }
    usleep(10000);
    checkchar(row, col, CURRENT_WINDOW_SIZE);

    glitch();
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

