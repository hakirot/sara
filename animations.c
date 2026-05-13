/*
--   █████╗ ███╗   ██╗██╗███╗   ███╗ █████╗ ████████╗██╗ ██████╗ ███╗   ██╗███████╗  --
--  ██╔══██╗████╗  ██║██║████╗ ████║██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║██╔════╝  --
--  ███████║██╔██╗ ██║██║██╔████╔██║███████║   ██║   ██║██║   ██║██╔██╗ ██║███████╗  --
--  ██╔══██║██║╚██╗██║██║██║╚██╔╝██║██╔══██║   ██║   ██║██║   ██║██║╚██╗██║╚════██║  --
--  ██║  ██║██║ ╚████║██║██║ ╚═╝ ██║██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║███████║  --
--  ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝     ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝.c--
                                                                                       */
#define NCURSES_WIDECHAR 1

#include "animations.h"
#include "globals.h"
#include "config.h"
#include "utils.h"
#include "sara.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// TODO: audit this list
void _none(Arg printColorbar);
void _down_wipes();
void _glitch(Arg bigmode);
void _neon();
void _neon_reverse();
void _shutter_slide();
void _pixel_fill();
void _tv_static();
void _bg();
void _fg(const char * glyph[]);
void _hd();
void _overlay(const char * glyph[], char fill);

void animate(animation_option option){
  if (option == none)                _none((Arg){0});
  if (option == down_wipes)          _down_wipes();
  if (option == glitch)              _glitch((Arg){.x = 0});
  if (option == glitch_full)         _glitch((Arg){.x = 1});
  if (option == neon)                _neon();
  if (option == neon_reverse)        _neon_reverse();
  if (option == shutter_slide)       _shutter_slide();
  if (option == shutter_slide_neon){ _shutter_slide(); _neon();}
  if (option == pixel_fill)          _pixel_fill();
  if (option == tv_static)           _tv_static();
  if (option == print_f)             _fg(fg);
  if (option == print_b)             _bg();
}

// TODO: implement offset
void _print_menu_borders(){

  attron(COLOR_PAIR(menu_c));
  wchar_t wc = MenuBorder[0];
  cchar_t cchar;
  setcchar(&cchar, &wc, 0, 0, NULL);
  mvadd_wch(ROW/2 - menu_y/2, COL/2 - menu_x/2, &cchar);

  wc = MenuBorder[1];
  setcchar(&cchar, &wc, 0, 0, NULL);
  mvadd_wch(ROW/2 - menu_y/2, COL/2 - menu_x/2 + menu_x - 1, &cchar);

  wc = MenuBorder[2];
  setcchar(&cchar, &wc, 0, 0, NULL);
  mvadd_wch(ROW/2 - menu_y/2 + menu_y - 1, COL/2 - menu_x/2, &cchar);

  wc = MenuBorder[3];
  setcchar(&cchar, &wc, 0, 0, NULL);
  mvadd_wch(ROW/2 - menu_y/2 + menu_y - 1, COL/2 - menu_x/2 + menu_x - 1, &cchar);

  wc = MenuBorder[4];
  setcchar(&cchar, &wc, 0, 0, NULL);
  for(int i = 1; i < menu_x - 1; i++){
    mvadd_wch(ROW/2 - menu_y/2, COL/2 - menu_x/2 + i, &cchar);
    mvadd_wch(ROW/2 - menu_y/2 + menu_y - 1, COL/2 - menu_x/2 + i, &cchar);
  }

  wc = MenuBorder[5];
  setcchar(&cchar, &wc, 0, 0, NULL);
  for(int i = 1; i < menu_y - 1; i++){
    mvadd_wch(ROW/2 - menu_y/2 + i, COL/2 - menu_x/2, &cchar);
    mvadd_wch(ROW/2 - menu_y/2 + i, COL/2 - menu_x/2 + menu_x - 1, &cchar);
  }
  attroff(COLOR_PAIR(menu_c));

  _clear_menu();

  refresh();
}

void _clear_menu(){
  wchar_t wc = L' ';
  cchar_t cchar;
  setcchar(&cchar, &wc, 0, 0, NULL);
  for(int i = 1; i < menu_y - 1; i++){
    for(int j = 1; j < menu_x - 1; j++){
      mvadd_wch(ROW/2 - menu_y/2 + i, COL/2 - menu_x/2 + j, &cchar);
    }
  }
}

// TODO: New test glyphs reveal undefined behavior with the header
// TODO: Place colorbar at same line as hd
void _glitch(Arg bigmode){

  _none((Arg){.x = 1});
  CACHE = ROW + COL;

  int rng_row, rng_shift, rng_backdrop = 0;
  int rng_backdrop_rng = 2; if(IM_SET) rng_backdrop_rng++;

  for( int i = 0 ; i < GLITCH_TIME; i++ ) {
    rng_row   = rand() % FG_GLYPH_HEIGHT;
    rng_shift = (rand() % 3) - 1;                       // RNG -1 and 1
    rng_backdrop = rand() % rng_backdrop_rng;           // RNG 0 and 2

    if (WIN_SIZE == NORMAL) {
      attron(COLOR_PAIR(FOREGROUND));
      if(use_bold_color_for_fg) attron(A_BOLD);
      if (rng_backdrop == 0){
        mvprintw(ROW/2 - FG_GLYPH_HEIGHT/2 + rng_row + fg_offset_y,
                (COL - FG_GLYPH_LENGTH)/2 - rng_shift,
                 "%s", fg[rng_row]);
      } else if (IM_SET){
        mvprintw(ROW/2 - FG_GLYPH_HEIGHT/2 + rng_row + fg_offset_y,
                (COL - FG_GLYPH_LENGTH)/2 - rng_shift,
                 "%s", im[rng_row]);
      }
      attroff(A_BOLD);
      attroff(COLOR_PAIR(FOREGROUND));
    }

    if (WIN_SIZE == BIG) {

      if(bigmode.x == 1){

        if(use_bold_color_for_bg) attron(A_BOLD);
        attron(COLOR_PAIR(BACKGROUND));
        int rng_rowx = rand() % BG_GLYPH_HEIGHT;
        int rng_shift = (rand() % 2) - 1;               // RNG -1 and 1

        mvprintw(ROW/2 - BG_GLYPH_HEIGHT/2 + bg_offset_y + rng_rowx,
                (COL - BG_GLYPH_LENGTH)/2 + bg_offset_x - rng_shift,
                 "%s", bg[rng_rowx]);

        attroff(COLOR_PAIR(BACKGROUND));
        attroff(A_BOLD);
      }

      if(use_bold_color_for_fg) attron(A_BOLD);
      attron(COLOR_PAIR(FOREGROUND));
      if (rng_backdrop == 0){
        mvprintw(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + rng_row,
                (COL - FG_GLYPH_LENGTH)/2 + fg_offset_x - rng_shift,
                 "%s", fg[rng_row]);

      } else if (rng_backdrop == 1){
        mvprintw(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + rng_row,
                (COL - FG_GLYPH_LENGTH)/2 + fg_offset_x - rng_shift,
                 "%s", fg[rng_row]);
      } else {
        mvprintw(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + rng_row,
                (COL - FG_GLYPH_LENGTH)/2 + fg_offset_x - rng_shift,
                 "%s", im[rng_row]);
      }
      attroff(COLOR_PAIR(FOREGROUND));
      attroff(A_BOLD);
    }

    check_char();
    if (HOLD_CHAR) mvprintw(ROW/2, COL/2, "%c", HOLD_CHAR);
    refresh();

    getmaxyx(stdscr, ROW, COL);
    if (CACHE != ROW + COL) break;

    usleep(GLITCH_FRAME_TIME);
  }

  _none((Arg){.x = 0});
}

// TODO: fg glyph not in correct position when im not defined
//       and WIN_SIZE == normal, but only when the width is constrained
//       to a certain size as well? huh. Just check the prints between im[-]
//       and fg
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
      if (WIN_SIZE == NORMAL && IM_SET){
        _fg(im);
      } else if (WIN_SIZE == NORMAL && !IM_SET){
        _overlay(fg, '+');
      } else {
        _bg();
      }

      // TODO: create colorbar options
      if (WIN_SIZE == BIG) {
        for(int i = 1; i < 9; i++){
          attron(COLOR_PAIR(i));
          mvaddwstr(ROW/2 + 4, (COL-FG_GLYPH_LENGTH)/2 + 3 + (i*3), L"\u2588\u2588\u2588"); // Unicode full block █
          attroff(COLOR_PAIR(i));
          refresh();
        }
      }
      first_frame = 1;
    }

    if(elapsed_time > 0.1 && second_frame == 0){
      if (WIN_SIZE == NORMAL){
        _fg(fg);
      } else {
        if (IM_SET){
          _overlay(im, 0);
        } else {
          _overlay(fg, '-');
        }
      }
      second_frame = 1;
    }

    if(elapsed_time > 0.2 && third_frame == 0){
      if (WIN_SIZE == NORMAL && hd[0] != 0){
        _hd();
      } else {
        _overlay(fg, 0);
      }
      third_frame = 1;
    }

    check_char();
    if (HOLD_CHAR != '\0') mvprintw(ROW/2, COL/2, "%c", HOLD_CHAR);
  }

  _none((Arg){.x = 0});
}

void _neon_reverse(){

  clock_t cycle_start = clock();
  double cycle_length = 0.2;
  double elapsed_time = 0;

  int first_frame = 0;
  int second_frame = 0;

  while(cycle_length > elapsed_time){

    elapsed_time = (double)(clock() - cycle_start) / CLOCKS_PER_SEC;

    if(elapsed_time > 0.00 && first_frame == 0){
      if (WIN_SIZE == NORMAL){
        if(IM_SET){
          _fg(im);
        } else {
          _overlay(fg, '-');
        }
      } else {

        if(IM_SET){
          _bg();
          _overlay(im, 0);
        } else {
          _overlay(fg, '-');
        }
      }
      first_frame = 1;
    }

    if(elapsed_time > 0.1 && second_frame == 0){
      clear();
      if (WIN_SIZE == NORMAL){
        _overlay(fg, '-');
      } else {
        _bg();
      }
      second_frame = 1;
    }

    check_char();
    if (HOLD_CHAR != '\0') mvprintw(ROW/2, COL/2, "%c", HOLD_CHAR);
  }
}

void _shutter_slide(){
  int num_frames = 8;
  int margin_width = (COL - FG_GLYPH_LENGTH) / 2;
  int left_stop = margin_width;
  int right_stop = COL - margin_width - FG_GLYPH_LENGTH;
  int l_idx = 0;
  int r_idx = COL - FG_GLYPH_LENGTH;
  int frame_travel_width = margin_width / 10;
  int j = 0;
  attron(COLOR_PAIR(FOREGROUND));
  while(j < num_frames){
    clear();
    for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
      if (i % 2 == 0){
        mvprintw(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, l_idx + (frame_travel_width * j) + fg_offset_x, "%s", fg[i]);
      } else {
        mvprintw(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, r_idx - (frame_travel_width * j) + fg_offset_x, "%s", fg[i]);
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
  attroff(COLOR_PAIR(FOREGROUND));
  return;
};


void _pixel_fill(){

  clear();
  refresh();
  if(WIN_SIZE == BIG) _bg();

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
      wchar_t wc;                               // Create wide character var
      // Converts character from iter_row to wide char `wc`
      // Also records length of character at *iter_row in len
      size_t len = mbrtowc(&wc, iter_row, MB_CUR_MAX, &state);

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

    getmaxyx(stdscr, ROW, COL);
    if (CACHE != ROW + COL) return;

    for(int i = 0; i < FG_GLYPH_HEIGHT; i++){

      mbstate_t state;
      memset(&state, 0, sizeof(mbstate_t));
      const char *iter_row = fg[i];
      int j = 0;
      while (*iter_row) {

        wchar_t wc;
        size_t len = mbrtowc(&wc, iter_row, MB_CUR_MAX, &state);

        if (fg_arr[i][j] == 1 && roll(2) == 2){
          fg_arr[i][j] = 2;
          count++;
          round_fill_count++;
          cchar_t cchar;
          setcchar(&cchar, &wc, 0, 0, NULL);
          attron(COLOR_PAIR(FOREGROUND));
          if(use_bold_color_for_fg) attron(A_BOLD);
          mvadd_wch(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (COL-FG_GLYPH_LENGTH)/2 + fg_offset_x + j, &cchar);
          attroff(COLOR_PAIR(FOREGROUND));
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
    check_char();
    if (HOLD_CHAR) mvprintw(ROW/2, COL/2, "%c", HOLD_CHAR);

    // TODO: make this configurable
    usleep(12000);

  }

  _overlay(fg, 0);
  _hd();
}

void _tv_static(){

  clear();
  refresh();

  int fg_arr[FG_GLYPH_HEIGHT][FG_GLYPH_LENGTH];
  int bg_arr[BG_GLYPH_HEIGHT][BG_GLYPH_LENGTH];

  if(WIN_SIZE == BIG){
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

  double elapsed_time = 0;
  clock_t cycle_start = clock();
  int j = 0;
  int roll_result;
  wchar_t wc;
  // TODO: make configurable
  while(elapsed_time < 0.020){

    getmaxyx(stdscr, ROW, COL);
    if (CACHE != ROW + COL) return;
    check_char();

    attron(COLOR_PAIR(FOREGROUND));
    if(use_bold_color_for_fg) attron(A_BOLD);
    for(int i = 0; i < FG_GLYPH_HEIGHT; i++){

      mbstate_t state;
      memset(&state, 0, sizeof(mbstate_t));
      const char *iter_row = fg[i];
      j = 0;
      while (*iter_row) {

        size_t len = mbrtowc(&wc, iter_row, MB_CUR_MAX, &state);

        roll_result = roll(2);

        // TODO: debug
        int idx = fg_arr[i][j];
        if(idx != 0 && idx != 1){
          char er[10] = {'\0'};
          sprintf(er, "%d", idx);
          crit(er);
        }

        if (idx == 1 && roll_result == 2){
          fg_arr[i][j] = 0;
          mvaddch(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (COL-FG_GLYPH_LENGTH)/2 + fg_offset_x + j, ' ');
        } else if (idx == 0 && roll_result == 2){
          fg_arr[i][j] = 1;
          cchar_t cchar;
          setcchar(&cchar, &wc, 0, 0, NULL);
          mvadd_wch(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (COL-FG_GLYPH_LENGTH)/2 + fg_offset_x + j, &cchar);
        }

        iter_row += len;
        j++;
      }
    }
    attroff(A_BOLD);
    attroff(COLOR_PAIR(FOREGROUND));

    if(WIN_SIZE == BIG){
      attron(COLOR_PAIR(BACKGROUND));
      if(use_bold_color_for_bg) attron(A_BOLD);
      for(int i = 0; i < BG_GLYPH_HEIGHT; i++){

        mbstate_t state;
        memset(&state, 0, sizeof(mbstate_t));
        const char *iter_row = bg[i];
        j = 0;
        while (*iter_row) {

          size_t len = mbrtowc(&wc, iter_row, MB_CUR_MAX, &state);

          roll_result = roll(2);

          int idx = bg_arr[i][j];
          // TODO: debug
          if(idx != 0 && idx != 1){
            char er[10] = {'\0'};
            sprintf(er, "%d", idx);
            crit(er);
          }

          if (idx == 1 && roll_result == 2){
            bg_arr[i][j] = 0;
            mvaddch(ROW/2 - BG_GLYPH_HEIGHT/2 + bg_offset_y + i, (COL-BG_GLYPH_LENGTH)/2 + bg_offset_x + j, ' ');
          } else if (idx == 0 && roll_result == 2){
            bg_arr[i][j] = 1;
            cchar_t cchar;
            setcchar(&cchar, &wc, 0, 0, NULL);
            mvadd_wch(ROW/2 - BG_GLYPH_HEIGHT/2 + bg_offset_y + i, (COL-BG_GLYPH_LENGTH)/2 + bg_offset_x + j, &cchar);
          }

          iter_row += len;
          j++;
        }
      }
      attroff(A_BOLD);
      attroff(COLOR_PAIR(FOREGROUND));
    }

    usleep(1000);
    refresh();
    elapsed_time = (double)(clock() - cycle_start) / CLOCKS_PER_SEC;
  }
}

// TODO: remove globals as parameters
void _none(Arg printColorbar){
  clear();
  if(dynamic_resize && WIN_SIZE == NORMAL){
    _fg(fg);
    _hd();
  } else if (WIN_SIZE == BIG){
    _bg();
    _overlay(fg, 0);
    _hd();

    // TODO: abstract this to separate function and add config.h options to it
    //   .. also this is no longer implemented
    if (printColorbar.x){
      for(int i = 1; i < 9; i++){
        attron(COLOR_PAIR(i));
        mvaddwstr(ROW/2 + 5, (COL-FG_GLYPH_LENGTH)/2 + 15 + (i*3), L"\u2588\u2588\u2588"); // Unicode full block █
        attroff(COLOR_PAIR(i));
      }
    }

    attroff(COLOR_PAIR(FOREGROUND + 8));
    attroff(COLOR_PAIR(FOREGROUND));
    attroff(COLOR_PAIR(FOREGROUND));
    attroff(A_BOLD);
    attroff(A_STANDOUT);
  }
}

void _bg() {
  clear();
  attron(COLOR_PAIR(BACKGROUND));
  if(use_bold_color_for_bg) attron(A_BOLD);
  for(int i = 0; i < BG_GLYPH_HEIGHT; i++){
    mvprintw(ROW/2 - BG_GLYPH_HEIGHT/2 + bg_offset_y + i, (COL-BG_GLYPH_LENGTH)/2 + bg_offset_x, "%s", bg[i]);
  }
  refresh();
  attroff(A_BOLD);
  attroff(COLOR_PAIR(BACKGROUND));
}

void _overlay(const char * glyph[], char fill){
  if(use_bold_color_for_fg) attron(A_BOLD);
  attron(COLOR_PAIR(FOREGROUND));
  for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
    mbstate_t state;
    memset(&state, 0, sizeof(mbstate_t));
    const char *iter_row = glyph[i];
    int iter_col = 0;

    while (*iter_row) {
      wchar_t wc;
      size_t len = mbrtowc(&wc, iter_row, MB_CUR_MAX, &state);

      if (*iter_row == ' '){
        iter_row += len;
        iter_col++;
        continue;
      }

      if(fill){
        mvaddch(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (COL-FG_GLYPH_LENGTH)/2 + iter_col + fg_offset_x, fill);
        iter_row += len;
        iter_col++;
      } else {
        cchar_t cchar;
        setcchar(&cchar, &wc, 0, 0, NULL);
        mvadd_wch(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (COL-FG_GLYPH_LENGTH)/2 + iter_col + fg_offset_x, &cchar);
        iter_row += len;
        iter_col++;
      }
    }
  }
  refresh();
  attroff(COLOR_PAIR(FOREGROUND));
  attroff(A_BOLD);
}

void _fg(const char * glyph[]){
  attron(COLOR_PAIR(FOREGROUND));
  if(use_bold_color_for_fg) attron(A_BOLD);
  for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
    mvprintw(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (COL-FG_GLYPH_LENGTH)/2 + fg_offset_x, "%s", glyph[i]);
  }
  refresh();
  attroff(COLOR_PAIR(FOREGROUND));
  attroff(COLOR_PAIR(A_BOLD));
}

// TODO: add wipe option for pixel_fill
void _hd(){

  if(hd[0] != 0){

    if((WIN_SIZE == BIG    && highlight_hd_in_full_mode)  ||
       (WIN_SIZE == NORMAL && highlight_hd_in_small_mode)) {
      if(use_bold_color_for_fg){
        attron(A_STANDOUT);
        attron(A_BOLD);
        attron(COLOR_PAIR(FOREGROUND + 16)); // .. sometimes I even amaze myself
      } else {
        attron(COLOR_PAIR(FOREGROUND + 8));
      }
    } else {
      if(use_bold_color_for_fg) attron(A_BOLD);
      attron(COLOR_PAIR(FOREGROUND));
    }

    if(WIN_SIZE == BIG){
      mvprintw(ROW/2 + FG_GLYPH_HEIGHT/2 + hd_offset_y, (COL - FG_GLYPH_HEIGHT)/2 + hd_offset_x, hd);
    } else {
      mvprintw(ROW/2 + FG_GLYPH_HEIGHT/2 + hd_offset_y_min, (COL - FG_GLYPH_HEIGHT)/2 + hd_offset_x_min, hd);
    }

    refresh();
    attroff(A_BOLD);
    attroff(A_STANDOUT);
    attroff(COLOR_PAIR(FOREGROUND));
    attroff(COLOR_PAIR(FOREGROUND + 8));
    attroff(COLOR_PAIR(FOREGROUND + 16));
  }
}

void _down_wipes(){

  int wipe_time = 12000; // TODO: make this configurable

  clear();
  refresh();
  if (WIN_SIZE == NORMAL){
    attron(COLOR_PAIR(FOREGROUND));
    if(use_bold_color_for_fg) attron(A_BOLD);
    for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
      mvprintw(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (COL-FG_GLYPH_LENGTH)/2 + fg_offset_x, "%s", fg[i]);
      check_char();
      if(HOLD_CHAR) mvprintw(ROW/2, COL/2, "%c", HOLD_CHAR);
      refresh();
      usleep(wipe_time);
    }
    attroff(A_BOLD);
    attroff(COLOR_PAIR(FOREGROUND));
    _hd();

  } else if (WIN_SIZE == BIG){

    if(use_bold_color_for_bg) attron(A_BOLD);
    attron(COLOR_PAIR(BACKGROUND));
    for(int i = 0; i < BG_GLYPH_HEIGHT; i++){
      mvprintw(ROW/2 - BG_GLYPH_HEIGHT/2 + bg_offset_y + i, (COL-BG_GLYPH_LENGTH)/2 + bg_offset_x, "%s", bg[i]);
      if(HOLD_CHAR) mvprintw(ROW/2, COL/2, "%c", HOLD_CHAR);
      refresh();
      usleep(wipe_time);
    }
    attroff(A_BOLD);
    attroff(COLOR_PAIR(BACKGROUND));

    if(use_bold_color_for_fg) attron(A_BOLD);
    attron(COLOR_PAIR(FOREGROUND));
    for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
      mbstate_t state;
      memset(&state, 0, sizeof(mbstate_t));
      const char *iter_row = fg[i];
      int iter_col = 0;

      while (*iter_row) {
        wchar_t wc;
        size_t len = mbrtowc(&wc, iter_row, MB_CUR_MAX, &state);

        if (*iter_row == ' '){
          iter_row += len;
          iter_col++;
          continue;
        }

        cchar_t cchar;
        setcchar(&cchar, &wc, 0, 0, NULL);
        mvadd_wch(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (COL-FG_GLYPH_LENGTH)/2 + iter_col + fg_offset_x, &cchar);
        iter_row += len;
        iter_col++;
      }
      refresh();
      usleep(wipe_time);
    }
  }
  attroff(A_BOLD);
  attroff(COLOR_PAIR(FOREGROUND));
}
