
/*
    (y,x)
*/

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>


char * acronym = "SEMINAL ATMOSPHERE RANGING AREA";

int main() {

  setlocale(LC_ALL, "");
  char * title1 = "███████╗    █████╗    ██████╗     █████╗    \n";
  char * title2 = "██╔════╝   ██╔══██╗   ██╔══██╗   ██╔══██╗   ";
  char * title3 = "███████╗   ███████║   ██████╔╝   ███████║   ";
  char * title4 = "╚════██║   ██╔══██║   ██╔══██╗   ██╔══██║   ";
  char * title5 = "███████║██╗██║  ██║██╗██║  ██║██╗██║  ██║██╗";
  char * title6 = "╚══════╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝";
  char * title7 = "            SEMINAL ATMOSPHERE RANGING AREA ";

  //int status = system("figlet -f 'ANSI Shadow' S.A.R.A");
  initscr();
  raw();                // Pass F1, ^C to program w/o signals
                        // Also disables line buffering like cbreak()
  noecho();             // Don't print input to screen

  keypad(stdscr, TRUE); // Enable reading of F1/2, arrow keys, etc

  for(int i = 0; i < 8; i++){
    printw(title1);
  }

  refresh();
  getch();

  refresh();
  getch();

  endwin();

  return 0;
}
