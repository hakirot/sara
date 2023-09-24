#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

int main() {
  int status = system("figlet -f 'ANSI Shadow' S.A.R.A");
  char * acronym = "Single Altitude Ranging Area";
  return 0;
}
