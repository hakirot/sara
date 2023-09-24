#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

char * acronym = "Sightly Altitude Ranger Area";

int main() {

  //int status = system("figlet -f 'ANSI Shadow' S.A.R.A");
  initscr();
  printw("Hello World !!");
  refresh();
  getch();
  endwin();


  return 0;
}
