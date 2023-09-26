/* TODO
 *    Press F1 to exit
 *    Dynamically resize with window
*/
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>

// length 44 per line
const char * title[7][99] = {
  { "███████╗    █████╗    ██████╗     █████╗    " },
  { "██╔════╝   ██╔══██╗   ██╔══██╗   ██╔══██╗   " },
  { "███████╗   ███████║   ██████╔╝   ███████║   " },
  { "╚════██║   ██╔══██║   ██╔══██╗   ██╔══██║   " },
  { "███████║██╗██║  ██║██╗██║  ██║██╗██║  ██║██╗" },
  { "╚══════╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝" },
  { "           SOFTWARE ARCHITECT RANGING AREA  " },
};

int main(int argc, char* argv[] ) {

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
  noecho();                 // Don't print input to screen when using getch()
  keypad(stdscr, TRUE);     // Enable reading of F1/2, arrow keys, etc
  int row, col;             // Store the number of rows/cols
  curs_set(FALSE);          // No cursor

  getmaxyx(stdscr,row,col); // Get total screen dimensions
  while (col < 44 || row < 7){
    mvprintw(row/2, (col-10)/2, "%s", "Resize me!");
    refresh();
    sleep(1);
    getmaxyx(stdscr,row,col); // Get total screen dimensions
    clear();
  }

  for(int i = 0; i < 7; i++){
    //printw("%s\n", title[i][0]);
    mvprintw(row/2 - 3 + i, (col-44)/2, "%s", title[i][0]);
    refresh();
    usleep(50000);          // Add some sexy timing
  }

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

  // TODO: Add stars w/ . and +
  // Should the main title and animations be handled independently?
  //   how would that work..?
  //   never render over the center box..?
  //   cut out the dimensions of the title itslef within in the terminal
  //
  // Will probably need to work with time instead of sleep, update
  //   and print the screen with different objects after a certain 
  //   amount of time.
  //
  // ANIMATION IDEAS
  //  STARS: . and + twinkles
  //  lIGHTSPEED: ----- lines moving right to left
  //  FIREWORKS: Up and boom

  refresh();
  getch();

  endwin();

  return 0;
}
