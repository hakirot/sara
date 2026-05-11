/*
--  ██╗   ██╗████████╗██╗██╗     ███████╗  --
--  ██║   ██║╚══██╔══╝██║██║     ██╔════╝  --
--  ██║   ██║   ██║   ██║██║     ███████╗  --
--  ██║   ██║   ██║   ██║██║     ╚════██║  --
--  ╚██████╔╝   ██║   ██║███████╗███████║  --
--   ╚═════╝    ╚═╝   ╚═╝╚══════╝╚══════╝.h--
                                               */

#include "utils.h"
#include "globals.h"
#include "config.h"
#include "sara.h"
#include "animations.h"
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
#include <string.h>
#include <fcntl.h>

int __key__(){

  int valid_input = 0;
  char input = getch();

  if (input != ERR && input != '\n' && input != EOF && input > 31 && input < 127) {
    valid_input = 1;

    if(strchr(global_chars, input)){
      if(strchr(commandkeys_chars, input)){
        __command__(input);
      } else if(strchr(builtins_chars, input)){
        __builtin__(input);
      } else if(strchr(menukeys_chars, input)){
        __menu__(input);
      } else {
        crit("Something is broken :[");
      }

    } else if(WIN_SIZE != SMALL){
      LAST_INPUT_TIME = clock();
      HOLD_CHAR = input;
      mvprintw(ROW/2, COL/2, "%c", HOLD_CHAR);
      refresh();
    }
  }

  double time_since_input = (double)(clock() - LAST_INPUT_TIME)
                            / CLOCKS_PER_SEC;

  if(time_since_input > HOLD_CHAR_TIME && WIN_SIZE != SMALL){
    HOLD_CHAR = '\0';
  }

  return valid_input;
}

void __command__(char input){

  const Command* command = NULL;
  for(int i = 0; i < commandkeys_len; i++){
    if(commandkeys[i].smashkey == input){
      command = &commandkeys[i];
      break;
    }
  }

  animate(command->pre_animation);
  endwin();

  if(strcmp("ranger", ((char **)command->cmd)[0]) == 0){
    _ranger(command);
    return;
  }

  if(command->option == WAIT || command->option == WAIT_ON_ERR || command->option == WAIT_NO_OUT){

    CACHE = ROW + COL;
    endwin();
    clear();
    pid_t pid = fork();
    if (pid < 0) {
      perror("fork");
      exit(EXIT_FAILURE);

    } else if (pid == 0) {
//    print_clear_terminal();
      if(command->option == WAIT_NO_OUT){
        int fd = open("/dev/null", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        dup2(fd, 1);
        dup2(fd, 2);

        close(fd);
      }

      execvp(((char **)command->cmd)[0], (char **)command->cmd);
      crit("ERROR: execlp __command__");
    } else {

      int status;

      while(kill(pid, 0) == 0){
        waitpid(pid, &status, 0);
      }

      if(command->option == WAIT_ON_ERR && status != 0){
        printf("Press enter to continue");
        getchar();
        fflush(stdin);
      }
      getmaxyx(stdscr, ROW, COL);
      if(CACHE != ROW + COL) return;
    }

  } else if (command->option == EXEC || command->option == EXEC_NO_OUT) {

    if(command->option == EXEC_NO_OUT){
      int fd = open("/dev/null", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
      dup2(fd, 1);
      dup2(fd, 2);
      close(fd);
    }


    endwin();
    execvp(((char **)command->cmd)[0], (char **)command->cmd);
    crit("ERROR: execlp __command__");
  }

  animate(command->post_animation);
}

void __builtin__(char input){

  internal selection = quit;
  for(int i = 0; i < commandkeys_len; i++){
    if(builtins[i].smashkey == input){
      selection = builtins[i].option;
      break;
    }
  }

  if(selection == quit){
    _quit();
  } else if (selection == pshd){
    _pshd();
  } else if (selection == rave){
    _toggle_rave();
  } else if (selection == invert_colors){
    _invert_colors();
  } else if (selection == randomize_colors){
    _randomize_colors();
  }
}

void __menu__(char input){

}

void load_command_config(){
  memset(global_chars, '\0', KEY_ARRAY_SIZE * sizeof(char));
  memset(commandkeys_chars, '\0', KEY_ARRAY_SIZE * sizeof(char));
  memset(builtins_chars, '\0', KEY_ARRAY_SIZE * sizeof(char));
  memset(menukeys_chars, '\0', KEY_ARRAY_SIZE * sizeof(char));

  commandkeys_len = sizeof(commandkeys)/sizeof(commandkeys[0]);
  builtins_len = sizeof(builtins)/sizeof(builtins[0]);
  menukeys_len = sizeof(menukeys)/sizeof(menukeys[0]);

  int global_idx = 0;
  for(int i = 0; i < commandkeys_len; i++){
    global_chars[global_idx] = commandkeys[i].smashkey;
    commandkeys_chars[i] = commandkeys[i].smashkey;
    global_idx++;
  }

  for(int i = 0; i < builtins_len; i++){
    global_chars[global_idx] = builtins[i].smashkey;
    builtins_chars[i] = builtins[i].smashkey;
    global_idx++;
  }

  for(int i = 0; i < menukeys_len; i++){
    global_chars[global_idx] = menukeys[i].smashkey;
    menukeys_chars[i] = menukeys[i].smashkey;
    global_idx++;
  }
}

void _ranger(const Command * command){

}

// TODO: PREFLIGHT CHECK
void preflight_check() {

  int commandkeys_len = sizeof(commandkeys)/sizeof(commandkeys[0]);
  int builtins_len = sizeof(builtins)/sizeof(builtins[0]);
  int menukeys_len = sizeof(menukeys)/sizeof(menukeys[0]);
  int total = commandkeys_len + builtins_len + menukeys_len;

  if(total > 128){
    char err[256];
    sprintf(err, "ERROR: Configured Key Limit exceeded\nKey Limit is %d\n commandkeys: %d\n builtins: %d\n menukeys: %d\n",
      KEY_ARRAY_SIZE,
      commandkeys_len,
      builtins_len,
      menukeys_len);
    crit("Key limit exceeded");
  }

  int global_chars_len = strlen(global_chars);
  for(int idx = 0; idx < global_chars_len; idx++){
    for(int j = idx + 1; j < global_chars_len; j++){
      if(global_chars[idx] == global_chars[j]){
        char err[64];
        sprintf(err, "WARNING: Duplicate char: %c\n", global_chars[idx]);
        crit(err);
      }
    }
  }

  for(int i = 0; i < BG_GLYPH_HEIGHT; i++){
    if((mbstowcs(NULL, bg[i], 0)) != (ulong)BG_GLYPH_LENGTH){
      crit("bad bg dimensions");
    }
  }

  for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
    if((mbstowcs(NULL, fg[i], 0)) != (ulong)FG_GLYPH_LENGTH){
      crit("bad fg dimensions");
    }
  }

  // ensure all command menus terminate with commands
  // ensure all commands are installed
  // ensure use_fg_c_for_hd_as_bg and use_bg_c_for_hd_as_bg are both not true
}

// TODO: implement
void warning(char * err) {
  clear();
  mvprintw(ROW/2, COL/2, "%s", err);
}

void crit(char * err) {
  endwin();
  printf("%s\n", err);
  exit(1);
}

int is_char_in_search(wchar_t wc, const wchar_t * search_str) {

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
      crit(error_str);
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
          crit("setenv error");
				}

        execlp("sudo", "sudo", "--askpass", "chmod", "666", file_path, NULL);
        crit("ERROR: execv sudo");

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
      crit(error_str);
    }
  }
}

void print_clear_terminal(){
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

void special_chars(){
  setlocale(LC_ALL, "");    // wide character support
}

void launch_window(){
  initscr();                // Initialize screen
  start_color();            // Must be called right after initscr()
  use_default_colors();
  init_pair(black, COLOR_BLACK, -1); // Foreground black, no background
  init_pair(red, COLOR_RED, -1);
  init_pair(green, COLOR_GREEN, -1);
  init_pair(yellow, COLOR_YELLOW, -1);
  init_pair(blue, COLOR_BLUE, -1);
  init_pair(magenta, COLOR_MAGENTA, -1);
  init_pair(cyan, COLOR_CYAN, -1);
  init_pair(white, COLOR_WHITE, -1);

  init_pair(white_black, COLOR_WHITE, COLOR_BLACK);
  init_pair(black_red, COLOR_BLACK, COLOR_RED);
  init_pair(black_green, COLOR_BLACK, COLOR_GREEN);
  init_pair(black_yellow, COLOR_BLACK, COLOR_YELLOW);
  init_pair(black_blue, COLOR_BLACK, COLOR_BLUE);
  init_pair(black_magenta, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(black_cyan, COLOR_BLACK, COLOR_CYAN);
  init_pair(black_white, COLOR_BLACK, COLOR_WHITE);

  init_pair(white_blackd, COLOR_WHITE, COLOR_BLACK);
  init_pair(red_black, COLOR_RED, COLOR_BLACK);
  init_pair(green_black, COLOR_GREEN, COLOR_BLACK);
  init_pair(yellow_black, COLOR_YELLOW, COLOR_BLACK);
  init_pair(blue_black, COLOR_BLUE, COLOR_BLACK);
  init_pair(magenta_black, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(cyan_black, COLOR_CYAN, COLOR_BLACK);
  init_pair(black_whited, COLOR_WHITE, COLOR_BLACK);

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
//printf("  -c            Constant effects\n");
  printf("  -G            Constant glitch effect\n");
//printf("  -M            Constant MEGA glitch effect\n");
  printf("  -H            Holographic background\n");
  printf("  -f [color]    set custom FOREGROUND color\n");
  printf("  -b [color]    set custom BACKGROUND color\n");
  printf("  -F [follow]   On exit, Writes PWD to file\n");
  printf("  -r            Generate a random color profile. Not compatible with -f or -b options\n");
  exit(0);
}

int roll(int sides){
  return rand() % sides + 1;
}

void _toggle_rave(){
  if(HOLOGRAPHIC == 0){
    HOLOGRAPHIC = 1;
  } else {
    HOLOGRAPHIC = 0;
  }
}

void _randomize_colors(){
  BACKGROUND = rand() % 7 + 1;    // RNG 1 and 7
  FOREGROUND = rand() % 7 + 2;    // RNG 2 and 8
  while(FOREGROUND == BACKGROUND){
    FOREGROUND = rand() % 7 + 2;
  }
  animate(none);
}

void _invert_colors(){
  int temp = BACKGROUND;
  BACKGROUND = FOREGROUND;
  FOREGROUND = temp;
  animate(none);
}

void _quit(){
  if (FOLLOW){
    ensure_cache_dir();
    char cache_file[256] = {'\0'};
    char * env_home = getenv("HOME");
    char * env_pwd = getenv("PWD");
    sprintf(cache_file, "%s%s", env_home, "/.cache/sara/saraexit");
    FILE * fp = fopen(cache_file, "w");
    fprintf(fp, "%s", env_pwd);
    fclose(fp);
  }

  clear();
  refresh();
  move(0, 0);
  endwin();
  exit(0);
}

void set_glyph_dimensions(){
  BG_GLYPH_HEIGHT = sizeof(bg)/sizeof(bg[0]);
  FG_GLYPH_HEIGHT = sizeof(fg)/sizeof(fg[0]);
  BG_GLYPH_LENGTH = mbstowcs(NULL, bg[0], 0);
  FG_GLYPH_LENGTH = mbstowcs(NULL, fg[0], 0);
  if(im[0] == NULL) IM_SET = false;
}
