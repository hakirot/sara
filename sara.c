
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>

/*
    (y,x)
*/

char * acronym = "SEMINAL ATMOSPHERE RANGING AREA";

int main() {

  setlocale(LC_ALL, "");

  const char * title[7][100] = {
    { "███████╗    █████╗    ██████╗     █████╗    " },
    { "██╔════╝   ██╔══██╗   ██╔══██╗   ██╔══██╗   " },
    { "███████╗   ███████║   ██████╔╝   ███████║   " },
    { "╚════██║   ██╔══██║   ██╔══██╗   ██╔══██║   " },
    { "███████║██╗██║  ██║██╗██║  ██║██╗██║  ██║██╗" },
    { "╚══════╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝" },
    { "            SEMINAL ATMOSPHERE RANGING AREA " },
  };

//  for(int i = 0; i < 7; i++){                       // PASS
//    printf("%s\n", title[i][0]);
//  }

  initscr();
  raw();                // Pass F1, ^C to program w/o signals
                        // Also disables line buffering like cbreak()
  noecho();             // Don't print input to screen

  keypad(stdscr, TRUE); // Enable reading of F1/2, arrow keys, etc

  for(int i = 0; i < 7; i++){
    printw("%s\n", title[i][0]);
    refresh();
    usleep(50000);      // Add some sexy timing
  }

  refresh();
  getch();

  endwin();

  return 0;
}
