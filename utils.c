/*
--  ██╗   ██╗████████╗██╗██╗     ███████╗  --
--  ██║   ██║╚══██╔══╝██║██║     ██╔════╝  --
--  ██║   ██║   ██║   ██║██║     ███████╗  --
--  ██║   ██║   ██║   ██║██║     ╚════██║  --
--  ╚██████╔╝   ██║   ██║███████╗███████║  --
--   ╚═════╝    ╚═╝   ╚═╝╚══════╝╚══════╝.h--
                                               */

#define NCURSES_WIDECHAR 1
#define _key(...) __key((key_arg){__VA_ARGS__});

#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <dirent.h>
#include <wait.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include "limits.h"
#include "utils.h"
#include "globals.h"
#include "config.h"
#include "sara.h"
#include "animations.h"

int __key(key_arg arg){

  char arg_out = arg.i ? arg.i : 0;

  if (arg_out != 0){
    if(strchr(global_chars, arg_out)){
      _state->key_lock = 0;
      animate(none);
    } else {
      return 0;
    }
  }

  return key(arg_out);
}

int key(char input){

  if(!input) input = getch();
  int valid_input = 0;

  if (input != ERR && input != '\n' && input != EOF && input > 31 && input < 127) {
    valid_input = 1;

    if(strchr(global_chars, input)){
      if(strchr(commandkeys_chars, input)){
        __command__(input);
      } else if(strchr(builtinkeys_chars, input)){
        __builtin__(input);
      } else if(strchr(menukeys_chars, input)){
        if(_state->key_lock == 1) return valid_input;
        _state->key_lock = 1;
        __topmenu__(input);
        _state->key_lock = 0;
      } else {
        crit("Something broke :[");
      }

      valid_input = 2;

    } else if(_state->win_size != t_tiny){
      _state->last_input_time = clock();
      _state->hold_char = input;
      mvprintw(_state->t_rows/2, _state->t_cols/2, "%c", _state->hold_char);
      refresh();
    }
  }

  double time_since_input = (double)(clock() - _state->last_input_time)
                            / CLOCKS_PER_SEC;

  if(time_since_input > _state->hold_char_time && _state->win_size != t_tiny){
    _state->hold_char = '\0';
  }

  return valid_input;
}

void __command__(char input){

  if(_state->key_lock == 1) return;
  _state->key_lock = 1;

  const Command* command = NULL;
  for(int i = 0; i < commandkeys_len; i++){
    if(commandkeys[i].smashkey == input){
      command = &commandkeys[i];
      break;
    }
  }

  __execute__(command);

  _state->key_lock = 0;

  refresh();
  getmaxyx(stdscr, _state->t_rows, _state->t_cols);
  if(_state->t_cache != _state->t_rows + _state->t_cols) {
    return;
  }

  animate(command->post_animation);
}

int _get_confirm(const Command * command){
  int selection = false;
  animate(none);
  _print_confirm_window(((char **)command->cmd)[0]);
  _print_confirm_selection(selection);
  while(1){
    getmaxyx(stdscr, _state->t_rows, _state->t_cols);
    if (_state->t_cache != _state->t_rows + _state->t_cols) break;

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

  if(command->extra_args.confirmation == CONFIRM &&
    _get_confirm(command) != true)
  {
    return false;
  }

  int RANGER_FLAG=0;

  if(command->extra_args.chdir != NULL){
    _chdir(command->extra_args.chdir);
  }

  if(command->cmd == NULL) return true;
  if(((char**)command->cmd)[0] == NULL) return true;

  animate(command->pre_animation);
  endwin();

  if(strcmp("ranger", ((char **)command->cmd)[0]) == 0 &&
    command->option != EXEC)
  {
    command = ranger_command(command);
    RANGER_FLAG=1;
  }

  if(command->option != EXEC){

    pid_t pid = fork();
    if (pid < 0) {
      perror("fork");
      exit(EXIT_FAILURE);

    } else if (pid == 0) {
      if(command->extra_args.output_option == NO_OUT){
        int fd = open("/dev/null", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        dup2(fd, 1);
        dup2(fd, 2);

        close(fd);
      }

      if(FOLLOW) _write_exit_dir();
      execvp(((char **)command->cmd)[0], (char **)command->cmd);
      perror("execvp");
      crit("execvp critical error :|");
    } else {

      int status;
      if(command->option != NOWAIT){
        while(kill(pid, 0) == 0){
          waitpid(pid, &status, 0);
        }
      }

      if(_state->interrupt){
        return true;
      }

      if ((command->option == STOP_ON_ERR && status != 0) ||
          STOP == command->option) {
        printf("\x1b[33m" "Press enter to continue" "\x1b[0m");

        while(1){
          size_t bufsize = 256;
          char *buff = (char *)malloc(bufsize * sizeof(char));
          if(buff == NULL) crit("Unable to allocate buff");
          fgets(buff, 256, stdin);
          free(buff);
          refresh();
          _get_dims();
          if(!_resized()){
            break;
          } else {
            endwin();
          }
        }
      }
    }

  } else {

    if(command->extra_args.output_option == NO_OUT){
      int fd = open("/dev/null", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
      dup2(fd, 1);
      dup2(fd, 2);
      close(fd);
    }

    if(FOLLOW) _write_exit_dir();
    execvp(((char **)command->cmd)[0], (char **)command->cmd);
    perror("execvp");
    crit("execvp critical error :|");
  }

  if(RANGER_FLAG) _free_range(command);
  return true;
}

void __builtin__(char input){

  internal selection = quit;
  for(int i = 0; i < builtinkeys_len; i++){
    if(builtinkeys[i].smashkey == input){
      selection = builtinkeys[i].option;
      break;
    }
  }

  if(selection == quit){
    _quit(0);
  } else if (selection == pshd){
    _pshd();
  } else if (selection == rave){
    _toggle_rave();
  } else if (selection == invert_colors){
    _invert_colors();
  } else if (selection == randomize_colors){
    _randomize_colors();
  } else if (selection == path_run){
    _path_run();
  } else if (selection == pwd){
    _get_pwd();
  } else if (selection == fetch){
    _fetch();
  } else if (selection == fx_glint_fg){
    if (_state->fx == glint_f) {
      _state->fx = 0;
      _reset_glyph_state();
    }
    else  _state->fx = glint_f;
  } else if (selection == fx_glint_bg){
    if (_state->fx == glint_b) {
      _state->fx = 0;
      _reset_glyph_state();
    }
    else  _state->fx = glint_b;
  } else if (selection == fx_glint_full){
    if (_state->fx == glint_a) {
      _state->fx = 0;
      _reset_glyph_state();
    }
    else  _state->fx = glint_a;
  } else if (selection == colors){
    if(_state->show_colors == false){
      _state->show_colors = true;
    } else {
      _state->show_colors = false;
    }
    _show_colors();
  }
}

void _get_pwd(){
  slap(getenv("PWD"));
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
    if(menu_y < _state->t_rows){
      dim_y = 3;
    } else {
      dim_y = _state->t_rows;
    }
  } else if(_state->t_rows < menu_y){
    dim_y = _state->t_rows;    
  } else {
    dim_y = menu_y;
  }

  if(menu_x < 3){
    dim_x = 3;
  } else if(_state->t_cols < menu_x){
    dim_x = _state->t_cols;
  } else {
    dim_x = menu_x;
  }

  _menuselect(menukey->submenu);
}

void _menuselect(const Menu * menu){

  Menu * ptr = (Menu *)menu;
  int entries = 0;
  while(1){
    if(ptr == NULL || ptr->name == NULL) break;
    ptr++;
    entries++;
  }

  int dim_y = _deduce_dim_y(menu_y, entries);
  int dim_x = _deduce_dim_x(menu_x);
  int offset_y = _deduce_offset_y(dim_y, menu_offset_y);
  int offset_x = _deduce_offset_x(dim_x, menu_offset_x);


  int selection = 0;
  _print_menu_borders(dim_y, dim_x, offset_y, offset_x, menu_c);
  while(1){

    _get_dims();
    if (_resized()) break;

    // make this block a function
    int input = getch();
    if (input != ERR && input != '\n' && input != EOF && input > 105 && input < 108) {
      if (input == 'j'){
        selection = (selection + 1) % entries;
      } else {
        selection = (selection + (entries - 1)) % entries;
      }
    } else if (input == 'q' || input == 27){
      animate(none);
      return;
    } else if (input == 4){
      // ctrl+d
      selection += (dim_y-2)/2;
      if(selection >= entries) selection = entries-1;
    } else if (input == 6){
      selection += (dim_y-2);
      if(selection >= entries) selection = entries-1;
    } else if (input == 21){
      //ctrl+u
      selection -= (dim_y-2)/2;
      if(selection < 0) selection = 0;
    } else if (input == 2){
      selection -= (dim_y-2);
      if(selection < 0) selection = 0;
    } else if (input == 'G' || input == 'L'){
      selection = entries-1;
    } else if (input == 'g' || input == 'H'){
      selection = 0;
    } else if(input == 'M'){
      selection = entries/2;
    } else if (input == '\n'){
      if(menu[selection].type == SUBMENU){
        _clear_menu(dim_y+2, dim_x+2, offset_y, offset_x);
        _menuselect(menu[selection].next.submenu);
      } else if(menu[selection].type == COMMAND){
        int result = __execute__(&menu[selection].next.command);
        if(result == true){
          _state->key_lock = 0;
          animate(menu[selection].next.command.post_animation);
        } else {
          animate(none);
        }
      }
      return;
    } else if (input != -1){
      int result = _key(input);
      if(result > 0) {
        return;
      }
    }

    _print_menu_selection(menu, selection, entries, dim_y, dim_x, offset_y, offset_x);
    usleep(20000);
  }
}

void _print_menu_selection(const Menu * menu, int selection, int len, int dim_y, int dim_x, int offset_y, int offset_x){

  if(_state->win_size == t_tiny) clear();

  if(_state->fx != nofx){
    _affect(_state->fx);
    _print_menu_borders(dim_y, dim_x, offset_y, offset_x, menu_c);
  } else {
    _clear_menu(dim_y, dim_x, offset_y, offset_x);
  }
  int window_size = dim_y - 2;
  if(len < window_size) window_size = len;

  if(dim_y < 3) {
    window_size = _state->t_rows;
    offset_y = -1;
  }

  int i = 0;
  int k = 0;
  if(selection > len - window_size) i = len - window_size;
  else i = selection;

  if(menu_c_bold == true) attron(A_BOLD);

  for(i; k < window_size; i++){

    if(i == selection && dim_y != 1 && dim_y != 3){
      if(menu_c_bold == true) {
        attron(A_STANDOUT);
        attron(COLOR_PAIR(menu_c));
      } else {
        attron(COLOR_PAIR(menu_c));
        attron(A_STANDOUT);
      }
    } else {
      attron(COLOR_PAIR(menu_c));
    }

    int str_len = (int)strlen(menu[i].name);
    if(str_len > dim_x - 2){
      str_len = dim_x - 2;
    }
    for(int j = 0; j < str_len; j++){
      mvaddch(_state->t_rows/2 - dim_y/2 + 1 + k + offset_y, _state->t_cols/2 - dim_x/2 + 1 + j + offset_x, menu[i].name[j]);
    }

    attroff(A_STANDOUT);
    attroff(COLOR_PAIR(menu_c));
    if(i == len - 1) break;
    k++;
  }
  attroff(A_BOLD);
  refresh();
}

int _deduce_dim_y(int requested_dim_y, int entries){

  int dim_y = 0;

  // base case
  if(requested_dim_y < 3){
    if(requested_dim_y < _state->t_rows){
      dim_y = 3;
    } else {
      dim_y = _state->t_rows;
    }
  } else if(_state->t_rows < requested_dim_y){
    dim_y = _state->t_rows;    
  } else {
    dim_y = requested_dim_y;
  }

  if(entries < dim_y - 2 || 
    (entries < requested_dim_y - 2 && entries < _state->t_rows - 2))
  {
    dim_y = entries+2;
  }

  if(_state->t_rows < dim_y){
    dim_y = _state->t_rows;
  }

  return dim_y;
}

int _deduce_dim_x(int requested_dim_x){

  int dim_x = 0;

  if(menu_x < 3){
    dim_x = 3;
  } else if(_state->t_cols < requested_dim_x){
    dim_x = _state->t_cols;
  } else {
    dim_x = requested_dim_x;
  }
  
  return dim_x;
}

int _deduce_offset_y(int dim_y, int req){

  int lower_bound = _state->t_rows/2 - dim_y/2 + dim_y + req;

  if(lower_bound < _state->t_rows) return req;

  while(lower_bound > _state->t_rows){
    lower_bound--;
    req--;
  }

  return req;
}

int _deduce_offset_x(int dim_x, int requested_offset_x){

  if((_state->t_cols/2 - dim_x/2 + dim_x - 1 + requested_offset_x) >= _state->t_cols){
    int ret_val = _state->t_cols/2 - dim_x/2;
    if((_state->t_cols/2 - dim_x/2 + dim_x - 1 + ret_val) < _state->t_cols) ret_val++;
    if((_state->t_cols/2 - dim_x/2 + dim_x - 1 + ret_val) >= _state->t_cols) ret_val--;
    return ret_val;
  }

  if((_state->t_cols/2 - dim_x/2 + requested_offset_x) < 0){
    int ret_val = -(_state->t_cols/2 - dim_x/2);
    return ret_val;
  }

  return requested_offset_x;
}

void load_command_config(){
  memset(global_chars,      '\0', KEY_ARRAY_SIZE * sizeof(char));
  memset(commandkeys_chars, '\0', KEY_ARRAY_SIZE * sizeof(char));
  memset(builtinkeys_chars, '\0', KEY_ARRAY_SIZE * sizeof(char));
  memset(menukeys_chars,    '\0', KEY_ARRAY_SIZE * sizeof(char));

  commandkeys_len = sizeof(commandkeys)/sizeof(commandkeys[0]);
  builtinkeys_len = sizeof(builtinkeys)/sizeof(builtinkeys[0]);
  menukeys_len    = sizeof(menukeys)/sizeof(menukeys[0]);

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
    new_cmd[rows + i] = strdup(extras[i]);
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
  free(kill_me[rows]);
  free((char**)command->cmd);
  free((void*)command);

  char cache_file[256] = {'\0'};
  pid_t cur_pid = getpid();
  char * env_home = getenv("HOME");
  sprintf(cache_file, "%s%s%d", env_home, "/.cache/sara/sara", cur_pid);

  FILE *fp;
  fp = fopen(cache_file, "r");
  if(!fp){
    refresh();
    slap("error reading pwd cache file");
    return;
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
    slap("setenv error");
  }
}

void _chdir(char * target_dir){
  chdir(target_dir);
  if (setenv("PWD", target_dir, 1) != 0) {  
    slap("setenv error");
  }
}

void _preflight_check() {

  clock_t preflight_start = clock();;

  int warning_flag = 0;
  int commandkeys_len = sizeof(commandkeys)/sizeof(commandkeys[0]);
  int builtinkeys_len = sizeof(builtinkeys)/sizeof(builtinkeys[0]);
  int menukeys_len = sizeof(menukeys)/sizeof(menukeys[0]);
  int total = commandkeys_len + builtinkeys_len + menukeys_len;

  if(total > 128){
    char err[256];
    sprintf(err, "Warning: Configured Key Limit exceeded\nKey Limit is %d\n commandkeys: %d\n builtinkeys: %d\n menukeys: %d\n",
      KEY_ARRAY_SIZE,
      commandkeys_len,
      builtinkeys_len,
      menukeys_len);
    warning_flag = 1;
    warn(err);
  }

  int global_chars_len = strlen(global_chars);
  for(int idx = 0; idx < global_chars_len; idx++){
    for(int j = idx + 1; j < global_chars_len; j++){
      if(global_chars[idx] == global_chars[j]){
        char err[128];
        sprintf(err, "Warning: '%c' key is configured more than once, secondary mappings will be ignored\n", global_chars[idx]);
        warning_flag = 1;
        warn(err);
      }
    }
  }

  for(int i = 0; i < BG_GLYPH_HEIGHT; i++){
    int len = mbstowcs(NULL, bg[i], 0);
    if(len != BG_GLYPH_LENGTH){
      int len = mbstowcs(NULL, bg[i], 0);
      if(len > BG_GLYPH_LENGTH){
        char err[128];
        sprintf(err, "%s%d%s%d%s", "Warning: bg[]: bg[", i, "] too long, should match bg[0] length at ", BG_GLYPH_LENGTH, " characters");
        warning_flag = 1;
        warn(err);
      } else {
        char err[128];
        sprintf(err, "%s%d%s%d%s", "Warning: bg[]: bg[", i, "] too short, should match bg[0] length at ", BG_GLYPH_LENGTH, " characters");
        warning_flag = 1;
        warn(err);
      }
    }
  }

  for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
    int len = mbstowcs(NULL, fg[i], 0);
    if(len != FG_GLYPH_LENGTH){
      if(len > FG_GLYPH_LENGTH){
        char err[128];
        sprintf(err, "%s%d%s%d%s", "Warning: fg[]: fg[", i, "] too long, should match fg[0] length at ", FG_GLYPH_LENGTH, " characters");
        warning_flag = 1;
        warn(err);
      } else {
        char err[128];
        sprintf(err, "%s%d%s%d%s", "Warning: fg[]: fg[", i, "] too short, should match fg[0] length at ", FG_GLYPH_LENGTH, " characters");
        warning_flag = 1;
        warn(err);
      }
    }
  }

  // assert all commands are installed in $PATH
  ensure_cache_dir();
  char run_file[256] = {'\0'};
  char * env_home = getenv("HOME");
  sprintf(run_file, "%s%s", env_home, "/.cache/sara/sara_run");
  remove(run_file);
  generate_path_run_file();

  const Menu * menu_ptr = NULL;
  for(int i = 0; i < menukeys_len; i++){
    menu_ptr = menukeys[i].submenu;
    if(_check_menu(menu_ptr, preflight_start)) warning_flag = 1;
  }

  const Command * command_ptr = NULL;
  const char * binary = NULL;
  for(int i = 0; i < commandkeys_len; i++){
    command_ptr = &commandkeys[i];
    binary = ((char**)command_ptr->cmd)[0];
    if(binary != NULL && _is_binary_in_path(binary) == 1){
      char warning[256];
      sprintf(warning, "%s%s%s", "Warning: '", binary, "' either not in $PATH or is not an executable file");
      warning_flag = 1;
      warn(warning);
    }
  }

  // assert MenuBorder length is 6
  int menu_border_len = wcslen(MenuBorder);
  if(menu_border_len != 6){
    char warning[256];
    sprintf(warning, "%s", "Warning: MenuBorder should contain 6 characters");
    warning_flag = 1;
    warn(warning);
  }

  // assert resize_x/y values are larger than tiny_mode_x/y
  if(tiny_mode == true && dynamic_resize == true){
    if (resize_x < tiny_mode_x) {
      char err[128];
      sprintf(err, "%s", "Warning: resize_x shan't be less than tiny_mode_x");
      warning_flag = 1;
      warn(err);
    }
    if (resize_y < tiny_mode_y) {
      char err[128];
      sprintf(err, "%s", "Warning: resize_y shan't be less than tiny_mode_y");
      warning_flag = 1;
      warn(err);
    }
  }

  // assert at least one key is `quit`
  int quit_check = 0;
  const Builtin * builtin_ptr = NULL;
  for(int i = 0; i < builtinkeys_len; i++){
    builtin_ptr = &builtinkeys[i];
    if(builtin_ptr->option == quit){
      quit_check = 1;
      break;
    }
  }
  if(quit_check == 0){
    char err[256];
    sprintf(err, "%s", "Warning: builtinkeys[]: no mapping for 'quit'");
    warning_flag = 1;
    warn(err);
  }

//if(warning_flag == 0) printf("\x1b[32m" "Precheck passed" "\x1b[0m\n");
  _quit(warning_flag);
} 

int _check_menu(const Menu * menu_ptr, clock_t start_time){
  int warning_flag = 0;
  while(menu_ptr != NULL && menu_ptr->name != NULL){
    if(menu_ptr->type == SUBMENU){
      const Menu * submenu_ptr = menu_ptr->next.submenu;
      if(_check_menu(submenu_ptr, start_time)) warning_flag = 1;
    } else {
      const Command * binary_command = &menu_ptr->next.command;
      const char * binary = NULL;
      if(binary_command->cmd != NULL) binary = ((char**)binary_command->cmd)[0];

      if(binary != NULL && _is_binary_in_path(binary) == 1){
        char warning[128];
        sprintf(warning, "%s%s%s", "Warning: '", binary, "' either not in $PATH or is not an executable file");
        warning_flag = 1;
        warn(warning);
      }
    }

    clock_t elapsed = clock() - start_time;
    int msec = elapsed * 1000 / CLOCKS_PER_SEC;
    if(msec > 500){
      char warning[256];
      sprintf(warning, "%s", "Warning: A menu is likeyly not NULL terminated");
      warning_flag = 1;
      warn(warning);
    }
    menu_ptr++;
  }
  return warning_flag;
}

int _is_binary_in_path(const char * binary) {

  if(binary[0] == '/'){
    if (access(binary, F_OK) != 0){
      return 1;
    }
    return 0;
  } else if(binary == NULL){
    return 0;
  } else {
    int len = strlen(binary);
    char run_file[256] = {'\0'};
    char * env_home = getenv("HOME");
    sprintf(run_file, "%s%s", env_home, "/.cache/sara/sara_run");
    FILE * fp = fopen(run_file, "r");
    char line[256] = {'\0'};

    if(fp == NULL){
      crit("hwat");
    }

    while(fgets(line, sizeof(line), fp)) {
      line[strcspn(line, "\n")] = 0;
      if(strncmp(binary, line, len) == 0){
        fclose(fp);
        return 0;
      }
    }
    fclose(fp);
    return 1;
  }
}

void warn(char * warning) {
  endwin();
  printf("\x1b[31m%s\n\x1b[0m", warning);
}

void slap(char * slap_msg) {
  mvprintw(0, 0, "%s", slap_msg);
  memset(_state->slap_str, 0, 256 * sizeof(char));
  strncpy(_state->slap_str, slap_msg, 256);
  gettimeofday(&slap_time, NULL);
}

int _slap_timer() {
  struct timeval t2;
  double elapsed_time;
  gettimeofday(&t2, NULL);

  elapsed_time = (t2.tv_sec - slap_time.tv_sec) * 1000.0;

  int slap_display_time = 1750;
  if(elapsed_time > slap_display_time){
    memset(_state->slap_str, 0, 256 * sizeof(char));
    return 1;
  } else {
    return 0;
  }
}

void crit(char * err) {
  endwin();
  printf("\x1b[31m%s\n\x1b[0m", err);
  _quit(1);
}

int is_char_in_search(wchar_t wc, const wchar_t * search_str) {

    for (size_t i = 0; i < wcslen(search_str); i++) {
        if (wc == search_str[i]) {
            return 1; // Character found
        }
    }
    return 0; // not found
}

/* 0 if no-op */
/* 1 if created cache_file */
/* CRIT if ERR */
int ensure_cache_dir(){
  int EXIT_VAL = 0;
  const char *env_home = getenv("HOME");
  const char *cache_wd = "/.cache";
  const char *sara_wd = "/.cache/sara";
  char cache_dir[256] = {'\0'};

  sprintf(cache_dir, "%s%s", env_home, cache_wd);
  DIR* dir = opendir(cache_dir);
  if (dir) {
    closedir(dir);
  } else if (ENOENT == errno) {
    char dir_name[256] = {'\0'};
    sprintf(dir_name, "%s%s", env_home, "/.cache");
    int status = mkdir(dir_name, 0755);

    if (status != 0) {
      char error_str[256] = {'\0'};
      sprintf(error_str, "%s%s" , "Error creating directory ", dir_name);
      crit(error_str);
      EXIT_VAL = 2; /* CRIT */
    }
    EXIT_VAL = 1;
  }

  sprintf(cache_dir, "%s%s", env_home, sara_wd);
  dir = opendir(cache_dir);
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
      EXIT_VAL = 2; /* CRIT */
    }
    EXIT_VAL = 1;
  }
  return EXIT_VAL;
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
  set_escdelay(25);
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
//clear();
//refresh();                // clear screen
}

void get_helped(int exit_code) {
  printf("Usage: %s [OPTIONS]\n", "sara");
  printf("Visit \x1b[31mhakipaks.org/sara/getting-started\x1b[0m for docs\n");
  printf("  -h, --help      Get helped\n");
  printf("  -v, --version   Get version\n");
  printf("  -H              HOLO mode\n");
  printf("  -f [color]      set FOREGROUND color\n");
  printf("  -b [color]      set BACKGROUND color\n");
  printf("  -F [follow]     On exit, write PWD to file\n");
  printf("  -r              Generate a random color profile. Not compatible with -f or -b options\n");
  printf("  -C              Run in preflight_check mode to catch common detectable config.h issues and exit");
  exit(exit_code);
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
  _state->bg_c = rand() % 7 + 1;    // RNG 1 and 7
  _state->fg_c = rand() % 7 + 2;    // RNG 2 and 8
  _state->hd_c = rand() % 7 + 2;    // RNG 2 and 8
  while(_state->fg_c == _state->bg_c){
    _state->fg_c = rand() % 7 + 2;
  }
  animate(none);
}

void _show_colors(){
  for(int j = 0; j < 2; j++){
    for(int i = 1; i < 9; i++){
      attron(COLOR_PAIR(i));
      mvaddwstr(_state->t_rows - 2 + j, (_state->t_cols-27) + (i*3), L"\u2588\u2588\u2588"); // Unicode full block █
      attroff(COLOR_PAIR(i));
    }
    attron(A_BOLD);
  }
  attroff(A_BOLD);
  refresh();
}

void _invert_colors(){
  int temp = _state->bg_c;
  _state->bg_c = _state->fg_c;
  _state->fg_c = _state->hd_c;
  _state->hd_c =  temp;
  animate(none);
}

void _quit(int exit_code){
  if (FOLLOW) _write_exit_dir();

  clear();
  refresh();
  move(0, 0);
  endwin();

  for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
    free(_state->fg_state[i]); _state->fg_state[i] = NULL;
  }

  for(int i = 0; i < BG_GLYPH_HEIGHT; i++){
    free(_state->bg_state[i]); _state->bg_state[i] = NULL;
  }

  free(_state->fg_state); _state->fg_state = NULL;
  free(_state->bg_state); _state->bg_state = NULL;
  free(_state);

  exit(exit_code);
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
}

void __interrupt__(int signum){
  _state->interrupt = 1;
  __execute__(FORCE_ECHO);
}

void _deflect_signals(){
  struct sigaction signal_action = { 0 };  
  signal_action.sa_handler = __interrupt__;
  sigemptyset(&signal_action.sa_mask);
  signal_action.sa_flags = 0;

  sigaction(SIGINT, &signal_action, NULL);
}

void generate_path_run_file(){

  char run_file[256] = {'\0'};
  char * env_home = getenv("HOME");
  sprintf(run_file, "%s%s", env_home, "/.cache/sara/.sara_run");
  FILE * fp;
  if (access(run_file, F_OK) != 0){
    fp = fopen(run_file, "a");
    char * env_path = getenv("PATH");
    char path[1028] = "\0";
    strncpy(path, env_path, 1028);
    char * token = strtok(path, ":");
    while(token != NULL){
      DIR *d;
      struct dirent *dir;
      d = opendir(token);
      if (d){
        while ((dir = readdir(d)) != NULL) {
          if(strncmp(".", dir->d_name, 1) == 0){
            continue;
          }

          char link_path[256];
          sprintf(link_path, "%s/%s", token, dir->d_name);
          struct stat link_path_stat;
          int stat_result = stat(link_path, &link_path_stat);
          if (dir->d_type == DT_LNK){
            if (stat_result == -1) {
              continue;
            }
          } else if (stat_result == 0){
            if (!(link_path_stat.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))) {
              continue;
            } else if (!S_ISREG(link_path_stat.st_mode)) {
              continue;
            }
          }
          char output_str[128];
          sprintf(output_str, "%s%s", dir->d_name, "\n");
          fputs(output_str, fp);
        }
        closedir(d);
      } else {
        char error_str[64];
        sprintf(error_str, "%s%s",
            "ERROR: Could not open $PATH directory: ",
            token);
        crit(error_str);
      }
      token = strtok(NULL, ":");
    }

    fclose(fp);
    char sys_sort_cmd[256] = "\0";

    char run_file_sorted[256] = {'\0'};
    sprintf(run_file_sorted, "%s%s", env_home, "/.cache/sara/sara_run");
    sprintf(sys_sort_cmd, "%s%s%s%s%s%s", "sort ", run_file, " | ", "uniq", " > ", run_file_sorted);

    system(sys_sort_cmd);
  }
  remove(run_file);
}

void _path_run(){
  ensure_cache_dir();
  generate_path_run_file();
  _run_menu();
}

void _run_menu(){

  int dim_y = 0;
  int dim_x = 0;

  if(run_y < 3){
    if(run_y < _state->t_rows){
      dim_y = 3;
    } else {
      dim_y = _state->t_rows;
    }
  } else if(_state->t_rows < run_y){
    dim_y = _state->t_rows;    
  } else {
    dim_y = run_y;
  }

  if(run_x < 3){
    dim_x = 3;
  } else if(_state->t_cols < run_x){
    dim_x = _state->t_cols;
  } else {
    dim_x = run_x;
  }

  int offset_y = _deduce_offset_y(dim_y, run_offset_y);
  int offset_x = _deduce_offset_x(dim_x, run_offset_x);

  _print_menu_borders(dim_y, dim_x, offset_y, offset_x, run_c);

  if (dim_y == 2){
    offset_y = 0;
  } else if(dim_y < 3){
    offset_y = -1;
  }

  char filter_buffer[128] = {"\0"};
  char selection[128] = {"\0"};
  int idx = 0;
  char input;
  _populate_run_body(dim_y, dim_x, offset_y, offset_x, filter_buffer, selection);

  int refresh_ui = true;
  while(1){

    if(_state->fx != nofx){
      _affect(_state->fx);
      refresh_ui = true;
    }

    getmaxyx(stdscr, _state->t_rows, _state->t_cols);
    if (_state->t_cache != _state->t_rows + _state->t_cols){
      return;
    }

    if(refresh_ui) {
      _print_menu_borders(dim_y, dim_x, offset_y, offset_x, run_c);

      attron(COLOR_PAIR(run_c));
      if(run_c_bold) {
        attron(A_STANDOUT);
        attron(A_BOLD);
      } else {
        attron(COLOR_PAIR(run_c));
        attron(A_STANDOUT);
      }
      mvprintw(_state->t_rows/2 - dim_y/2 + offset_y, _state->t_cols/2 - dim_x/2 + offset_x, "RUN");
      attroff(A_STANDOUT);
      mvaddch(_state->t_rows/2 - dim_y/2 + offset_y, _state->t_cols/2 - dim_x/2 +  3 + offset_x, ' ');

      attron(COLOR_PAIR(run_c));
      if(run_c_bold) attron(A_BOLD);

      for(int i = 0; i < (int)strlen(filter_buffer); i++){
        mvaddch(_state->t_rows/2 - dim_y/2 + offset_y, _state->t_cols/2 - dim_x/2 +  4 + i + offset_x , filter_buffer[i]);
      }

      wchar_t cursor_w[2] = { L'\u2588', L'\0' };
      cchar_t cchar;
      setcchar(&cchar, cursor_w, 0, 0, NULL);
      mvadd_wch(_state->t_rows/2 - dim_y/2 + offset_y, _state->t_cols/2 - dim_x/2 +  3 + idx + offset_x + 1, &cchar);
      attroff(COLOR_PAIR(pshd_c));
      attroff(A_BOLD);
      _populate_run_body(dim_y, dim_x, offset_y, offset_x, filter_buffer, selection);
      refresh();
      refresh_ui = false;
    }

    // this aparantly fucking refreshes like ahem fuck you just get the
    // input you shitty ass fuck can't you see what I'm trying to do here?
    input = getch();

    if (input != ERR && input != '\n' && input != EOF && input > 32 && input < 127) {

      filter_buffer[idx] = input;
      idx++;
      if(idx == 256) break;
      refresh_ui++;

    } else if (input == '\n') {
      _state->key_lock = 1;
      if(_run_exec(selection) == 0){
        animate(start_animation);
      } else {
        animate(glitch_full);
      }
      _state->key_lock = 0;
      break;
    } else if (input == 32){
      // space
      if(strlen(selection) != 0) {
        int ret_val = _run_args(offset_y, offset_x, selection);
        if (ret_val == 0){
          animate(start_animation);
        }
      }
      break;
    } else if (input == 27){
      // Escape
      break;
    } else if (input > 0) {
      // Backspace
      if(idx == 0){
        break;
      }
      if(dim_y == 2) mvaddch(_state->t_rows/2 - dim_y/2 + offset_y, _state->t_cols/2 - dim_x/2 +  3 + idx + offset_x + 1, ' ');
      idx--;
      filter_buffer[idx] = '\0';
      refresh_ui++;
    }

    usleep(10000);
  }

  animate(none);
  return;
}

void _populate_run_body(int dim_y, int dim_x, int offset_y, int offset_x, char * filter_buffer, char * selection) {
  int border_buffer = 2;
  if(dim_y < 3) border_buffer = 0;

  if(dim_y != 2){
    _clear_menu(dim_y, dim_x, offset_y, offset_x);
  } else {
    wchar_t wc[2] = { L' ', L'\0' };
    cchar_t cchar;
    setcchar(&cchar, wc, 0, 0, NULL);
    for(int j = 1; j < dim_x - 1; j++){
      mvadd_wch(_state->t_rows/2 - dim_y/2 + 1, _state->t_cols/2 - dim_x/2 + j + offset_x, &cchar);
    }
  }

  char sara_run_file[256] = {'\0'};
  char * env_home = getenv("HOME");

  sprintf(sara_run_file, "%s%s", env_home, "/.cache/sara/sara_run");
  FILE *file = fopen(sara_run_file, "r");
  if(file == NULL){
    crit(strerror(errno));
  }

  char line[256] = {'\0'};
  int i = 0;

  attron(COLOR_PAIR(run_c));
  if(run_c_bold) attron(A_BOLD);

  while((fgets(line, sizeof(line), file) && (i < (dim_y - border_buffer)))){
    line[strcspn(line, "\n")] = 0;
    if(strncmp(filter_buffer, line, strlen(filter_buffer)) == 0){
      int len = strlen(line);
      if(i == 0){
        strncpy(selection, line, 128);
        attron(A_STANDOUT);
        for(int j = 0; j < len; j++){
          if((j) == dim_x - 2) break;
          mvaddch(_state->t_rows/2 - dim_y/2 + i + 1 + offset_y, _state->t_cols/2 - dim_x/2 + j + 1 + offset_x, line[j]);
        }
        attroff(A_STANDOUT);
      } else {
        for(int j = 0; j < len; j++){
          if((j) == dim_x - 2) break;
          mvaddch(_state->t_rows/2 - dim_y/2 + i + 1 + offset_y, _state->t_cols/2 - dim_x/2 + j + 1 + offset_x, line[j]);
        }
      }
      i++;
      attroff(run_c);
    } else if (strlen(filter_buffer) == 0){
      char err[128];
      sprintf(err, "%lu", strlen(filter_buffer));
      crit(err);
      int len = strlen(line);
      for(int j = 0; j < len; j++){
        if((j) == dim_x - 2) break;
        mvaddch(_state->t_rows/2 - dim_y/2 + i + 1 + offset_y, _state->t_cols/2 - dim_x/2 + j + 1 + offset_x, line[j]);
      }
      i++;
      if (i == dim_y) break;
    }
  }

  if(i == 0){
    memset(selection, '\0', 128 * sizeof(char));
  }

  attroff(COLOR_PAIR(run_c));
  attroff(A_STANDOUT);
  attroff(A_BOLD);
  fclose(file);
  return;
}

int _run_exec(char * selection){

  if(strlen(selection) == 0) {
    return 1;
  }

  Command * command       = (Command*)malloc(sizeof(Command));
  memset(command, 0, sizeof(Command));

  char** cmd = malloc(2 * sizeof(char *));
  cmd[0] = strdup(selection);
  cmd[1] = NULL;

  ExtraArgs extra_args    = {NULL, NOCONFIRM, OUTS};
  command->smashkey       = 0;
  command->extra_args     = extra_args;
  command->option         = STOP;
  command->pre_animation  = none;
  command->post_animation = start_animation;
  command->cmd            = cmd;

  __execute__(command);

  char** free_me = (char**)command->cmd;
  free(free_me[0]);
  free(free_me[1]);
  free((char**)command->cmd);
  free((void*)command);

  return 0;
}

int _run_args(int offset_y, int offset_x, char * selection){
  animate(none);

  char args_buffer[256] = {'\0'};
  int buffer_idx = 0;

  char input = {'\0'};

  int refresh_ui = true;
  while(1){

    if(_state->fx != nofx){
      _affect(_state->fx);
      refresh_ui = true;
    }

    getmaxyx(stdscr, _state->t_rows, _state->t_cols);
    if (_state->t_cache != _state->t_rows + _state->t_cols){
      return 0;
    }

    if (refresh_ui == true) {
      _run_args_refresh(offset_y, offset_x, selection, args_buffer);
      refresh();
      refresh_ui = false;
    }

    input = getch();
    if (input != ERR && input != '\n' && input != EOF && input > 31 && input < 127) {
      args_buffer[buffer_idx] = input;
      buffer_idx++;
      if(buffer_idx > 255) {
        animate(glitch_full);
        return 1;
      }
      refresh_ui = true;
    } else if (input == '\n') {
      int ret_val = _execute_run_args(selection, args_buffer);
      if(ret_val == 1){
        animate(glitch_full);
        return 1;
      }
      return 0;

    } else if (input == 27){
      // escape
      return 0;
    } else if (input > 0) {
      // backspace
      buffer_idx--;
      if(buffer_idx < 0){
        return 1;
      }
      args_buffer[buffer_idx] = 0;
      refresh_ui = true;
    }

    usleep(10000);
  }

  refresh();
  attroff(COLOR_PAIR(run_c));
  return 0;
}

void _run_args_refresh(int offset_y, int offset_x, char *selection, char * args_buffer){

  if(_state->t_rows > 1 && _state->fx == nofx) animate(none);
  if(_state->t_rows == 1) clear();
  int dim_y = 3;
  int dim_x = run_x; if(run_x < 3) dim_x = 3; if(_state->t_cols < dim_x) dim_x = _state->t_cols;

  offset_y -= run_y/2;
  if(run_y > _state->t_rows) offset_y = 1 - _state->t_rows/2;
  if(_state->t_rows == 1) offset_y = 0;
  if(_state->t_rows == 2) offset_y = -1;

  wchar_t cursor_w[2] = { L'\u2588', L'\0' };
  cchar_t cursor_c;
  setcchar(&cursor_c, cursor_w, 0, 0, NULL);
  if(_state->t_rows > 1) _print_menu_borders(dim_y, dim_x, offset_y, offset_x, run_c);
  attron(COLOR_PAIR(run_c));
  if(run_c_bold == true) attron(A_BOLD);
  mvaddch(_state->t_rows/2 - dim_y/2 + 1 + offset_y, _state->t_cols/2 - dim_x/2 + 2 + offset_x, '>');
  int selection_len = (int)strlen(selection);
  int args_buffer_len = (int)strlen(args_buffer);

  if(args_buffer_len + selection_len < dim_x - 6){
    for(int i = 0; i < selection_len; i++){
      mvaddch(_state->t_rows/2 - dim_y/2 + 1 + offset_y, _state->t_cols/2 - dim_x/2 + 4 + i + offset_x, selection[i]);
    }
    for(int i = 0; i < args_buffer_len; i++){
      mvaddch(_state->t_rows/2 - dim_y/2 + 1 + offset_y, _state->t_cols/2 - dim_x/2 + 5 + selection_len + i + offset_x, args_buffer[i]);
    }
    mvadd_wch(_state->t_rows/2 - dim_y/2 + 1 + offset_y, _state->t_cols/2 - dim_x/2 + 5 + selection_len + args_buffer_len + offset_x, &cursor_c);
  } else {

    int buffer_size = selection_len + args_buffer_len + 4;

    char * overflow_buffer = (char*)malloc(buffer_size * sizeof(char));
    memset(overflow_buffer, '\0', buffer_size);
    overflow_buffer[0] = ' ';
    overflow_buffer[1] = '>';
    overflow_buffer[2] = ' ';
    for(int i = 0; i < selection_len; i++){
      overflow_buffer[3 + i] = selection[i];
    }
    overflow_buffer[3 + selection_len] = ' ';
    for(int i = 0; i < args_buffer_len; i++){
      overflow_buffer[4 + selection_len + i] = args_buffer[i];
    }

    mvadd_wch(_state->t_rows/2 - dim_y/2 + 1 + offset_y, _state->t_cols/2 - dim_x/2 + dim_x - 2 + offset_x, &cursor_c);
    for(int i = 0; i < dim_x - 3; i++){
      mvaddch(_state->t_rows/2 - dim_y/2 + 1 + offset_y, _state->t_cols/2 - dim_x/2 + dim_x - 3 - i + offset_x, overflow_buffer[buffer_size - 1 - i]);
    }
    free(overflow_buffer);
  }

  attroff(COLOR_PAIR(run_c));
  return;

}

int _execute_run_args(char * selection, char * args_buffer){

  char execute_str[256] = {0};
  int selection_len = strlen(selection);
  strncpy(execute_str, selection, selection_len);
  int buffer_remainer = 256 - selection_len;
  if(buffer_remainer - 1 < (int)strlen(args_buffer)) return 1;
  strncat(execute_str, " ", buffer_remainer);
  strncat(execute_str, args_buffer, buffer_remainer - 1);

  int execute_len = (int)strlen(execute_str);
  int search_idx = 0;
  int count = 0;
  while (search_idx < execute_len){
    if(execute_str[search_idx] == '&'){
      count++;
    }
    search_idx++;
  }

  if(count > 1) {
    slap("too many '&'"); // slap
    return 1;
  } else if(count == 1 && execute_str[execute_len - 1] != '&'){
    slap("bad '&' position"); // slap
    return 1;
  } else if(count == 1 && execute_str[execute_len - 2] != ' '){
    slap("typo?"); // slap slap slap
    return 1;
  }

  int ampersand = 0;
  if(count == 1) ampersand = 1;

  char token_str[256] = {0};
  strncpy(token_str, execute_str, 256);
  count = 0;
  char * token = strtok(token_str, " ");

  int nohup_flag = 0;
  if(strncmp(token, "nohup", 5) == 0) nohup_flag = 1;

  while(token != NULL){
    count++;
    token = strtok(NULL, " ");
  }

  if(nohup_flag && ampersand) count--;
  char** cmd = malloc((count + 1) * sizeof(char *));

  if(ampersand == 1 && nohup_flag == 0){
    cmd[0] = strdup("nohup");
    token = strtok(execute_str, " ");
    cmd[1] = strdup(token);
  } else if (ampersand == 1 && nohup_flag == 1) {
    token = strtok(execute_str, " ");
    cmd[0] = strdup(token);
    token = strtok(NULL, " ");
    cmd[1] = strdup(token);
  } else {
    token = strtok(execute_str, " ");
    cmd[0] = strdup(token);
  }

  for(int i = 1 + ampersand; i < count; i++){
    token = strtok(NULL, " ");
    cmd[i] = strdup(token);
  }

  cmd[count] = NULL;

  Command * command = (Command*)malloc(sizeof(Command));
  memset(command, 0, sizeof(Command));

  ExtraArgs extra_args;
  extra_args.chdir = NULL;
  extra_args.confirmation = NOCONFIRM;

  if(ampersand) extra_args.output_option = NO_OUT;
  else extra_args.output_option = OUTS;

  command->smashkey       = 0;
  command->extra_args     = extra_args;

  if(ampersand){
    command->option       = NOWAIT;
  } else {
    command->option       = STOP;
  }

  command->pre_animation  = none;
  command->post_animation = start_animation;
  command->cmd            = cmd;

  __execute__(command);

  char** free_me = (char**)command->cmd;
  for(int i = 0; i < count + 1; i++){
    free(free_me[i]);
  }
  free((char**)command->cmd);
  free((void*)command);

  return 0;
}

void _args(int argc, char* argv[]){
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--help") == 0) get_helped(0);
    else if (strcmp(argv[i], "--version") == 0) version();
  }

  int opt;
  while ((opt = getopt(argc, argv, "f:b:vVhrHFC")) != -1){
    switch (opt) {
      case 'r':
        _so_random();
        break;
      case 'H': HOLOGRAPHIC = 1; break;
      case 'F': FOLLOW = 1; break;
      case 'C': _state->preflight_check = true; break;
      case 'v': version(); break;
      case 'V': version(); break;
      case 'f': _state->fg_c = input_color(optarg); break;
      case 'b': _state->bg_c = input_color(optarg); break;
      case 'h': get_helped(0); break;
      default: exit(1);
    }
  }
}

void _init_state(){

  _state = (state *)malloc(sizeof(state));

  _state->show_colors       = false;              /* implemented */
  _state->interrupt         = false;              /* implemented */
  _state->preflight_check   = false;              /* implemented */
  _state->last_input_time   = clock();            /* implemented */
  _state->t_rows            = 1;                  /* implemented */
  _state->t_cols            = 1;                  /* implemented */
  _state->t_cache           = 10;                 /* implemented */
  _state->win_size          = t_full;             /* implemented */
  _state->fg_c              = fg_c;               /* implemented */
  _state->bg_c              = bg_c;               /* implemented */
  _state->hd_c              = hd_c;               /* implemented */
  if(rand_c) _so_random();
  _state->glitch_frame_time = 3500;               /* implemented */
  _state->hold_char_time    = 0.00001;            /* implemented */
  _state->hold_char         = '\0';               /* implemented */
  memset(_state->slap_str, 0, 256);
  _state->key_lock          = 0;                  /* implemented */
  _state->reprint           = true;               /* implemented */
  _state->fx                = start_fx;           /* implemented */

  _state->fg_state          = (char**)malloc(FG_GLYPH_HEIGHT * sizeof(char*));
  _state->bg_state          = (char**)malloc(BG_GLYPH_HEIGHT * sizeof(char*));

  for(int i = 0; i < FG_GLYPH_HEIGHT; i++){
    _state->fg_state[i] = (char*)malloc(FG_GLYPH_LENGTH * sizeof(char));
  }

  for(int i = 0; i < BG_GLYPH_HEIGHT; i++){
    _state->bg_state[i] = (char*)malloc(BG_GLYPH_LENGTH * sizeof(char));
  }
}

void _reset_glyph_state(){

  for(int i = 0; i < BG_GLYPH_HEIGHT; i++){
    mbstate_t state;
    memset(&state, 0, sizeof(mbstate_t));
    const char *iter_row = bg[i];
    int iter_col = 0;
    while (*iter_row) {

      wchar_t wc;
      size_t len = mbrtowc(&wc, iter_row, MB_CUR_MAX, &state);

      if(*iter_row == ' '){
        _state->bg_state[i][iter_col] = 0;
      } else {
        _state->bg_state[i][iter_col] = 1;
      }

      iter_row += len;
      iter_col++;
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
        _state->fg_state[i][iter_col] = 0;
      } else {
        _state->fg_state[i][iter_col] = 1;
      }

      iter_row += len;
      iter_col++;
    }
  }
}

void _so_random(){
  _state->bg_c = (rand() % 7) + 1;    // RNG 1 and 7
  _state->fg_c = (rand() % 7) + 2;    // RNG 2 and 8
  _state->hd_c = (rand() % 7) + 1;        // RNG 1 and 7
  while(_state->fg_c == _state->bg_c){
    _state->fg_c = rand() % 7 + 2;
  }
}

void version(){
  printf("%s %s \n", PROGRAM_NAME, PROGRAM_VERSION);
  _quit(0);
}

void _get_dims(){
  getmaxyx(stdscr, _state->t_rows, _state->t_cols);
}

int  _resized(){
  int ret_val = 0;
  if(_state->t_cache != _state->t_rows + _state->t_cols) ret_val = 1;
  else ret_val = 0;

  _state->t_cache = _state->t_rows + _state->t_cols;

  return ret_val;
}

char * _run_fetch(){
  ensure_cache_dir();

  const char *env_home = getenv("HOME");
  const char *home_to_fetch = "/.cache/sara/fetch";
  char * fetch_path = (char*)malloc(256 * sizeof(char));
  sprintf(fetch_path, "%s%s", env_home, home_to_fetch);

  pid_t pid = fork();
  if (pid < 0) {
    crit("forked it up");
  } else if (pid == 0) {

    int fd = open(fetch_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    dup2(fd, 1);
    dup2(fd, 2);
    close(fd);

    execlp("fastfetch", NULL);
    perror("execl");
    crit("fastfetch exec critical err");
  }
  int status;
  while(kill(pid, 0) == 0){
    waitpid(pid, &status, 0);
  }
  return fetch_path;
}

void _fetch_reduce(char* fetch_path){

  FILE* fp = fopen(fetch_path, "r");
  if(!fp){
    exit(1);
  }
  char line[256] = {'\0'};

  if(!fgets(line, sizeof(line), fp)){
    fclose(fp);
    exit(1);
  }

  char * user = getenv("USER");
  if(!user){
    fclose(fp);
    exit(1);
  }

  char * match = strstr(line, user);
  if(!match){
    fclose(fp);
    exit(1);
  }

  int offset = (int)(match - line);
  memmove(line, line + offset, strlen(line + offset) + 1);

  const char *home_to_output = "/.cache/sara/stripped_fetch";
  const char *env_home = getenv("HOME");
  if(!env_home){
    fclose(fp);
    exit(1);
  }

  char output_file[512] = {'\0'};
  snprintf(output_file, sizeof(output_file), "%s%s", env_home, home_to_output);

  FILE* fp_out = fopen(output_file, "w");
  if(!fp_out){
    fclose(fp);
    exit(1);
  }
  fputs(line, fp_out);

  while(fgets(line, sizeof(line), fp)){
    size_t len = strlen(line);
    if(len <= (size_t)offset || strspn(line + offset, " \t\r\n") == len - offset){
      break;
    }
    memmove(line, line + offset, len - offset + 1);
    fputs(line, fp_out);
  }

  fclose(fp);
  fclose(fp_out);

  char mv_cmd[256];
  sprintf(mv_cmd, "%s %s %s", "mv", output_file, fetch_path);
  system(mv_cmd);
}


int _longest_line(const char *path){

  FILE *fp = fopen(path, "r");
  if(!fp) return 0;
  
  char *line = NULL;
  size_t capacity = 0;
  int longest = 0;
  
  ssize_t len;
  
  while((len = getline(&line, &capacity, fp)) != -1){

    // Exclude trailing newline
    if(len > 0 && line[len - 1] == '\n') len--;
    if(len > longest) longest = len;
  }

  free(line);
  fclose(fp);

  return longest;
}
