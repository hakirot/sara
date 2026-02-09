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
#include <ncurses.h>
#include <unistd.h>

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
//      mvprintw(ROW/2 - 2 + i, 0 + (frame_length*j), "%s", title[i]);
        mvprintw(ROW/2 - 2 + i, l_idx + (frame_travel_width * j), "%s", title[i]);
      } else {
//      mvprintw(ROW/2 - 2 + i, COL - NORMAL_GLYPH_LENGTH - (frame_length*j), "%s", title[i]);
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
//  usleep(100000);
    j++;
  }
  attroff(COLOR_PAIR(FOREGROUND));
  return;
};
