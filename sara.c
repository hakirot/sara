/*
--  ███████╗ █████╗ ██████╗  █████╗   --
--  ██╔════╝██╔══██╗██╔══██╗██╔══██╗  --
--  ███████╗███████║██████╔╝███████║  --
--  ╚════██║██╔══██║██╔══██╗██╔══██║  --
--  ███████║██║  ██║██║  ██║██║  ██║  --
--  ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝.c--
                                           */

#define NCURSES_WIDECHAR 1
#define _key(...) __key((key_arg){__VA_ARGS__});

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <wait.h>
#include "sara.h"
#include "utils.h"
#include "config.h"
#include "globals.h"
#include "animations.h"

int main(int argc, char* argv[]){

  srand((unsigned)time(0));


  special_chars();
  load_command_config();
  set_glyph_dimensions();
  _init_state();
  reset_glyph_state();
  _args(argc, argv);
  if(_state->preflight_check) _preflight_check();
  launch_window();
  _deflect_signals();

  _state->t_cache = check_size();
  int signal_reprint = true;
  int result = 0;

  while(1){

    result = _key();

    if(_state->interrupt){
      _state->interrupt = false;
      signal_reprint = true;
    }

    getmaxyx(stdscr, _state->t_rows, _state->t_cols);
    _state->t_cache = check_size();

    if(strlen(SLAP_STR) > 0 && _slap_timer() == 1 ) {
      signal_reprint = true;
    }

    if(HOLOGRAPHIC == 1 && _state->win_size == t_full){
      _state->bg_c++;
      _state->fg_c++;
      _state->hd_c++;
      if (_state->bg_c > 7) _state->bg_c = 2;
      if (_state->fg_c > 7) _state->fg_c = 2;
      if (_state->hd_c > 7) _state->hd_c = 2;
      signal_reprint = true;
    }

    if(_state->fx != nofx) _affect(_state->fx);

    // print only once after the HOLD_CHAR flips back to EOF and HOLD_CHAR_TIME is exceeded
    if (result == 0 && signal_reprint == true && HOLD_CHAR == '\0'){
      animate(none);
      signal_reprint = false;
    } else if (result >= 1){
      signal_reprint = true;
    }
    usleep(40000); // chill
  }

  refresh();
  endwin();

  return 0;
}

// Check screen size, return updated sum of dimensions
int check_size(){

  if (_state->t_cache != _state->t_rows + _state->t_cols){

    clear();

    // small win size jail
    while (get_win_size() == t_tiny){
      _state->win_size = t_tiny;
      clear();
      tinyprint();
      refresh();

      // 2 milliseconds
      usleep(2000);
      _key();

      getmaxyx(stdscr,_state->t_rows,_state->t_cols);
      _state->t_cache = _state->t_rows + _state->t_cols;
    }

    if ((get_win_size() == t_full) || dynamic_resize == false){
      _state->win_size = t_full;
    } else {
      _state->win_size = t_small;
    }
    animate(start_animation);
  }

  return _state->t_rows + _state->t_cols;
}

screen_size get_win_size(){
    if (_state->t_rows > resize_y && _state->t_cols > resize_x){
      return t_full;
    } else if (_state->t_cols < tiny_mode_x || _state->t_rows < tiny_mode_y){
      return t_tiny;
    } else {
      return t_small;
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
      char err[128];
      sprintf(err, "%s %s", "Bad Arg:", arg);
      get_helped(1);
  }
  return -1;
}


// TODO quickedit pshd
void _pshd(){

  _state->t_cache = _state->t_rows + _state->t_cols;
  ensure_config_dir();

  const char *env_home = getenv("HOME");
  const char *sara_wd = "/.config/sara/pshd";
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

  if(_state->t_rows >= count+2 && pshd_y-2 > count){
    dim_y = count+2;
  } else if(_state->t_rows < pshd_y) {
    dim_y = _state->t_rows;
  } else if (count <= pshd_y - 2) {
    dim_y = count + 2;
  } else {
    dim_y = pshd_y;
  }

  if(_state->t_cols < pshd_x){
    dim_x = _state->t_cols;
  } else {
    dim_x = pshd_x;
  }

  int offset_y = _deduce_offset_y(dim_y, pshd_offset_y);
  int offset_x = _deduce_offset_x(dim_x, pshd_offset_x);

  _print_menu_borders(dim_y, dim_x, offset_y, offset_x, pshd_c);
  refresh();

  int home_len = strlen(env_home);
  int line_offset = 4;
  int input;
  char input_digits[16] = {'\0'};
  int input_digits_idx = 0;
  int selection = 0;
  int reprint = true;

  if (dim_y < 3 || _state->t_rows < 3) {
    dim_y = 3;
    offset_y = 0;
  }

  while(1){

    getmaxyx(stdscr, _state->t_rows, _state->t_cols);
    if (_state->t_cache != _state->t_rows + _state->t_cols){
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

      int result = chdir(line);

      if(result != 0) {
        slap(strerror(errno));
      } else {
        setenv("PWD", line, 1);
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

      if(pshd_c_bold) {
        attron(COLOR_PAIR(pshd_c));
        attron(A_STANDOUT);
        attron(A_BOLD);
      } else {
        attron(COLOR_PAIR(pshd_c));
        attron(A_STANDOUT);
      }
      mvprintw(_state->t_rows/2 - dim_y/2 + offset_y, _state->t_cols/2 - dim_x/2 + offset_x, "FILTER");
      attroff(A_BOLD);
      attroff(A_STANDOUT);

      mvaddch(_state->t_rows/2 - dim_y/2 + offset_y, _state->t_cols/2 - dim_x/2 + offset_x + 6, ' ');

      refresh();
      char search_buffer[256] = {'\0'};
      int char_idx = 0;
      int chdir_at_seletion = false;

      while(1){
        //attron(pshd_c);
        input = getch();

        getmaxyx(stdscr, _state->t_rows, _state->t_cols);
        if (_state->t_cache != _state->t_rows + _state->t_cols){
          fclose(file);
          return;
        }

        if (input > 31 && input < 127){
          _clear_menu(dim_y, dim_x, offset_y, offset_x);
          search_buffer[char_idx] = (char)input;
          char_idx++;

          attron(COLOR_PAIR(pshd_c));
          if(pshd_c_bold) attron(A_BOLD);
          mvaddch(_state->t_rows/2 - dim_y/2 + offset_y, _state->t_cols/2 - dim_x/2 +  6 + char_idx + offset_x, (char)input);
          mvaddch(_state->t_rows/2 - dim_y/2 + offset_y, _state->t_cols/2 - dim_x/2 +  6 + char_idx + offset_x + 1, ' ');
          attroff(COLOR_PAIR(pshd_c));
          attroff(A_BOLD);

          refresh();
          reprint = true;

        } else if (input == '\n'){
          reprint = true;
          chdir_at_seletion = true;
        } else if (input == 27){
          _print_menu_borders(dim_y, dim_x, offset_y, offset_x, pshd_c);
          attron(COLOR_PAIR(pshd_c));
          if(pshd_c_bold) attron(A_BOLD);
          reprint = true;
          break;
        } else if (input > 0) {
          // Backspaces
          if(char_idx == 0){
            _print_menu_borders(dim_y, dim_x, offset_y, offset_x, pshd_c);
            reprint = true;
            break;
          }
          _print_menu_borders(dim_y, dim_x, offset_y, offset_x, pshd_c);
          if(pshd_c_bold) {
            attron(A_STANDOUT);
            attron(COLOR_PAIR(pshd_c));
            attron(A_BOLD);
          } else {
            attron(COLOR_PAIR(pshd_c));
            attron(A_STANDOUT);
          }
          mvprintw(_state->t_rows/2 - dim_y/2 + offset_y, _state->t_cols/2 - dim_x/2 + offset_x, "FILTER");
          attroff(A_BOLD);
          attroff(A_STANDOUT);

          attron(COLOR_PAIR(pshd_c));
          mvaddch(_state->t_rows/2 - dim_y/2 + offset_y, _state->t_cols/2 - dim_x/2 + offset_x + 6 + char_idx, ' ');
          mvaddch(_state->t_rows/2 - dim_y/2 + offset_y, _state->t_cols/2 - dim_x/2 + offset_x + 6, ' ');
          attroff(COLOR_PAIR(pshd_c));

          char_idx--;
          search_buffer[char_idx] = '\0';
          attron(COLOR_PAIR(pshd_c));
          if(pshd_c_bold) {
            attron(A_BOLD);
          } 
          mvprintw(_state->t_rows/2 - dim_y/2 + offset_y, _state->t_cols/2 - dim_x/2 + 7 + offset_x, "%s", search_buffer);
          attroff(A_BOLD);
          attroff(A_STANDOUT);
          attroff(COLOR_PAIR(pshd_c));
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

              attron(COLOR_PAIR(pshd_c));
              if(pshd_c_bold) attron(A_BOLD);
              mvprintw(_state->t_rows/2 - dim_y/2 + i + 1 + offset_y, _state->t_cols/2 - dim_x/2 + 2 + offset_x, "%d", k);
              attroff(COLOR_PAIR(pshd_c));

              if(i == 0) {
                selection = k;
                if(chdir_at_seletion){

                  int result = chdir(line);

                  if(result != 0) {
                    slap(strerror(errno));
                  } else {
                    setenv("PWD", line, 1);
                  }

                  if (setenv("PWD", line, 1) != 0) {
                    crit("setenv error");
                  }
                  fclose(file);
                  animate(neon);
                  return;
                }
                if(pshd_c_bold) {
                  attron(COLOR_PAIR(pshd_c));
                  attron(A_STANDOUT);
                  attron(A_BOLD);
                } else {
                  attron(COLOR_PAIR(pshd_c));
                  attron(A_STANDOUT);
                }
              } else {
                attron(COLOR_PAIR(pshd_c));
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
                mvaddch(_state->t_rows/2 - dim_y/2 + i + 1 + offset_y, _state->t_cols/2 - dim_x/2 + j + 1 + line_offset + offset_x, line[j]);
                if((j + 8) > dim_x) break;
              }
              attroff(A_BOLD);
              attroff(A_STANDOUT);
              attroff(COLOR_PAIR(pshd_c));

              i++;
            }
            k++;
          }

          reprint = false;
          rewind(file);

        }

        usleep(1000);
      }
    } else if (input != -1){
      int result = _key(input);
      if(result > 0) {
        fclose(file);
        return;
      }
    }

    if(reprint){
      _clear_menu(dim_y, dim_x, offset_y, offset_x);
      _reprint_pshd(dim_y, dim_x, offset_y, offset_x, selection, file, count);
      reprint = false;
    }
    usleep(2000);
  }

  refresh();
  return;

}

void _reprint_pshd(int dim_y, int dim_x, int offset_y, int offset_x, int selection, FILE* file, int count){
  int i = 0;
  int k = -1;
  const char *env_home = getenv("HOME");
  int home_len = strlen(env_home);
  char line[256] = {'\0'};
  int line_offset = 4;
  if(pshd_c_bold) attron(A_BOLD);
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

    attron(COLOR_PAIR(pshd_c));
    if(pshd_c_bold) attron(A_BOLD);

    int len = strlen(line);
    mvprintw(_state->t_rows/2 - dim_y/2 + i + 1 + offset_y, _state->t_cols/2 - dim_x/2 + 2 + offset_x, "%d", k);

    if(k == selection) {
      attron(COLOR_PAIR(pshd_c));
      if(dim_y > 3) {
        attron(A_STANDOUT);
      }
    }

    for(int j = 0; j < len; j++){
      mvaddch(_state->t_rows/2 - dim_y/2 + i + 1 + offset_y, _state->t_cols/2 - dim_x/2 + j + 1 + line_offset + offset_x, line[j]);
      if((j + 8) > dim_x) break;
    }

    if(selection == k) {
      attroff(COLOR_PAIR(pshd_c));
      attroff(A_STANDOUT);
      attroff(A_BOLD);
    }

    i++;
    refresh(); //debug
  }
  if(pshd_c_bold) attron(A_BOLD);
  rewind(file);
}
