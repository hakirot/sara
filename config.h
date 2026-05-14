#ifndef CONFIG_H
#define CONFIG_H
#include "globals.h"
#include "wchar.h"

/*   ██████╗ ██████╗ ███╗   ██╗███████╗██╗ ██████╗   --
--  ██╔════╝██╔═══██╗████╗  ██║██╔════╝██║██╔════╝   --
--  ██║     ██║   ██║██╔██╗ ██║█████╗  ██║██║  ███╗  --
--  ██║     ██║   ██║██║╚██╗██║██╔══╝  ██║██║   ██║  --
--  ╚██████╗╚██████╔╝██║ ╚████║██║     ██║╚██████╔╝  --
--   ╚═════╝ ╚═════╝ ╚═╝  ╚═══╝╚═╝     ╚═╝ ╚═════╝.h --
                                                                         */

static const int run_preflight_check = false;
static const animation_option start_animation = neon;

/* Toggle if you wrap sara in chdir command */
static int FOLLOW = true;

/* COLOR SETTINGS */
static const int fg_c = red;         /* fg color */
static const int bg_c = yellow;      /* bg color */
static const int hd_c = magenta;     /* hd color */

static const int use_bold_color_for_bg = true;
static const int use_bold_color_for_fg = true;
static const int use_bold_color_for_hd = true;

/* header highlight options*/
static const int highlight_hd_in_full_mode  = 0;
static const int highlight_hd_in_small_mode = 0;

/* CUSTOM GLYPHS */ 

    /* RESTRICTIONS

       Ensure strings in character array are equal length 

       im[], if defined, must have identical dimensions to fg[]   */


/* background glyph */
//static const char * bg[] = {
//  "                     -`                     ",
//  "                    .o+`                    ",
//  "                   `ooo/                    ",
//  "                  `+oooo:                   ",
//  "                 `+oooooo:                  ",
//  "                 -+oooooo+:                 ",
//  "               `/:-:++oooo+:                ",
//  "              `/++++/+++++++:               ",
//  "             `/++++++++++++++:              ",
//  "            `/+++ooooooooooooo/`            ",
//  "           ./ooosssso++osssssso+`           ",
//  "          .oossssso-````/ossssss+`          ",
//  "         -osssssso.      :ssssssso.         ",
//  "        : sssssss/        osssso+++.        ",
//  "       /ossssssss/        +ssssooo/-        ",
//  "     `/ossssso+/:-        -:/+osssso+-      ",
//  "    `+sso+:-`                 `.-/+oso:     ",
//  "   `++:.                           `-/+/    ",
//  "   .`                                 `/    ",
//};

//static const char * bg[] = {
//  " ████    ████    ████          ",
//  "  ████    ████    ████         ",
//  "    ███     ███     ███        ",
//  "     ███     ███     ███       ",
//  "     █ ██    █ ██    █ ██      ",
//  "        ██      ██      ██     ",
//  "         ██      ██      ██    ",
//  "          ██      ██      ██   ",
//  "           ██      ██      ██  ",
//  "            █       █       █  ",
//  "             █       █       █ ",
//};

static const char * bg[] = {
  "                                  *                   *  ",
  "                         ✦                     ,         ",
  "                                ,      *                 ",
  "                                                        ✦",
  "    .             ✦           ,           *    .         ",
  "        .             ✦             ✦                    ",
  "   *           .                          ✦              ",
  "           .   ✦        ,         *                      ",
  "          ,                                              ",
  "        .                                                ",
  "  ✦        ,              .                              ",
  "   *           .                                         ",
  "         .             .                                 ",
  "˚          ,      .                                      ",
  "          .                                              ",
  "   *            ✦                                        ",
  "        .                                                ",
};


/* foreground glyph */
static const char * fg[] = {
  "███████╗    █████╗    ██████╗     █████╗    ",
  "██╔════╝   ██╔══██╗   ██╔══██╗   ██╔══██╗   ",
  "███████╗   ███████║   ██████╔╝   ███████║   ",
  "╚════██║   ██╔══██║   ██╔══██╗   ██╔══██║   ",
  "███████║██╗██║  ██║██╗██║  ██║██╗██║  ██║██╗",
  "╚══════╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝",
};


/* optional intermediate glyph - an effect layer for the following animations:
   - glitch
   - neon
*/
static const char * im[] = {
  "╔══════╗    ╔════╗    ╔═════╗     ╔════╗    ",
  "║ ╔════╝   ╔╝╔══╗╚╗   ║ ╔══╗╚╗   ╔╝╔══╗╚╗   ",
  "║ ╚════╗   ║ ╚══╝ ║   ║ ╚══╝╔╝   ║ ╚══╝ ║   ",
  "╚════╗ ║   ║ ╔══╗ ║   ║ ╔══╗╚╗   ║ ╔══╗ ║   ",
  "╔════╝ ║╔═╗║ ║  ║ ║╔═╗║ ║  ║ ║╔═╗║ ║  ║ ║╔═╗",
  "╚══════╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝",
};
/* Set empty if desired */
//static const char * im[] = {NULL};


/* optional header */
static const char hd[] = "SPECIAL APPLICATION RANGING AREA";
/* Set empty if desired */
//static const char hd[] = {'\0'};


/* POSITION */
/* Reposition glyphs here
   - Glyphs are printed centered to screen
   - Positive x values will position glyphs DOWNWARD
   - Negative x values will position glyphs UPWARD
*/
static const int bg_offset_y = -1;
static const int bg_offset_x = -2;
static const int fg_offset_y = 0;
static const int fg_offset_x = 0;

/* header position when fullsize mode */
static const int hd_offset_y = 0;
static const int hd_offset_x = -9;

/* header position when small window mode */
static const int hd_offset_y_min = 0;
static const int hd_offset_x_min = -9;

/* Set dynamic_resize to skip printing bg at defined constraints */
static const int dynamic_resize = true; // TODO ensure these value is greater than tiny mode
static const int resize_x = 44;
static const int resize_y = 7;

/* Set tiny_mode to */
static const int tiny_mode = true;
static const char tn[] = "S.A.R.A";
static const int tiny_mode_y = 7;
static const int tiny_mode_x = 44;


/* automate this and perform checks [make a perform check cmd-line option]
     - ensure no duplicate chars */

/* ANIMATION SETTINGS */

/* ANIMATION OPTIONS:

  print
  down_wipes
  glitch
  neon
  neon_reverse
  shutter_slide
  pixel_fill
  tv_static

*/

static const int GLITCH_FRAME_TIME = 3500;
static const float HOLD_CHAR_TIME = 0.00001;

/* CUSTOM COMMANDS */
static const char *test_cmd[]         = { "mkdir", "TEST_DIR_PLS_DELETE",        NULL};
static const char *r_cmd[]            = { "ranger",                              NULL};
static const char *R_cmd[]            = { "rtorrent",                            NULL};
static const char *w_cmd[]            = { "ranger", "/home/hakirot/pix/walls/",      NULL};
static const char *t_cmd[]            = { "nvim", "/home/hakirot/dox/.notes/tasks",  NULL};
static const char *P_cmd[]            = { "nohup", "bash", "-c", "bar.sh",       NULL};
//static const char *newlook_cmd[]      = { "reskin",                              NULL};
static const char *v_cmd[]            = { "nvim",                                NULL};
static const char *V_cmd[]            = { "xdo", "1",                            NULL};
static const char *O_cmd[]            = { "xdo", "2",                            NULL};
static const char *F_cmd[]            = { "xdo", "3",                            NULL};
static const char *y_cmd[]            = { "yay",                                 NULL};
static const char *m_cmd[]            = { "make",                                NULL};
static const char *M_cmd[]            = { "rmpc",                                NULL};
static const char *Q_cmd[]            = { "tmux", "kill-pane",                   NULL};
static const char *X_cmd[]            = { "nohup", "bash", "-c", "kill-session", NULL};


static const ExtraArgs defaults = {
  NULL,        /* Optionally change directory before executing command */
  NOCONFIRM,   /* Optionally confirm command with: CONFIRM             */
  OUTS,        /* Optionally suppress command output with NO_OUT       */
};

/* CUSTOM COMMAND CONFIG */
/* TODO: issue a warning of duplicates or crash if necessary in a precheck function*/
/* TODO: add optional Args: chdir (tasks, rtorrent, walls), confirm (newlook), no_output */
static const Command commandkeys[] = {
/*  KEY        COMMAND       WAIT_OPTION     PRE_ANIMATION,        POST ANIMATION   ARGS    */
//{ 'a',      test_cmd,             WAIT,     neon_reverse,                  neon,   {"/home/hakirot/dls", NOCONFIRM, OUTS} },
  { 'r',         r_cmd,             WAIT,     neon_reverse,                  neon,                             defaults },
  { 'R',         R_cmd,             WAIT,     neon_reverse,                  neon,   {"/home/hakirot/dls", NOCONFIRM, OUTS} },
  { 'w',         w_cmd,             WAIT,     neon_reverse,                  neon,                             defaults },
  { 't',         t_cmd,             WAIT,      glitch_full,                  neon,                             defaults },
  { 'P',         P_cmd,             WAIT,             none,                  none,            {NULL, NOCONFIRM, NO_OUT} },
//{ 'n',   newlook_cmd,             EXEC,      glitch_full,                  neon,                             defaults },
  { 'v',         v_cmd,             WAIT,      glitch_full,                  neon,                             defaults },
  { 'V',         V_cmd,             WAIT,     neon_reverse,                  neon,                             defaults },
  { 'O',         O_cmd,             WAIT,     neon_reverse,                  neon,                             defaults },
//{ 'F',         F_cmd,             EXEC,             none,                  neon,                             defaults },
  { 'y',         y_cmd,      STOP_ON_ERR,     neon_reverse,                  neon,                             {NULL, CONFIRM, OUTS} },
  { 'm',         m_cmd,      STOP_ON_ERR,             none,    shutter_slide_neon,                             defaults },
  { 'M',         M_cmd,             WAIT,             none,            pixel_fill,                             defaults },
  { 'Q',         Q_cmd,             EXEC,             none,                  none,                             defaults },
  { 'X',         X_cmd,             EXEC,             none,                  none,            {NULL, NOCONFIRM, NO_OUT} },
};


/* BUILTIN FUNCTIONS */
static const Builtin builtinkeys[] = {
  { 'I', randomize_colors },
  { 'i', invert_colors },
  { 'H', rave },
  { 'p', pshd },
  { 'q', quit },
  { 'D', check },
};


/* MENUS CONFIGURATION */

static const wchar_t MenuBorder[] = L"╔╗╚╝═║";
static const unsigned short menu_c            = fg_c;
static const unsigned short bold_color_menu   = use_bold_color_for_fg; // TODO: implement
static const unsigned short menu_x            = 44;       // Must be > 2
static const unsigned short menu_y            = 7;        // Must be > 2
static const unsigned short menu_offset_y     = 0;
static const unsigned short menu_offset_x     = 0;

/* MENU b: BLUETOOTH/BACKLIGHT) */
static const char *connect_xm5_cmd[] = { "bluetoothctl", "connect", "AC:80:0A:19:89:A8", NULL };
static const Command connect_xm5 = { 0, connect_xm5_cmd, STOP_ON_ERR, none, shutter_slide, defaults };

static const char *connect_acoustic_cmd[] = { "bluetoothctl", "connect", "FC:58:FA:9B:D7:3D", NULL };
static const Command connect_acoustic = { 0, connect_acoustic_cmd, STOP_ON_ERR, none, shutter_slide, defaults };

static const char *connect_onforu_cmd[] = { "bluetoothctl", "connect", "31:51:27:F9:1D:62", NULL };
static const Command connect_onforu = { 0, connect_onforu_cmd, STOP_ON_ERR, none, shutter_slide, defaults };

static const char *disconnect_xm5_cmd[] = { "bluetoothctl", "disconnect", "AC:80:0A:19:89:A8", NULL };
static const Command disconnect_xm5 = { 0, disconnect_xm5_cmd, STOP_ON_ERR, none, shutter_slide, defaults };

static const char *disconnect_acoustic_cmd[] = { "bluetoothctl", "disconnect", "FC:58:FA:9B:D7:3D", NULL };
static const Command disconnect_acoustic = { 0, disconnect_acoustic_cmd, STOP_ON_ERR, none, shutter_slide, defaults };

static const char *disconnect_onforu_cmd[] = { "bluetoothctl", "disconnect", "31:51:27:F9:1D:62", NULL };
static const Command disconnect_onforu = { 0, disconnect_onforu_cmd, STOP_ON_ERR, none, shutter_slide, defaults };

static const char *fake_backlight_cmd[] = { "mkdir", "FAKE_BACKLIGHT_CMD", NULL };
static const Command fake_backlight = { 0, fake_backlight_cmd, STOP_ON_ERR, none, shutter_slide, defaults };

static const Menu connect_devices[] = {
  { "XM5",      COMMAND, { .command = connect_xm5         } },
  { "ACOUSTIC", COMMAND, { .command = connect_acoustic    } },
  { "ONFORU",   COMMAND, { .command = connect_onforu      } },
  END_OF_MENU
};

static const Menu disconnect_devices[] = {
  { "XM5",      COMMAND, { .command = disconnect_xm5      } },
  { "ACOUSTIC", COMMAND, { .command = disconnect_acoustic } },
  { "ONFORU",   COMMAND, { .command = disconnect_onforu   } },
  END_OF_MENU
};

static const Menu connect_options[] = {
  { "connect",    SUBMENU, { .submenu = connect_devices    } },
  { "disconnect", SUBMENU, { .submenu = disconnect_devices } },
  END_OF_MENU
};

static const Menu b_menu[] = {
  { "bluetooth", SUBMENU, { .submenu = connect_options } },
  END_OF_MENU
};

/* MENU S: SHUTDOWN COMMANDS */
static const char *shutdown_cmd[] = { "shutdown", "now", NULL };
static const Command shutdown = { 0, shutdown_cmd, EXEC, none, none, {NULL, CONFIRM, NO_OUT} };

static const char *reboot_cmd[] = { "shutdown", "-r", "now", NULL };
static const Command reboot = { 0, reboot_cmd, EXEC, none, none, defaults  };

static const Menu S_menu[] = {
  { "SHUTDOWN", COMMAND, { .command = shutdown } },
  { "REBOOT",   COMMAND, { .command = reboot   } },
  END_OF_MENU
};

static const char *firefox_cmd[] = { "nohup", "firefox", NULL };
static const Command firefox = { 0, firefox_cmd, WAIT, glitch_full, glitch, {NULL, CONFIRM, NO_OUT}  };

static const Menu F_menu[] = {
  { "FIREFOX", COMMAND, { .command = firefox } },
  END_OF_MENU
};

/* menukey assignments */
static const MenuKey menukeys[] = {
  {'b', b_menu },
//{'S', S_menu },
  {'F', F_menu },
};

#endif
