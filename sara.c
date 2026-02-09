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
#include "animations.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>
#include <time.h>
#include <wait.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

typedef enum {
  SMALL,
  NORMAL,
  BIG
} screen_size;

typedef enum {
  QUICK,
  STANDARD,
  NEON,
  EMPTY
} start_animation;

clock_t LAST_INPUT_TIME;
screen_size WIN_SIZE;
clock_t WAIT_START;

int GLYPH_LENGTH;
int GLYPH_HEIGHT;

int FOREGROUND;
int BACKGROUND;
int HOLOGRAPHIC;
int FOLLOW;

int ROW;
int COL;
int CACHE;

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
const int BLACK_WHITED  = 17;
const int RED_BLACK     = 18;
const int GREEN_BLACK   = 19;
const int YELLOW_BLACK  = 20;
const int BLUE_BLACK    = 21;
const int MAGENTA_BLACK = 22;
const int CYAN_BLACK    = 23;
const int WHITE_BLACKD  = 24;

start_animation START_ANIMATION = EMPTY;
char HOLD_CHAR;
int GLITCH_FRAME_TIME = 7000;

int main(int argc, char* argv[]) {

  srand((unsigned)time(0));

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
        if (FOREGROUND == -1){
          char errstr[256] = {'\0'};
          sprintf(errstr, "Bad arg: '%s'", argv[i + 1]);
          error(errstr);
        }
      } else {
        get_helped();
        error("bad arg\n");
      }
    } else if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--background") == 0){
      if(i + 1 < argc){
        BACKGROUND = input_color(argv[i + 1]);
        if (BACKGROUND == -1){
          char errstr[256] = {'\0'};
          sprintf(errstr, "Bad arg: '%s'", argv[i + 1]);
          error(errstr);
        }
      } else {
        get_helped();
        error("bad arg\n");
      }
    }
  }

  double WAIT_BUFFER = 0.10000;

  int opt;
  while ((opt = getopt(argc, argv, "cMFfbhrH")) != -1){
    switch (opt) {
      case 'c': WAIT_BUFFER = 0.00005; break;
      case 'r':
        BACKGROUND = (rand() % 7) + 1;    // RNG 1 and 7
        FOREGROUND = (rand() % 6) + 2;    // RNG 2 and 7
        if (FOREGROUND > 7 || FOREGROUND < 1 || BACKGROUND > 7 || BACKGROUND < 1){
          error("error");
        }
        break;
      case 'H': HOLOGRAPHIC = 1; break;
      case 'F': FOLLOW = 1; break;
    }
  }

  double time_idle;
  WAIT_START = clock();
  LAST_INPUT_TIME = clock();
  bool should_print = false;
  init_window();

  while(1){

    check_char(); // check input for this cycle

//  get current screen dimensions
    getmaxyx(stdscr, ROW, COL);
    CACHE = check_size();
    if (START_ANIMATION == EMPTY) print_start_animation();

    time_idle = (double)(clock() - WAIT_START) / CLOCKS_PER_SEC;

//  if(time_idle >= WAIT_BUFFER){
//    glitch(ROW, COL);
//    WAIT_START = clock();
//  }

    if(HOLOGRAPHIC > 0 && WIN_SIZE == BIG){
      BACKGROUND++;
      if (BACKGROUND > 7) BACKGROUND = 2;
      quickprint(FOREGROUND, BACKGROUND, 0);
    }

    // print only once after the HOLD_CHAR goes back to EOF
    if (HOLD_CHAR == '\0' && should_print == true){
      quickprint(FOREGROUND, BACKGROUND, 0);
      should_print = false;
    } else if (HOLD_CHAR != '\0'){
      should_print = true;
    }
    usleep(50000); // chill
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

void check_char() {

  char input = getch();

  if (input != ERR && input != '\n' && input != EOF && input > 31 && input < 127) {

    if(input == 'q'){
      if (FOLLOW){
        ensure_cache_dir();
        char cache_file[256] = {'\0'};
        char * env_home = getenv("HOME");
        char * env_pwd = getenv("PWD");
        sprintf(cache_file, "%s%s", env_home, "/.cache/sara/saraexit");
        FILE * fp = fopen(cache_file, "w");
        fprintf(fp, "%s", env_pwd);
        fclose(fp);
      }

      clear();
      refresh();
      move(0, 0);
      endwin();
      exit(0);
    } else if(input == 'r'){

      CACHE = ROW + COL;

      // ranger man
      //--choosedir=targetfile
      //    Allows you to pick a directory with ranger.  When you exit ranger, it will write the last visited directory into targetfile.

      ensure_cache_dir();
      char cache_file[256] = {'\0'};

      pid_t cur_pid = getpid();

      char * env_home = getenv("HOME");
      sprintf(cache_file, "%s%s%d", env_home, "/.cache/sara/sara", cur_pid);

      pid_t pid = fork();
      if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
      } else if (pid == 0) {
        endwin();

        execlp("ranger", "ranger", "--choosedir", cache_file, NULL);
        error("ERROR: execv ranger");

      } else {
        endwin();
        int status;

        while(kill(pid, 0) == 0){
          waitpid(pid, &status, 0);
        }

        FILE *fp;

        fp = fopen(cache_file, "r");
        if(!fp){
          refresh();
          return;
        }

        char target_chdir[256] = {'\0'};
        if(fgets(target_chdir, 256, fp) == NULL) {
          char error_str[256] = {'\0'};
          sprintf(error_str, "%s%s" , "Error reading target_chdir file ", target_chdir);
          error(error_str);
        }

        fclose(fp);

        remove(cache_file) ? error("No file to be deleted"): 0;

        chdir(target_chdir);

				if (setenv("PWD", target_chdir, 1) != 0) {  
          error("setenv error");
				}
      }

      clear();           // getmaxyx requires refresh() to get a new screensize
      refresh();
      getmaxyx(stdscr, ROW, COL);
      if(CACHE == ROW + COL){
        neon();
      }

    } else if(input == 'R'){

      const char *home_dir = getenv("HOME");
      const char *dls = "/dls";
      char dl_dir[256] = {'\0'};
      sprintf(dl_dir, "%s%s", home_dir, dls);
      chdir(dl_dir);
      if (setenv("PWD", dl_dir, 1) != 0) {
        error("setenv error");
      } else {

        pid_t pid = fork();
        if (pid < 0) {
          perror("fork");
          exit(EXIT_FAILURE);
        } else if (pid == 0) {
          endwin();
          execv("/usr/bin/rtorrent", NULL);
          error("ERROR: execv rtorrent");
        } else {
          endwin();
          int status;

          while(kill(pid, 0) == 0){
            waitpid(pid, &status, 0);
          }
        }
      }
      neon();

    } else if(input == 'S'){

      char error_check = getch();
      if (error_check == 'A') {
        char * buffer = NULL;
        size_t bufsize = 0;
        getline(&buffer, &bufsize, stdin);
        glitch(10, 0);
        return;
      }

      char * choices[1]={'\0'};
      choices[0]="SHUTDOWN";
      const char* selection =  select_option_window(choices, 1);

      if (selection == choices[0]){

        pid_t pid = fork();
        if (pid < 0) {
          perror("fork");
          exit(EXIT_FAILURE);
        } else if (pid == 0) {
          endwin();
          execv("/home/hakirot/skps/cleanup.sh", NULL);
          error("ERROR: execv cleanup");
        } else {
          endwin();
          int status;

          // wait for cleanup
          while(kill(pid, 0) == 0){
            waitpid(pid, &status, 0);
          }

          execlp("shutdown", "shutdown", "now", NULL);
          error("shutdown err");
        }
      } else {
        glitch(46, 0);
      }

    } else if(input == 'i'){
      int temp = BACKGROUND;
      BACKGROUND = FOREGROUND;
      FOREGROUND = temp;
      quickprint(FOREGROUND, BACKGROUND, 0);
    } else if(input == 'I'){
      BACKGROUND = rand() % 7 + 1;    // RNG 1 and 7
      FOREGROUND = rand() % 7 + 2;    // RNG 2 and 8
      quickprint(FOREGROUND, BACKGROUND, 0);
    } else if(input == 'H'){
      if (HOLOGRAPHIC == 1){
        HOLOGRAPHIC = 0;
      } else {
        HOLOGRAPHIC = 1;
      }
    } else if(input == 'w'){
      pid_t pid = fork();
      if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
      } else if (pid == 0) {
        endwin();
        execlp("ranger", "ranger", "/home/hakirot/pix/walls/", NULL);
        error("execlp");
      } else {
        endwin();
        int status;

        // kill(_, 0) checks if ranger exited, as it will reload itself when resized
        while(kill(pid, 0) == 0){
          waitpid(pid, &status, 0);
        }
        neon();
      }
    } else if(input == 'g'){
      glitch(46, 0);
//    xray(ROW, COL);
    } else if(input == 't'){

      CACHE = ROW + COL;

      pid_t pid = fork();
      if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);

      } else if (pid == 0) {
        char * notes_dir = "/home/hakirot/dox/.notes/";
        chdir("notes_dir");
        if (setenv("PWD", notes_dir, 1) != 0) {
          error("setenv error");
        }
        glitch(20, 1);
        endwin();
        execlp("nvim", "nvim", "/home/hakirot/dox/.notes/tasks", NULL);
        error("ERROR: execlp nvim");
      } else {

        int status;

        // kill(_, 0) checks if ranger exited naturally, ranger will reload itself when resized
        while(kill(pid, 0) == 0){
          waitpid(pid, &status, 0);
        }

        endwin();
        clear();
        refresh();
        getmaxyx(stdscr, ROW, COL);
        if(CACHE == ROW + COL){
          neon();
        }
      }

    } else if(input == 'p'){
      pshd();

    } else if(input == 'P'){

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

      neon();

    } else if(input == 'n'){

      prompt_newlook();

    } else if(input == 'b'){

      // Display menu

      char* choices[2]={'\0'};
      choices[0]="BLUETOOTH";
      choices[1]="BACKLIGHT";
      const char * selection = select_option_window(choices, 2);

      if (selection == choices[0]){ // When selected with `selection`

        char* bluetooth_choices[2]={'\0'};
        bluetooth_choices[0]="CONNECT";
        bluetooth_choices[1]="DISCONNECT";
        selection = select_option_window(bluetooth_choices, 2);

        if (selection == bluetooth_choices[0]){
          char* connect_choices[2]={'\0'};
          connect_choices[0]="WH-1000XM5";
          connect_choices[1]="ACOUSTIC";
          selection = select_option_window(connect_choices, 2);

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
          selection = select_option_window(disconnect_choices, 2);

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

        neon();

      } if (selection == choices[1]){

        char * file_path = "/sys/class/backlight/intel_backlight/brightness";

        ensure_path_perm(file_path, 'w');

				FILE *fptr = fopen(file_path, "r");
        if(!fptr) error("fopen(file_path)");

        char buff[8];
        memset(&buff, '\0', 8 * sizeof(char));
        fgets(buff, 8, fptr);
        fclose(fptr);

        // Read buff to int
        int brightness;
        sscanf(buff, "%d", &brightness);

        int offset = 0;
        if(WIN_SIZE != BIG) offset = 1;

        quickprint(FOREGROUND, FOREGROUND, 0);

        attron(COLOR_PAIR(BACKGROUND));
        for (int i = 0; i < NORMAL_GLYPH_HEIGHT; i++){
          mvprintw(ROW/2-2 + i - offset, (COL-GLYPH_LENGTH)/2, "%s", option_window[i]);
        }
        attroff(COLOR_PAIR(BACKGROUND));

        // print current brightness
        attron(COLOR_PAIR(FOREGROUND + 8));
        mvaddstr(ROW/2 - offset - 1, COL/2 - 11, buff);
        attroff(COLOR_PAIR(FOREGROUND + 8));

        patch_backlight();
        patch_border();
        refresh();

        CACHE = ROW + COL;

        while(1){

          int input = getch();
          char str[16];

          if (input != ERR && input != '\n' && input != EOF && input != 'q' && input != '\n') {
            if (input == 'j'){
              brightness -= 20;
              if (brightness < 0) brightness = 0;
            } else if (input == 'J'){
              brightness -= 100;
              if (brightness < 0) brightness = 0;
            } else if (input == 'k'){
              brightness += 20;
              if (brightness > 1500) brightness = 1500;
            } else if (input == 'K'){
              brightness += 100;
              if (brightness > 1500) brightness = 1500;
            }

            // Update UI
            sprintf(str, "%d", brightness);

            mvaddstr(ROW/2 - offset - 1, COL/2 - 11, "     ");
            attron(COLOR_PAIR(FOREGROUND + 8));
            mvaddstr(ROW/2 - offset - 1, COL/2 - 11, str);
            attroff(COLOR_PAIR(FOREGROUND + 8));

            patch_border();

            refresh();

            FILE *ptr = fopen(file_path, "w");

            if(ptr){
              // Write brightness to file
              fprintf(ptr, "%s", str);
              fclose(ptr);
            } else {
              error("Not writable");
            }

          } else if (input == 'q' || input == '\n'){
            break;
          }

          getmaxyx(stdscr, ROW, COL);
          if (CACHE != ROW + COL) break;
          usleep(1000);
        }
      }

    neon();

    } else if (input == 'v') {

      CACHE = ROW + COL;

      pid_t pid = fork();
      if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);

      } else if (pid == 0) {
        glitch(20, 1);
        endwin();
        execv("/usr/bin/nvim", NULL);
        error("ERROR: execv nvim");
      } else {
        int status;

        while(kill(pid, 0) == 0){
          waitpid(pid, &status, 0);
        }

        endwin();
        clear();
        refresh();
        getmaxyx(stdscr, ROW, COL);
        if(CACHE == ROW + COL){
          neon();
        }
      }

    } else if (input == 'y') {
      CACHE = ROW + COL;

      pid_t pid = fork();
      if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);

      } else if (pid == 0) {
        endwin();
        execlp("yay", "yay", NULL);
        error("ERROR: execlp yay");
      } else {
        endwin();
        int status;

        while(kill(pid, 0) == 0){
          waitpid(pid, &status, 0);
        }

        clear();
        refresh();
        getmaxyx(stdscr, ROW, COL);
        if(CACHE == ROW + COL){
          neon();
        }
      }

    } else if (input == 'm') {
      CACHE = ROW + COL;
      pid_t pid = fork();
      if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);

      } else if (pid == 0) {
        endwin();
        execlp("make", "make", NULL);
        error("ERROR: execlp make");
      } else {

        endwin();
        int status;

        while(kill(pid, 0) == 0){
          waitpid(pid, &status, 0);
        }

        if(status != 0){
          clear();
          printf("Press enter");
          getchar();
        }

        clear();
        getmaxyx(stdscr, ROW, COL);
        if(CACHE != ROW + COL) return;
        if (status == 0){
          slide_in();
          neon();
        } else {
          slide_in();
          glitch(20, 0);
        }

        refresh();
      }
    } else if (input == 'M') {

      CACHE = ROW + COL;
      pid_t pid = fork();
      if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);

      } else if (pid == 0) {
        endwin();
        execlp("rmpc", "rmpc", NULL);
        error("ERROR: execlp rmpc");
      } else {

        endwin();
        int status;

        while(kill(pid, 0) == 0){
          waitpid(pid, &status, 0);
        }

        clear();
        refresh();
        getmaxyx(stdscr, ROW, COL);
        if(CACHE == ROW + COL){
          neon();
        }
      }

    } else if (input == 'Q') {

      endwin();
      execlp("tmux", "tmux", "kill-pane", NULL);
      error("ERROR: execlp tmux kill-pane");

    } else if (input == 'X') {

      endwin();
      execlp("tmux", "tmux", "kill-session", NULL);
      error("ERROR: execlp tmux kill-session");

    } else if (WIN_SIZE != SMALL) {
      LAST_INPUT_TIME = clock();
      HOLD_CHAR = input;
      mvprintw(ROW/2, COL/2, "%c", HOLD_CHAR);
      refresh();
    }
  } 

  double time_since_input = (double)(clock() - LAST_INPUT_TIME) / CLOCKS_PER_SEC;
  if(time_since_input >= 0.0005 && WIN_SIZE != SMALL){
    HOLD_CHAR = '\0';
  }
}

void printstandard(){

  if (WIN_SIZE == NORMAL){
    attron(COLOR_PAIR(FOREGROUND));
    for(int i = 0; i < GLYPH_HEIGHT; i++){
      mvprintw(ROW/2 - 3 + i, (COL-GLYPH_LENGTH)/2, "%s", title[i]);
      check_char();
      if(HOLD_CHAR) mvprintw(ROW/2, COL/2, "%c", HOLD_CHAR);
      refresh();
      usleep(20000);
    }
    attroff(COLOR_PAIR(FOREGROUND));

  } else if (WIN_SIZE == BIG){

    attron(COLOR_PAIR(FOREGROUND));
    for(int i = 0; i < GLYPH_HEIGHT; i++){
      mvprintw(ROW/2 - 9 + i, (COL-GLYPH_LENGTH)/2, "%s", arch[i]);
      if(HOLD_CHAR) mvprintw(ROW/2, COL/2, "%c", HOLD_CHAR);
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

        // Write wide char to `cchar` for mvadd_wch()
        cchar_t cchar;
        setcchar(&cchar, &wc, 0, 0, NULL);
        mvadd_wch(ROW/2 + 3 - i, (COL-GLYPH_LENGTH)/2 + iter_col, &cchar);

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

void quickprint(int fg, int bg, int printColorbar){
  clear();
  if (WIN_SIZE == NORMAL){
    attron(COLOR_PAIR(FOREGROUND));
    for(int i = 0; i < GLYPH_HEIGHT; i++){
      mvprintw(ROW/2 - 3 + i, (COL-GLYPH_LENGTH)/2, "%s", title[i]);
    }
    attroff(COLOR_PAIR(FOREGROUND));
  } else if (WIN_SIZE == BIG){
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
        mvadd_wch(ROW/2 - 9 + i, (COL-GLYPH_LENGTH)/2 + iter_col, &cchar);
        attroff(COLOR_PAIR(fg));
        attroff(COLOR_PAIR(bg));
        iter_row += len;
        iter_col++;
      }
    }
    // print name with background
    attron(COLOR_PAIR(fg + 8));
    mvprintw(ROW/2 + 4, (COL-GLYPH_LENGTH)/2 + 10, "%s", specialApplicationRangingArea);
    attroff(COLOR_PAIR(fg + 8));

    // print colorbar
    if (printColorbar == 1){
      for(int i = 1; i < 9; i++){
        attron(COLOR_PAIR(i));
        mvaddwstr(ROW/2 + 5, (COL-GLYPH_LENGTH)/2 + 15 + (i*3), L"\u2588\u2588\u2588"); // Unicode full block █
        attroff(COLOR_PAIR(i));
      }
    }
  }
  if(HOLD_CHAR) mvprintw(ROW/2, COL/2, "%c", HOLD_CHAR);
  refresh();
}

const char * select_option_window(char** choices, int len){

  int selection = 0;
  CACHE = ROW + COL;

  quickprint(FOREGROUND, FOREGROUND, 0); // hardcoded style choice

  int offset = 0;
  if (WIN_SIZE != BIG) offset = 1;

  attron(COLOR_PAIR(BACKGROUND));
  for (int i = 0; i < NORMAL_GLYPH_HEIGHT; i++){
    mvprintw(ROW/2-2 + i - offset, (COL-GLYPH_LENGTH)/2, "%s", option_window[i]);
  }
  attroff(COLOR_PAIR(BACKGROUND));

  while(1){

    if (WIN_SIZE != BIG){
      offset = 1;
    } else {
      offset = 0;
    }

    getmaxyx(stdscr, ROW, COL);
    if (CACHE != ROW + COL) break;

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
      mvprintw(ROW/2 + i - 1 - offset, (COL-GLYPH_LENGTH)/2 + 1, "%s", choices[i]);
      attroff(COLOR_PAIR(FOREGROUND + 8));
      attroff(COLOR_PAIR(FOREGROUND));
    }

    refresh();
    usleep(1000); // chill
  }
}

void neon() {

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
        attron(COLOR_PAIR(FOREGROUND));
        for(int i = 0; i < 6; i++){
          mvprintw(ROW/2 - 3 + i, (COL-GLYPH_LENGTH)/2, "%s", backdrop[i]);
        }
        attroff(COLOR_PAIR(FOREGROUND));
      } else { // screen is BIG
        for(int i = 0; i < 19; i++){
          attron(COLOR_PAIR(BACKGROUND));
          mvprintw(ROW/2 - 9 + i, (COL-GLYPH_LENGTH)/2, "%s", arch[i]);
          attroff(COLOR_PAIR(BACKGROUND));
        }
      }

      if (colorbar_printed == false ){
        if (WIN_SIZE == BIG) {
          for(int i = 1; i < 9; i++){
            attron(COLOR_PAIR(i));
            mvaddwstr(ROW/2 + 4, (COL-GLYPH_LENGTH)/2 + 3 + (i*3), L"\u2588\u2588\u2588"); // Unicode full block █
            attroff(COLOR_PAIR(i));
          }
        }
        colorbar_printed == true;
      }

      first_frame = 1;
    }

    if(elapsed_time > 0.1 && second_frame == 0){
      if (WIN_SIZE == NORMAL){
        attron(COLOR_PAIR(FOREGROUND));
        for(int i = 0; i < 6; i++){
          mvprintw(ROW/2 - 3 + i, (COL-GLYPH_LENGTH)/2, "%s", title[i]);
        }
        attroff(COLOR_PAIR(FOREGROUND));
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
            mvadd_wch(ROW/2 - 2 + i, (COL-GLYPH_LENGTH)/2 + iter_col, &cchar);
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
        attron(COLOR_PAIR(FOREGROUND));
        for(int i = 0; i < 6; i++){
          mvprintw(ROW/2 - 3 + i, (COL-GLYPH_LENGTH)/2, "%s", title[i]);
        }
        attroff(COLOR_PAIR(FOREGROUND));
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
            mvadd_wch(ROW/2 - 2 + i, (COL-GLYPH_LENGTH)/2 + iter_col, &cchar);
            attroff(COLOR_PAIR(BACKGROUND));
            attroff(COLOR_PAIR(FOREGROUND));
            iter_row += len;
            iter_col++;
          }
        }

      }
      third_frame = 1;
    }

    check_char();
    if (HOLD_CHAR != '\0') mvprintw(ROW/2, COL/2, "%c", HOLD_CHAR);
    refresh();
  }

  quickprint(FOREGROUND, BACKGROUND, 0);

//mvprintw(ROW/2 + 3, (COL-GLYPH_LENGTH)/2, "%s", title[6]);
  refresh();
}

void print_start_animation() {

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
    neon();
  } else if (START_ANIMATION == QUICK){
    quickprint(FOREGROUND, BACKGROUND, 0);
  } else {
    printstandard();
    glitch(46, 0);
  }
}

// Check screen size, return updated sum of dimensions
int check_size(){

  if (CACHE != ROW + COL){

//  optionally changing animations when resized
    START_ANIMATION = EMPTY;

    clear();

//  small win size jail
    while (COL < NORMAL_GLYPH_LENGTH || ROW < NORMAL_GLYPH_HEIGHT){
      WIN_SIZE = SMALL;
      clear();
      mvprintw(ROW/2, (COL-10)/2, "%s", "S.A.R.A.");
      refresh();

//    sleep 2 milliseconds
      usleep(2000);
      check_char();

      getmaxyx(stdscr,ROW,COL); // Get total screen dimensions again
    }

    if (ROW > BIG_GLYPH_HEIGHT && COL > BIG_GLYPH_LENGTH){
      WIN_SIZE = BIG;
      GLYPH_LENGTH = BIG_GLYPH_LENGTH;
      GLYPH_HEIGHT = BIG_GLYPH_HEIGHT;
    } else {
      WIN_SIZE = NORMAL;
      GLYPH_LENGTH = NORMAL_GLYPH_LENGTH;
      GLYPH_HEIGHT = NORMAL_GLYPH_HEIGHT;
    }
  }

  return ROW + COL;
}

void glitch(int numFrames, int full){

  CACHE = ROW + COL;

  int rng_row, rng_shift, rng_backdrop = 0;
  quickprint(FOREGROUND, BACKGROUND, 1);

  for( int i = 0 ; i < numFrames; i++ ) {
    rng_row   = rand() % NORMAL_GLYPH_HEIGHT;    // RNG 0 and 6
    rng_shift = (rand() % 3) - 1;       // RNG -1 and 1
    rng_backdrop = rand() % 3;          // RNG 0 and 2

    if (WIN_SIZE == NORMAL) {
      attron(COLOR_PAIR(FOREGROUND));
      if (rng_backdrop == 0){
        mvprintw(ROW/2 - 3 + rng_row, (COL - GLYPH_LENGTH)/2 - rng_shift, "%s", title[rng_row]);
      } else if (rng_backdrop == 1){
        mvprintw(ROW/2 - 3 + rng_row, (COL - GLYPH_LENGTH)/2 - rng_shift, "%s", backdrop[rng_row]);
      } else {
        mvprintw(ROW/2 - 3 + rng_row, (COL - GLYPH_LENGTH)/2 - rng_shift, "%s", foreground[rng_row]);
      }
      attroff(COLOR_PAIR(FOREGROUND));
    }

    if (WIN_SIZE == BIG) {

      if(full == 1){

        attron(COLOR_PAIR(BACKGROUND));
        int rng_rowx = rand() % BIG_GLYPH_HEIGHT;
        int rng_shift = (rand() % 7) - 1;       // RNG -1 and 1
        if(rng_rowx < 7 || rng_rowx > 13){
          mvprintw(ROW/2 - 9 + rng_rowx, (COL - GLYPH_LENGTH)/2 - rng_shift, "%s", arch[rng_rowx]);
        }
        attroff(COLOR_PAIR(BACKGROUND));
      }

      attron(COLOR_PAIR(FOREGROUND));
      if (rng_backdrop == 0){
        mvprintw(ROW/2 - 2 + rng_row, (COL - GLYPH_LENGTH)/2 - rng_shift, "%s", title[rng_row]);
      } else if (rng_backdrop == 1){
        mvprintw(ROW/2 - 2 + rng_row, (COL - GLYPH_LENGTH)/2 - rng_shift, "%s", backdrop[rng_row]);
      } else {
        mvprintw(ROW/2 - 2 + rng_row, (COL - GLYPH_LENGTH)/2 - rng_shift, "%s", foreground[rng_row]);
      }
      attroff(COLOR_PAIR(FOREGROUND));
    }

    check_char();
    if (rng_row == 3 && HOLD_CHAR != '\0') mvprintw(ROW/2, COL/2, "%c", HOLD_CHAR);
    refresh();

    getmaxyx(stdscr, ROW, COL);
    if (CACHE != ROW + COL) break;

    usleep(GLITCH_FRAME_TIME);
  }

  quickprint(FOREGROUND, BACKGROUND, 0);
}

void mega_glitch(int duration){

}

void patch_backlight(){

  attron(COLOR_PAIR(FOREGROUND));
  int offset = 0;
  if (WIN_SIZE != BIG) offset = 1;
  mvaddstr(ROW/2 - offset - 1, (COL-GLYPH_LENGTH)/2 + 1, "BACKLIGHT");
  attroff(COLOR_PAIR(FOREGROUND));
}

void patch_border(){

  int offset = 0;
  if (WIN_SIZE != BIG) offset = 1;

  wchar_t wc;
  mbstate_t state;
  memset(&state, 0, sizeof(mbstate_t));
  size_t len = mbrtowc(&wc, "║", MB_CUR_MAX, &state);

  cchar_t cchar;
  setcchar(&cchar, &wc, 0, 0, NULL);
  attron(COLOR_PAIR(BACKGROUND));
  mvadd_wch(ROW/2 - offset - 1, (COL/2 + GLYPH_LENGTH/2) - 1, &cchar);
  attroff(COLOR_PAIR(BACKGROUND));
}

void prompt_newlook() {

  clear();

  wchar_t search_str_doubles_lines[] = L"╔╗╚╝═║";
  wchar_t search_str_block[] = L"█";

  int offset = 0;
  if (WIN_SIZE != BIG) offset = 1;

  int selection = 1;
  CACHE = ROW + COL;
  char input = 0;
  int first_flag = 0;
  int exit_glitch_flag = 0;
  char * wall;
  while(1){

    input = getch();

    if (input == 'j' || input == 'l' || input == 'h' || first_flag == 0) {
      first_flag = 1;

      if (selection == 0) {
        selection = 1;
      } else {
        selection = 0;
      }

      for (int i = 0; i < BIG_GLYPH_HEIGHT; i++){
        selection == 1 ? attron(COLOR_PAIR(FOREGROUND)) : attron(COLOR_PAIR(BLACK));
        mvprintw(ROW/2 - 9 + i - offset, (COL-GLYPH_LENGTH)/2 - 0, arch[i]);
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
          mvprintw(ROW/2 - 2 + i - offset, (COL-GLYPH_LENGTH)/2, no_yes_window[i]);
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
          }

          mvadd_wch(ROW/2 - 2 + i - offset, (COL-GLYPH_LENGTH)/2 + iter_col, &cchar);
          attroff(COLOR_PAIR(FOREGROUND)); // no foreground manipulation in function
          attroff(COLOR_PAIR(BACKGROUND));
          attroff(COLOR_PAIR(BLUE));
          iter_row += len;
          iter_col++;
        }
      }

      refresh();

    } else if (input == 'k') {
      wall = prompt_fuzzy();
      getmaxyx(stdscr, ROW, COL);
      if (CACHE != ROW + COL || wall == NULL) {
        exit_glitch_flag = 1;
        break;
      } else {
        fork_newlook(wall);
      }

      // break
    } else if (input == 'q' || input == 'n') {
      exit_glitch_flag = 1;
      break;
    } else if (input == '\n' || input == 'y') {
      if (selection == 1 || input == 'y'){
        fork_newlook(NULL);
      } else if (selection == 0 || CACHE != ROW + COL) {
          exit_glitch_flag = 1;
      }

      break;
    }

    usleep(10000);
    getmaxyx(stdscr, ROW, COL);
    if (CACHE != ROW + COL) {
      exit_glitch_flag = 1;
      break;
    }
  }

  if(exit_glitch_flag){
    glitch(46, 0);
  } else {
    neon();
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

void xray(){

  quickprint(FOREGROUND, BACKGROUND, 0);

  CACHE = ROW + COL;
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
    mvprintw(ROW/2 - 2 + i - offset, (COL-GLYPH_LENGTH)/2, backdrop_glyph_pointer[i]);
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

        mvadd_wch(ROW/2 - 2 + i + prev - offset, (COL-GLYPH_LENGTH)/2 + iter_col, &cchar);
        attron(COLOR_PAIR(WHITE));
        attroff(COLOR_PAIR(FOREGROUND));
        attroff(COLOR_PAIR(FOREGROUND + 8));
        attroff(COLOR_PAIR(BACKGROUND));
        iter_row += len;
        iter_col++;
      }
    }

    refresh();
//  usleep(15000);
    usleep(100000);

    getmaxyx(stdscr, ROW, COL);
    if (CACHE != ROW + COL) return;

    check_char();

//  DEBUG
//  mvaddch(ROW/2,COL/2, i + 48);

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
      mvprintw(ROW/2 - 9 + i - offset, (COL-GLYPH_LENGTH)/2 - 0, arch[i]);
      attroff(COLOR_PAIR(BACKGROUND));
    }
    refresh();
    usleep(80000);
    for (int i = 0; i < BIG_GLYPH_HEIGHT; i++){
      attron(COLOR_PAIR(BACKGROUND));
      mvprintw(ROW/2 - 9 + i - offset, (COL-GLYPH_LENGTH)/2 - 0, archsarazap[i]);
      attroff(COLOR_PAIR(BACKGROUND));
    }
    refresh();
    usleep(80000);
    quickprint(FOREGROUND, BACKGROUND, 0);
  } else {
    neon();
  }
}

char * prompt_fuzzy(){

  int offset = 0;
  if (WIN_SIZE != BIG) offset = 1;

  attron(COLOR_PAIR(FOREGROUND));
  mvprintw(ROW/2 - 2 - offset, (COL-GLYPH_LENGTH)/2, option_window[0]);
//mvprintw(ROW/2 + 5 - offset, (COL-GLYPH_LENGTH)/2, option_window[6]);
  attroff(COLOR_PAIR(FOREGROUND));

  char * wall_dir = "/home/hakirot/pix/walls";
  struct file_node * list = init_dir_list(wall_dir);

  int rng_row, rng_shift, rng_backdrop = 0;
  char input_char;
  char selection[256] = {'\0'};
  int i = 0;
  while(1){

    input_char = getch();
    if (input_char == 27) break;

    rng_row   = rand() % 5;             // 0-5, fuzzy glyph
    rng_shift = (rand() % 3) - 1;       // -1 and 1

    attron(COLOR_PAIR(FOREGROUND));
    mvprintw(ROW/2 - 1 + rng_row, (COL - GLYPH_LENGTH)/2 - rng_shift + 1, "%s", fuzzy[rng_row]);
    attroff(COLOR_PAIR(FOREGROUND));
    attron(COLOR_PAIR(BLACK));
    mvaddwstr(ROW/2 - 1 + rng_row, (COL/2) + 21, L"║");
    mvaddwstr(ROW/2 - 1 + rng_row, (COL/2) - 22, L"║");
    attroff(COLOR_PAIR(BLACK));

    getmaxyx(stdscr, ROW, COL);
    if(CACHE != ROW + COL){
      break;
    }

    refresh();
    usleep(GLITCH_FRAME_TIME);
  }

  free_dir_list(list);
  return NULL;
}

void fork_newlook(char * file){
  pid_t pid = fork();

  if (pid < 0) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    if(file == NULL){
      execl("/usr/bin/bash", "bash", "/home/hakirot/.local/bin/newlook", (char *)NULL);
    } else {
      char * wall_dir = "/home/hakirot/pix/walls";
      char file_path[256];
      sprintf(file_path, "%s%s", wall_dir, file);
      execl("/usr/bin/bash", "bash", "/home/hakirot/.local/bin/newlook", file_path, (char *)NULL);
    }
    perror("execl");
  } else {
    clear();
    refresh();
    int status;
    waitpid(pid, &status, 0);
  } // Not doing a proper wait
}

void pshd(){

  CACHE = ROW + COL;
  FILE *file = fopen("/home/hakirot/.config/pshd/dir", "r");

  if (file == NULL){
    error("ERROR: error opening pshd");
  }

  char line[256] = {'\0'};
  int i = 0;
  attron(COLOR_PAIR(FOREGROUND));
  while(fgets(line, sizeof(line), file)){
    line[strcspn(line, "\n")] = 0;
    int len = strlen(line);
    if(len < 42){
      if (i < 10) {
        memset(line + len, ' ', 38 - len);
      } else {
        memset(line + len, ' ', 37 -  len);
        line[37] = '\0';
      }
    }
    if(i == 0){
      mvprintw(i + 1, 2, "%s", option_window[0]);
      mvprintw(i + 2, 3, "[%d] %s", i, line);
    } else if (i > 0){
      mvprintw(i + 2, 3, "[%d] %s", i, line);
    }
    refresh();
    i++;
    usleep(10000);
  }
  mvprintw(i + 2, 2, "%s", option_window[6]);
  attroff(COLOR_PAIR(FOREGROUND));
  refresh();

  char entry[16] = {'\0'};
  char input;
  int j = 0;
  int entry_as_int = -1;
  int prev_sel = -1;
  char prev_line[256] = {'\0'};

  while(1){

    getmaxyx(stdscr, ROW, COL);
    if (CACHE != ROW + COL){
      CACHE = check_size();
      break;
    }

    input = getch();
    if (input > 47 && input < 58 || input == 'j' || input == 'k'){
      if(input == 'j'){
        entry_as_int++;
        if (entry_as_int == i){
          entry_as_int = i - 1;
        } else {
          prev_sel = entry_as_int - 1;
        }
        sprintf(entry, "%d", entry_as_int);
      } else if (input == 'k'){
        entry_as_int--;
        if (entry_as_int == -1){
          entry_as_int = 0;
        } else {
          prev_sel = entry_as_int + 1;
        }
        sprintf(entry, "%d", entry_as_int);
      } else {
        entry[j] = input;
        j++;
      }
      sscanf(entry, "%d", &entry_as_int);
      if(entry_as_int >= i){
        attron(COLOR_PAIR(FOREGROUND));
        mvprintw(prev_sel + 2, 3, "[%d] %s", prev_sel, prev_line);
        refresh();
        attroff(COLOR_PAIR(FOREGROUND));
        j = 0;
        memset(entry, '\0', 16);
        memset(prev_line, '\0', 256);
        entry_as_int = -1;
        prev_sel = -1;
      }
      rewind(file);
      int k = 0;
      if(entry_as_int > -1){

        if(prev_sel > -1){
          attron(COLOR_PAIR(FOREGROUND));
          mvprintw(prev_sel + 2, 3, "[%d] %s", prev_sel, prev_line);
          refresh();
          attroff(COLOR_PAIR(FOREGROUND));
          prev_sel = -1;
        }

        while(fgets(line, sizeof(line), file)){
          line[strcspn(line, "\n")] = 0;
          if(k == entry_as_int){

            prev_sel = entry_as_int;
            strncpy(prev_line, line, 256);

            attron(COLOR_PAIR(FOREGROUND + 8));
            mvprintw(k + 2, 3, "[%d] %s", k, line);
            attroff(COLOR_PAIR(FOREGROUND + 8));
            refresh();
          }
          k++;
        }
      }
    } else if (input == 7){ // this could be an ST implementation only..
      j = 0;
      memset(entry, '\0', 16);
      mvprintw(1, 2, "%s", option_window[0]);
      if(prev_sel > -1){
        attron(COLOR_PAIR(FOREGROUND));
        mvprintw(prev_sel + 2, 3, "[%d] %s", prev_sel, prev_line);
        refresh();
        attroff(COLOR_PAIR(FOREGROUND));
      }
      prev_sel = -1;
      refresh();

    } else if (input == '\n'){

      fclose(file);

      if(strlen(prev_line) > 0){

        prev_line[strcspn(prev_line, "\n")] = 0;
        chdir(prev_line);
        if (setenv("PWD", prev_line, 1) != 0) {
          error("setenv error");
        }
        neon();
      } else {
        glitch(16, 1);

      }
      return;
    } else if (input == 'q'){
      fclose(file);
      glitch(16, 1);
      return;
    } else if (input == 'f' || input == '/') {

      attron(COLOR_PAIR(FOREGROUND));
      mvprintw(prev_sel + 2, 3, "[%d] %s", prev_sel, prev_line);
      refresh();
      attroff(COLOR_PAIR(FOREGROUND));

      char buffer[256] = {'\0'};
      attron(COLOR_PAIR(FOREGROUND +  16));
      mvprintw(1, 2, "%s", " FIND:               ");
      attroff(COLOR_PAIR(FOREGROUND + 16));
      refresh();
      int l = 0;
      char * padding = "                                            ";
      char sel_line[256] = {'\0'};
      while(1){
        input = getch();
        if (input == 27){

          rewind(file);
          mvprintw(1, 2, "%s", option_window[0]);

          // duplicated code
          attron(COLOR_PAIR(BACKGROUND));
          for(int n = 0; n < i; n++){
            mvprintw(2 + n, 2, "%s", padding);
          }
          attroff(COLOR_PAIR(BACKGROUND));

          // duplicated code
          i = 0;
          attron(COLOR_PAIR(FOREGROUND));
          while(fgets(line, sizeof(line), file)){
            line[strcspn(line, "\n")] = 0;
            int len = strlen(line);
            if(len < 42){
              if (i < 10) {
                memset(line + len, ' ', 38 - len);
              } else {
                memset(line + len, ' ', 37 -  len);
                line[37] = '\0';
              }
            }
            if(i == 0){
              mvprintw(i + 1, 2, "%s", option_window[0]);
              mvprintw(i + 2, 3, "[%d] %s", i, line);
            } else if (i > 0){
              mvprintw(i + 2, 3, "[%d] %s", i, line);
            }
            refresh();
            i++;
          }
          mvprintw(i + 2, 2, "%s", option_window[6]);
          attroff(COLOR_PAIR(FOREGROUND));
          refresh();

          break;
        } else if (input > 31 && input < 127) {
          memset(sel_line, '\0', 256);

          buffer[l] = input;

          attron(COLOR_PAIR(FOREGROUND + 8));
          mvaddch(1, 9 + l, buffer[l]);
          attroff(COLOR_PAIR(FOREGROUND + 8));
          l++;

          attron(COLOR_PAIR(BACKGROUND));
          for(int n = 0; n < i; n++){
            mvprintw(2 + n, 2, "%s", padding);
          }
          attroff(COLOR_PAIR(BACKGROUND));
          refresh();

          int m = 0;
          int n = 0;
          rewind(file);
          while(fgets(line, sizeof(line), file)){
            line[strcspn(line, "\n")] = 0;
            if(strstr(line, buffer)){

              if (m == 0){
                strncpy(sel_line, line, 256);
                attron(COLOR_PAIR(FOREGROUND + 8));
                mvprintw(2 + m, 3, "[%d] %s", n, line);
                attroff(COLOR_PAIR(FOREGROUND + 8));
              } else {
                attron(COLOR_PAIR(FOREGROUND));
                mvprintw(2 + m, 3, "[%d] %s", n, line);
                attroff(COLOR_PAIR(FOREGROUND));
              }
              m++;
            }
            n++;
          }
          refresh();
        } else if (input == 7){ // this could be an ST implementation only..
          l--;
          buffer[l] = 0;
          if (l < 0) l = 0;
          attron(COLOR_PAIR(WHITE_BLACK));
          mvaddch(1, 9 + l, ' ');
          attroff(COLOR_PAIR(WHITE_BLACK));

          // copied from above
          attron(COLOR_PAIR(BACKGROUND));
          for(int n = 0; n < i; n++){
            mvprintw(2 + n, 2, "%s", padding);
          }
          attroff(COLOR_PAIR(BACKGROUND));

          rewind(file);
          int m = 0;
          int n = 0;
          while(fgets(line, sizeof(line), file)){
            line[strcspn(line, "\n")] = 0;
            if(strstr(line, buffer)){

              if (m == 0){
                strncpy(sel_line, line, 256);
                attron(COLOR_PAIR(FOREGROUND + 8));
                mvprintw(2 + m, 3, "[%d] %s", n, line);
                attroff(COLOR_PAIR(FOREGROUND + 8));
              } else {
                attron(COLOR_PAIR(FOREGROUND));
                mvprintw(2 + m, 3, "[%d] %s", n, line);
                attroff(COLOR_PAIR(FOREGROUND));
              }
              m++;
            }
            n++;
            usleep(10000);
          }

          refresh();
        } else if (input == '\n'){

          fclose(file);
          if(strlen(sel_line) > 0){
            chdir(sel_line);
            if (setenv("PWD", sel_line, 1) != 0) {
              error("setenv error");
            }
            neon();
          } else {
            glitch(46, 0);
          }
          return;
        }
        usleep(50000);
      }
    }
    usleep(50000);
  }
}

void ensure_path_perm(char * file_path, char perm){

  if(perm == 'w'){
    // open file
    FILE *writeptr = fopen(file_path, "w");

    // is it writable?
    if(writeptr){
      fclose(writeptr);
      return;
    } else if (errno == 13){
      // test if pw file exists
      char * pw_path = "/home/hakirot/.config/pw.gpg";
      FILE * pw_file = fopen(pw_path, "r");
      if (ENOENT == errno){
        int result = generate_pw_file();
      } else {
        fclose(pw_file);
      }

      // fork
      pid_t pid = fork();
      if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
      } else if (pid == 0) {
        endwin();

        // setenv SUDO_ASKPASS
				if (setenv("SUDO_ASKPASS", "/home/hakirot/skps/secret.sh", 1) != 0) {
          error("setenv error");
				}

        execlp("sudo", "sudo", "--askpass", "chmod", "666", file_path, NULL);
        error("ERROR: execv sudo");

      } else {
        endwin();
        int status;

        while(kill(pid, 0) == 0){
          waitpid(pid, &status, 0);
        }
      }

    } else {
      char error_str[32] = {'\0'};
      sprintf(error_str, "A new errno: %d", errno);
      error(error_str);
    }
  }
}

void ensure_cache_dir(){
  const char *env_home = getenv("HOME");
  const char *sara_wd = "/.cache/sara";
  char cache_dir[256] = {'\0'};
  sprintf(cache_dir, "%s%s", env_home, sara_wd);

  // Check if dir exists
  DIR* dir = opendir(cache_dir);
  if (dir) {
    closedir(dir);
  } else if (ENOENT == errno) {
    char dir_name[256] = {'\0'};
    sprintf(dir_name, "%s%s", env_home, "/.cache/sara");
    int status = mkdir(dir_name, 0755);

    if (status != 0) {
      char error_str[256] = {'\0'};
      sprintf(error_str, "%s%s" , "Error creating directory ", dir_name);
      error(error_str);
    }
  }
}

struct file_node * init_dir_list(char * dir){
  return NULL;
}

void free_dir_list(struct file_node * list){

  if (list == NULL){
    return;
  }

  struct file_node * next;
  while(list != NULL){
    next = list->next;
    free(list);
    list = next;
  }
}

int generate_pw_file(){
  clear();

  attron(COLOR_PAIR(WHITE_BLACK));
  mvprintw(ROW/2 - 1, COL/2 - 8, "%s", "Enter Password");
  attroff(COLOR_PAIR(WHITE_BLACK));
  refresh();

  char pw[256] = {'\0'};
  int i = 0;
  char input;
  while(1){
    input = getch();
      if (input != ERR && input != '\n' && input != EOF && input > 19 && input < 127) {

        mvaddch(ROW/2, COL/2 - 8 + i, '*');
        refresh();

        pw[i] = input;
        i++;

      } else if (input == 7){ // this could be an ST implementation only..

        pw[i] = '\0';
        i--;
        if (i < 0) i = 0;
        mvaddch(ROW/2, COL/2 - 8 + i, ' ');

      } else if (input == '\n'){
        break;
      }
      if (i == 256) return 1;
    usleep(50000); // chill
  }
  pw[i] = '\n';

  char popen_command[256] = {'\0'};
  sprintf(popen_command, "%s %s", "gpg --symmetric --output", "/home/hakirot/.config/pw.gpg");
  FILE * gpg = popen(popen_command, "w");
  if (!gpg) error("gpg");
  fputs(pw, gpg);
  int result = pclose(gpg);

  return 0;
}

void get_helped() {
  printf("Usage: %s [OPTIONS]\n", "sara");
  printf("  --help, -h    Get helped\n");
  printf("  -c            Constant effects\n");
  printf("  -G            Constant glitch effect\n");
  printf("  -M            Constant MEGA glitch effect\n");
  printf("  -H            Holographic background\n");
  printf("  -f [color]    set custom FOREGROUND color\n");
  printf("  -b [color]    set custom BACKGROUND color\n");
  printf("  -F [follow]   On exit, Writes PWD to file\n");
  exit(0);
}

void init_window(){
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

  init_pair(BLACK_WHITED, COLOR_WHITE, COLOR_BLACK);
  init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK);
  init_pair(GREEN_BLACK, COLOR_GREEN, COLOR_BLACK);
  init_pair(YELLOW_BLACK, COLOR_YELLOW, COLOR_BLACK);
  init_pair(BLUE_BLACK, COLOR_BLUE, COLOR_BLACK);
  init_pair(MAGENTA_BLACK, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(CYAN_BLACK, COLOR_CYAN, COLOR_BLACK);
  init_pair(WHITE_BLACKD, COLOR_WHITE, COLOR_BLACK);

  cbreak();                 // Disable line buffering
  noecho();                 // Don't display keyboard presses in window
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);     // Enable reading of F1/2, arrow keys, etc
  curs_set(FALSE);          // No cursor
  clear();
  refresh();                // clear screen
}

void error(char * err) {
  endwin();
  printf("%s\n", err);
  exit(1);
}

