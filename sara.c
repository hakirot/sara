
/*
--  ███████╗ █████╗ ██████╗  █████╗   --
--  ██╔════╝██╔══██╗██╔══██╗██╔══██╗  --
--  ███████╗███████║██████╔╝███████║  --
--  ╚════██║██╔══██║██╔══██╗██╔══██║  --
--  ███████║██║  ██║██║  ██║██║  ██║  --
--  ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝.c--
                                           */

#include "sara.h"
#include "glyphs.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>
#include <time.h>
#include <wait.h>
clock_t LAST_INPUT_TIME;
screen_size WIN_SIZE;
clock_t WAIT_START;

int GLYPH_LENGTH = 44;
int GLYPH_HEIGHT = 7;

int FOREGROUND = 3;
int BACKGROUND = 2;
int HOLOGRAPHIC = 0;

wchar_t SEARCH_STR[] = L"`+so:-./";

const int BLACK         = 1;
const int RED           = 2;
const int GREEN         = 3;
const int YELLOW        = 4;
const int BLUE          = 5;
const int MAGENTA       = 6;
const int CYAN          = 7;
const int WHITE         = 8;
const int WHITE_BLACK   = 9;
const int BLACK_RED     = 10;
const int BLACK_GREEN   = 11;
const int BLACK_YELLOW  = 12;
const int BLACK_BLUE    = 13;
const int BLACK_MAGENTA = 14;
const int BLACK_CYAN    = 15;
const int BLACK_WHITE   = 16;

const int EFFECT_MUTE   = 0;

start_animation START_ANIMATION = EMPTY;
char HOLD_CHAR;

int main(int argc, char* argv[]) {

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
      get_helped();
    }
  }

  // Override fg and bg colors
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--foreground") == 0){
      if(i + 1 < argc){
        FOREGROUND = input_color(argv[i + 1]);
        if (FOREGROUND == -1) error("bad arg\n");
      } else {
        get_helped();
        error("bad arg\n");
      }
    } else if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--background") == 0){
      if(i + 1 < argc){
        BACKGROUND = input_color(argv[i + 1]);
        if (BACKGROUND == -1) error("bad arg");
      } else {
        get_helped();
        error("bad arg\n");
      }
    }
  }

  double WAIT_BUFFER = 0.10000;

  int opt;
  while ((opt = getopt(argc, argv, "cMfbhrH")) != -1){
    switch (opt) {
      case 'c': WAIT_BUFFER = 0.00005; break;
      case 'r':
        srand((unsigned)time(0));
        FOREGROUND = (rand() % 7) + 1;    // RNG 1 and 7
        BACKGROUND = (rand() % 7) + 1;    // RNG 1 and 7
        if (FOREGROUND > 7 || FOREGROUND < 1 || BACKGROUND > 7 || BACKGROUND < 1){
          error("error");
        }
        break;
      case 'H': HOLOGRAPHIC = 1;
    }
  }

  setlocale(LC_ALL, "");    // Allow special characters, initscr()
  initscr();                // Initialize screen
  start_color();            // Must be called right after initscr()
  use_default_colors();
  init_pair(BLACK, COLOR_BLACK, -1); // Foreground black, no background
  init_pair(RED, COLOR_RED, -1);
  init_pair(GREEN, COLOR_GREEN, -1);
  init_pair(YELLOW, COLOR_YELLOW, -1);
  init_pair(BLUE, COLOR_BLUE, -1);
  init_pair(MAGENTA, COLOR_MAGENTA, -1);
  init_pair(CYAN, COLOR_CYAN, -1);
  init_pair(WHITE, COLOR_WHITE, -1);
  init_pair(WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
  init_pair(BLACK_RED, COLOR_BLACK, COLOR_RED);
  init_pair(BLACK_GREEN, COLOR_BLACK, COLOR_GREEN);
  init_pair(BLACK_YELLOW, COLOR_BLACK, COLOR_YELLOW);
  init_pair(BLACK_BLUE, COLOR_BLACK, COLOR_BLUE);
  init_pair(BLACK_MAGENTA, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(BLACK_CYAN, COLOR_BLACK, COLOR_CYAN);
  init_pair(BLACK_WHITE, COLOR_BLACK, COLOR_WHITE);

  cbreak();                 // Disable line buffering
  noecho();                 // Don't display keyboard presses in window
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);     // Enable reading of F1/2, arrow keys, etc
  curs_set(FALSE);          // No cursor

  int cache = 10000;
  int row, col = 0;         // Num of current rows/cols in window

  refresh();                // clear screen

  double time_idle;
  WAIT_START = clock();
  LAST_INPUT_TIME = clock();
  bool should_print = false;

  while(1){

//  get current screen dimensions
    getmaxyx(stdscr, row, col);
    cache = check_size(row, col, cache);

    if (START_ANIMATION == EMPTY) print_start_animation(row, col);

    check_char(row, col); // check input for this cycle

    time_idle = (double)(clock() - WAIT_START) / CLOCKS_PER_SEC;

//  if(time_idle >= WAIT_BUFFER){
//    glitch(row, col);
//    WAIT_START = clock();
//  }

    if(HOLOGRAPHIC > 0 && WIN_SIZE == BIG){
      int i = BACKGROUND;
      i++;
      if (BACKGROUND > 7) i = 2;
      BACKGROUND = i;
      quickprint(row, col, FOREGROUND, BACKGROUND, 0);
    }

    // print only once after the HOLD_CHAR goes back to EOF
    if (HOLD_CHAR == '\0' && should_print == true){
      quickprint(row, col, FOREGROUND, BACKGROUND, 0);
      should_print = false;
    } else if (HOLD_CHAR != '\0'){
      should_print = true;
    }
    usleep(80000); // chill
  }

  refresh();
  endwin();

  return 0;
}

int is_char_in_search(wchar_t wc, wchar_t * search_str) {

//  Iterate through the wide-character array
    for (size_t i = 0; i < wcslen(search_str); i++) {
        if (wc == search_str[i]) {
            return 1; // Character found
        }
    }
    return 0; // not found
}

void check_char(int row, int col) {

  char input = getch();

  if (input != ERR && input != '\n' && input != EOF && input > 19 && input < 127) {
    if(input == 'q'){
      endwin();
      exit(0);
    } else if(input == 'r'){
      endwin();
      execv("/usr/bin/ranger", NULL);
      exit(1);
    } else if(input == 'i'){
      int temp = BACKGROUND;
      BACKGROUND = FOREGROUND;
      FOREGROUND = temp;
      quickprint(row, col, FOREGROUND, BACKGROUND, 0);
    } else if(input == 'w'){
      endwin();
      execlp("ranger", "ranger", "/home/hakirot/pix/wall/", NULL);
      exit(1);
    } else if(input == 'g'){
      glitch(row, col);
    } else if(input == 't'){
      endwin();
      execlp("nvim", "nvim", "/home/hakirot/dox/.notes/tasks", NULL);
      exit(1);

    } else if(input == 'p'){

      char* choices[2]={'\0'};
      choices[0]="PSHD";
      choices[1]="POLYBAR";
      const char* selection =  select_option_window(row, col, choices, 2);

      if (selection == choices[0]){
        pshd(row, col);
      } else if (selection == choices[1]){

        pid_t pid = fork();
        if (pid < 0) {
          perror("fork");
          exit(EXIT_FAILURE);
        } else if (pid == 0) {
          execl("/bin/bash", "bash", "/home/hakirot/.config/polybar/bar.sh", (char *)NULL);
          perror("execl");
        } else {
          int status;
          waitpid(pid, &status, 0);
        }
      }

      neon(row, col);

    } else if(input == 'n'){

      prompt_newlook(row, col);

    } else if(input == 'b'){

      // Display menu

      char* choices[2]={'\0'};
      choices[0]="BLUETOOTH";
      choices[1]="BACKLIGHT";
      const char * selection = select_option_window(row, col, choices, 2);

      if (selection == choices[0]){ // When selected with `selection`

        char* bluetooth_choices[2]={'\0'};
        bluetooth_choices[0]="CONNECT";
        bluetooth_choices[1]="DISCONNECT";
        selection = select_option_window(row, col, bluetooth_choices, 2);

        if (selection == bluetooth_choices[0]){
          char* connect_choices[2]={'\0'};
          connect_choices[0]="WH-1000XM5";
          connect_choices[1]="ACOUSTIC";
          selection = select_option_window(row, col, connect_choices, 2);

          if (selection == connect_choices[0]){
            pid_t pid = fork();

            if (pid < 0) {
              perror("fork");
              exit(EXIT_FAILURE);
            } else if (pid == 0) {
              execl("/usr/bin/bluetoothctl", "bluetoothctl", "connect", "AC:80:0A:19:89:A8", (char *)NULL);
              perror("execl");
            } else {
              clear();
              refresh();
              int status;
              waitpid(pid, &status, 0);
            }

          } else if (selection == connect_choices[1]){
            pid_t pid = fork();

            if (pid < 0) {
              perror("fork");
              exit(EXIT_FAILURE);
            } else if (pid == 0) {
              execl("/usr/bin/bluetoothctl", "bluetoothctl", "connect", "FC:58:FA:9B:D7:3D", (char *)NULL);
              perror("execl");
            } else {
              clear();
              refresh();
              int status;
              waitpid(pid, &status, 0);
            }
          }
        } else if (selection == bluetooth_choices[1]){
          char* disconnect_choices[2]={'\0'};
          disconnect_choices[0]="WH-1000XM5";
          disconnect_choices[1]="ACOUSTIC";
          selection = select_option_window(row, col, disconnect_choices, 2);

          if (selection == disconnect_choices[0]){
            pid_t pid = fork();

            if (pid < 0) {
              perror("fork");
              exit(EXIT_FAILURE);
            } else if (pid == 0) {
              execl("/usr/bin/bluetoothctl", "bluetoothctl", "disconnect", "AC:80:0A:19:89:A8", (char *)NULL);
              perror("execl");
            } else {
              clear();
              refresh();
              int status;
              waitpid(pid, &status, 0);
            }
          } else if (selection == disconnect_choices[1]){
            pid_t pid = fork();

            if (pid < 0) {
              perror("fork");
              exit(EXIT_FAILURE);
            } else if (pid == 0) {
              execl("/usr/bin/bluetoothctl", "bluetoothctl", "disconnect", "FC:58:FA:9B:D7:3D", (char *)NULL);
              perror("execl");
            } else {
              clear();
              refresh();
              int status;
              waitpid(pid, &status, 0);
            }
          }
        }

        neon(row, col);

      } if (selection == choices[1]){
				FILE *fptr = fopen("/sys/class/backlight/intel_backlight/brightness", "r");
        if(fptr){

          char buff[8];
          memset(&buff, '\0', 8 * sizeof(char));
          fgets(buff, 8, fptr);
          fclose(fptr);

          // Read buff to int
          int brightness;
          sscanf(buff, "%d", &brightness);

          int offset = 0;
          if(WIN_SIZE != BIG) offset = 1;

          // print file contents
          attron(COLOR_PAIR(FOREGROUND + 8));
          mvaddstr(row/2 - offset, col/2 - 11, buff);
          attroff(COLOR_PAIR(FOREGROUND + 8));

          patch_backlight(row, col);
          patch_border(row, col);
          refresh();

          int cache = row + col;

          while(1){

            int input = getch();
            char str[16];

            if (input != ERR && input != '\n' && input != EOF && input > 105 && input < 108) {
              if (input == 'j'){
                // Decrement the string and write it to file
                brightness -= 20;
                if (brightness < 0) brightness = 0;
              } else if (input == 'k'){
                brightness += 20;
                if (brightness > 1500) brightness = 1500;
              }

              // Update UI
              sprintf(str, "%d", brightness);

              mvaddstr(row/2 - offset, col/2 - 11, "     ");
              attron(COLOR_PAIR(FOREGROUND + 8));
              mvaddstr(row/2 - offset, col/2 - 11, str);
              attroff(COLOR_PAIR(FOREGROUND + 8));

              patch_border(row, col);

              refresh();

              FILE *writeptr = fopen("/sys/class/backlight/intel_backlight/brightness", "w");
              if(writeptr){
                // Write brightness to file
                fprintf(writeptr, "%s", str);
                fclose(writeptr);
              }

            } else if (input == 'q' || input == '\n'){
              break;
            }

            getmaxyx(stdscr, row, col);
            if (cache != row + col) break;
            usleep(1000);
          }
        }
      }

    neon(row, col);

    } else if (input == 'v') {
      endwin();
      execv("/usr/bin/nvim", NULL);
    } else if (input == 'm') {
      endwin();
      execlp("rmpc", "rmpc", NULL);
      exit(1);
    } else if (WIN_SIZE != SMALL) {
      LAST_INPUT_TIME = clock();
      HOLD_CHAR = input;
      mvprintw(row/2, col/2, "%c", HOLD_CHAR);
      refresh();
    }
  } 

  double time_since_input = (double)(clock() - LAST_INPUT_TIME) / CLOCKS_PER_SEC;
  if(time_since_input >= 0.0005 && WIN_SIZE != SMALL){
    HOLD_CHAR = '\0';
  }
}

void printstandard(int row, int col){

  if (WIN_SIZE == NORMAL){
    for(int i = 0; i < GLYPH_HEIGHT; i++){
      mvprintw(row/2 - 3 + i, (col-GLYPH_LENGTH)/2, "%s", title[i]);
      check_char(row, col);
      if(HOLD_CHAR) mvprintw(row/2, col/2, "%c", HOLD_CHAR);
      refresh();
      usleep(20000);
    }

  } else if (WIN_SIZE == BIG){

    attron(COLOR_PAIR(FOREGROUND));
    for(int i = 0; i < GLYPH_HEIGHT; i++){
      mvprintw(row/2 - 9 + i, (col-GLYPH_LENGTH)/2, "%s", arch[i]);
      if(HOLD_CHAR) mvprintw(row/2, col/2, "%c", HOLD_CHAR);
      refresh();
      usleep(10000);
    }
    attroff(COLOR_PAIR(FOREGROUND));

    for(int i = 0; i < 6; i++){

      mbstate_t state;                            // Tracks state of mbrtowc function when converting between types of chars
      memset(&state, 0, sizeof(mbstate_t));
      const char *iter_row = titlefill[5 - i];    // Grabs a line from glyph
      int iter_col = 0;                           // Track the column position
      while (*iter_row) {                         // Iterate through chars in row
        wchar_t wc;                               // Create wide character var
        // Converts character from iter_row to wide char `wc`
        // Also records length of character at *iter_row in len
        size_t len = mbrtowc(&wc, iter_row, MB_CUR_MAX, &state);

        is_char_in_search(wc, SEARCH_STR) ? attron(COLOR_PAIR(BACKGROUND)) : attron(COLOR_PAIR(FOREGROUND)) ;

        // Write wide char to `cchar` for mvaddwc()
        cchar_t cchar;
        setcchar(&cchar, &wc, 0, 0, NULL);
        mvadd_wch(row/2 + 3 - i, (col-GLYPH_LENGTH)/2 + iter_col, &cchar);

        attroff(COLOR_PAIR(FOREGROUND));
        attroff(COLOR_PAIR(BACKGROUND));
        iter_row += len;                          // Increment the pointer one character
        iter_col++;                               // Increment col
      }
        usleep(20000);
        refresh();
    }
  }
}

void quickprint(int row, int col, int fg, int bg, int printColorbar){
  clear();
  if (WIN_SIZE == NORMAL){
    for(int i = 0; i < GLYPH_HEIGHT; i++){
      mvprintw(row/2 - 3 + i, (col-GLYPH_LENGTH)/2, "%s", title[i]);
    }
  }
  if (WIN_SIZE == BIG){
    for(int i = 0; i < GLYPH_HEIGHT; i++){

      mbstate_t state;
      memset(&state, 0, sizeof(mbstate_t));
      const char *iter_row = archsarafull[i];
      int iter_col = 0; // Track the column position
      while (*iter_row) {

        wchar_t wc;
        size_t len = mbrtowc(&wc, iter_row, MB_CUR_MAX, &state); // Convert to wide char
        cchar_t cchar;

        setcchar(&cchar, &wc, 0, 0, NULL);

        is_char_in_search(wc, SEARCH_STR) ? attron(COLOR_PAIR(bg)) : attron(COLOR_PAIR(fg));
        mvadd_wch(row/2 - 9 + i, (col-GLYPH_LENGTH)/2 + iter_col, &cchar);
        attroff(COLOR_PAIR(fg));
        attroff(COLOR_PAIR(bg));
        iter_row += len;
        iter_col++;
      }
    }
    // print name with background
    attron(COLOR_PAIR(fg + 8));
    mvprintw(row/2 + 4, (col-GLYPH_LENGTH)/2 + 10, "%s", specialApplicationRangingArea);
    attroff(COLOR_PAIR(fg + 8));

    // print colorbar
    if (printColorbar == 1){
      for(int i = 1; i < 9; i++){
        attron(COLOR_PAIR(i));
        mvaddwstr(row/2 + 5, (col-GLYPH_LENGTH)/2 + 15 + (i*3), L"\u2588\u2588\u2588"); // Unicode full block █
        attroff(COLOR_PAIR(i));
      }
    }
  }
  if(HOLD_CHAR) mvprintw(row/2, col/2, "%c", HOLD_CHAR);
  refresh();
}

const char * select_option_window(int row, int col, char** choices, int len){

  int selection = 0;
  int cache = row + col;

  quickprint(row, col, FOREGROUND, FOREGROUND, 0); // hardcoded style choice

  int offset = 0;
  if (WIN_SIZE != BIG) offset = 1;

  attron(COLOR_PAIR(BACKGROUND));
  for (int i = 0; i < NORMAL_GLYPH_HEIGHT; i++){
    mvprintw(row/2-2 + i - offset, (col-GLYPH_LENGTH)/2, "%s", option_window[i]);
  }
  attroff(COLOR_PAIR(BACKGROUND));

  while(1){

    if (WIN_SIZE != BIG){
      offset = 1;
    } else {
      offset = 0;
    }

    getmaxyx(stdscr, row, col);
    if (cache != row + col) break;

    int input = getch();

    if (input != ERR && input != '\n' && input != EOF && input > 105 && input < 108) {

      if (input == 'j'){
        selection = (selection + 1) % len;
      } else {
        selection = (selection + (len - 1)) % len;
      }

    } else if (input == 'q'){
      return NULL;
    } else if (input == '\n'){
      return choices[selection];
    }

    for (int i = 0; i < len; i++){
      i == selection ? attron(COLOR_PAIR(FOREGROUND + 8)) : attron(COLOR_PAIR(FOREGROUND));
      mvprintw(row/2 + i - 1 - offset, (col-GLYPH_LENGTH)/2 + 1, "%s", choices[i]);
      attroff(COLOR_PAIR(FOREGROUND + 8));
      attroff(COLOR_PAIR(FOREGROUND));
    }

    refresh();
    usleep(1000); // chill
  }

}

void neon(int row, int col) {

  clock_t cycle_start = clock();
  double cycle_length = 0.3;
  double elapsed_time = 0;

  int first_frame = 0;
  int second_frame = 0;
  int third_frame = 0;
  bool colorbar_printed = 0;

  clear();
  refresh();

  while(cycle_length > elapsed_time){

    elapsed_time = (double)(clock() - cycle_start) / CLOCKS_PER_SEC;

    if(elapsed_time > 0.05 && first_frame == 0){
      if (WIN_SIZE == NORMAL){
        for(int i = 0; i < 6; i++){
          mvprintw(row/2 - 3 + i, (col-GLYPH_LENGTH)/2, "%s", backdrop[i]);
        }
      } else { // screen is BIG
        for(int i = 0; i < 19; i++){
          attron(COLOR_PAIR(BACKGROUND));
          mvprintw(row/2 - 9 + i, (col-GLYPH_LENGTH)/2, "%s", arch[i]);
          attroff(COLOR_PAIR(BACKGROUND));
        }
      }

      if (colorbar_printed == false ){
        if (WIN_SIZE == BIG) {
          for(int i = 1; i < 9; i++){
            attron(COLOR_PAIR(i));
            mvaddwstr(row/2 + 4, (col-GLYPH_LENGTH)/2 + 3 + (i*3), L"\u2588\u2588\u2588"); // Unicode full block █
            attroff(COLOR_PAIR(i));
          }
        }
        colorbar_printed == true;
      }

      first_frame = 1;
    }

    if(elapsed_time > 0.1 && second_frame == 0){
      if (WIN_SIZE == NORMAL){
        for(int i = 0; i < 6; i++){
          mvprintw(row/2 - 3 + i, (col-GLYPH_LENGTH)/2, "%s", title[i]);
        }
      } else { // screen is BIG

        for(int i = 0; i < 6; i++){

          mbstate_t state;
          memset(&state, 0, sizeof(mbstate_t));
          const char *iter_row = backdropfill[i];
          int iter_col = 0; // Track the column position
          while (*iter_row) {
            wchar_t wc;
            size_t len = mbrtowc(&wc, iter_row, MB_CUR_MAX, &state); // Convert to wide char

            cchar_t cchar;
            setcchar(&cchar, &wc, 0, 0, NULL);

            is_char_in_search(wc, SEARCH_STR) ? attron(COLOR_PAIR(BACKGROUND)) : attron(COLOR_PAIR(FOREGROUND)) ;
            mvadd_wch(row/2 - 2 + i, (col-GLYPH_LENGTH)/2 + iter_col, &cchar);
            attroff(COLOR_PAIR(BACKGROUND));
            attroff(COLOR_PAIR(FOREGROUND));
            iter_row += len;
            iter_col++;
          }
        }

      }
      second_frame = 1;
    }

    if(elapsed_time > 0.2 && third_frame == 0){
      if (WIN_SIZE == NORMAL){
        for(int i = 0; i < 6; i++){
          mvprintw(row/2 - 3 + i, (col-GLYPH_LENGTH)/2, "%s", title[i]);
        }
      } else { // screen is big

        for(int i = 0; i < 6; i++){

          mbstate_t state;
          memset(&state, 0, sizeof(mbstate_t));
          const char *iter_row = titlefill[i];
          int iter_col = 0; // Track the column position
          while (*iter_row) {
            wchar_t wc;
            size_t len = mbrtowc(&wc, iter_row, MB_CUR_MAX, &state); // Convert to wide char

            cchar_t cchar;
            setcchar(&cchar, &wc, 0, 0, NULL);

            is_char_in_search(wc, SEARCH_STR) ? attron(COLOR_PAIR(BACKGROUND)) : attron(COLOR_PAIR(FOREGROUND)) ;
            mvadd_wch(row/2 - 2 + i, (col-GLYPH_LENGTH)/2 + iter_col, &cchar);
            attroff(COLOR_PAIR(BACKGROUND));
            attroff(COLOR_PAIR(FOREGROUND));
            iter_row += len;
            iter_col++;
          }
        }

      }
      third_frame = 1;
    }

    check_char(row, col);
    if (HOLD_CHAR != '\0') mvprintw(row/2, col/2, "%c", HOLD_CHAR);
    refresh();
  }

  quickprint(row, col, FOREGROUND, BACKGROUND, 0);

//mvprintw(row/2 + 3, (col-GLYPH_LENGTH)/2, "%s", title[6]);
  refresh();
}

void print_start_animation(int row, int col) {

  srand((unsigned)time(0));

//if (START_ANIMATION == EMPTY){
//  int start_roll = rand() % 3;
//  if (start_roll == 0){
//    START_ANIMATION = QUICK;
//  } else if (start_roll == 1){
//    START_ANIMATION = STANDARD;
//  } else {
//    START_ANIMATION = NEON;
//  }
//}

  START_ANIMATION = NEON; // override 'cus is's my favorite

  if (START_ANIMATION == NEON){
    neon(row, col);
  } else if (START_ANIMATION == QUICK){
    quickprint(row, col, FOREGROUND, BACKGROUND, 0);
  } else {
    printstandard(row, col);
    glitch(row, col);
  }
}

// Check screen size, return updated sum of dimensions
int check_size(int row, int col, int cache){

  if (cache != row + col){

//  optionally changing animations when resized
    START_ANIMATION = EMPTY;

    clear();

//  small win size jail
    while (col < NORMAL_GLYPH_LENGTH || row < NORMAL_GLYPH_HEIGHT){
      WIN_SIZE = SMALL;
      clear();
      mvprintw(row/2, (col-10)/2, "%s", "S.A.R.A.");
      refresh();

//    sleep 2 milliseconds
      usleep(2000);
      check_char(row, col);

      getmaxyx(stdscr,row,col); // Get total screen dimensions again
    }

    if (row > BIG_GLYPH_HEIGHT && col > BIG_GLYPH_LENGTH){
      WIN_SIZE = BIG;
      GLYPH_LENGTH = BIG_GLYPH_LENGTH;
      GLYPH_HEIGHT = BIG_GLYPH_HEIGHT;
    } else {
      WIN_SIZE = NORMAL;
      GLYPH_LENGTH = NORMAL_GLYPH_LENGTH;
      GLYPH_HEIGHT = NORMAL_GLYPH_HEIGHT;
    }
  }

  return row + col;
}

void glitch(int row, int col){

  int cache = row + col;

  int rng_row, rng_shift, rng_backdrop = 0;
  quickprint(row, col, FOREGROUND, BACKGROUND, 1); // only printing for colorbar, should create
                           // separate print_colorbar

  for( int i = 0 ; i < 46; i++ ) {
    rng_row   = rand() % NORMAL_GLYPH_HEIGHT;    // RNG 0 and 6
    rng_shift = (rand() % 3) - 1;       // RNG -1 and 1
    rng_backdrop = rand() % 3;          // RNG 0 and 2

    if (WIN_SIZE == NORMAL) {
      if (rng_backdrop == 0){
        mvprintw(row/2 - 3 + rng_row, (col - GLYPH_LENGTH)/2 - rng_shift, "%s", title[rng_row]);
      } else if (rng_backdrop == 1){
        mvprintw(row/2 - 3 + rng_row, (col - GLYPH_LENGTH)/2 - rng_shift, "%s", backdrop[rng_row]);
      } else {
        mvprintw(row/2 - 3 + rng_row, (col - GLYPH_LENGTH)/2 - rng_shift, "%s", foreground[rng_row]);
      }
    }

    if (WIN_SIZE == BIG) {
      attron(COLOR_PAIR(FOREGROUND));
      if (rng_backdrop == 0){
        mvprintw(row/2 - 2 + rng_row, (col - GLYPH_LENGTH)/2 - rng_shift, "%s", title[rng_row]);
      } else if (rng_backdrop == 1){
        mvprintw(row/2 - 2 + rng_row, (col - GLYPH_LENGTH)/2 - rng_shift, "%s", backdrop[rng_row]);
      } else {
        mvprintw(row/2 - 2 + rng_row, (col - GLYPH_LENGTH)/2 - rng_shift, "%s", foreground[rng_row]);
      }
      attroff(COLOR_PAIR(FOREGROUND));
    }

    check_char(row, col);
    if (rng_row == 3 && HOLD_CHAR != '\0') mvprintw(row/2, col/2, "%c", HOLD_CHAR);
    refresh();

    getmaxyx(stdscr, row, col);
    if (cache != row + col) break;

    usleep(07000);
  }

  quickprint(row, col, FOREGROUND, BACKGROUND, 0);
}

void mega_glitch(int row, int col, int duration){

}

void patch_backlight(int row, int col){

  attron(COLOR_PAIR(FOREGROUND));
  int offset = 0;
  if (WIN_SIZE != BIG) offset = 1;
  mvaddstr(row/2 - offset, (col-GLYPH_LENGTH)/2 + 1, "BACKLIGHT");
  attroff(COLOR_PAIR(FOREGROUND));

}

void patch_border(int row, int col){

  int offset = 0;
  if (WIN_SIZE != BIG) offset = 1;

  wchar_t wc;
  mbstate_t state;  // Tracks state of mbrtowc function when converting between types of chars
  // Converts character from iter_row to wide char `wc`
  // Also records length of character at *iter_row in len
  size_t len = mbrtowc(&wc, "║", MB_CUR_MAX, &state);
  cchar_t cchar;
  setcchar(&cchar, &wc, 0, 0, NULL);
  attron(COLOR_PAIR(BACKGROUND));
  mvadd_wch(row/2 - offset, (col/2 + GLYPH_LENGTH/2) - 1, &cchar);
  attroff(COLOR_PAIR(BACKGROUND));
}

void prompt_newlook(int row, int col) {

  clear();

  wchar_t search_str_doubles_lines[] = L"╔╗╚╝═║";
  wchar_t search_str_block[] = L"█";

  int offset = 0;
  if (WIN_SIZE != BIG) offset = 1;

  int selection = 1;
  int cache = row + col;
  char input = 0;
  int first_flag = 0;
  while(1){

    input = getch();

    if (input == 'j' || input == 'k' || input == 'l' || input == 'h' || first_flag == 0) {
      first_flag = 1;

      if (selection == 0) {
        selection = 1;
      } else {
        selection = 0;
      }

      for (int i = 0; i < BIG_GLYPH_HEIGHT; i++){
        selection == 1 ? attron(COLOR_PAIR(FOREGROUND)) : attron(COLOR_PAIR(BLACK));
        mvprintw(row/2 - 9 + i - offset, (col-GLYPH_LENGTH)/2 - 0, arch[i]);
        attroff(COLOR_PAIR(BACKGROUND));
        attroff(COLOR_PAIR(FOREGROUND));
      }

      for(int i = 0; i < 7; i++){
        mbstate_t state;
        memset(&state, 0, sizeof(mbstate_t));
        const char *iter_row = no_yes_window[i];
        int iter_col = 0; // Track the column position

        if (i == 0 || i == 6) {
          attron(COLOR_PAIR(FOREGROUND));
          mvprintw(row/2 - 2 + i - offset, (col-GLYPH_LENGTH)/2, no_yes_window[i]);
          attroff(COLOR_PAIR(FOREGROUND));
          refresh();
          continue;
        }

        while (*iter_row) {
          wchar_t wc;
          size_t len = mbrtowc(&wc, iter_row, MB_CUR_MAX, &state); // Convert to wide char

          cchar_t cchar;
          setcchar(&cchar, &wc, 0, 0, NULL);

          for (int j = 0; j < wcslen(search_str_block); j++) {

            int search_result = is_char_in_search(wc, search_str_block);
            // side borders when YES
            if ((iter_col == 0 || iter_col > 42) && selection == 1) {
              attron(COLOR_PAIR(BLACK));
            // side borders when NO
            } else if ((iter_col == 0 || iter_col > 42) && selection == 0) {
              attron(COLOR_PAIR(BLACK));
            // 'YES' dots when not selected
            } else if(search_result == 1 && iter_col > 19 && selection == 0){
              attron(COLOR_PAIR(FOREGROUND));
              wc = L'.';
              setcchar(&cchar, &wc, 0, 0, NULL);
            // 'NO' blocks when selected
            } else if (search_result == 1 && iter_col < 20 && selection == 0) {
              attron(COLOR_PAIR(FOREGROUND));
            // NO decorator blocks in rows 1 to 5 when selected
            } else if (search_result == 0 && selection == 0 && iter_col < 21){
              attron(COLOR_PAIR(FOREGROUND));
            //
            } else if (search_result == 0 && selection == 0 && iter_col > 19){
              attron(COLOR_PAIR(BLACK));
            // NO blocks when not selected
            } else if (search_result == 1 && iter_col < 20 && selection == 1){
              attron(COLOR_PAIR(FOREGROUND));
              wc = L'.';
              setcchar(&cchar, &wc, 0, 0, NULL);
            // YES blocks when selected
            } else if (search_result == 1 && iter_col > 19 && selection == 1){
              attron(COLOR_PAIR(FOREGROUND));
            // NO decorator blocks when not selected
            } else if (is_char_in_search(wc, search_str_doubles_lines) == 1 && iter_col < 21) {
              attron(COLOR_PAIR(BLACK));
            // YES decorator blocks when selected
            } else if (iter_col > 20 && selection == 1){
              attron(COLOR_PAIR(FOREGROUND));
            } 
//            
          }

          mvadd_wch(row/2 - 2 + i - offset, (col-GLYPH_LENGTH)/2 + iter_col, &cchar);
          attroff(COLOR_PAIR(FOREGROUND)); // no foreground manipulation in function
          attroff(COLOR_PAIR(BACKGROUND));
          attroff(COLOR_PAIR(BLUE));
          iter_row += len;
          iter_col++;
        }
      }

//    attron(COLOR_PAIR(FOREGROUND));
//    mvprintw(row/2 - 2 - offset, (col-GLYPH_LENGTH)/2 + 3, " RESKIN ? ");
//    attroff(COLOR_PAIR(FOREGROUND));
      refresh();

    } else if (input == 'q' || input == 'n') {
      glitch(row, col);
      break;
    } else if (input == '\n' || input == 'y') {
      if (selection == 1 || input == 'y'){
        pid_t pid = fork();

        if (pid < 0) {
          perror("fork");
          exit(EXIT_FAILURE);
        } else if (pid == 0) {
          execl("/usr/bin/bash", "bash", "/home/hakirot/.local/bin/newlook", (char *)NULL);
          perror("execl");
        } else {
          clear();
          refresh();
          int status;
          waitpid(pid, &status, 0);
        }

        neon(row, col);

      }
      glitch(row, col);
      break;
    }

    usleep(10000);
    getmaxyx(stdscr, row, col);
    if (cache != row + col) break;
  }
}

int input_color(char * arg){
  if (strcmp(arg, "black") == 0) {
      return 1;
  } else if (strcmp(arg, "red") == 0) {
      return 2;
  } else if (strcmp(arg, "green") == 0) {
      return 3;
  } else if (strcmp(arg, "yellow") == 0) {
      return 4;
  } else if (strcmp(arg, "blue") == 0) {
      return 5;
  } else if (strcmp(arg, "magenta") == 0) {
      return 6;
  } else if (strcmp(arg, "cyan") == 0) {
      return 7;
  } else if (strcmp(arg, "white") == 0) {
      return 8;
  } else {
      return -1;
  }
}

void xray(int row, int col){

  quickprint(row, col, FOREGROUND, BACKGROUND, 0);

  int cache = (row + col);
  int i = 0;
  int k = 0;
  int offset = 0;
  int toggle = 0;
  int exit_flag = 0;
  wchar_t search_str_xray[] = L"╔╗╚╝═║█SPECIALTONRG";
  const char** glyph_pointer;
  const char** backdrop_glyph_pointer;

  if (WIN_SIZE == BIG){
    glyph_pointer = titlefill;
    backdrop_glyph_pointer = backdropfill;
  } else {
    glyph_pointer = title;
    backdrop_glyph_pointer = backdrop;
    offset = 1;
  }

  while(1){

    attron(COLOR_PAIR(BACKGROUND));
    mvprintw(row/2 - 2 + i - offset, (col-GLYPH_LENGTH)/2, backdrop_glyph_pointer[i]);
    attroff(COLOR_PAIR(BACKGROUND));

    int previous_row = 0;

    const char *iter_row =  glyph_pointer[0];
    if (toggle == 0 && i == 0){}
    else {

      mbstate_t state;
      memset(&state, 0, sizeof(mbstate_t));
      if (toggle == 0) { iter_row = glyph_pointer[i - 1]; }
      else { iter_row = glyph_pointer[i + 1]; }
      int iter_col = 0; // Track the column position

      while (*iter_row) {
        wchar_t wc;
        size_t len = mbrtowc(&wc, iter_row, MB_CUR_MAX, &state); // Convert to wide char

        cchar_t cchar;
        setcchar(&cchar, &wc, 0, 0, NULL);

        if (i == 5 && toggle == 1 && iter_col > 9 && iter_col < 42 && WIN_SIZE == BIG){
          attron(COLOR_PAIR(FOREGROUND + 8));
        } else if(is_char_in_search(wc, search_str_xray) && WIN_SIZE == BIG){
          attron(COLOR_PAIR(FOREGROUND));
        } else if (is_char_in_search(wc, search_str_xray)){
          attron(COLOR_PAIR(WHITE));
        } else {
          attron(COLOR_PAIR(BACKGROUND)) ;
        }

        int prev = 0;
        if (toggle == 1) { prev = 1; }
        else { prev = -1; }

        mvadd_wch(row/2 - 2 + i + prev - offset, (col-GLYPH_LENGTH)/2 + iter_col, &cchar);
        attron(COLOR_PAIR(WHITE));
        attroff(COLOR_PAIR(FOREGROUND));
        attroff(COLOR_PAIR(FOREGROUND + 8));
        attroff(COLOR_PAIR(BACKGROUND));
        iter_row += len;
        iter_col++;
      }
    }

    refresh();
    usleep(15000);
//  usleep(800000);

    getmaxyx(stdscr, row, col);
    if (cache != row + col) return;

    check_char(row, col);

//  DEBUG
//  mvaddch(row/2,col/2, i + 48);

    // maybe extract this block to a range oscillation handler function
    // i = oscillator(i, min, max, toggle);
    if(i == 0) toggle = 0;
    if (i == 6) toggle = 1;
    if(toggle == 0) i++;
    if(toggle == 1) i--;
//  if(i == 7) error("out of upper bounds");
//  if(i == -1) error("out of lower bounds");
    if(exit_flag) break;
    if(i == 0) k++;
    if (i== 0 && k == 2)exit_flag = 1;
  }

  if(WIN_SIZE == BIG){
    clear();
    refresh();
    usleep(80000);
    for (int i = 0; i < BIG_GLYPH_HEIGHT; i++){
      attron(COLOR_PAIR(BACKGROUND));
      mvprintw(row/2 - 9 + i - offset, (col-GLYPH_LENGTH)/2 - 0, arch[i]);
      attroff(COLOR_PAIR(BACKGROUND));
    }
    refresh();
    usleep(80000);
    for (int i = 0; i < BIG_GLYPH_HEIGHT; i++){
      attron(COLOR_PAIR(BACKGROUND));
      mvprintw(row/2 - 9 + i - offset, (col-GLYPH_LENGTH)/2 - 0, archsarazap[i]);
      attroff(COLOR_PAIR(BACKGROUND));
    }
    refresh();
    usleep(80000);
    quickprint(row,col, FOREGROUND, BACKGROUND, 0);
  } else {
    neon(row, col);
  }
}

void pshd(int row, int col){

}

void get_helped() {
  printf("Usage: %s [OPTIONS]\n", "sara");
  printf("  --help, -h    Get helped\n");
  printf("  -c            Constant effects\n");
  printf("  -G            Constant glitch effect\n");
  printf("  -M            Constant MEGA glitch effect\n");
  printf("  -f [color]         set custom FOREGROUND color\n");
  printf("  -b [color]         set custom BACKGROUND color\n");
  exit(0);
}

void error(char * err) {
  endwin();
  printf("%s\n", err);
  exit(1);
}

