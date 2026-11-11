/*
--   █████╗ ███╗   ██╗██╗███╗   ███╗ █████╗ ████████╗██╗ ██████╗ ███╗   ██╗███████╗  --
--  ██╔══██╗████╗  ██║██║████╗ ████║██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║██╔════╝  --
--  ███████║██╔██╗ ██║██║██╔████╔██║███████║   ██║   ██║██║   ██║██╔██╗ ██║███████╗  --
--  ██╔══██║██║╚██╗██║██║██║╚██╔╝██║██╔══██║   ██║   ██║██║   ██║██║╚██╗██║╚════██║  --
--  ██║  ██║██║ ╚████║██║██║ ╚═╝ ██║██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║███████║  --
--  ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝     ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝.c--
                                                                                       */
#define NCURSES_WIDECHAR 1
#define _key(...) __key((key_arg){__VA_ARGS__});

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "animations.h"
#include "globals.h"
#include "config.h"
#include "utils.h"

void animate(animation option){

  if(_state->win_size == t_tiny){
    tinyprint();
    return;
  }

  if (option == none)                _none((Arg){0});
  if (option == down_wipes)          _down_wipes();
  if (option == glitch)              _glitch((Arg){.x = 0});
  if (option == glitch_full)         _glitch((Arg){.x = 1});
  if (option == neon)                _neon();
  if (option == neon_reverse)        _neon_reverse();
  if (option == shutter_slide)       _shutter_slide();
  if (option == shutter_slide_neon){ _shutter_slide(); _neon(); }
  if (option == pixel_fill)          _pixel_fill();
  if (option == tv_static)           _tv_static();
  if (option == print_f)             _fg(fg);
  if (option == print_b)             _bg();
  if (option == blink)               _blink();
}

void _blink(){

  double cycle_length = 0.3;
  double elapsed_time = 0;
  clock_t cycle_start = clock();

  int first_frame = 0;
  int second_frame = 0;
  int third_frame = 0;
  char ch;

  clear();
  refresh();

  while(cycle_length > elapsed_time){

    elapsed_time = (double)(clock() - cycle_start) / CLOCKS_PER_SEC;

    if(elapsed_time > 0.05 && first_frame == 0){
      ch = '.';
      if (_state->win_size == t_small){
        _overlay(fg, ch);
      }
      if (_state->win_size == t_full) {
        _overlay_bg(ch);
        _overlay(fg, ch);
      }
      first_frame = 1;
      refresh();
    }

    if(elapsed_time > 0.1 && second_frame == 0){
      ch = '_';
      if (_state->win_size == t_small){
        _overlay(fg, ch);
      } else {
        _overlay_bg(ch);
        _overlay(fg, ch);
      }
      second_frame = 1;
      refresh();
    }

    ch = '-';
    if(elapsed_time > 0.2 && third_frame == 0){
      if (_state->win_size == t_small){
        _overlay(fg, ch);
      } else {
        _overlay_bg(ch);
        _overlay(fg, ch);
      }
      third_frame = 1;
    }

    int result = _key();
    if(result == 2) return;
    if (_state->hold_char != '\0') mvprintw(_state->t_rows/2, _state->t_cols/2, "%c", _state->hold_char);
  }

  _none((Arg){.x = 0});
}

void _print_menu_borders(int dim_y, int dim_x, int offset_y, int offset_x, int c){

  if(dim_y < 3){
    //clear();
    return;
  }

  attron(COLOR_PAIR(c));
  if(c == menu_c && menu_c_bold) attron(A_BOLD);
  if(c == pshd_c && pshd_c_bold) attron(A_BOLD);
  if(c == run_c && run_c_bold) attron(A_BOLD);
  wchar_t wc[2] = { MenuBorder[0], L'\0' };
  cchar_t cchar;
  setcchar(&cchar, wc, 0, 0, NULL);
  mvadd_wch(_state->t_rows/2 - dim_y/2 + offset_y, _state->t_cols/2 - dim_x/2 + offset_x, &cchar);

  wc[0] = MenuBorder[1];
  setcchar(&cchar, wc, 0, 0, NULL);
  mvadd_wch(_state->t_rows/2 - dim_y/2 + offset_y, _state->t_cols/2 - dim_x/2 + dim_x - 1 + offset_x, &cchar);

  wc[0] = MenuBorder[2];
  setcchar(&cchar, wc, 0, 0, NULL);
  mvadd_wch(_state->t_rows/2 - dim_y/2 + dim_y - 1 + offset_y, _state->t_cols/2 - dim_x/2 + offset_x, &cchar);

  wc[0] = MenuBorder[3];
  setcchar(&cchar, wc, 0, 0, NULL);
  mvadd_wch(_state->t_rows/2 - dim_y/2 + dim_y - 1 + offset_y, _state->t_cols/2 - dim_x/2 + dim_x - 1 + offset_x, &cchar);

  wc[0] = MenuBorder[4];
  setcchar(&cchar, wc, 0, 0, NULL);
  for(int i = 1; i < dim_x - 1; i++){
    mvadd_wch(_state->t_rows/2 - dim_y/2 + offset_y, _state->t_cols/2 - dim_x/2 + i + offset_x, &cchar);
    mvadd_wch(_state->t_rows/2 - dim_y/2 + dim_y - 1 + offset_y, _state->t_cols/2 - dim_x/2 + i + offset_x, &cchar);
  }

  wc[0] = MenuBorder[5];
  setcchar(&cchar, wc, 0, 0, NULL);
  for(int i = 1; i < dim_y - 1; i++){
    mvadd_wch(_state->t_rows/2 - dim_y/2 + i + offset_y, _state->t_cols/2 - dim_x/2 + offset_x, &cchar);
    mvadd_wch(_state->t_rows/2 - dim_y/2 + i + offset_y, _state->t_cols/2 - dim_x/2 + dim_x - 1 + offset_x, &cchar);
  }
  attroff(A_BOLD);
  attroff(COLOR_PAIR(c));

  _clear_menu(dim_y, dim_x, offset_y, offset_x);
//refresh();
}

void _print_confirm_window(char * command_str){

  // clear();
  int r_stop = _state->t_cols - 2;
  int l_stop = 1;
  int width = _state->t_cols - 3;

  if(_state->t_cols > 20){
    r_stop = _state->t_cols/2 + 8;
    l_stop = _state->t_cols/2 - 8;;
    width = 16;
  }

  _clear_menu(3, width + 1, 0, 0);

  attron(COLOR_PAIR(menu_c));
  wchar_t wc[2] = { MenuBorder[0], L'\0' };
  cchar_t cchar;
  setcchar(&cchar, wc, 0, 0, NULL);
  mvadd_wch(_state->t_rows/2 - 1, l_stop, &cchar);

  wc[0] = MenuBorder[1];
  setcchar(&cchar, wc, 0, 0, NULL);
  mvadd_wch(_state->t_rows/2 - 1, r_stop, &cchar);

  wc[0] = MenuBorder[2];
  setcchar(&cchar, wc, 0, 0, NULL);
  mvadd_wch(_state->t_rows/2 + 1, l_stop, &cchar);

  wc[0] = MenuBorder[3];
  setcchar(&cchar, wc, 0, 0, NULL);
  mvadd_wch(_state->t_rows/2 + 1, r_stop, &cchar);

  wc[0] = MenuBorder[4];
  setcchar(&cchar, wc, 0, 0, NULL);
  for(int i = 1; i < width; i++){
    mvadd_wch(_state->t_rows/2 - 1, l_stop + i, &cchar);
    mvadd_wch(_state->t_rows/2 + 1, l_stop + i, &cchar);
  }

  wc[0] = MenuBorder[5];
  setcchar(&cchar, wc, 0, 0, NULL);
  for(int i = 1; i < width - 1; i++){
    mvadd_wch(_state->t_rows/2, l_stop, &cchar);
    mvadd_wch(_state->t_rows/2, r_stop, &cchar);
  }

  // attroff(COLOR_PAIR(menu_c));
  // attron(COLOR_PAIR(menu_c));
  attron(A_STANDOUT);

  // TODO: skip nohup strings
  char * str = "Execute ";
  int len1 = strlen(str);
  int len2 = strlen(command_str);
  int len3 = len1 + len2;

  for(int i = 0; i < len1; i++){
    mvaddch(_state->t_rows/2 - 2, _state->t_cols/2 - len3/2 + i, str[i]);
  }

  for(int i = 0; i < len2; i++){

    if(command_str[i] > 96 && command_str[i] < 123){
      mvaddch(_state->t_rows/2 - 2, _state->t_cols/2 - len3/2 + i + len1, command_str[i] - 32);
    } else {
      mvaddch(_state->t_rows/2 - 2, _state->t_cols/2 - len3/2 + i + len1, command_str[i]);
    }
  }

  mvaddch(_state->t_rows/2 - 2, _state->t_cols/2 - len3/2 + len3, '?');

  attroff(COLOR_PAIR(menu_c));
  attroff(A_STANDOUT);
  refresh();
}

void _print_confirm_selection(int selection){

  int r_stop = _state->t_cols - 2;
  int l_stop = 1;

  if(_state->t_cols > 20){
    r_stop = _state->t_cols/2 + 8;
    l_stop = _state->t_cols/2 - 8;;
  }

  char * yes = "Yes";
  char * no = "No";

  if(selection == true){
    attron(COLOR_PAIR(white));
    attron(A_STANDOUT);

  } else {
    attron(COLOR_PAIR(white));
  }

  for(int i = 0; i < 3; i++){
    mvaddch(_state->t_rows/2, r_stop - 4 + i, yes[i]);
  }
  attroff(A_STANDOUT);

  if(selection == false){
    attron(COLOR_PAIR(white));
    attron(A_STANDOUT);
  } else {
    attron(COLOR_PAIR(white));
  }

  for(int i = 0; i < 2; i++){
    mvaddch(_state->t_rows/2, l_stop + 2 + i, no[i]);
  }

  attroff(A_STANDOUT);
  attroff(COLOR_PAIR(white));
}

void _clear_menu(int dim_y, int dim_x, int offset_y, int offset_x){

  if(dim_y < 3){
    clear();
    return;
  }

  wchar_t wc[2] = { L' ', L'\0' };
  cchar_t cchar;
  setcchar(&cchar, wc, 0, 0, NULL);
  for(int i = 1; i < dim_y - 1; i++){
    for(int j = 1; j < dim_x - 1; j++){
      mvadd_wch(_state->t_rows/2 - dim_y/2 + i + offset_y, _state->t_cols/2 - dim_x/2 + j + offset_x, &cchar);
    }
  }
}

void _glitch(Arg bigmode){

  _none((Arg){.x = 1});
  _state->t_cache = _state->t_rows + _state->t_cols;

  int rng_row, rng_shift, rng_backdrop = 0;
  int rng_backdrop_rng = 2;

  for( int i = 0 ; i < GLITCH_TIME; i++ ) {
    rng_row   = rand() % FG_GLYPH_HEIGHT;
    rng_shift = (rand() % 3) - 1;                       // RNG -1 and 1
    rng_backdrop = rand() % rng_backdrop_rng;           // RNG 0 and 2

    if (_state->win_size == t_small) {
      attron(COLOR_PAIR(_state->fg_c));
      if(fg_c_bold) attron(A_BOLD);
      if (rng_backdrop == 0){
        mvprintw(_state->t_rows/2 - FG_GLYPH_HEIGHT/2 + rng_row + fg_offset_y,
                (_state->t_cols - FG_GLYPH_LENGTH)/2 - rng_shift,
                 "%s", fg[rng_row]);
      }
      attroff(A_BOLD);
      attroff(COLOR_PAIR(_state->fg_c));
    }

    if (_state->win_size == t_full) {

      if(bigmode.x == 1){

        if(bg_c_bold) attron(A_BOLD);
        attron(COLOR_PAIR(_state->bg_c));
        int rng_rowx = rand() % BG_GLYPH_HEIGHT;
        int rng_shift = (rand() % 2) - 1;               // RNG -1 and 1

        mvprintw(_state->t_rows/2 - BG_GLYPH_HEIGHT/2 + bg_offset_y + rng_rowx,
                (_state->t_cols - BG_GLYPH_LENGTH)/2 + bg_offset_x - rng_shift,
                 "%s", bg[rng_rowx]);

        attroff(COLOR_PAIR(_state->bg_c));
        attroff(A_BOLD);
      }

      if(fg_c_bold) attron(A_BOLD);
      attron(COLOR_PAIR(_state->fg_c));
      if (rng_backdrop == 0){
        mvprintw(_state->t_rows/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + rng_row,
                (_state->t_cols - FG_GLYPH_LENGTH)/2 + fg_offset_x - rng_shift,
                 "%s", fg[rng_row]);

      } else if (rng_backdrop == 1){
        mvprintw(_state->t_rows/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + rng_row,
                (_state->t_cols - FG_GLYPH_LENGTH)/2 + fg_offset_x - rng_shift,
                 "%s", fg[rng_row]);
      }
      attroff(COLOR_PAIR(_state->fg_c));
      attroff(A_BOLD);
    }

    int result = _key();
    if(result == 2) return;
    if (_state->hold_char) mvprintw(_state->t_rows/2, _state->t_cols/2, "%c", _state->hold_char);
    refresh();

    getmaxyx(stdscr, _state->t_rows, _state->t_cols);
    if (_state->t_cache != _state->t_rows + _state->t_cols) break;

    usleep(_state->glitch_frame_time);
  }

  _none((Arg){.x = 0});
}

void _neon(){

  double cycle_length = 0.3;
  double elapsed_time = 0;
  clock_t cycle_start = clock();

  int first_frame = 0;
  int second_frame = 0;
  int third_frame = 0;

  clear();
  refresh();

  while(cycle_length > elapsed_time){

    elapsed_time = (double)(clock() - cycle_start) / CLOCKS_PER_SEC;

    if(elapsed_time > 0.05 && first_frame == 0){
      if (_state->win_size == t_small){
        _overlay(fg, '_');
      } else {
          _overlay_bg(0);
      }

      _colorbar();
      first_frame = 1;
    }

    if(elapsed_time > 0.1 && second_frame == 0){
      if (_state->win_size == t_small){
        _fg(fg);
      } else {
        _overlay(fg, '_');
      }
      second_frame = 1;
    }

    if(elapsed_time > 0.2 && third_frame == 0){
      if (_state->win_size == t_small && hd[0] != 0){
        _hd();
      } else {
        _overlay(fg, 0);
      }
      third_frame = 1;
    }

    int result = _key();
    if(result == 2) return;
    if (_state->hold_char != '\0') mvprintw(_state->t_rows/2, _state->t_cols/2, "%c", _state->hold_char);
  }

  _none((Arg){.x = 0});
}

void _neon_reverse(){

  clear();
  _colorbar();

  clock_t cycle_start = clock();
  double cycle_length = 0.2;
  double elapsed_time = 0;

  int first_frame = 0;
  int second_frame = 0;

  while(cycle_length > elapsed_time){

    elapsed_time = (double)(clock() - cycle_start) / CLOCKS_PER_SEC;

    if(elapsed_time > 0.00 && first_frame == 0){
      if (_state->win_size == t_small){
        _overlay(fg, '_');
      } else {
        _bg();
        _overlay(fg, '_');
      }
      first_frame = 1;
    }

    if(elapsed_time > 0.1 && second_frame == 0){
      clear();
      _colorbar();
      if (_state->win_size == t_small){
        _overlay(fg, '_');
      } else {
        _bg();
      }
      second_frame = 1;
    }

    int result = _key();
    if(result == 2) return;
    if (_state->hold_char != '\0') mvprintw(_state->t_rows/2, _state->t_cols/2, "%c", _state->hold_char);
  }
}

void _shutter_slide(){
  int num_frames = 8;
  int margin_width = (_state->t_cols - FG_GLYPH_LENGTH) / 2;
  int left_stop = margin_width;
  int right_stop = _state->t_cols - margin_width - FG_GLYPH_LENGTH;
  int l_idx = 0;
  int r_idx = _state->t_cols - FG_GLYPH_LENGTH;
  int frame_travel_width = margin_width / 10;
  int j = 0;
  _colorbar();
  if(fg_c_bold) attron(A_BOLD);
  attron(COLOR_PAIR(_state->fg_c));
  while(j < num_frames){
    clear();
    for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
      if (i % 2 == 0){
        mvprintw(_state->t_rows/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, l_idx + (frame_travel_width * j) + fg_offset_x, "%s", fg[i]);
      } else {
        mvprintw(_state->t_rows/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, r_idx - (frame_travel_width * j) + fg_offset_x, "%s", fg[i]);
      }
    }
    l_idx++;
    r_idx--;

    if (l_idx >= left_stop || r_idx <= right_stop){
      break;
    }

    refresh();
    usleep(10000);
    j++;
  }
  attroff(A_BOLD);
  attroff(COLOR_PAIR(_state->fg_c));
  return;
};


void _pixel_fill(){

  clear();
  refresh();
  if(_state->win_size == t_full) _bg();

  // 0 == !'█'
  // 1 == '█'
  // 2 == '█' and successfully rolled to print to screen
  int fg_arr[FG_GLYPH_HEIGHT][FG_GLYPH_LENGTH];
  memset(fg_arr, 0, sizeof(fg_arr));

  int total_fg_chars = 0;

  for(int i = 0; i < FG_GLYPH_HEIGHT; i++){

    mbstate_t state;                            // Tracks state of mbrtowc function when converting between types of chars
    memset(&state, 0, sizeof(mbstate_t));
    const char *iter_row = fg[i];               // Grabs a line from glyph
    int iter_col = 0;                           // Track the column position
    while (*iter_row) {                         // Iterate through chars in row
      wchar_t wc[2] = { L'\0', L'\0' };          // Create wide character var
      // Converts character from iter_row to wide char `wc`
      // Also records length of character at *iter_row in len
      size_t len = mbrtowc(wc, iter_row, MB_CUR_MAX, &state);

      if(*iter_row != ' '){
        fg_arr[i][iter_col] = 1;
        total_fg_chars++;
      }

      iter_row += len;                          // Increment the pointer one character
      iter_col++;                               // Increment col
    }
  }

  int count = 0;
  int round_fill_max = 1;
  int round_fill_count = 0;
//int break_flag = 0;                           // break flag code will print line-by-line with pixel_fill(2000);
  while(count <= total_fg_chars - 3){

    getmaxyx(stdscr, _state->t_rows, _state->t_cols);
    if (_state->t_cache != _state->t_rows + _state->t_cols) return;

    for(int i = 0; i < FG_GLYPH_HEIGHT; i++){

      mbstate_t state;
      memset(&state, 0, sizeof(mbstate_t));
      const char *iter_row = fg[i];
      int j = 0;
      while (*iter_row) {

        wchar_t wc[2] = { L'\0', L'\0' };
        size_t len = mbrtowc(&wc[0], iter_row, MB_CUR_MAX, &state);

        if (fg_arr[i][j] == 1 && roll(2) == 2){
          fg_arr[i][j] = 2;
          count++;
          round_fill_count++;
          cchar_t cchar;
          setcchar(&cchar, wc, 0, 0, NULL);
          attron(COLOR_PAIR(_state->fg_c));
          if(fg_c_bold) attron(A_BOLD);
          mvadd_wch(_state->t_rows/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (_state->t_cols-FG_GLYPH_LENGTH)/2 + fg_offset_x + j, &cchar);
          attroff(COLOR_PAIR(_state->fg_c));
          attroff(A_BOLD);
          if (round_fill_count == round_fill_max){
            round_fill_count = 0;
//          break_flag = 1; // break flag code will print line-by-line with pixel_fill(2000);
            break;
          }
        }

        iter_row += len;
        j++;
      }
//    if(break_flag){// break flag code will print line-by-line with pixel_fill(2000);
//      break_flag = 0;
//      break;
//    }
    }

    refresh();
    int result = _key();
    if(result == 2) return;
    if (_state->hold_char) mvprintw(_state->t_rows/2, _state->t_cols/2, "%c", _state->hold_char);

    usleep(12000);

  }

  _overlay(fg, 0);
  _hd();
}

void _tv_static(){

  clear();
  _colorbar();
  refresh();

  int fg_arr[FG_GLYPH_HEIGHT][FG_GLYPH_LENGTH];
  int bg_arr[BG_GLYPH_HEIGHT][BG_GLYPH_LENGTH];

  if(_state->win_size == t_full){
    for(int i = 0; i < BG_GLYPH_HEIGHT; i++){
      mbstate_t state;
      memset(&state, 0, sizeof(mbstate_t));
      const char *iter_row = bg[i];
      int iter_col = 0;
      while (*iter_row) {

        wchar_t wc;
        size_t len = mbrtowc(&wc, iter_row, MB_CUR_MAX, &state);

        if(*iter_row == ' '){
          bg_arr[i][iter_col] = 0;
        } else {
          bg_arr[i][iter_col] = 1;
        }

        iter_row += len;
        iter_col++;
      }
    }
  }

  for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
    mbstate_t state;
    memset(&state, 0, sizeof(mbstate_t));
    const char *iter_row = fg[i];
    int iter_col = 0;
    while (*iter_row) {

      wchar_t wc;
      size_t len = mbrtowc(&wc, iter_row, MB_CUR_MAX, &state);

      if(*iter_row == ' '){
        fg_arr[i][iter_col] = 0;
      } else {
        fg_arr[i][iter_col] = 1;
      }

      iter_row += len;
      iter_col++;
    }
  }

  clock_t cycle_start = clock();
  int j = 0;
  int roll_result;
  wchar_t wc[2] = { L'\0', L'\0' };

  int frames = 0;
  int total_frames = 15;
  struct timespec start, next_frame, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  next_frame = start;
  while(frames < total_frames){

    _get_dims();
    if (_resized()) return;

    int result = _key();
    if(result == 2) return;

    attron(COLOR_PAIR(_state->fg_c));
    if(fg_c_bold) attron(A_BOLD);

    for(int i = 0; i < FG_GLYPH_HEIGHT; i++){

      mbstate_t state;
      memset(&state, 0, sizeof(mbstate_t));
      const char *iter_row = fg[i];
      j = 0;
      while (*iter_row) {

        size_t len = mbrtowc(&wc[0], iter_row, MB_CUR_MAX, &state);

        roll_result = roll(2);

        int idx = fg_arr[i][j];

        if (idx == 1 && roll_result == 2){
          fg_arr[i][j] = 0;
          mvaddch(_state->t_rows/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (_state->t_cols-FG_GLYPH_LENGTH)/2 + fg_offset_x + j, ' ');
        } else if (idx == 0 && roll_result == 2){
          fg_arr[i][j] = 1;
          cchar_t cchar;
          setcchar(&cchar, wc, 0, 0, NULL);
          mvadd_wch(_state->t_rows/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (_state->t_cols-FG_GLYPH_LENGTH)/2 + fg_offset_x + j, &cchar);
        }

        iter_row += len;
        j++;
      }
    }
    attroff(A_BOLD);
    attroff(COLOR_PAIR(_state->fg_c));

    if(_state->win_size == t_full){
      attron(COLOR_PAIR(_state->bg_c));
      if(bg_c_bold) attron(A_BOLD);
      for(int i = 0; i < BG_GLYPH_HEIGHT; i++){

        mbstate_t state;
        memset(&state, 0, sizeof(mbstate_t));
        const char *iter_row = bg[i];
        j = 0;
        while (*iter_row) {

          size_t len = mbrtowc(wc, iter_row, MB_CUR_MAX, &state);

          roll_result = roll(2);

          int idx = bg_arr[i][j];

          if (idx == 1 && roll_result == 2){
            bg_arr[i][j] = 0;
            mvaddch(_state->t_rows/2 - BG_GLYPH_HEIGHT/2 + bg_offset_y + i, (_state->t_cols-BG_GLYPH_LENGTH)/2 + bg_offset_x + j, ' ');
          } else if (idx == 0 && roll_result == 2){
            bg_arr[i][j] = 1;
            cchar_t cchar;
            setcchar(&cchar, wc, 0, 0, NULL);
            mvadd_wch(_state->t_rows/2 - BG_GLYPH_HEIGHT/2 + bg_offset_y + i, (_state->t_cols-BG_GLYPH_LENGTH)/2 + bg_offset_x + j, &cchar);
          }

          iter_row += len;
          j++;
        }
      }
      attroff(A_BOLD);
      attroff(COLOR_PAIR(_state->fg_c));
    }

    refresh();

    _inc_nanosecs(&next_frame, FRAME_TIME_NS);
    int clock_result = EINTR;
    while (clock_result == EINTR) {
      clock_result = clock_nanosleep(
          CLOCK_MONOTONIC,
          TIMER_ABSTIME,
          &next_frame,
          NULL
      );
    }

    frames++;
  }

  _none((Arg){.x = 0});
}

void _none(Arg printColorbar){
  clear();

  if(dynamic_resize && _state->win_size == t_small){
    _fg(fg);
    _hd();
  } else if (_state->win_size == t_full){
    _bg();
    _overlay(fg, 0);
    _hd();

    // TODO: abstract this to separate function and add config.h options to it
    //   .. also this is no longer implemented
    if (printColorbar.x){
      _colorbar();
    }

    attroff(COLOR_PAIR(_state->fg_c + 8));
    attroff(COLOR_PAIR(_state->fg_c));
    attroff(COLOR_PAIR(_state->fg_c));
    attroff(A_BOLD);
    attroff(A_STANDOUT);

  }

  if(_state->show_colors){
    _show_colors();
  }

  if(_state->slap_str[0] != 0){
    mvprintw(0, 0, "%s", _state->slap_str);
  }
}

void _bg() {
  _overlay_bg(0);
}

void _overlay(const char * glyph[], char fill){
  if(fg_c_bold) attron(A_BOLD);
  attron(COLOR_PAIR(_state->fg_c));
  for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
    mbstate_t state;
    memset(&state, 0, sizeof(mbstate_t));
    const char *iter_row = glyph[i];
    int iter_col = 0;

    while (*iter_row) {
      wchar_t wc[2] = { L'\0', L'\0' };
      size_t len = mbrtowc(wc, iter_row, MB_CUR_MAX, &state);

      if (len == (size_t)-1 || len == (size_t)-2 || len == 0) break;

      if (*iter_row == ' '){
        iter_row += len;
        iter_col++;
        continue;
      }

      if(fill){
        mvaddch(_state->t_rows/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (_state->t_cols-FG_GLYPH_LENGTH)/2 + iter_col + fg_offset_x, fill);
        iter_row += len;
        iter_col++;
      } else {
        cchar_t cchar;
        setcchar(&cchar, wc, 0, 0, NULL);
        mvadd_wch(_state->t_rows/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (_state->t_cols-FG_GLYPH_LENGTH)/2 + iter_col + fg_offset_x, &cchar);
        iter_row += len;
        iter_col++;
      }
    }
  }
  refresh();
  attroff(COLOR_PAIR(_state->fg_c));
  attroff(A_BOLD);
}

void _overlay_bg(char ch){
  attron(COLOR_PAIR(_state->bg_c));
  if(bg_c_bold) attron(A_BOLD);
  for(int i = 0; i < BG_GLYPH_HEIGHT; i++){
    mbstate_t state;
    memset(&state, 0, sizeof(mbstate_t));
    const char *iter_row = bg[i];
    int iter_col = 0;
    while (*iter_row) {
      wchar_t wc[2] = { L'\0', L'\0' };
      size_t len = mbrtowc(wc, iter_row, MB_CUR_MAX, &state);

      if (len == (size_t)-1 || len == (size_t)-2 || len == 0) break;

      if(*iter_row != ' ' && ch != 0){
        mvaddch(_state->t_rows/2 - BG_GLYPH_HEIGHT/2 + bg_offset_y + i, (_state->t_cols-BG_GLYPH_LENGTH)/2 + bg_offset_x + iter_col, ch);
      } else if(wc[0] != ' '){
        cchar_t cchar;
        setcchar(&cchar, wc, 0, 0, NULL);
        mvadd_wch(_state->t_rows/2 - BG_GLYPH_HEIGHT/2 + bg_offset_y + i, (_state->t_cols-BG_GLYPH_LENGTH)/2 + bg_offset_x + iter_col, &cchar);
      }

      iter_row += len;
      iter_col++;
    }
  }
  attroff(A_BOLD);
  attroff(COLOR_PAIR(_state->bg_c));
}

void _fg(const char * glyph[]){
  attron(COLOR_PAIR(_state->fg_c));
  if(fg_c_bold) attron(A_BOLD);
  for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
    mvprintw(_state->t_rows/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (_state->t_cols-FG_GLYPH_LENGTH)/2 + fg_offset_x, "%s", glyph[i]);
  }
  refresh();
  attroff(COLOR_PAIR(_state->fg_c));
  attroff(COLOR_PAIR(A_BOLD));
}

// TODO: add wipe option for pixel_fill
void _hd(){

  if(hd[0] != 0){

    int offset_y = 0;
    int offset_x = 0;

    if((_state->win_size == t_full    && highlight_hd_in_full_mode)  ||
       (_state->win_size == t_small && highlight_hd_in_small_mode)) {
      if(hd_c_bold){
        attron(A_STANDOUT);
        attron(A_BOLD);
        attron(COLOR_PAIR(_state->hd_c + 16)); // .. sometimes I even amaze myself
      } else {
        attron(COLOR_PAIR(_state->hd_c + 8));
      }
    } else {
      if(hd_c_bold == true) {
        attron(A_BOLD);
      } else {
        attroff(A_BOLD);
      }
      attron(COLOR_PAIR(_state->hd_c));
    }

    if(_state->win_size == t_full){
      offset_y = hd_offset_y;
      offset_x = hd_offset_x;
    } else {
      offset_y = hd_offset_y_min;
      offset_x = hd_offset_x_min;
    }

    int center_offset = (int)strlen(hd) / 2;

    cchar_t cchar;

    mbstate_t state;
    memset(&state, 0, sizeof(mbstate_t));
    const char *iter_row = hd;
    int iter_col = 0;
    while (*iter_row) {
      wchar_t wc[2] = { L'\0', L'\0' };
      size_t len = mbrtowc(wc, iter_row, MB_CUR_MAX, &state);

      setcchar(&cchar, wc, 0, 0, NULL);
      mvadd_wch(_state->t_rows/2 + offset_y, _state->t_cols/2 + offset_x + iter_col - center_offset, &cchar);

      iter_row += len;
      iter_col++;
    }

    refresh();
    attroff(A_BOLD);
    attroff(A_STANDOUT);
    attroff(COLOR_PAIR(_state->hd_c));
    attroff(COLOR_PAIR(_state->hd_c + 8));
    attroff(COLOR_PAIR(_state->hd_c + 16));
  }
}

void _down_wipes(){

  int wipe_time = 12000; // TODO: make this configurable

  clear();
  refresh();
  if (_state->win_size == t_small){
    attron(COLOR_PAIR(_state->fg_c));
    if(fg_c_bold) attron(A_BOLD);
    for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
      mvprintw(_state->t_rows/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (_state->t_cols-FG_GLYPH_LENGTH)/2 + fg_offset_x, "%s", fg[i]);
      int result = _key();
      if(result == 2) return;
      if(_state->hold_char) mvprintw(_state->t_rows/2, _state->t_cols/2, "%c", _state->hold_char);
      refresh();
      usleep(wipe_time);
    }
    attroff(A_BOLD);
    attroff(COLOR_PAIR(_state->fg_c));
    _hd();

  } else if (_state->win_size == t_full){

    if(bg_c_bold) attron(A_BOLD);
    attron(COLOR_PAIR(_state->bg_c));
    for(int i = 0; i < BG_GLYPH_HEIGHT; i++){
      mvprintw(_state->t_rows/2 - BG_GLYPH_HEIGHT/2 + bg_offset_y + i, (_state->t_cols-BG_GLYPH_LENGTH)/2 + bg_offset_x, "%s", bg[i]);
      if(_state->hold_char) mvprintw(_state->t_rows/2, _state->t_cols/2, "%c", _state->hold_char);
      refresh();
      usleep(wipe_time);
      int result = _key();
      if(result == 2) return;
    }
    attroff(A_BOLD);
    attroff(COLOR_PAIR(_state->bg_c));

    if(fg_c_bold) attron(A_BOLD);
    attron(COLOR_PAIR(_state->fg_c));
    for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
      mbstate_t state;
      memset(&state, 0, sizeof(mbstate_t));
      const char *iter_row = fg[i];
      int iter_col = 0;

      while (*iter_row) {
        wchar_t wc[2] = { L'\0', L'\0' };
        size_t len = mbrtowc(wc, iter_row, MB_CUR_MAX, &state);

        if (*iter_row == ' '){
          iter_row += len;
          iter_col++;
          continue;
        }

        cchar_t cchar;
        setcchar(&cchar, wc, 0, 0, NULL);
        mvadd_wch(_state->t_rows/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (_state->t_cols-FG_GLYPH_LENGTH)/2 + iter_col + fg_offset_x, &cchar);
        iter_row += len;
        iter_col++;
      }
      refresh();
      usleep(wipe_time);
      int result = _key();
      if(result == 2) return;
    }
  }
  attroff(A_BOLD);
  attroff(COLOR_PAIR(_state->fg_c));
}

void tinyprint() {
  attron(COLOR_PAIR(_state->fg_c));
  if(fg_c_bold) attron(A_BOLD);
  int len = strlen(tn);
  for(int i = 0; i < len; i++){
    mvaddch(_state->t_rows/2, _state->t_cols/2 - len/2 + i, tn[i] );
  }
  attroff(COLOR_PAIR(_state->fg_c));
  attroff(A_BOLD);
}

void _colorbar(){
  if (_state->win_size == t_full) {
    for(int i = 1; i < 9; i++){
      attron(COLOR_PAIR(i));
      mvaddwstr(_state->t_rows - 1, (_state->t_cols-27) + (i*3), L"\u2588\u2588\u2588"); // Unicode full block █
      attroff(COLOR_PAIR(i));
      refresh();
    }
  }
}

void _glint(fx mode) {

  int j = 0;
  int roll_result;
  wchar_t wc[2] = { L'\0', L'\0' };

  if(mode == glint_f || mode == glint_a){
    attron(COLOR_PAIR(_state->fg_c));
    if(fg_c_bold) attron(A_BOLD);
    for(int i = 0; i < FG_GLYPH_HEIGHT; i++){

      mbstate_t state;
      memset(&state, 0, sizeof(mbstate_t));
      const char *iter_row = fg[i];
      j = 0;
      while (*iter_row) {

        int idx = _state->fg_state[i][j];
        size_t len = mbrtowc(wc, iter_row, MB_CUR_MAX, &state);

        roll_result = roll(2);
        if (idx == 1 && roll_result == 2){
          _state->fg_state[i][j] = 2;
          mvaddch(_state->t_rows/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (_state->t_cols-FG_GLYPH_LENGTH)/2 + fg_offset_x + j, ' ');
        } else if (idx == 2 && roll_result == 2){
          _state->fg_state[i][j] = 1;
          cchar_t cchar;
          setcchar(&cchar, wc, 0, 0, NULL);
          mvadd_wch(_state->t_rows/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (_state->t_cols-FG_GLYPH_LENGTH)/2 + fg_offset_x + j, &cchar);
        }

        iter_row += len;
        j++;
      }
    }
    attroff(A_BOLD);
    attroff(COLOR_PAIR(_state->fg_c));
  }

  if(mode == glint_b || mode == glint_a && _state->win_size == t_full){
    attron(COLOR_PAIR(_state->bg_c));
    if(bg_c_bold) attron(A_BOLD);
    for(int i = 0; i < BG_GLYPH_HEIGHT; i++){

      mbstate_t state;
      memset(&state, 0, sizeof(mbstate_t));
      const char *iter_row = bg[i];
      j = 0;
      while (*iter_row) {
        size_t len = mbrtowc(wc, iter_row, MB_CUR_MAX, &state);

        int idx = _state->bg_state[i][j];

        roll_result = roll(2);
        if (idx == 1 && roll_result == 2){
          _state->bg_state[i][j] = 2;
          mvaddch(_state->t_rows/2 - BG_GLYPH_HEIGHT/2 + bg_offset_y + i, (_state->t_cols-BG_GLYPH_LENGTH)/2 + bg_offset_x + j, ' ');
        } else if (idx == 2 && roll_result == 2){
          _state->bg_state[i][j] = 1;
          cchar_t cchar;
          setcchar(&cchar, wc, 0, 0, NULL);
          mvadd_wch(_state->t_rows/2 - BG_GLYPH_HEIGHT/2 + bg_offset_y + i, (_state->t_cols-BG_GLYPH_LENGTH)/2 + bg_offset_x + j, &cchar);
        }

        iter_row += len;
        j++;
      }
    }
    attroff(A_BOLD);
    attroff(COLOR_PAIR(_state->fg_c));
  }

  // no refresh() for _affect() functions

}

void _affect(int fx){

  if(HOLOGRAPHIC) _affect_holo();

  if(fx == glint_f){
     _glint(glint_f);
  } else if(fx == glint_b){
     _glint(glint_b);
  } else if(fx == glint_a){
     _glint(glint_a);
  }
}

void _inc_nanosecs(struct timespec *timespec, long long nanosecs) {
  timespec->tv_sec += nanosecs / 1000000000LL;
  timespec->tv_nsec += nanosecs % 1000000000LL;

  if (timespec->tv_nsec >= 1000000000L) {
    timespec->tv_sec++;
    timespec->tv_nsec -= 1000000000L;
  }
}

void _affect_holo() {
  if(_state->win_size == t_full){
    _state->bg_c++;
    _state->fg_c++;
    _state->hd_c++;
    if (_state->bg_c > 7) _state->bg_c = 2;
    if (_state->fg_c > 7) _state->fg_c = 2;
    if (_state->hd_c > 7) _state->hd_c = 2;
  }
  if(_state->fx == nofx) _state->reprint = true;
}

void _fetch(){

  if (_is_binary_in_path("fastfetch") == 1) {
    slap("fastfetch is not installed");
    return;
  }
  char * fetch_path = _run_fetch();
  int len = _get_num_fetch_lines(fetch_path);



  free(fetch_path);
}
