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
#include "glyphs.h"
#include "utils.h"
#include "sara.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void glitch(int numFrames, int full){

  quickprint(FOREGROUND, BACKGROUND, 1);
  CACHE = ROW + COL;

  int rng_row, rng_shift, rng_backdrop = 0;
  int rng_backdrop_rng = 2; if(IM_SET) rng_backdrop_rng++;

  for( int i = 0 ; i < numFrames; i++ ) {
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

      if(full == 1){

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

  quickprint(FOREGROUND, BACKGROUND, 0);
}

// TODO: fg glyph not in correct position when im not defined
//       and WIN_SIZE == normal, but only when the width is constrained
//       to a certain size as well? huh. Just check the prints between im[-]
//       and fg
void neon(){

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
        print_fg(im);
      } else if (WIN_SIZE == NORMAL && !IM_SET){
        print_overlay(fg, '+');
      } else {
        print_bg();
      }

      // TODO: create colorbar options
      if (WIN_SIZE == BIG) {
        for(int i = 1; i < 9; i++){
          attron(COLOR_PAIR(i));
          mvaddwstr(ROW/2 + 4, (COL-GLYPH_LENGTH)/2 + 3 + (i*3), L"\u2588\u2588\u2588"); // Unicode full block █
          attroff(COLOR_PAIR(i));
          refresh();
        }
      }
      first_frame = 1;
    }

    if(elapsed_time > 0.1 && second_frame == 0){
      if (WIN_SIZE == NORMAL){
        print_fg(fg);
      } else {
        if (IM_SET){
          print_overlay(im, 0);
        } else {
          print_overlay(fg, '-');
        }
      }
      second_frame = 1;
    }

    if(elapsed_time > 0.2 && third_frame == 0){
      if (WIN_SIZE == NORMAL && hd[0] != 0){
        print_header();
      } else {
        print_overlay(fg, 0);
      }
      third_frame = 1;
    }

    check_char();
    if (HOLD_CHAR != '\0') mvprintw(ROW/2, COL/2, "%c", HOLD_CHAR);
  }

  quickprint(FOREGROUND, BACKGROUND, 0);
}

void neon_reverse(){

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
          print_fg(im);
        } else {
          print_overlay(fg, '-');
        }
      } else {

        if(IM_SET){
          print_bg();
          print_overlay(im, 0);
        } else {
          print_overlay(fg, '-');
        }
      }
      first_frame = 1;
    }

    if(elapsed_time > 0.1 && second_frame == 0){
      clear();
      if (WIN_SIZE == NORMAL){
        print_overlay(fg, '-');
      } else {
        print_bg();
      }
      second_frame = 1;
    }

    check_char();
    if (HOLD_CHAR != '\0') mvprintw(ROW/2, COL/2, "%c", HOLD_CHAR);
  }
}

void shutter_slide(){
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


void pixel_fill(int usleep_time){

  clear();
  refresh();
  double elapsed_time = 0;

  // 0 == !'█'
  // 1 == '█'
  // 2 == '█' and successfully rolled to print to screen
  int fg_arr[FG_GLYPH_HEIGHT][FG_GLYPH_LENGTH];
  int bg_arr[BG_GLYPH_HEIGHT][BG_GLYPH_LENGTH];
  memset(fg_arr, 0, sizeof(fg_arr));
  memset(bg_arr, 0, sizeof(bg_arr));

  int fg_total = 0;
  int bg_total = 0;

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
        fg_total++;
      }

      iter_row += len;                          // Increment the pointer one character
      iter_col++;                               // Increment col
    }
  }

  print_bg();

  clock_t cycle_start = clock();
  int count = 0;
  int round_fill_max = 1;
  int round_fill_count = 0;
  int break_flag = 0;
  while(count <= fg_total - 4){

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
          mvadd_wch(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (COL-FG_GLYPH_LENGTH)/2 + fg_offset_x + j, &cchar);
          attroff(COLOR_PAIR(FOREGROUND));
          if (round_fill_count == round_fill_max){
            round_fill_count = 0;
//          break_flag = 1; // break flag code will print line-by-line with pixel_fill(2000);
            break;
          }
        }

        iter_row += len;
        j++;
      }
//    if(break_flag){
//      break_flag = 0;
//      break;
//    }
    }

    refresh();
    check_char();
    if (HOLD_CHAR) mvprintw(ROW/2, COL/2, "%c", HOLD_CHAR);
    usleep(usleep_time);
    elapsed_time = (double)(clock() - cycle_start) / CLOCKS_PER_SEC;

  }

  print_overlay(fg, 0);
  print_header();
}

void tv_static(double cycle_length){

  clear();
  refresh();

  double elapsed_time = 0;

  int arr[BIG_GLYPH_HEIGHT][BIG_GLYPH_LENGTH];

  for(int i = 0; i < BIG_GLYPH_HEIGHT; i++){

    mbstate_t state;
    memset(&state, 0, sizeof(mbstate_t));
    const char *iter_row = archsarafull[i];
    int iter_col = 0;
    while (*iter_row) {

      wchar_t wc;
      size_t len = mbrtowc(&wc, iter_row, MB_CUR_MAX, &state);

      if(is_char_in_search(wc, FG_STR)){
        arr[i][iter_col] = 2;
      } else if (is_char_in_search(wc, BG_STR)){
        arr[i][iter_col] = 1;
      } else if (!is_char_in_search(wc, L" ")) {
        arr[i][iter_col] = 3;
      } else {
        arr[i][iter_col] = 0;
      }

      iter_row += len;
      iter_col++;
    }
  }

  clock_t cycle_start = clock();
  cchar_t cchar;
  int j = 0;
  while(elapsed_time < cycle_length){

    getmaxyx(stdscr, ROW, COL);
    if (CACHE != ROW + COL) return;

    for(int i = 0; i < BIG_GLYPH_HEIGHT; i++){

      mbstate_t state;
      memset(&state, 0, sizeof(mbstate_t));
      const char *iter_row = archsarafull[i];
      j = 0;
      int roll_result;
      while (*iter_row) {

        wchar_t wc;
        size_t len = mbrtowc(&wc, iter_row, MB_CUR_MAX, &state);

        roll_result = roll(2);

        if(roll_result == 1){
          iter_row += len;
          j++;
          continue;
        }

        int idx = arr[i][j];

        if (idx == 1 || idx == 2 || idx ==  3){
          arr[i][j] = idx + 3;
          mvaddch(ROW/2 - 9 + i, (COL-BIG_GLYPH_LENGTH)/2 + j, ' ');

        } else if (idx == 4){

          arr[i][j] = idx - 3;
          attron(COLOR_PAIR(BACKGROUND));
          setcchar(&cchar, &wc, 0, 0, NULL);
          mvadd_wch(ROW/2 - 9 + i, (COL-BIG_GLYPH_LENGTH)/2 + j, &cchar);
          attroff(COLOR_PAIR(BACKGROUND));

        } else if (idx == 5){

          arr[i][j] = idx - 3;
          attron(COLOR_PAIR(FOREGROUND));
          setcchar(&cchar, &wc, 0, 0, NULL);
          mvadd_wch(ROW/2 - 9 + i, (COL-BIG_GLYPH_LENGTH)/2 + j, &cchar);
          attroff(COLOR_PAIR(FOREGROUND));

        } else if (idx == 6) {

          arr[i][j] = idx - 3;
          attron(COLOR_PAIR(FOREGROUND + 8));
          setcchar(&cchar, &wc, 0, 0, NULL);
          mvadd_wch(ROW/2 - 9 + i, (COL-BIG_GLYPH_LENGTH)/2 + j, &cchar);
          attroff(COLOR_PAIR(FOREGROUND + 8));
        }

        iter_row += len;
        j++;
      }
    }
    usleep(1000);
    refresh();
    elapsed_time = (double)(clock() - cycle_start) / CLOCKS_PER_SEC;
  }
}

// TODO: remove globals as parameters
void quickprint(int fg_color, int bg_color, int printColorbar){
  clear();
  if(dynamic_resize && WIN_SIZE == NORMAL){
    print_fg(fg);
    print_header();
  } else if (WIN_SIZE == BIG){
    print_bg();
    print_overlay(fg, 0);
    print_header();

    // colorbar TODO: abstract this to separate function and add config.h options to it
    if (printColorbar){
      for(int i = 1; i < 9; i++){
        attron(COLOR_PAIR(i));
        mvaddwstr(ROW/2 + 5, (COL-FG_GLYPH_LENGTH)/2 + 15 + (i*3), L"\u2588\u2588\u2588"); // Unicode full block █
        attroff(COLOR_PAIR(i));
      }
    }

    attroff(COLOR_PAIR(fg_color + 8));
    attroff(COLOR_PAIR(fg_color));
    attroff(COLOR_PAIR(FOREGROUND));
    attroff(A_BOLD);
    attroff(A_STANDOUT);
  }
}

void print_bg() {
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

void print_overlay(const char * glyph[], char fill){
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

void print_fg(const char * glyph[]){
  attron(COLOR_PAIR(FOREGROUND));
  if(use_bold_color_for_fg) attron(A_BOLD);
  for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
    mvprintw(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (COL-FG_GLYPH_LENGTH)/2 + fg_offset_x, "%s", glyph[i]);
  }
  refresh();
  attroff(COLOR_PAIR(FOREGROUND));
  attroff(COLOR_PAIR(A_BOLD));
}

void print_header(){

  if(hd[0] != 0){

    if(WIN_SIZE == BIG    && highlight_hd_in_full_mode  ||
       WIN_SIZE == NORMAL && highlight_hd_in_small_mode) {
      if(use_bold_color_for_fg){
        attron(A_STANDOUT);
        attron(A_BOLD);
        attron(COLOR_PAIR(FOREGROUND + 16)); // .. sometimes I even amaze myself
      } else {
        attron(COLOR_PAIR(FOREGROUND + 8));
      }
    } else {
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

void mega_glitch(int duration){

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

        if(is_char_in_search(wc, BG_STR)){
          attron(A_BOLD);
          attron(COLOR_PAIR(BACKGROUND));
        } else {
          attron(COLOR_PAIR(FOREGROUND));
        }

        // Write wide char to `cchar` for mvadd_wch()
        cchar_t cchar;
        setcchar(&cchar, &wc, 0, 0, NULL);
        mvadd_wch(ROW/2 + 3 - i, (COL-GLYPH_LENGTH)/2 + iter_col, &cchar);

        attroff(COLOR_PAIR(FOREGROUND));
        attroff(COLOR_PAIR(BACKGROUND));
        attroff(A_BOLD);
        iter_row += len;                          // Increment the pointer one character
        iter_col++;                               // Increment col
      }
        usleep(20000);
        refresh();
    }
  }
}
