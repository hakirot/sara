/*
--   █████╗ ███╗   ██╗██╗███╗   ███╗ █████╗ ████████╗██╗ ██████╗ ███╗   ██╗███████╗  --
--  ██╔══██╗████╗  ██║██║████╗ ████║██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║██╔════╝  --
--  ███████║██╔██╗ ██║██║██╔████╔██║███████║   ██║   ██║██║   ██║██╔██╗ ██║███████╗  --
--  ██╔══██║██║╚██╗██║██║██║╚██╔╝██║██╔══██║   ██║   ██║██║   ██║██║╚██╗██║╚════██║  --
--  ██║  ██║██║ ╚████║██║██║ ╚═╝ ██║██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║███████║  --
--  ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝╚═╝     ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝.c--
                                                                                       */
#include "globals.h"
#include "glyphs.h"
#include "utils.h"
#include "sara.h"
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

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

      if(is_char_in_search(wc, SARA_BLOCK)){
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

        if (arr[i][j] == 1 && roll(num_sides) == 1){
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
  return;
}
