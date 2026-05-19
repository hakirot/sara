/*
--  ██╗   ██╗████████╗██╗██╗     ███████╗  --
--  ██║   ██║╚══██╔══╝██║██║     ██╔════╝  --
--  ██║   ██║   ██║   ██║██║     ███████╗  --
--  ██║   ██║   ██║   ██║██║     ╚════██║  --
--  ╚██████╔╝   ██║   ██║███████╗███████║  --
--   ╚═════╝    ╚═╝   ╚═╝╚══════╝╚══════╝.h--
                                               */

#define NCURSES_WIDECHAR 1

#include "utils.h"
#include "globals.h"
#include "config.h"
#include "sara.h"
#include "animations.h"
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
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
      } else if(strchr(builtinkeys_chars, input)){
        __builtin__(input);
      } else if(strchr(menukeys_chars, input)){
        if(KEY_LOCK == 1) return valid_input;
        KEY_LOCK = 1;
        __topmenu__(input);
        KEY_LOCK = 0;
      } else {
        crit("Something broke :[");
      }

      valid_input = 2;

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

  if(KEY_LOCK == 1) return;
  KEY_LOCK = 1;

  const Command* command = NULL;
  for(int i = 0; i < commandkeys_len; i++){
    if(commandkeys[i].smashkey == input){
      command = &commandkeys[i];
      break;
    }
  }

  __execute__(command);

  KEY_LOCK = 0;

  refresh();
  getmaxyx(stdscr, ROW, COL);
  if(CACHE != ROW + COL) {
    return;
  }

  animate(command->post_animation);
}

int _get_confirm(const Command * command){
  int selection = false;
  _print_confirm_window(((char **)command->cmd)[0]);
  _print_confirm_selection(selection);
  while(1){
    getmaxyx(stdscr, ROW, COL);
    if (CACHE != ROW + COL) break;

    int input = getch();
    if (input != ERR && input != '\n' && input != EOF && input > 103 && input < 109) {
      if (input == 'j' || input == 'k' || input == 'h' || input == 'l'){
        if(selection == true){
          selection = false;
        } else {
          selection = true;
        }
      }
    } else if (input == 'q' || input == 27){
      return false;
    } else if (input == '\n'){
      return selection;
    }

    _print_confirm_selection(selection);
    usleep(2000);
  }
  return false;
}

int __execute__(const Command * command){

  if(command->extra_args.confirmtion == CONFIRM &&
    _get_confirm(command) != true)
  {
    return false;
  }

  int RANGER_FLAG=0;

  if(command->extra_args.chdir != NULL){
    _chdir(command->extra_args.chdir);
  }

  animate(command->pre_animation);
  endwin();
  system("clear");

  if(strcmp("ranger", ((char **)command->cmd)[0]) == 0 &&
    command->option != EXEC)
  {
    command = ranger_command(command);
    RANGER_FLAG=1;
  }
  if(command->option != EXEC){

    endwin();
    clear();
    pid_t pid = fork();
    if (pid < 0) {
      perror("fork");
      exit(EXIT_FAILURE);

    } else if (pid == 0) {
//    print_clear_terminal();
//    if(command->option == WAIT_NO_OUT){
      if(command->extra_args.output_option == NO_OUT){
        int fd = open("/dev/null", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        dup2(fd, 1);
        dup2(fd, 2);

        close(fd);
      }

      execvp(((char **)command->cmd)[0], (char **)command->cmd);
      perror("execvp");
      crit("");
    } else {

      int status;
      if(command->option != NOWAIT){
        while(kill(pid, 0) == 0){
          waitpid(pid, &status, 0);
        }
      }

      if ((command->option == STOP_ON_ERR && status != 0) ||
          STOP == command->option) {
        printf("Press enter to continue");
        getchar();
        fflush(stdin);
      }
    }

  } else {

    if(command->extra_args.output_option == NO_OUT){
      int fd = open("/dev/null", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
      dup2(fd, 1);
      dup2(fd, 2);
      close(fd);
    }

    endwin();
    if(FOLLOW) _write_exit_dir();
    execvp(((char **)command->cmd)[0], (char **)command->cmd);
    perror("execvp");
    crit("");
  }

  if(RANGER_FLAG) _free_range(command);
  return true;
}

void __builtin__(char input){

  internal selection = quit;
  for(int i = 0; i < commandkeys_len; i++){
    if(builtinkeys[i].smashkey == input){
      selection = builtinkeys[i].option;
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
  } else if (selection == check){
    _check();
  } else if (selection == colors){
    if(SHOW_COLORS == false){
      SHOW_COLORS = true;
    } else {
      SHOW_COLORS = false;
    }
    _show_colors();
  }

}

void __topmenu__(char input){

  const MenuKey* menukey = NULL;
  for(int i = 0; i < menukeys_len; i++){
    if(menukeys[i].smashkey == input){
      menukey = &menukeys[i];
      break;
    }
  }

  int dim_y = 0;
  int dim_x = 0;

  if(menu_y < 3){
    dim_y = ROW;
  } else if(ROW < menu_y){
    dim_y = ROW;    
  } else {
    dim_y = menu_y;
  }

  if(COL < menu_x){
    dim_x = COL;
  } else {
    dim_x = menu_x;
  }

  _menuselect(menukey->submenu, dim_y, dim_x);
}

void _menuselect(const Menu * menu, int dim_y, int dim_x){

  Menu * ptr = (Menu *)menu;
  int len = 0;
  while(1){
    if(strcmp("END_OF_MENU", ptr->name) == 0) break;
    ptr++;
    len++;
  }

  if(len < dim_y - 2 || (len < menu_y - 2 && len < ROW - 2)){
    dim_y = len+2;
  }

  _print_menu_borders(dim_y, dim_x);

  int selection = 0;
  while(1){

    getmaxyx(stdscr, ROW, COL);
    if (CACHE != ROW + COL) break;

    // make this block a function
    int input = getch();
    if (input != ERR && input != '\n' && input != EOF && input > 105 && input < 108) {
      if (input == 'j'){
        selection = (selection + 1) % len;
      } else {
        selection = (selection + (len - 1)) % len;
      }
    } else if (input == 'q' || input == 27){
      animate(none);
      return;
    } else if (input == 4){
      // ctrl+d
      selection += (dim_y-2)/2;
      if(selection >= len) selection = len-1;
    } else if (input == 6){
      selection += (dim_y-2);
      if(selection >= len) selection = len-1;
    } else if (input == 21){
      //ctrl+u
      selection -= (dim_y-2)/2;
      if(selection < 0) selection = 0;
    } else if (input == 2){
      selection -= (dim_y-2);
      if(selection < 0) selection = 0;
    } else if (input == 'G'){
      selection = len-1;
    } else if (input == 'g'){
      selection = 0;
    } else if (input == '\n'){
      if(menu[selection].type == SUBMENU){
        _menuselect(menu[selection].next.submenu, dim_y, dim_x);
      } else if(menu[selection].type == COMMAND){
        int result = __execute__(&menu[selection].next.command);
        if(result == true){
          KEY_LOCK = 0;
          animate(menu[selection].next.command.post_animation);
        } else {
          animate(none);
        }
      }
      return;
    }

    _print_menu_selection(menu, selection, len, dim_y, dim_x);
    usleep(2000);
  }
}

void _print_menu_selection(const Menu * menu, int selection, int len, int dim_y, int dim_x){

  _clear_menu(dim_y, dim_x);
  int window_size = dim_y - 2;
  if(len < window_size) window_size = len;

  int offset_y = menu_offset_y;
  int offset_x = menu_offset_x;

  if(dim_y < 3) {
    window_size = ROW;
    offset_y = -1;
    offset_x = 0;
  }

  int i = 0;
  int k = 0;
  if(selection > len - window_size) i = len - window_size;
  else i = selection;

  if(bold_color_menu == true){
    attron(A_BOLD);
  }
  for(i; k < window_size; i++){

    if(i == selection && dim_y != 1 && dim_y != 3){
      if(bold_color_menu == true) {
        attron(COLOR_PAIR(menu_c + 16));
      } else {
        attron(COLOR_PAIR(menu_c + 8));
      }
    } else {
      attron(COLOR_PAIR(menu_c));
    }

    int str_len = (int)strlen(menu[i].name);
    if(str_len > dim_x - 2){
      str_len = dim_x - 2;
    }
    for(int j = 0; j < str_len; j++){
      mvaddch(ROW/2 - dim_y/2 + 1 + k + offset_y, COL/2 - dim_x/2 + 1 + j + offset_x, menu[i].name[j]);
      // TODO: account for small windows
    }

    attroff(COLOR_PAIR(menu_c + 8));
    attroff(COLOR_PAIR(menu_c));
    if(i == len - 1) break;
    k++;
  }
  attroff(A_BOLD);
}

void load_command_config(){
  memset(global_chars, '\0', KEY_ARRAY_SIZE * sizeof(char));
  memset(commandkeys_chars, '\0', KEY_ARRAY_SIZE * sizeof(char));
  memset(builtinkeys_chars, '\0', KEY_ARRAY_SIZE * sizeof(char));
  memset(menukeys_chars, '\0', KEY_ARRAY_SIZE * sizeof(char));

  commandkeys_len = sizeof(commandkeys)/sizeof(commandkeys[0]);
  builtinkeys_len = sizeof(builtinkeys)/sizeof(builtinkeys[0]);
  menukeys_len = sizeof(menukeys)/sizeof(menukeys[0]);

  int global_idx = 0;
  for(int i = 0; i < commandkeys_len; i++){
    global_chars[global_idx] = commandkeys[i].smashkey;
    commandkeys_chars[i] = commandkeys[i].smashkey;
    global_idx++;
  }

  for(int i = 0; i < builtinkeys_len; i++){
    global_chars[global_idx] = builtinkeys[i].smashkey;
    builtinkeys_chars[i] = builtinkeys[i].smashkey;
    global_idx++;
  }

  for(int i = 0; i < menukeys_len; i++){
    global_chars[global_idx] = menukeys[i].smashkey;
    menukeys_chars[i] = menukeys[i].smashkey;
    global_idx++;
  }
}

Command * ranger_command(const Command * command){

  Command * rangercmd       = (Command*)malloc(sizeof(Command));
  memset(rangercmd, 0, sizeof(Command));

  rangercmd->smashkey       = command->smashkey;
  rangercmd->extra_args     = command->extra_args;
  rangercmd->option         = command->option;
  rangercmd->pre_animation  = command->pre_animation;
  rangercmd->post_animation = command->post_animation;

  int rows = 0;
  while ((char**)command->cmd && ((char**)command->cmd)[rows] != NULL) {
    rows++;
  }

  int new_rows = rows + 2;

  char** new_cmd = malloc((new_rows + 1) * sizeof(char *));
  if(!new_cmd) crit("malloc error");

  for (int i = 0; i < rows; i++) {
    new_cmd[i] = strdup(((char**)command->cmd)[i]);
  }

  ensure_cache_dir();
  char cache_file[256] = {'\0'};
  pid_t cur_pid = getpid();
  char * env_home = getenv("HOME");
  sprintf(cache_file, "%s%s%d", env_home, "/.cache/sara/sara", cur_pid);

  const char *extras[] = { "--choosedir", cache_file, NULL};
  for (int i = 0; i < 2; i++) {
    new_cmd[rows + i] = strdup(extras[i]); // segfault
  }

  new_cmd[new_rows] = NULL;
  rangercmd->cmd = new_cmd;

  return rangercmd;
}

void _free_range(const Command * command){
  int rows = 0;
  while ((char**)command->cmd && ((char**)command->cmd)[rows] != NULL) {
    rows++;
  }
  char** kill_me = (char**)command->cmd;

  for(int i = 0; i < rows; i++){
    free(kill_me[i]);
  }
  free((void*)command);

  char cache_file[256] = {'\0'};
  pid_t cur_pid = getpid();
  char * env_home = getenv("HOME");
  sprintf(cache_file, "%s%s%d", env_home, "/.cache/sara/sara", cur_pid);

  FILE *fp;
  fp = fopen(cache_file, "r");
  if(!fp){
    refresh();
    crit("error reading pwd cache file");
  }

  char target_chdir[256] = {'\0'};
  if(fgets(target_chdir, 256, fp) == NULL) {
    char error_str[256] = {'\0'};
    sprintf(error_str, "%s%s" , "Error reading target_chdir file ", target_chdir);
    crit(error_str);
  }

  fclose(fp);
  remove(cache_file) ? crit("No file to be deleted"): 0;

  chdir(target_chdir);
  if (setenv("PWD", target_chdir, 1) != 0) {
    crit("setenv error");
  }
}

void _chdir(char * target_dir){
  chdir(target_dir);
  if (setenv("PWD", target_dir, 1) != 0) {  
    crit("setenv error");
  }
}

// TODO: PREFLIGHT CHECK
void preflight_check() {

  int commandkeys_len = sizeof(commandkeys)/sizeof(commandkeys[0]);
  int builtinkeys_len = sizeof(builtinkeys)/sizeof(builtinkeys[0]);
  int menukeys_len = sizeof(menukeys)/sizeof(menukeys[0]);
  int total = commandkeys_len + builtinkeys_len + menukeys_len;

  if(total > 128){
    char err[256];
    sprintf(err, "ERROR: Configured Key Limit exceeded\nKey Limit is %d\n commandkeys: %d\n builtinkeys: %d\n menukeys: %d\n",
      KEY_ARRAY_SIZE,
      commandkeys_len,
      builtinkeys_len,
      menukeys_len);
    crit("Key limit exceeded");
  }

  int global_chars_len = strlen(global_chars);
  for(int idx = 0; idx < global_chars_len; idx++){
    for(int j = idx + 1; j < global_chars_len; j++){
      if(global_chars[idx] == global_chars[j]){
        char err[64];
        sprintf(err, "Error: '%c' key is configured more than once\n", global_chars[idx]);
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

  // assert all command menus terminate with commands
  // assert all commands are installed
  // assert --choosedir flag not present in any ranger command
  // assert any chdir arg directories exist
  // assert MenuBorder length is 6
  // assert menu_y > 1
  // assert resize_x/y values are larger than tiny_mode_x/y
  // assert MenuBorder is the correct length
  // assert pshd_x < 258
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

void ensure_config_dir(){
  const char *env_home = getenv("HOME");
  const char *sara_wd = "/.config/sara";
  char config_dir[256] = {'\0'};
  sprintf(config_dir, "%s%s", env_home, sara_wd);

  DIR* dir = opendir(config_dir);
  if (dir) {
    closedir(dir);
  } else if (ENOENT == errno) {
    char dir_name[256] = {'\0'};
    sprintf(dir_name, "%s%s", env_home, "/.cache/sara");
    int status = mkdir(dir_name, 0755);

    if (status != 0) {
      char err[256] = {'\0'};
      sprintf(err, "%s%s" , "Error creating directory ", dir_name);
      crit(err);
    }
  }
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
  HEADER = rand() % 7 + 2;    // RNG 2 and 8
  while(FOREGROUND == BACKGROUND){
    FOREGROUND = rand() % 7 + 2;
  }
  animate(none);
}

void _show_colors(){
  for(int j = 0; j < 2; j++){
    for(int i = 1; i < 9; i++){
      attron(COLOR_PAIR(i));
      mvaddwstr(ROW - 2 + j, (COL-27) + (i*3), L"\u2588\u2588\u2588"); // Unicode full block █
      attroff(COLOR_PAIR(i));
    }
    attron(A_BOLD);
  }
  attroff(A_BOLD);
  refresh();
//getchar();
}

void _check(){

  return;
}

void _invert_colors(){
  int temp = BACKGROUND;
  BACKGROUND = FOREGROUND;
  FOREGROUND = HEADER;
  HEADER =  temp;
  animate(none);
}

void _quit(){
  if (FOLLOW){
    _write_exit_dir();
  }

  clear();
  refresh();
  move(0, 0);
  endwin();
  system("clear");
  exit(0);
}

void _write_exit_dir(){
  ensure_cache_dir();
  char cache_file[256] = {'\0'};
  char * env_home = getenv("HOME");
  char * env_pwd = getenv("PWD");
  sprintf(cache_file, "%s%s", env_home, "/.cache/sara/saraexit");
  FILE * fp = fopen(cache_file, "w");
  fprintf(fp, "%s", env_pwd);
  fclose(fp);
}

void set_glyph_dimensions(){
  BG_GLYPH_HEIGHT = sizeof(bg)/sizeof(bg[0]);
  FG_GLYPH_HEIGHT = sizeof(fg)/sizeof(fg[0]);
  BG_GLYPH_LENGTH = mbstowcs(NULL, bg[0], 0);
  FG_GLYPH_LENGTH = mbstowcs(NULL, fg[0], 0);
  HD_LENGTH = mbstowcs(NULL, hd, 0);
  if(im[0] == NULL) IM_SET = false;
}
