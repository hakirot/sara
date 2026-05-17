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
#include "glyphs.h"
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

//double WAIT_BUFFER = 0.10000;
  srand((unsigned)time(0));

  int opt;
  while ((opt = getopt(argc, argv, "cMFfbhrH")) != -1){
    switch (opt) {
//    case 'c': WAIT_BUFFER = 0.00005; break;
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
    usleep(50000); // chill
  }

  refresh();
  endwin();

  return 0;
}

// TODO: remove the second argument and infer length
// TODO: implement auto scroll for lists exceeding window size
// TODO: move this function (and others) elsewhere
// int len = sizeof(choices)/sizeof(choices[0]); // test this for ** pointer
const char * select_option_window(char* choices[], int len){

  int selection = 0;
  CACHE = ROW + COL;

  animate(none);

//int offset = 0;
//if (WIN_SIZE != BIG) offset = 1;

  attron(COLOR_PAIR(BACKGROUND));
  if(use_bold_color_for_bg) attron(A_BOLD);
  for (int i = 0; i < option_window_height; i++){
    mvprintw(ROW/2 + fg_offset_y + i - 3, (COL-option_window_length)/2, "%s", option_window[i]);
  }
  attroff(COLOR_PAIR(BACKGROUND));
  attroff(A_BOLD);

  while(1){

//  if (WIN_SIZE != BIG){
//    offset = 1;
//  } else {
//    offset = 0;
//  }

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
      mvprintw(ROW/2 + i - 2 + fg_offset_y, (COL-option_window_length)/2 + 1, "%s", choices[i]);
      attroff(COLOR_PAIR(FOREGROUND + 8));
      attroff(COLOR_PAIR(FOREGROUND));
    }

    refresh();
    usleep(1000);
  }

  return NULL;
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

void patch_backlight(){

  attron(COLOR_PAIR(FOREGROUND));
  int offset = 0;
  if (WIN_SIZE != BIG) offset = 1;
  mvaddstr(ROW/2 - offset - 1, (COL-option_window_length)/2 + 1, "BACKLIGHT");
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
  mvadd_wch(ROW/2 - offset - 1, (COL/2 + option_window_length/2) - 1, &cchar);
  attroff(COLOR_PAIR(BACKGROUND));
}

// TODO: Break this out into a separate program
void prompt_newlook(){

  clear();

  wchar_t search_str_doubles_lines[] = L"╔╗╚╝═║";
  wchar_t search_str_block[] = L"█";

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

      for (int i = 0; i < BG_GLYPH_HEIGHT; i++){
        selection == 1 ? attron(COLOR_PAIR(FOREGROUND)) : attron(COLOR_PAIR(BACKGROUND));
        animate(print_b);
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
          mvprintw(ROW/2 - 2 + i, (COL-44)/2, "%s", no_yes_window[i]);
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
              attron(COLOR_PAIR(black));
            // side borders when NO
            } else if ((iter_col == 0 || iter_col > 42) && selection == 0) {
              attron(COLOR_PAIR(black));
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
              attron(COLOR_PAIR(black));
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
              attron(COLOR_PAIR(black));
            // YES decorator blocks when selected
            } else if (iter_col > 20 && selection == 1){
              attron(COLOR_PAIR(FOREGROUND));
            }
          }

          mvadd_wch(ROW/2 - 2 + i, (COL-44)/2 + iter_col, &cchar);
          attroff(COLOR_PAIR(FOREGROUND)); // no foreground manipulation in function
          attroff(COLOR_PAIR(BACKGROUND));
          iter_row += len;
          iter_col++;
        }
      }

      refresh();

    } else if (input == 'k') {
      wall = theme_select();
      getmaxyx(stdscr, ROW, COL);
      if (CACHE != ROW + COL || wall == NULL) {
        exit_glitch_flag = 1;
        break;
      } else {
        fork_newlook(wall);
        clear();
        refresh();
        animate(neon);
      }

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
    CACHE = check_size();
    animate(glitch);
  } else {
    animate(neon);
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

char * prompt_fuzzy(){

  int offset = 0;
  if (WIN_SIZE != BIG) offset = 1;

  attron(COLOR_PAIR(FOREGROUND));
  mvprintw(ROW/2 - 2 - offset, (COL-option_window_length)/2, "%s", option_window[0]);
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
    mvprintw(ROW/2 - 1 + rng_row, (COL - option_window_length)/2 - rng_shift + 1, "%s", fuzzy[rng_row]);
    attroff(COLOR_PAIR(FOREGROUND));
    attron(COLOR_PAIR(black));
    mvaddwstr(ROW/2 - 1 + rng_row, (COL/2) + 21, L"║");
    mvaddwstr(ROW/2 - 1 + rng_row, (COL/2) - 22, L"║");
    attroff(COLOR_PAIR(black));

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

char * theme_select(){

// Payload
// {"action":"add","identifier":"preview","max_height":40,"max_width":40,"path":"/home/hakirot/pix/sara/sara_deck","x":0,"y":0}

  return NULL;
}

void fork_newlook(char * file){

  char path_to_respawn[256] = {'\0'};
  char * env_home = getenv("HOME");
  sprintf(path_to_respawn, "%s%s", env_home, "/git/sara/bash/respawn.sh");

  pid_t pid = fork();
  if (pid < 0) {
    crit("fork_newlook");
  } else if (pid == 0) {

    int fd = open("/dev/null", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    dup2(fd, 1);
    dup2(fd, 2);
    close(fd);

    if(file == NULL){
      execlp("nohup", "nohup", "bash", "-c", path_to_respawn, NULL);
    } else {
      char * wall_dir = "/home/hakirot/pix/walls";
      char file_path[256];
      sprintf(file_path, "%s%s", wall_dir, file);
      execlp("nohup", "nohup", "bash", "-c", path_to_respawn, file, NULL);
    }
    perror("execl");
  }
}

// TODO: fix the multiple out-of-bounds printing problem
// TODO: implement bold fg color if use_bold_color_for_fg is set
// TODO: make ESC also exit pshd
// TODO: backspace reprints top border white
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

  if(ROW <= pshd_y) {
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
  attron(COLOR_PAIR(FOREGROUND));

  while(1){

    getmaxyx(stdscr, ROW, COL);
    if (CACHE != ROW + COL) return;

    input = getch();
    if (input != ERR && input != '\n' && input != EOF && input > 105 && input < 108) {
      reprint = true;
      if (input == 'j'){
        selection = (selection + 1) % count;
      } else {
        selection = (selection + (count - 1)) % count;
      }
    } else if (input == 'q' || input == 27){
      animate(glitch);
      return;
    } else if (input == '\n'){
      animate(neon);
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
      return;
    } else if (input > 47 && input < 58){

        input_digits[input_digits_idx] = input;
        input_digits[input_digits_idx + 1] = 0;
        input_digits_idx++;
        selection = atoi(input_digits);

        if(selection > count){
          selection = 0;
          memset(&input_digits, 0, 16);
          input_digits_idx = 1;
        }

        reprint = true;
    } else if (input == '/' || input == 'f') {

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
        if(k == selection) attroff(COLOR_PAIR(FOREGROUND));
        if(k == selection) attroff(COLOR_PAIR(FOREGROUND + 8));

        i++;
        refresh(); //debug
      }
      rewind(file);
      reprint = false;
    }
  }

  refresh();
  return;

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

  attron(COLOR_PAIR(white_black));
  mvprintw(ROW/2 - 1, COL/2 - 8, "%s", "Enter Password");
  attroff(COLOR_PAIR(white_black));
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
  if (!gpg) crit("gpg");
  fputs(pw, gpg);
  int result = pclose(gpg);

  return 0;
}
