/*
--  ███████╗ █████╗ ██████╗  █████╗   --
--  ██╔════╝██╔══██╗██╔══██╗██╔══██╗  --
--  ███████╗███████║██████╔╝███████║  --
--  ╚════██║██╔══██║██╔══██╗██╔══██║  --
--  ███████║██║  ██║██║  ██║██║  ██║  --
--  ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝.c--
                                           */
#define NCURSES_WIDECHAR 1

#include "sara.h"
#include "animations.h"
#include "globals.h"
#include "config.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <fcntl.h>

int main(int argc, char* argv[]){

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
      get_helped();
    }
  }

  // Override arg fg and bg colors
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--foreground") == 0){
      if(i + 1 < argc){
        FOREGROUND = input_color(argv[i + 1]);
        if (FOREGROUND == -1){
          char errstr[256] = {'\0'};
          sprintf(errstr, "Bad arg: '%s'", argv[i + 1]);
          crit(errstr);
        }
      } else {
        get_helped();
        crit("bad arg\n");
      }
    } else if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--background") == 0){
      if(i + 1 < argc){
        BACKGROUND = input_color(argv[i + 1]);
        if (BACKGROUND == -1){
          char errstr[256] = {'\0'};
          sprintf(errstr, "Bad arg: '%s'", argv[i + 1]);
          crit(errstr);
        }
      } else {
        get_helped();
        crit("bad arg\n");
      }
    }
  }

  // double WAIT_BUFFER = 0.10000;
  srand((unsigned)time(0));

  int opt;
  while ((opt = getopt(argc, argv, "cMFfbhrH")) != -1){
    switch (opt) {
      // case 'c': WAIT_BUFFER = 0.00005; break;
      case 'r':
        BACKGROUND = (rand() % 7) + 1;    // RNG 1 and 7
        FOREGROUND = (rand() % 7) + 2;    // RNG 2 and 8
        while(FOREGROUND == BACKGROUND){
          FOREGROUND = rand() % 7 + 2;
        }
        if (FOREGROUND > 8 || FOREGROUND < 2 || BACKGROUND > 7 || BACKGROUND < 1){
          crit("rand error");
        } else if (FOREGROUND == BACKGROUND) {
          crit("F == B");
        }
        break;
      case 'H': HOLOGRAPHIC = 1; break;
      case 'F': FOLLOW = 1; break;
    }
  }

  special_chars();
  load_command_config();
  set_glyph_dimensions(); // TODO: get away from "*glyph*"
  launch_window();
  if(run_preflight_check) preflight_check();

  CACHE = check_size();

  LAST_INPUT_TIME = clock();
  int should_print = true;
  while(1){

    int result = __key__();

    getmaxyx(stdscr, ROW, COL);
    CACHE = check_size();

    // _blink();
    if(HOLOGRAPHIC == 1 && WIN_SIZE == BIG){
      BACKGROUND++;
      if (BACKGROUND > 7) BACKGROUND = 2;
      animate(none);
    }

    // print only once after the HOLD_CHAR flips back to EOF and HOLD_CHAR_TIME is exceeded
    if (result == 0 && should_print == true && HOLD_CHAR == '\0'){
      animate(none);
      should_print = false;
    } else if (result >= 1){
      should_print = true;
    }
    usleep(20000); // chill
  }

  refresh();
  endwin();

  return 0;
}

// Check screen size, return updated sum of dimensions
int check_size(){

  if (CACHE != ROW + COL){

    clear();

    // small win size jail
    while (get_win_size() == SMALL){
      WIN_SIZE = SMALL;
      clear();
      tinyprint();
      refresh();

      // 2 milliseconds
      usleep(2000);
      __key__();

      getmaxyx(stdscr,ROW,COL);
      CACHE = ROW + COL;
    }

    if ((get_win_size() == BIG) || dynamic_resize == false){
      WIN_SIZE = BIG;
    } else {
      WIN_SIZE = NORMAL;
    }
    animate(start_animation);
  }

  return ROW + COL;
}

screen_size get_win_size(){
    if (ROW > resize_y && COL > resize_x){
      return BIG;
    } else if (COL < tiny_mode_x || ROW < tiny_mode_y){
      return SMALL;
    } else {
      return NORMAL;
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


// TODO: implement bold fg color if use_bold_color_for_fg is set
// TODO: fix filter color bug
void _pshd(){

  CACHE = ROW + COL;
  ensure_config_dir();

  const char *env_home = getenv("HOME");
  const char *sara_wd = "/.config/sara/dir";
  char pshd_file[256] = {'\0'};
  sprintf(pshd_file, "%s%s", env_home, sara_wd);


  FILE *file = fopen(pshd_file, "r");

  // TODO: Make this a usererror and return if it fails
  if (file == NULL){
    return;
  }

  int count = 0;
  char line[256] = {'\0'};
  while(fgets(line, sizeof(line), file)){
    count++;
  }

  rewind(file);

  unsigned short dim_x = 0;
  unsigned short dim_y = 0;

  if(ROW >= count+2 && pshd_y-2 > count){
    dim_y = count+2;
  } else if(ROW < pshd_y) {
    dim_y = ROW;
  } else if (count <= pshd_y - 2) {
    dim_y = count + 2;
  } else {
    dim_y = pshd_y;
  }

  if(COL < pshd_x){
    dim_x = COL;
  } else {
    dim_x = pshd_x;
  }

  _print_pshd_borders(dim_y, dim_x);
  refresh();

  int home_len = strlen(env_home);
  int line_offset = 4;
  int input;
  char input_digits[16] = {'\0'};
  int input_digits_idx = 0;
  int selection = 0;
  int reprint = true;
//attron(COLOR_PAIR(FOREGROUND));

  while(1){

    getmaxyx(stdscr, ROW, COL);
    if (CACHE != ROW + COL){
      fclose(file);
      return;
    }

    input = getch();
    if (input != ERR && input != '\n' && input != EOF && input > 105 && input < 108) {
      reprint = true;
      if (input == 'j'){
        selection = (selection + 1) % count;
      } else {
        selection = (selection + (count - 1)) % count;
      }
    } else if (input == 'q' || input == 27){
      fclose(file);
      animate(none);
      return;
    } else if (input == 4){
      // ctrl+d
      selection += (dim_y-2)/2;
      if(selection >= count) selection = count-1;
      reprint = true;
    } else if (input == 6){
      selection += (dim_y-2);
      if(selection >= count) selection = count-1;
      reprint = true;
    } else if (input == 21){
      //ctrl+u
      selection -= (dim_y-2)/2;
      if(selection < 0) selection = 0;
      reprint = true;
    } else if (input == 2){
      selection -= (dim_y-2);
      if(selection < 0) selection = 0;
      reprint = true;
    } else if (input == 'G' || input == 'L'){
      selection = count-1;
      reprint = true;
    } else if (input == 'g' || input == 'H'){
      selection = 0;
      reprint = true;
    } else if (input == 'M'){
      selection = count/2;
      reprint = true;
    } else if (input == '\n'){
      int i = 0;
      while(fgets(line, sizeof(line), file)){
        line[strcspn(line, "\n")] = 0;
        if(i == selection) break;
        i++;
      }

      chdir(line);

      if (setenv("PWD", line, 1) != 0) {
        crit("setenv error");
      }
      fclose(file);
      animate(blink);
      return;
    } else if (input > 47 && input < 58){

        input_digits[input_digits_idx] = input;
        input_digits[input_digits_idx + 1] = 0;
        input_digits_idx++;
        selection = atoi(input_digits);

        if(selection > count){
          selection = 0;
          memset(&input_digits, 0, 16);
          input_digits_idx = 0;
        }

        reprint = true;
    } else if (input == '/' || input == 'f') {
      // attron(FOREGROUND);
      mvprintw(ROW/2 - dim_y/2, COL/2 - dim_x/2, "FILTER ");
      // attroff(FOREGROUND);
      refresh();
      char search_buffer[256] = {'\0'};
      int char_idx = 0;
      int chdir_at_seletion = false;

      while(1){
        //attron(FOREGROUND);
        input = getch();

        getmaxyx(stdscr, ROW, COL);
        if (CACHE != ROW + COL){
          fclose(file);
          return;
        }

        if (input > 31 && input < 127){
          _clear_pshd_window(dim_y, dim_x);
          search_buffer[char_idx] = (char)input;
          char_idx++;
          // attron(FOREGROUND);
          attron(FOREGROUND + 8);
          mvaddch(ROW/2 - dim_y/2, COL/2 - dim_x/2 +  6 + char_idx, (char)input);
          attroff(FOREGROUND + 8);
          // refresh();
          // getchar();
          // attroff(FOREGROUND);
          reprint = true;


        } else if (input == '\n'){
          reprint = true;
          chdir_at_seletion = true;
        } else if (input == 27){
          _print_pshd_borders(dim_y, dim_x);
          reprint = true;
          break;
        } else if (input > 0) {
          // Backspaces
          if(char_idx == 0){
            _print_pshd_borders(dim_y, dim_x);
            reprint = true;
            break;
          }
          _print_pshd_borders(dim_y, dim_x);
          attron(FOREGROUND);
          mvprintw(ROW/2 - dim_y/2, COL/2 - dim_x/2, "FILTER ");
          attroff(FOREGROUND);

          char_idx--;
          search_buffer[char_idx] = '\0';
          attron(FOREGROUND);
          mvprintw(ROW/2 - dim_y/2, COL/2 - dim_x/2 + 7, "%s", search_buffer);
          attroff(FOREGROUND);
          if(char_idx < 0) char_idx = 0;

          refresh();
          reprint = true;
        }

        if(reprint == true){
          int i = 0;
          int k = 0;
          while((fgets(line, sizeof(line), file) && (i < (dim_y -  2)))){

            line[strcspn(line, "\n")] = 0;
            if(strstr(line, search_buffer)){

              attron(COLOR_PAIR(FOREGROUND));
              mvprintw(ROW/2 - dim_y/2 + i + 1, COL/2 - dim_x/2 + 2, "%d", k);
              attroff(COLOR_PAIR(FOREGROUND));

              if(i == 0) {
                selection = k;
                if(chdir_at_seletion){

                  chdir(line);

                  if (setenv("PWD", line, 1) != 0) {
                    crit("setenv error");
                  }
                  fclose(file);
                  animate(neon);
                  return;
                }
                attron(COLOR_PAIR(FOREGROUND + 8));
              } else {
                attron(COLOR_PAIR(FOREGROUND));
              }

              if(strstr(line, env_home) != NULL){
                line[0] = '~';
                line[1] = '/';
                for(int j = 2; j < (int)strlen(line); j++){
                  line[j] = line[j + home_len - 1];
                }
              }

              int len = strlen(line);
              for(int j = 0; j < len; j++){
                mvaddch(ROW/2 - dim_y/2 + i + 1,COL/2 - dim_x/2 + j + 1 + line_offset, line[j]);
                if((j + 8) > dim_x) break;
              }
              attroff(COLOR_PAIR(FOREGROUND));
              attroff(COLOR_PAIR(FOREGROUND + 8));

              i++;
            }
            k++;
          }

          reprint = false;
          rewind(file);

        }

        usleep(2000);
      }
    }

    if(reprint){
      _clear_pshd_window(dim_y, dim_x);
      int i = 0;
      int k = -1;
      while((fgets(line, sizeof(line), file) && (i < (dim_y -  2)))){

        k++;
        if(k < selection && k < (count - (dim_y - 2))){
          continue;
        }

        if(strstr(line, env_home) != NULL){
          line[0] = '~';
          line[1] = '/';
          for(int j = 2; j < (int)strlen(line); j++){
            line[j] = line[j + home_len - 1];
          }
        }

        line[strcspn(line, "\n")] = 0;

        attron(COLOR_PAIR(FOREGROUND));

        int len = strlen(line);
        mvprintw(ROW/2 - dim_y/2 + i + 1, COL/2 - dim_x/2 + 2, "%d", k);

        if(k == selection) attron(COLOR_PAIR(FOREGROUND + 8));

        for(int j = 0; j < len; j++){
          mvaddch(ROW/2 - dim_y/2 + i + 1,COL/2 - dim_x/2 + j + 1 + line_offset, line[j]);
          if((j + 8) > dim_x) break;
        }
        if(selection == k) attroff(COLOR_PAIR(FOREGROUND));
        if(selection == k) attroff(COLOR_PAIR(FOREGROUND + 8));
        // attroff(COLOR_PAIR(FOREGROUND));
        // attroff(COLOR_PAIR(FOREGROUND + 8));

        i++;
        refresh(); //debug
      }
      rewind(file);
      reprint = false;
    }
    usleep(2000);
  }

  refresh();
  return;

}
