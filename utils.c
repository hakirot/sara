/*
--  ██╗   ██╗████████╗██╗██╗     ███████╗  --
--  ██║   ██║╚══██╔══╝██║██║     ██╔════╝  --
--  ██║   ██║   ██║   ██║██║     ███████╗  --
--  ██║   ██║   ██║   ██║██║     ╚════██║  --
--  ╚██████╔╝   ██║   ██║███████╗███████║  --
--   ╚═════╝    ╚═╝   ╚═╝╚══════╝╚══════╝.h--
                                               */
#include "globals.h"
#include "sara.h"
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <wait.h>

void error(char * err) {
  endwin();
  printf("%s\n", err);
  exit(1);
}

int is_char_in_search(wchar_t wc, wchar_t * search_str) {

//  Iterate through the wide-character array
    for (size_t i = 0; i < wcslen(search_str); i++) {
        if (wc == search_str[i]) {
            return 1; // Character found
        }
    }
    return 0; // not found
}

void ensure_cache_dir(){
  const char *env_home = getenv("HOME");
  const char *sara_wd = "/.cache/sara";
  char cache_dir[256] = {'\0'};
  sprintf(cache_dir, "%s%s", env_home, sara_wd);

  // Check if dir exists
  DIR* dir = opendir(cache_dir);
  if (dir) {
    closedir(dir);
  } else if (ENOENT == errno) {
    char dir_name[256] = {'\0'};
    sprintf(dir_name, "%s%s", env_home, "/.cache/sara");
    int status = mkdir(dir_name, 0755);

    if (status != 0) {
      char error_str[256] = {'\0'};
      sprintf(error_str, "%s%s" , "Error creating directory ", dir_name);
      error(error_str);
    }
  }
}

void ensure_path_perm(char * file_path, char perm){

  if(perm == 'w'){
    // open file
    FILE *writeptr = fopen(file_path, "w");

    // is it writable?
    if(writeptr){
      fclose(writeptr);
      return;
    } else if (errno == 13){
      // test if pw file exists
      char * pw_path = "/home/hakirot/.config/pw.gpg";
      FILE * pw_file = fopen(pw_path, "r");
      if (ENOENT == errno){
        int result = generate_pw_file();
      } else {
        fclose(pw_file);
      }

      // fork
      pid_t pid = fork();
      if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
      } else if (pid == 0) {
        endwin();

        // setenv SUDO_ASKPASS
				if (setenv("SUDO_ASKPASS", "/home/hakirot/skps/secret.sh", 1) != 0) {
          error("setenv error");
				}

        execlp("sudo", "sudo", "--askpass", "chmod", "666", file_path, NULL);
        error("ERROR: execv sudo");

      } else {
        endwin();
        int status;

        while(kill(pid, 0) == 0){
          waitpid(pid, &status, 0);
        }
      }

    } else {
      char error_str[32] = {'\0'};
      sprintf(error_str, "A new errno: %d", errno);
      error(error_str);
    }
  }
}

void print_clear_terminal(){
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

void init_window(){
  setlocale(LC_ALL, "");    // Allow special characters, initscr()
  initscr();                // Initialize screen
  start_color();            // Must be called right after initscr()
  use_default_colors();
  init_pair(BLACK, COLOR_BLACK, -1); // Foreground black, no background
  init_pair(RED, COLOR_RED, -1);
  init_pair(GREEN, COLOR_GREEN, -1);
  init_pair(YELLOW, COLOR_YELLOW, -1);
  init_pair(BLUE, COLOR_BLUE, -1);
  init_pair(MAGENTA, COLOR_MAGENTA, -1);
  init_pair(CYAN, COLOR_CYAN, -1);
  init_pair(WHITE, COLOR_WHITE, -1);

  init_pair(WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
  init_pair(BLACK_RED, COLOR_BLACK, COLOR_RED);
  init_pair(BLACK_GREEN, COLOR_BLACK, COLOR_GREEN);
  init_pair(BLACK_YELLOW, COLOR_BLACK, COLOR_YELLOW);
  init_pair(BLACK_BLUE, COLOR_BLACK, COLOR_BLUE);
  init_pair(BLACK_MAGENTA, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(BLACK_CYAN, COLOR_BLACK, COLOR_CYAN);
  init_pair(BLACK_WHITE, COLOR_BLACK, COLOR_WHITE);

  init_pair(WHITE_BLACKD, COLOR_WHITE, COLOR_BLACK);
  init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK);
  init_pair(GREEN_BLACK, COLOR_GREEN, COLOR_BLACK);
  init_pair(YELLOW_BLACK, COLOR_YELLOW, COLOR_BLACK);
  init_pair(BLUE_BLACK, COLOR_BLUE, COLOR_BLACK);
  init_pair(MAGENTA_BLACK, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(CYAN_BLACK, COLOR_CYAN, COLOR_BLACK);
  init_pair(BLACK_WHITED, COLOR_WHITE, COLOR_BLACK);

  cbreak();                 // Disable line buffering
  noecho();                 // Don't display keyboard presses in window
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);     // Enable reading of F1/2, arrow keys, etc
  curs_set(FALSE);          // No cursor
  clear();
  refresh();                // clear screen
}

void get_helped() {
  printf("Usage: %s [OPTIONS]\n", "sara");
  printf("  --help, -h    Get helped\n");
  printf("  -c            Constant effects\n");
  printf("  -G            Constant glitch effect\n");
  printf("  -M            Constant MEGA glitch effect\n");
  printf("  -H            Holographic background\n");
  printf("  -f [color]    set custom FOREGROUND color\n");
  printf("  -b [color]    set custom BACKGROUND color\n");
  printf("  -F [follow]   On exit, Writes PWD to file\n");
  exit(0);
}

int roll(int num_sides){
  int random_roll = rand() % num_sides;    // RNG 2 and 7
  if(random_roll == 0){
    return 1;
  }
  return 0;
}
