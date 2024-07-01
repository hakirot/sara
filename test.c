#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

void test_one(){
  initscr();                // Initialize screen
//raw();                    // Pass F1, ^C to program w/o signals, needed for ANIMATED
  noecho();                 // Don't print input to screen when using getch()
  cbreak();                 // Disables line buffering
//timeout(-1);
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);     // Enable reading of F1/2, arrow keys, etc
  curs_set(FALSE);          // No cursor
  setlocale(LC_ALL, "");    // Needed to print special characters

  int row, col;
  getmaxyx(stdscr,row,col); // Get total screen dimensions

  char ch = 'a';
  while(1){

    char input = getch();
    fflush(stdin);
    if (input != ERR && input != '\n' && input != EOF && input > 19 && input < 127) {
      if(input == 'q'){
        break;
      } else {
        ch = input;
      }
    }

    //erase();
    mvprintw(row/2, col/2, "%c", ch);
    refresh();
  }

  endwin();

};

void test_two(){
  int i;
  int n = 50;
  time_t t;
  srand((unsigned) time(&t));

   /* Print 5 random numbers from 0 to 49 */
   for( i = 0 ; i < n ; i++ ) {
      printf("%d\n", rand() % 2);
   }
}

int main(int argc, char* argv[]) {

  test_two();

  return 0;
}

