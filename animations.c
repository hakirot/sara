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
    rng_shift = (rand() % 3) - 1;       // RNG -1 and 1
    rng_backdrop = rand() % rng_backdrop_rng;          // RNG 0 and 2

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
        int rng_shift = (rand() % 2) - 1;       // RNG -1 and 1

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

void neon(){

  double cycle_length = 0.3;
  double elapsed_time = 0;
  clock_t cycle_start = clock();

  int first_frame = 0;
  int second_frame = 0;
  int third_frame = 0;
  int colorbar_printed = false;

  clear();
  refresh();

//char err[32];
//sprintf(err, "%d", IM_SET);
//error(err);

  while(cycle_length > elapsed_time){

    elapsed_time = (double)(clock() - cycle_start) / CLOCKS_PER_SEC;

    if(elapsed_time > 0.05 && first_frame == 0){
      if (WIN_SIZE == NORMAL && IM_SET){
        print_fg(im);
      } else if (WIN_SIZE == NORMAL && !IM_SET){
        print_overlay(fg, '+');
      } else { // screen is BIG
        print_bg();
      }

      if (colorbar_printed == false ){
        if (WIN_SIZE == BIG) {
          for(int i = 1; i < 9; i++){
            attron(COLOR_PAIR(i));
            mvaddwstr(ROW/2 + 4, (COL-GLYPH_LENGTH)/2 + 3 + (i*3), L"\u2588\u2588\u2588"); // Unicode full block █
            attroff(COLOR_PAIR(i));
          }
        }
        colorbar_printed = true;
      }

      first_frame = 1;
    }

    if(elapsed_time > 0.1 && second_frame == 0){
      if (WIN_SIZE == NORMAL){
        attron(COLOR_PAIR(FOREGROUND));
        if(use_bold_color_for_fg) attron(A_BOLD);
        for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
          mvprintw(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (COL-FG_GLYPH_LENGTH)/2, "%s", fg[i]);
        }
        attroff(A_BOLD);
        attroff(COLOR_PAIR(FOREGROUND));
      } else { // screen is BIG

        if (IM_SET){
          print_overlay(im, 0);
        } else {
          print_overlay(fg, 0);
        }
      }
      second_frame = 1;
    }

    if(elapsed_time > 0.2 && third_frame == 0){
      if (WIN_SIZE == NORMAL && hd[0] != 0){
        if(use_bold_color_for_fg) attron(A_BOLD); // unsure
        attron(COLOR_PAIR(FOREGROUND));
        mvprintw(ROW/2 + FG_GLYPH_HEIGHT/2 + hd_offset_y_min, (COL - FG_GLYPH_HEIGHT)/2 + hd_offset_x_min, hd);
        attroff(COLOR_PAIR(FOREGROUND));
        attroff(A_BOLD);
      } else { // screen is big
        print_overlay(fg, 0);
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

void neon_reverse(){

  clock_t cycle_start = clock();
  double cycle_length = 0.2;
  double elapsed_time = 0;

  int first_frame = 0;
  int second_frame = 0;

//refresh();

  while(cycle_length > elapsed_time){

    elapsed_time = (double)(clock() - cycle_start) / CLOCKS_PER_SEC;

    if(elapsed_time > 0.00 && first_frame == 0){
      if (WIN_SIZE == NORMAL){
        attron(COLOR_PAIR(FOREGROUND));
        if(use_bold_color_for_fg) attron(A_BOLD);
        if(IM_SET){
          for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
            mvprintw(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (COL-FG_GLYPH_LENGTH)/2, "%s", im[i]);
          }
        } else {
          print_overlay(fg, '-');
        }
        attroff(COLOR_PAIR(FOREGROUND));
        attroff(A_BOLD);

      } else { // screen is BIG

        if(IM_SET){
          print_bg();

          // TODO all repeat code
          if(use_bold_color_for_fg) attron(A_BOLD);
          attron(COLOR_PAIR(FOREGROUND));
          for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
            mbstate_t state;
            memset(&state, 0, sizeof(mbstate_t));
            int iter_col = 0; // Track the column position

            const char *iter_row = im[i];
            while (*iter_row) {
              wchar_t wc;
              size_t len = mbrtowc(&wc, iter_row, MB_CUR_MAX, &state); // Convert to wide char

              if (*iter_row == ' '){
                iter_row += len;
                iter_col++;
                continue;
              }

              cchar_t cchar;
              setcchar(&cchar, &wc, 0, 0, NULL);
              mvadd_wch(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (COL-FG_GLYPH_LENGTH)/2 + iter_col, &cchar);
              iter_row += len;
              iter_col++;
            }
          }
          attroff(COLOR_PAIR(FOREGROUND));
          attroff(A_BOLD);
        } else {
          // TODO all repeat code
          for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
            mbstate_t state;
            memset(&state, 0, sizeof(mbstate_t));
            int iter_col = 0; // Track the column position
            const char *iter_row = fg[i];
            while (*iter_row) {
              wchar_t wc;
              size_t len = mbrtowc(&wc, iter_row, MB_CUR_MAX, &state); // Convert to wide char

              if (*iter_row == ' '){
                iter_row += len;
                iter_col++;
                continue;
              }

              mvaddch(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (COL-FG_GLYPH_LENGTH)/2 + iter_col, '-');
              iter_row += len;
              iter_col++;
            }
          }
        }
      }
      refresh();
      first_frame = 1;
    }

    if(elapsed_time > 0.1 && second_frame == 0){
      clear();
      if (WIN_SIZE == NORMAL){
        // TODO all repeat code
        attron(COLOR_PAIR(FOREGROUND));
        for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
          mbstate_t state;
          memset(&state, 0, sizeof(mbstate_t));
          int iter_col = 0; // Track the column position
          const char *iter_row = fg[i];
          while (*iter_row) {
            wchar_t wc;
            size_t len = mbrtowc(&wc, iter_row, MB_CUR_MAX, &state); // Convert to wide char

            if (*iter_row == ' '){
              iter_row += len;
              iter_col++;
              continue;
            }

            mvaddch(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (COL-FG_GLYPH_LENGTH)/2 + iter_col, '-');
            iter_row += len;
            iter_col++;
          }
        }
        attroff(COLOR_PAIR(FOREGROUND));
      } else { // screen is big
        for (int i = 0; i < BIG_GLYPH_HEIGHT; i++){
          if(use_bold_color_for_bg) attron(A_BOLD);
          attron(COLOR_PAIR(BACKGROUND));
          mvprintw(ROW/2 - 9 + i, (COL-GLYPH_LENGTH)/2 - 0, "%s", bg[i]);
          attroff(A_BOLD);
          attroff(COLOR_PAIR(BACKGROUND));
        }
      }
      refresh();
      second_frame = 1;
    }

    check_char();
    if (HOLD_CHAR != '\0') mvprintw(ROW/2, COL/2, "%c", HOLD_CHAR);
//  refresh();
  }
}

void shutter_slide(){
  int num_frames = 8;
  int margin_width = (COL - NORMAL_GLYPH_LENGTH) / 2;
  int left_stop = margin_width;
  int right_stop = COL - margin_width - NORMAL_GLYPH_LENGTH;
  int l_idx = 0;
  int r_idx = COL - NORMAL_GLYPH_LENGTH;
  int frame_travel_width = margin_width / 10;
  int j = 0;
  attron(COLOR_PAIR(FOREGROUND));
  while(j < num_frames){
    clear();
    for(int i = 0; i < NORMAL_GLYPH_HEIGHT; i++){
      if (i % 2 == 0){
        mvprintw(ROW/2 - 2 + i, l_idx + (frame_travel_width * j), "%s", title[i]);
      } else {
        mvprintw(ROW/2 - 2 + i, r_idx - (frame_travel_width * j), "%s", title[i]);
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


void pixel_fill(double cycle_length, int num_sides, int usleep_time){

  clear();
  refresh();
  double elapsed_time = 0;

  // 0 == !'█'
  // 1 == '█'
  // 2 == '█' and successfully rolled to print to screen
  int arr[NORMAL_GLYPH_HEIGHT][NORMAL_GLYPH_LENGTH];
  int total = 0;

  for(int i = 0; i < NORMAL_GLYPH_HEIGHT; i++){

    mbstate_t state;                            // Tracks state of mbrtowc function when converting between types of chars
    memset(&state, 0, sizeof(mbstate_t));
    const char *iter_row = titlefill[i];        // Grabs a line from glyph
    int iter_col = 0;                           // Track the column position
    while (*iter_row) {                         // Iterate through chars in row
      wchar_t wc;                               // Create wide character var
      // Converts character from iter_row to wide char `wc`
      // Also records length of character at *iter_row in len
      size_t len = mbrtowc(&wc, iter_row, MB_CUR_MAX, &state);

      if(is_char_in_search(wc, FG_STR)){
        arr[i][iter_col] = 1;
        total++;
      } else {
        arr[i][iter_col] = 0;
      }

      iter_row += len;                          // Increment the pointer one character
      iter_col++;                               // Increment col
    }
  }

  clock_t cycle_start = clock();
  int count = 0;
  while(elapsed_time < cycle_length){

    getmaxyx(stdscr, ROW, COL);
    if (CACHE != ROW + COL) return;

    for(int i = 0; i < NORMAL_GLYPH_HEIGHT; i++){

      mbstate_t state;
      memset(&state, 0, sizeof(mbstate_t));
      const char *iter_row = titlefill[i];
      int j = 0;
      while (*iter_row) {

        wchar_t wc;
        size_t len = mbrtowc(&wc, iter_row, MB_CUR_MAX, &state);

        if (arr[i][j] == 1 && roll(num_sides) == 2){
          arr[i][j] = 2;
          count++;
          if (count == total){
            quickprint(FOREGROUND, BACKGROUND, 0);
            return;
          }
          cchar_t cchar;
          setcchar(&cchar, &wc, 0, 0, NULL);
          attron(COLOR_PAIR(FOREGROUND));
          mvadd_wch(ROW/2 - 2 + i, (COL-GLYPH_LENGTH)/2 + j, &cchar);
          attroff(COLOR_PAIR(FOREGROUND));
        }

        iter_row += len;
        j++;
      }
    }

    usleep(usleep_time);
    refresh();
    elapsed_time = (double)(clock() - cycle_start) / CLOCKS_PER_SEC;
  }

  // TODO: remove globals as parameters
  quickprint(FOREGROUND, BACKGROUND, 0);
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

      iter_row += len;                          // Increment the pointer one character
      iter_col++;                               // Increment col
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

  // TODO: remove globals as parameters
  quickprint(FOREGROUND, BACKGROUND, 0);
}

void quickprint(int fg_color, int bg_color, int printColorbar){
  clear();
  if(dynamic_resize && WIN_SIZE == NORMAL){
    attron(COLOR_PAIR(fg_color));
    if(use_bold_color_for_fg) attron(A_BOLD);
    for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
      mvprintw(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (COL-FG_GLYPH_LENGTH)/2, "%s", fg[i]);
    }
    if(hd[0] != 0){
      attron(COLOR_PAIR(fg_color));
      mvprintw(ROW/2 + FG_GLYPH_HEIGHT/2 + hd_offset_y_min, (COL - FG_GLYPH_HEIGHT)/2 + hd_offset_x_min, hd);
    }
    attroff(A_BOLD);
    attroff(COLOR_PAIR(FOREGROUND));

  } else if (WIN_SIZE == BIG){

    print_bg();
    print_overlay(fg, 0);

    // header
    if(hd[0] != 0){
      if(use_bold_color_for_fg){
        attron(COLOR_PAIR(fg_color + 16));
        attron(A_BOLD);
        attron(A_STANDOUT);
      } else {
        attron(COLOR_PAIR(fg_color + 8));
      }
      mvprintw(ROW/2 + FG_GLYPH_HEIGHT/2 + hd_offset_y, (COL - FG_GLYPH_HEIGHT)/2 + hd_offset_x, hd);
      attroff(A_BOLD);
      attroff(COLOR_PAIR(fg_color));
      attroff(COLOR_PAIR(fg_color + 16));
      attroff(A_STANDOUT);
    }

    // colorbar todo: abstract this to separate function and add config.h options to it
    if (printColorbar){
      for(int i = 1; i < 9; i++){
        attron(COLOR_PAIR(i));
        mvaddwstr(ROW/2 + 5, (COL-GLYPH_LENGTH)/2 + 15 + (i*3), L"\u2588\u2588\u2588"); // Unicode full block █
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
        mvaddch(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (COL-FG_GLYPH_LENGTH)/2 + iter_col, fill);
        iter_row += len;
        iter_col++;
      } else {
        cchar_t cchar;
        setcchar(&cchar, &wc, 0, 0, NULL);
        mvadd_wch(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (COL-FG_GLYPH_LENGTH)/2 + iter_col, &cchar);
        iter_row += len;
        iter_col++;
      }
    }
  }
  attroff(COLOR_PAIR(FOREGROUND));
  attroff(A_BOLD);
}

void print_fg(const char * glyph[]){
  attron(COLOR_PAIR(FOREGROUND));
  if(use_bold_color_for_fg) attron(A_BOLD);
  for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
    mvprintw(ROW/2 - FG_GLYPH_HEIGHT/2 + fg_offset_y + i, (COL-FG_GLYPH_LENGTH)/2, "%s", im[i]);
  }
  attroff(COLOR_PAIR(FOREGROUND));
  attroff(COLOR_PAIR(A_BOLD));
}
