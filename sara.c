
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
  { "        SENTINELED ATMOSPHERIC RANGING AREA " },
};

int DELAY = 1500000;

int main() {

  setlocale(LC_ALL, "");    // Needed to print special characters

  initscr();                // Initialize screen
  //raw();                    // Pass F1, ^C to program w/o signals
                            // Also disables line buffering like cbreak()
  noecho();                 // Don't print input to screen
  keypad(stdscr, TRUE);     // Enable reading of F1/2, arrow keys, etc
  int row, col;             // Store the number of rows/cols
  curs_set(FALSE);          // No cursor

  getmaxyx(stdscr,row,col); // Get total screen dimensions


  for(int i = 0; i < 7; i++){
    //printw("%s\n", title[i][0]);
    mvprintw(row/2 - 3 + i, (col-44)/2, "%s", title[i][0]);
    refresh();
    usleep(50000);      // Add some sexy timing
  }

  while(1){

    usleep(DELAY);      // Add some sexy timing
    clear();
    for(int i = 0; i < 7; i++){
      //printw("%s\n", title[i][0]);
      mvprintw(row/2 - 4 + i, (col-44)/2, "%s", title[i][0]);
      refresh();
    }

    usleep(DELAY);      // Add some sexy timing
    clear();
    for(int i = 0; i < 7; i++){
      //printw("%s\n", title[i][0]);
      mvprintw(row/2 - 3 + i, (col-44)/2, "%s", title[i][0]);
      refresh();
    }
  }

  refresh();
  getch();

  endwin();

  return 0;
}
