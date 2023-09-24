
/*
  ███████╗    █████╗    ██████╗     █████╗    
  ██╔════╝   ██╔══██╗   ██╔══██╗   ██╔══██╗   
  ███████╗   ███████║   ██████╔╝   ███████║   
  ╚════██║   ██╔══██║   ██╔══██╗   ██╔══██║   
  ███████║██╗██║  ██║██╗██║  ██║██╗██║  ██║██╗
  ╚══════╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝
              SEMINAL ATMOSPHERE RANGING AREA
*/                                           

/*
    (y,x)
*/

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

char * acronym = "Sightly Altitude Ranger Area";

int main() {

  //int status = system("figlet -f 'ANSI Shadow' S.A.R.A");
  initscr();
  raw();
  noecho(); // don't print input to screen

  printw("S.A.R.A.");

  refresh();
  getch();

  refresh();
  getch();

  endwin();

  return 0;
}
