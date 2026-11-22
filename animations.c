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
  if (option == blink)               _blink();
  if (option == holo)                _holo();
  if (option == glimmer)             _glimmer((Arg){.x = 0});
  if (option == glimmer_full)        _glimmer((Arg){.x = 1});
  if (option == splode)              _splode();
  if (option == waves)               _waves((Arg){.x = 0});
  if (option == waves_full)          _waves((Arg){.x = 1});
  if (option == multiply)            _multiply();
  if (option == all)                 _a_all();
}

void _blink(){

  int first_frame = 0;
  int second_frame = 0;
  int third_frame = 0;
  char ch;

  clear();
  refresh();

  int frames = 0;
  int total_frames = 15;
  struct timespec next_frame;
  clock_gettime(CLOCK_MONOTONIC, &next_frame);
  while(frames < total_frames){

    if(frames > 0 && first_frame == 0){
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

    if(frames > 5 && second_frame == 0){
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
    if(frames > 10 && third_frame == 0){
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

    _fps_sleep(&next_frame);
    frames++;
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
}

void _print_confirm_window(){

  int r_stop = _state->t_cols - 0;
  int l_stop = 1;
  int width = _state->t_cols - 1;

  if(_state->t_cols > 20){
    r_stop = _state->t_cols/2 + 7;
    l_stop = _state->t_cols/2 - 7;;
    width = 14;
  }

  _clear_menu(3, width + 1, 0, 0);
  if(_state->t_rows == 1) return;

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

  char * str = "Are you sure?";
  int len1 = strlen(str);

  for(int i = 0; i < len1; i++){
    mvaddch(_state->t_rows/2 - 1, _state->t_cols/2 + i - 6, str[i]);
  }

  attroff(COLOR_PAIR(menu_c));
  attroff(A_STANDOUT);
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
    mvaddch(_state->t_rows/2, r_stop - 5 + i, yes[i]);
  }
  attroff(A_STANDOUT);

  if(selection == false){
    attron(COLOR_PAIR(white));
    attron(A_STANDOUT);
  } else {
    attron(COLOR_PAIR(white));
  }

  for(int i = 0; i < 2; i++){
    mvaddch(_state->t_rows/2, l_stop + 3 + i, no[i]);
  }

  attroff(A_STANDOUT);
  attroff(COLOR_PAIR(white));
  refresh();
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

  int total_frames = 15;
  struct timespec next_frame;
  clock_gettime(CLOCK_MONOTONIC, &next_frame);

  for( int i = 0 ; i < total_frames; i++ ) {
    for( int j = 0 ; j < 5; j++ ) {
      rng_row   = rand() % FG_GLYPH_HEIGHT;
      rng_shift = (rand() % 7) - 3;                       // RNG -1 and 1
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
          int rng_shift = (rand() % 7) - 3;               // RNG -1 and 1

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
    }

    int result = _key();
    if(result == 2) return;
    if (_state->hold_char) mvprintw(_state->t_rows/2, _state->t_cols/2, "%c", _state->hold_char);
    refresh();

    getmaxyx(stdscr, _state->t_rows, _state->t_cols);
    if (_state->t_cache != _state->t_rows + _state->t_cols) break;
    if(_state->holo) _affect_holo();
    _fps_sleep(&next_frame);
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
  int frames = 0;
  int total_frames =  15;
  struct timespec next_frame;
  clock_gettime(CLOCK_MONOTONIC, &next_frame);

  while(frames < total_frames){

    if(frames > 0 && first_frame == 0){
      if (_state->win_size == t_small){
        _overlay(fg, '_');
      } else {
          _overlay_bg(0);
      }

      _colorbar();
      first_frame = 1;
    } else if(frames > 5 && second_frame == 0){
      if (_state->win_size == t_small){
        _fg(fg);
      } else {
        _overlay(fg, '_');
      }
      second_frame = 1;
    } else if(frames > 10 && third_frame == 0){
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
    _fps_sleep(&next_frame);
    frames++;
  }


  _none((Arg){.x = 0});
}

void _neon_reverse(){

  clock_t cycle_start = clock();
  double cycle_length = 0.2;
  double elapsed_time = 0;

  int frames = 0;
  int total_frames = 15;
  int first_frame = 0;
  int second_frame = 0;

  struct timespec next_frame;
  clock_gettime(CLOCK_MONOTONIC, &next_frame);

  while(frames < total_frames){

    if(frames > 0 && first_frame == 0){
      clear();
      if (_state->win_size == t_small){
        _overlay(fg, '_');
      } else {
        _bg();
        _overlay(fg, '_');
      }
      _colorbar();
      first_frame = 1;
    } else if(frames > 5 && second_frame == 0){
      clear();
      if (_state->win_size == t_small){
        _overlay(fg, '_');
      } else {
        _bg();
      }
      second_frame = 1;
    }
    frames++;
    _fps_sleep(&next_frame);

    int result = _key();
    if(result == 2) return;
    if (_state->hold_char != '\0') mvprintw(_state->t_rows/2, _state->t_cols/2, "%c", _state->hold_char);
  }
}

void _shutter_slide(){

  struct timespec next_frame;
  clock_gettime(CLOCK_MONOTONIC, &next_frame);

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
      if(_state->holo){
        attroff(COLOR_PAIR(_state->fg_c));
        _affect_holo();
        attron(COLOR_PAIR(_state->fg_c));
      }
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
    _fps_sleep(&next_frame);
    j++;
  }
  attroff(A_BOLD);
  attroff(COLOR_PAIR(_state->fg_c));
  animate(none);
  return;
};


void _pixel_fill(){

  clear();
  refresh();

  struct timespec next_frame;
  clock_gettime(CLOCK_MONOTONIC, &next_frame);

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
      wchar_t wc[2] = { L'\0', L'\0' };         // Create wide character var
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
  _colorbar();
  if(fg_c_bold) attron(A_BOLD);
  attron(COLOR_PAIR(_state->fg_c));
  while(count <= total_fg_chars - 3){

    for(int i = 0; i < FG_GLYPH_HEIGHT; i++){

      mbstate_t state;
      memset(&state, 0, sizeof(mbstate_t));
      const char *iter_row = fg[i];
      int j = 0;
      while (*iter_row) {

        wchar_t wc[2] = { L'\0', L'\0' };
        size_t len = mbrtowc(wc, iter_row, MB_CUR_MAX, &state);

        if (fg_arr[i][j] == 1 && roll(2) == 2){
          fg_arr[i][j] = 2;
          count++;
          round_fill_count++;
          cchar_t cchar;
          setcchar(&cchar, wc, 0, 0, NULL);
          if(_state->holo && _state->fx != nofx){
            attroff(COLOR_PAIR(_state->fg_c));
            _affect_holo();
            attron(COLOR_PAIR(_state->fg_c));
          }
          mvadd_wch(_state->t_rows/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (_state->t_cols-FG_GLYPH_LENGTH)/2 + fg_offset_x + j, &cchar);
          if (round_fill_count == round_fill_max){
            round_fill_count = 0;
            break;
          }
        }

        iter_row += len;
        j++;
      }
    }

    refresh();
    int result = _key();
    if(result == 2 && _state->key_lock == 0) return;
    if (_state->hold_char) mvprintw(_state->t_rows/2, _state->t_cols/2, "%c", _state->hold_char);

    _get_dims();
    if (_resized()){
      animate(start_animation);
      return;
    }
    _fps_sleep(&next_frame);
  }
  attroff(COLOR_PAIR(_state->fg_c));
  attroff(A_BOLD);

  _none((Arg){.x = 0});
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
  struct timespec next_frame;
  clock_gettime(CLOCK_MONOTONIC, &next_frame);
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
            if(_state->holo){
              attroff(COLOR_PAIR(_state->bg_c));
              _affect_holo();
              attron(COLOR_PAIR(_state->bg_c));
            }
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
    _fps_sleep(&next_frame);

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

      if(_state->holo){
        attroff(COLOR_PAIR(_state->fg_c));
        _affect_holo();
        attron(COLOR_PAIR(_state->fg_c));
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

void _fg_offset(const char * glyph[], int offset_y, int offset_x){
  attron(COLOR_PAIR(_state->fg_c));
  if(fg_c_bold) attron(A_BOLD);
  for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
    mvprintw(_state->t_rows/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i + offset_y, (_state->t_cols-FG_GLYPH_LENGTH)/2 + fg_offset_x + offset_x, "%s", glyph[i]);
  }
  refresh();
  attroff(COLOR_PAIR(_state->fg_c));
  attroff(COLOR_PAIR(A_BOLD));
}

// reimp of _overlay_bg
void _bg_offset(char ch, int offset_y, int offset_x){
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
        mvaddch(_state->t_rows/2 - BG_GLYPH_HEIGHT/2 + bg_offset_y + i + offset_y, (_state->t_cols-BG_GLYPH_LENGTH)/2 + bg_offset_x + iter_col + offset_x, ch);
      } else if(wc[0] != ' '){
        cchar_t cchar;
        setcchar(&cchar, wc, 0, 0, NULL);
        mvadd_wch(_state->t_rows/2 - BG_GLYPH_HEIGHT/2 + bg_offset_y + i + offset_y, (_state->t_cols-BG_GLYPH_LENGTH)/2 + bg_offset_x + iter_col + offset_x, &cchar);
      }

      iter_row += len;
      iter_col++;
    }
  }
  attroff(A_BOLD);
  attroff(COLOR_PAIR(_state->bg_c));

}


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

void _hd_offset(int offset_y_in, int offset_x_in){

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
      mvadd_wch(_state->t_rows/2 + offset_y + offset_y_in, _state->t_cols/2 + offset_x + iter_col - center_offset + offset_x_in, &cchar);

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

  clear();
  refresh();

  struct timespec next_frame;
  clock_gettime(CLOCK_MONOTONIC, &next_frame);

  if (_state->win_size == t_small){
    attron(COLOR_PAIR(_state->fg_c));
    if(fg_c_bold) attron(A_BOLD);
    for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
      mvprintw(_state->t_rows/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (_state->t_cols-FG_GLYPH_LENGTH)/2 + fg_offset_x, "%s", fg[i]);
      int result = _key();
      if(result == 2) return;
      if(_state->hold_char) mvprintw(_state->t_rows/2, _state->t_cols/2, "%c", _state->hold_char);
      refresh();
      _fps_sleep(&next_frame);
    }
    attroff(A_BOLD);
    attroff(COLOR_PAIR(_state->fg_c));
    _hd();

  } else if (_state->win_size == t_full){

    if(!_state->holo){
      if(bg_c_bold) attron(A_BOLD);
      attron(COLOR_PAIR(_state->bg_c));
    }

    for(int i = 0; i < BG_GLYPH_HEIGHT; i++){

      if(_state->holo) {
        _affect_holo();
        if(fg_c_bold) attron(A_BOLD);
        attron(COLOR_PAIR(_state->fg_c));
      }

      mvprintw(_state->t_rows/2 - BG_GLYPH_HEIGHT/2 + bg_offset_y + i, (_state->t_cols-BG_GLYPH_LENGTH)/2 + bg_offset_x, "%s", bg[i]);
      if(_state->hold_char) mvprintw(_state->t_rows/2, _state->t_cols/2, "%c", _state->hold_char);
      refresh();
      _fps_sleep(&next_frame);
      int result = _key();
      if(result == 2) return;

      if(_state->holo) {
        if(fg_c_bold) attroff(A_BOLD);
      }
    }
    attroff(A_BOLD);
    attroff(COLOR_PAIR(_state->bg_c));

    if(fg_c_bold) attron(A_BOLD);
    if(!_state->holo){
      attron(COLOR_PAIR(_state->fg_c));
    }

    for(int i = 0; i < FG_GLYPH_HEIGHT; i++){

      if(_state->holo){
        _affect_holo();
        if(fg_c_bold) attron(A_BOLD);
        attron(COLOR_PAIR(_state->fg_c));
      }

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
      _fps_sleep(&next_frame);
      int result = _key();
      if(result == 2) return;

      if(_state->holo){
        attroff(COLOR_PAIR(_state->fg_c));
      }
    }
  }
  attroff(A_BOLD);
  attroff(COLOR_PAIR(_state->fg_c));
  _none((Arg){.x = 0});
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

  if(_state->holo) _affect_holo();

  if(fx == glint_f ||
     fx == glint_b ||
     fx == glint_a){
    _glint(fx);
  }
}

void _inc_nanosecs(struct timespec *timespec, long long nanosecs) {
  timespec->tv_sec  += nanosecs / 1000000000LL;
  timespec->tv_nsec += nanosecs % 1000000000LL;

  if (timespec->tv_nsec >= 1000000000L) {
    timespec->tv_sec++;
    timespec->tv_nsec -= 1000000000L;
  }
}

void _affect_holo() {
  if(_state->win_size == t_tiny) {
    return;
  }

  if(_state->win_size == t_full && _state->fx == nofx || _state->fx == glint_a){
    _state->bg_c++;
    _state->fg_c++;
    if (_state->bg_c > 7) _state->bg_c = 2;
    if (_state->fg_c > 7) _state->fg_c = 2;
  } else if(_state->fx == glint_f) {
    _state->fg_c++;
    if (_state->fg_c > 7) _state->fg_c = 2;
  } else if(_state->fx == glint_b) {
    _state->bg_c++;
    if (_state->bg_c > 7) _state->bg_c = 2;
  }

  if(_state->fx == nofx) _state->reprint = true;
}

void _fetch(){

  if (_is_binary_in_path("fastfetch") == 1) {
    generate_path_run_file();
    if (_is_binary_in_path("fastfetch") == 1) {
      slap("fastfetch is not installed");
    }
    return;
  }
  char * fetch_path = _run_fetch();
  if(_string_in_file(fetch_path, "No such file or directory")){
    slap("fastfetch is not installed");
    free(fetch_path);
    return;
  }
  _fetch_reduce(fetch_path);
  int num_fetch_cols = _longest_line(fetch_path);
  int num_ascii_width = _get_full_width(); // todo not complete

  clear();
  _fg_offset(fg, 0, -(num_ascii_width)/2 - 2);
  _bg_offset(0, 0, -(num_ascii_width)/2 - 2);
  _hd_offset(0, -(num_ascii_width)/2 - 2);

  FILE * file = fopen(fetch_path, "r");
  free(fetch_path);

  int entries = 0;
  char line[256] = {'\0'};
  while(fgets(line, sizeof(line), file)){
    entries++;
  }
  rewind(file);

  int line_num = 0;
  int first_flag = 0;
  int latter_half = 0;
  int sleep_flag = 0;
  struct timespec next_frame;
  clock_gettime(CLOCK_MONOTONIC, &next_frame);
  while(fgets(line, sizeof(line), file)){

    latter_half = 0;

    if (line_num != 1){
      attron(COLOR_PAIR(_state->bg_c));
      if(bg_c_bold) attron(A_BOLD);
    } else {
      attron(COLOR_PAIR(_state->fg_c));
      if(fg_c_bold) attron(A_BOLD);
    }

    for(int i = 0; i < (int)strlen(line); i++){

      if(line[i] == '@' && line_num == 0){
        attron(COLOR_PAIR(_state->fg_c));
        if(fg_c_bold) attron(A_BOLD);
        first_flag = 1;
      }

      if(latter_half && line[i] == ')'){
        attroff(COLOR_PAIR(blue));
        attron(COLOR_PAIR(_state->fg_c));
        if(fg_c_bold) attron(A_BOLD);
      }

      mvaddch(_state->t_rows/2 - entries/2 + line_num, _state->t_cols/2 + 2 + i, line[i]);

      if(first_flag){
        attroff(COLOR_PAIR(_state->bg_c));
        attroff(A_BOLD);
        attron(COLOR_PAIR(_state->bg_c));
        if(bg_c_bold) attron(A_BOLD);
        first_flag--;
      }

      if(latter_half && line[i] == '('){
        attroff(COLOR_PAIR(_state->fg_c));
        attroff(A_BOLD);
        attron(COLOR_PAIR(blue));
      }

      if(line[i] == ':'){
        latter_half = 1;
        attroff(COLOR_PAIR(_state->bg_c));
        attroff(A_BOLD);
        attron(COLOR_PAIR(_state->fg_c));
        if(fg_c_bold) attron(A_BOLD);
      }
      refresh();
    }
    if(sleep_flag > 1){
      _fps_sleep(&next_frame);
      sleep_flag = 0;
    } else {
      sleep_flag++;
    }
    int input_flag = 0;
    char input =  getch();
    if(input == 'q' || input == 27 || _input_is_fetch(input)) {
      fclose(file);
      animate(start_animation);
      return;
    } else {
      int result = _key(input);
      if(result > 0) {
        fclose(file);
        return;
      }
    }
    attroff(COLOR_PAIR(_state->bg_c));
    attroff(COLOR_PAIR(_state->fg_c));
    attroff(A_BOLD);
    line_num++;
  }

  fclose(file);

  refresh();

  char input;
  int input_flag = 0;
  while(1){

    input =  getch();
    if (input != ERR && input != '\n' && input != EOF && input > 31 && input < 127 || input == 27){
      if(input == 'q' || input == 27 || _input_is_fetch(input)) {
        _state->key_lock = 0;
        animate(start_animation);
        return;
      }
      input_flag++;
      break;
    }

    _get_dims();
    if(_resized()) {
      _fetch();
      return;
    }

    _fps_sleep(&next_frame);
  }

  int result = _key(input);
  if(result == 1){
    animate(start_animation);
  } else if (input_flag == 1 && result == 0 ) {
    animate(start_animation);
  }
}

void _a_all(){
  _down_wipes();
  _glitch((Arg){.x = 0});
  _glitch((Arg){.x = 1});
  _neon();
  _neon_reverse();
  _shutter_slide();
  _pixel_fill();
  _tv_static();
  _fg(fg);
  _bg();
  _blink();
  _holo();
  _glimmer((Arg){.x = 1});
  _splode();
  _glimmer((Arg){.x = 0});
}

void _fps_sleep(struct timespec *next_frame){
  _inc_nanosecs(next_frame, FRAME_TIME_NS);
  int clock_result = EINTR;
  while (clock_result == EINTR) {
    clock_result = clock_nanosleep(
        CLOCK_MONOTONIC,
        TIMER_ABSTIME,
        next_frame,
        NULL
    );
  }
}

void _holo(){
  int frames =  0;
  int total_frames = 15;
  int fg = _state->fg_c;
  int bg = _state->bg_c;
  int hd = _state->hd_c;
  struct timespec next_frame;
  clock_gettime(CLOCK_MONOTONIC, &next_frame);
  while(frames < total_frames){
    _state->bg_c++;
    _state->fg_c++;
    _state->hd_c++;
    if (_state->bg_c > 7) _state->bg_c = 2;
    if (_state->fg_c > 7) _state->fg_c = 2;
    if (_state->hd_c > 7) _state->hd_c = 2;
    _none((Arg){.x = 0});
    _colorbar();
    int result = _key();
    if(result == 2) break;
    _fps_sleep(&next_frame);
    frames++;
  }
  _state->fg_c = fg;
  _state->bg_c = bg;
  _state->hd_c = hd;
}

void _glimmer(Arg full){
  clear();
  int fg = _state->fg_c;
  int bg = _state->bg_c;
  int holo = _state->holo;
  _state->holo = 1;
  struct timespec next_frame;
  clock_gettime(CLOCK_MONOTONIC, &next_frame);
  int frames = 0;
  int total_frames = 15;
  if (_state->win_size == t_full) _bg();
  while(frames < total_frames){
    _state->bg_c++;
    _state->fg_c++;
    if (_state->bg_c > 7) _state->bg_c = 2;
    if (_state->fg_c > 7) _state->fg_c = 2;
    if(full.x == 1){
      _glint(glint_a);
    } else {
      _glint(glint_f);
    }
    _colorbar();
    int result = _key();
    if(result == 2) break;
    _fps_sleep(&next_frame);
    frames++;
  }
  _state->holo = holo;
  _state->fg_c = fg;
  _state->bg_c = bg;
  _none((Arg){.x = 0});
}

void _splode(){

  int row;
  int col;
  int vert_b = _state->t_cols/2;
  int hor_b  = _state->t_rows/2;
  int factor = 1;
  int offset_x = 1;
  int offset_y = 1;
  int frames = 0;
  int total_frames = 10;
  struct timespec next_frame;
  clock_gettime(CLOCK_MONOTONIC, &next_frame);
  while(frames < total_frames){
    clear();

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

        if(*iter_row == ' '){
          iter_row += len;
          iter_col++;
          continue;
        }

        row = _state->t_rows/2 - BG_GLYPH_HEIGHT/2 + bg_offset_y + i;
        if(row <= hor_b){
          offset_y = -factor;
        } else if(row > hor_b){
          offset_y = factor;
        }
        int adjustment;
        col = (_state->t_cols-BG_GLYPH_LENGTH)/2 + bg_offset_x + iter_col;
        if(col < vert_b){
          adjustment = (vert_b - col) * frames;
          offset_x = -factor - adjustment;
        } else if(col > vert_b){
          adjustment = (col - vert_b) * frames;
          offset_x = factor + adjustment;
        } else {
          offset_x = 0;
        }

        cchar_t cchar;
        setcchar(&cchar, wc, 0, 0, NULL);
        mvadd_wch(row + offset_y, col + offset_x, &cchar);

        iter_row += len;
        iter_col++;
      }
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
        if (len == (size_t)-1 || len == (size_t)-2 || len == 0) break;

        if (*iter_row == ' '){
          iter_row += len;
          iter_col++;
          continue;
        }

        row = _state->t_rows/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i;
        if(row <= hor_b){
          offset_y = -factor;
        } else if(row > hor_b){
          offset_y = factor;
        }
        int adjustment;
        col = (_state->t_cols-FG_GLYPH_LENGTH)/2 + iter_col + fg_offset_x;
        if(col < vert_b){
          adjustment = (vert_b - col) * frames;
          offset_x = -factor - adjustment;
        } else if (col > vert_b){
          adjustment = (col - vert_b) * frames;
          offset_x = factor + adjustment;
        } else {
          offset_x = 0;
        }

        cchar_t cchar;
        setcchar(&cchar, wc, 0, 0, NULL);
        mvadd_wch(row + offset_y, col + offset_x, &cchar);
        iter_row += len;
        iter_col++;
      }
    }
    attroff(COLOR_PAIR(_state->fg_c));
    attroff(A_BOLD);

    _colorbar();
    int result = _key();
    if(result == 2) return;
    if(_state->holo) _affect_holo();
    _fps_sleep(&next_frame);
    frames++;
    factor*=2;
  }
  _none((Arg){.x = 0});
}

void _waves(Arg full){
  int fg_shift[FG_GLYPH_HEIGHT];
  int bg_shift[BG_GLYPH_HEIGHT];
  memset(fg_shift, 0, sizeof(fg_shift));
  memset(bg_shift, 0, sizeof(bg_shift));
  int fg_val = 4;
  int fg_flag = 0;
  int fg_flip = 0;
  int fg_spread = 4;
  int fg_direction = 0;
  fg_shift[0] = fg_spread;
  int bg_val = 4;
  int bg_flag = 0;
  int bg_flip = 0;
  int bg_spread = 4;
  int bg_direction = 0;
  bg_shift[0] = bg_spread;
  int frames = 0;
  int total_frames = 22;
  struct timespec next_frame;
  clock_gettime(CLOCK_MONOTONIC, &next_frame);
  while(frames < total_frames){

    clear();

    if(full.x == 1){
      if(bg_c_bold) attron(A_BOLD);
      attron(COLOR_PAIR(_state->bg_c));
      for(int i = 0; i < BG_GLYPH_HEIGHT; i++){
        if(_state->fx == glint_a || _state->fx == glint_b){
          _affect(_state->fx);
        } else if(_state->holo){
          attroff(COLOR_PAIR(_state->bg_c));
          _affect_holo();
          attron(COLOR_PAIR(_state->bg_c));
        }
        mvprintw(_state->t_rows/2 - BG_GLYPH_HEIGHT/2 + bg_offset_y - i + BG_GLYPH_HEIGHT - 1,
                (_state->t_cols-BG_GLYPH_LENGTH)/2 + bg_offset_x + bg_shift[i],
                 "%s", bg[BG_GLYPH_HEIGHT - 1 - i]);
      }
      if(bg_c_bold) attroff(A_BOLD);
      attroff(COLOR_PAIR(_state->bg_c));

      if(bg_direction == 0 && bg_val <= -(bg_spread)){
        bg_direction = 1;
        bg_spread+=2;
        bg_val = -bg_spread + 1;
      } else if(bg_direction == 1 && bg_val >= bg_spread && !bg_flag){
        bg_val++;
        bg_spread+=2;
        bg_flag++;
      } else if(bg_direction == 1 && bg_val == bg_spread && bg_flag) {
        bg_direction = 0;
        bg_flag = 0;
      } else if(bg_direction == 1){
        bg_val++;
      } else {
        bg_val--;
      }
      if(bg_flip){
        bg_shift[0] = -bg_val;
        bg_flip = 0;
      } else {
        bg_shift[0] = bg_val;
        bg_flip++;
      }
    } else {
      if(_state->fx == glint_a || _state->fx == glint_b){
        if(_state->holo) _affect_holo();
        _affect(_state->fx);
      } else {
        _bg();
      }
    }

    if(fg_c_bold) attron(A_BOLD);
    attron(COLOR_PAIR(_state->fg_c));
    for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
      if(_state->holo){
        attroff(COLOR_PAIR(_state->fg_c));
        _affect_holo();
        attron(COLOR_PAIR(_state->fg_c));
      }
      mvprintw(_state->t_rows/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y - i + FG_GLYPH_HEIGHT - 1,
              (_state->t_cols-FG_GLYPH_LENGTH)/2 + fg_offset_x + fg_shift[i],
               "%s", fg[FG_GLYPH_HEIGHT - 1 - i]);
    }
    if(fg_c_bold) attroff(A_BOLD);
    attroff(COLOR_PAIR(_state->fg_c));

    if(fg_direction == 0 && fg_val <= -(fg_spread)){
      fg_direction = 1;
      fg_spread+=2;
      fg_val = -fg_spread + 1;
    } else if(fg_direction == 1 && fg_val >= fg_spread && !fg_flag){
      fg_val++;
      fg_spread+=2;
      fg_flag++;
    } else if(fg_direction == 1 && fg_val == fg_spread && fg_flag) {
      fg_direction = 0;
      fg_flag = 0;
    } else if(fg_direction == 1){
      fg_val++;
    } else {
      fg_val--;
    }
    _shift_arr(fg_shift, FG_GLYPH_HEIGHT);
    _shift_arr(bg_shift, BG_GLYPH_HEIGHT);
    if(fg_flip){
      fg_shift[0] = -fg_val;
      fg_flip = 0;
    } else {
      fg_shift[0] = fg_val;
      fg_flip++;
    }
    frames++;
    int result = _key();
    if(result == 2) return;
    _fps_sleep(&next_frame);
    _colorbar();
  }

  _none((Arg){.x = 0});
}

void _shift_arr(int arr[], int size){
  for(int i = 0; i < size - 1; i++){
    arr[size - 1 - i] = arr[size - 2 - i];
  }
}

void _multiply(){
  int frames = 0;
  int total_frames = 22;
  struct timespec next_frame;
  clock_gettime(CLOCK_MONOTONIC, &next_frame);
  int fg_y_r;
  int fg_x_r;
  int bg_y_r;
  int bg_x_r;
  while(frames < total_frames){
    if(_state->fx != nofx){
      _affect(_state->fx);
    } else if(_state->holo) {
      _affect_holo();
    }
    fg_y_r = rand() % _state->t_rows - FG_GLYPH_HEIGHT - FG_GLYPH_HEIGHT/2 - _state->t_rows/3;
    fg_x_r = rand() % _state->t_cols - FG_GLYPH_LENGTH;
    bg_y_r = rand() % _state->t_rows - BG_GLYPH_HEIGHT;
    bg_x_r = rand() % _state->t_cols - BG_GLYPH_LENGTH;

    _fg_offset(fg, fg_y_r, fg_x_r);
    _bg_offset(0, bg_y_r, bg_x_r);
    _colorbar();

//  char err[128];
//  sprintf(err, "%d %d %d %d", fg_y_r, fg_x_r, bg_y_r, bg_x_r);
//  mvprintw(0,0, "%s", "                                                   ");
//  mvprintw(0,0, "%s", err);

    int result = _key();
    if(result == 2) return;
    _fps_sleep(&next_frame);
    //usleep(800000);
    frames++;
  }
  _none((Arg){.x = 0});
}
