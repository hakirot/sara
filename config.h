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
static const animation_option start_animation = blink;

/* Toggle if you wrap sara in chdir command */
static int FOLLOW = true;

/* COLOR SETTINGS */
static const int fg_c = magenta;         /* fg color */
static const int bg_c = yellow;      /* bg color */
static const int hd_c = green;     /* hd color */

static const int use_bold_color_for_fg = false;
static const int use_bold_color_for_bg = false;
static const int use_bold_color_for_hd = true;

/* header highlight options*/
static const int highlight_hd_in_full_mode  = false;
static const int highlight_hd_in_small_mode = false;


/* PSHD UI CONFIG */

/* pshd window COLOR    */
static const unsigned short pshd_c            = red;
static const unsigned short bold_color_pshd   = false;

/* pshd dimensions      */
static const unsigned short pshd_y =          15;
static const unsigned short pshd_x =          28;

/* pshd position        */
static const short pshd_offset_y              = -200;
static const short pshd_offset_x              = 200;

/* MENU UI CONFIG       */

//static const wchar_t MenuBorder[] =         L"┌┐└┘─│";
//static const wchar_t MenuBorder[] =         L"╔╗╚╝═║";
static const wchar_t MenuBorder[] =           L"╔╗╚╝─│";

/* pshd window color  */
static const unsigned short menu_c            = cyan;
static const unsigned short bold_color_menu   = false;

/* pshd dimensions    */
static const unsigned short menu_y            = 12;
static const unsigned short menu_x            = 16;

/* pshd position      */
static const short menu_offset_y              = -200;
static const short menu_offset_x              = 200;


/* CUSTOM GLYPHS */ 

static const char * bg[] = {
  " ________            ________                ",
  " \\   ____\\           \\   __  \\               ",
  "  \\  \\_____           \\  \\_\\  \\              ",
  "   \\_____  \\           \\   _  _\\             ",
  "    _____\\  \\           \\  \\\\  \\             ",
  "    \\________\\           \\__\\\\ _\\            ",
};

static const char * fg[] = {
  "           ________            ________      ",
  "           \\   __  \\           \\   __  \\     ",
  "            \\  \\_\\  \\           \\  \\_\\  \\    ",
  "             \\   __  \\           \\   __  \\   ",
  "              \\  \\ \\  \\           \\  \\ \\  \\  ",
  "               \\__\\ \\__\\           \\__\\ \\__\\ ",
};

//static const char * bg[] = {
//  "___  ___  ________  ___  ___   ___      ",
//  "\\  \\ \\  \\ \\   __  \\ \\  \\ \\  \\  \\  \\    ",
//  " \\  \\_\\  \\ \\  \\_\\  \\ \\  \\/  /__ \\  \\   ",
//  "  \\   __  \\ \\   __  \\ \\   ___  \\ \\  \\  ",
//  "   \\  \\ \\  \\ \\  \\ \\  \\ \\  \\  \\  \\ \\  \\ ",
//  "    \\__\\ \\__\\ \\__\\ \\__\\ \\__\\  \\__\\ \\__\\",
//};

//static const char * fg[] = {
//  "________  ________  __________     ",
//  "\\   __  \\ \\   __  \\ \\___   ___\\   ",
//  " \\  \\_\\  \\ \\  \\ \\  \\    \\  \\      ",
//  "  \\   _  _\\ \\  \\ \\  \\    \\  \\    ",
//  "   \\  \\\\  \\  \\  \\_\\  \\    \\  \\   ",
//  "    \\__\\\\ _\\  \\_______\\    \\__\\  ",
//};

/* background glyph */
// static const char * bg[] = {
//   "                        -`                     ",
//   "                       .o+`                    ",
//   "                      `ooo/                    ",
//   "                     `+oooo:                   ",
//   "                    `+oooooo:                  ",
//   "                    -+oooooo+:                 ",
//   "                                               ",
//   "                                               ",
//   "                                               ",
//   "                                               ",
//   "                                               ",
//   "                                               ",
//   "            -osssssso.      :ssssssso.         ",
//   "           : sssssss/        osssso+++.        ",
//   "          /ossssssss/        +ssssooo/-        ",
//   "        `/ossssso+/:-        -:/+osssso+-      ",
//   "       `+sso+:-`                 `.-/+oso:     ",
//   "      `++:.                           `-/+/    ",
//   "      .`                                 `/    ",
// };

// static const char * bg[] = {
//   "                     -`                     ",
//   "                    .o+`                    ",
//   "                   `ooo/                    ",
//   "                  `+oooo:                   ",
//   "                 `+oooooo:                  ",
//   "                 -+oooooo+:                 ",
//   "               `/:-:++oooo+:                ",
//   "              `/++++/+++++++:               ",
//   "             `/++++++++++++++:              ",
//   "            `/+++ooooooooooooo/`            ",
//   "           ./ooosssso++osssssso+`           ",
//   "          .oossssso-````/ossssss+`          ",
//   "         -osssssso.      :ssssssso.         ",
//   "        : sssssss/        osssso+++.        ",
//   "       /ossssssss/        +ssssooo/-        ",
//   "     `/ossssso+/:-        -:/+osssso+-      ",
//   "    `+sso+:-`                 `.-/+oso:     ",
//   "   `++:.                           `-/+/    ",
//   "   .`                                 `/    ",
// };

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

//static const char * bg[] = {
//  "                                  *                   *  ",
//  "                         ✦                     ,         ",
//  "                                ,      *                 ",
//  "                                                        ✦",
//  "    .             ✦           ,           *    .         ",
//  "        .             ✦             ✦                    ",
//  "   *           .                          ✦              ",
//  "           .   ✦        ,         *                      ",
//  "          ,                                              ",
//  "        .                                                ",
//  "  ✦        ,              .                              ",
//  "   *           .                                         ",
//  "         .             .                                 ",
//  "˚          ,      .                                      ",
//  "          .                                              ",
//  "   *            ✦                                        ",
//  "        .                                                ",
//};

//static const char * fg[] = {
//  "    ________  ________  ________  ________     ",
//  "   |\\   ____\\|\\   __  \\|\\   __  \\|\\   __  \\    ",
//  "   \\ \\  \\___|\\ \\  \\|\\  \\ \\  \\|\\  \\ \\  \\|\\  \\   ",
//  "    \\ \\_____  \\ \\   __  \\ \\   _  _\\ \\   __  \\  ",
//  "     \\|____|\\  \\ \\  \\ \\  \\ \\  \\\\  \\\\ \\  \\ \\  \\ ",
//  "       ____\\_\\  \\ \\__\\ \\__\\ \\__\\\\ _\\\\ \\__\\ \\__\\",
//  "      |\\_________\\|__|\\|__|\\|__|\\|__|\\|__|\\|__|",
//  "      \\|_________|                             ",
//};

//static const char * fg[] = {
// "        /\\        ",
// "       /  \\       ",
// "      /    \\      ",
// "     _\\     \\     ",
// "    /        \\    ",
// "   /          \\   ",
// "  /     __   \\_\\  ",
// " /     /  \\     \\ ",
// "/__,--'    '--,__\\",
//};

//static const char * bg[] = {
//  "                     -`                     ",
//  "                    .o+`                    ",
//  "                   `ooo/                    ",
//  "                  `+oooo:                   ",
//  "                 `+oo  oo:                  ",
//  "                 -+o   oo+                  ",
//  "               `/:- /   o                   ",
//  "              `/++                          ",
//  "             `/++ /                         ",
//  "            `/++ /                          ",
//  "           ./oo                             ",
//  "          .oos                              ",
//  "         -oss                               ",
//  "        : ss    s/                          ",
//  "       /oss   -/:                           ",
//  "     `/oss   -/                             ",
//  "    `+sso   -                               ",
//  "   `++:.                                    ",
//  "   .`                                       ",
//};

//static const char * fg[] = {
// "     |-|           ",
// " |-|               ",
// "      |-|          ",
//};

//static const char * bg[] = {
//  "      ,------..         ",
//  "   ,-\":%%%%::==.        ",
//  "  /:::__ ::%::==        ",
//  " /::.'  `.:%%=:=`-=,    ",
//  "Y:::f    j :%%%%:::=::  ",
//  "l   `.__+ :::%%%%:::_;[ ",
//  "|^~'-------------\"\"~:^| ",
//  "! ::::::::::%%%%==:{    ",
//  " \\ `::====: ==== :='    ",
//  "  \\:== :: == :=='       ",
//  "   \"-. =_::::           ",
//  "      `-------'         ",
//};

//static const char * bg[] = {
//  "      ,------..             ",
//  "   ,-\":%%%%::==.             ",
//  "  /:::__ ::%::==            ",
//  " /::.'  `.:%%=:=`-=,        ",
//  "Y:::f    j :%%%%:::=::      ",
//  "l   `.__+ :::%%%%:::_;[     ",
//  "|^~'-------------\"\"~:^|     ",
//  "! ::::::::::%%%%==:{        ",
//  " \\ `::====: ==== :='  |-|    ",
//  "  \\:== :: == :=='       |-|  ",
//  "   \"-. =_::::      |-|       ",
//  "      `-------'             ",
//};





/* foreground glyph */
//static const char * fg[] = {
//  "███████╗    █████╗    ██████╗     █████╗    ",
//  "██╔════╝   ██╔══██╗   ██╔══██╗   ██╔══██╗   ",
//  "███████╗   ███████║   ██████╔╝   ███████║   ",
//  "╚════██║   ██╔══██║   ██╔══██╗   ██╔══██║   ",
//  "███████║██╗██║  ██║██╗██║  ██║██╗██║  ██║██╗",
//  "╚══════╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═╝",
//};

//static const char * fg[] = { "",};

/* optional intermediate glyph - an effect layer for the following animations:
   - glitch
   - neon
*/

/* optional header */
//static const char hd[] = "SPECIAL APPLICATION RANGING AREA";
//static const char hd[] = "© Entity"; // TODO make this work
/* Set empty if desired */
static const char hd[] = {'\0'};


/* POSITION */
/* Reposition glyphs here
   - Glyphs are printed centered to screen
   - Positive x values will position glyphs DOWNWARD
   - Negative x values will position glyphs UPWARD
*/
static const int bg_offset_y = 0;
static const int bg_offset_x = 0;
static const int fg_offset_y = 0;
static const int fg_offset_x = 0;
static const int hd_offset_y = 1;
static const int hd_offset_x = 22;

/* header position when small window mode */
static const int hd_offset_y_min = -1;
static const int hd_offset_x_min = -34;

/* Set dynamic_resize to skip printing bg at defined constraints */
static const int dynamic_resize = true;
static const int resize_x = 10;
static const int resize_y = 3;

/* Set tiny_mode to */
static const int tiny_mode = true;
static const char tn[] = "SARA";
static const int tiny_mode_y = 3;
static const int tiny_mode_x = 10;


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
  shutter_slide_neon
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
static const char *n_cmd[]            = { "respawn.sh",                          NULL};
static const char *x_cmd[]            = { "xcolor",                              NULL};
static const char *N_cmd[]            = { "fastfetch",                           NULL};
static const char *G_cmd[]            = { "nodemon",                             NULL};
static const char *Reload_cmd[]       = { "sara_reload",                         NULL};

static const ExtraArgs defaults = {
  NULL,        /* Optionally change directory before executing command */
  NOCONFIRM,   /* Optionally confirm command with: CONFIRM             */
  OUTS,        /* Optionally suppress command output with NO_OUT       */
};

static const ExtraArgs confirms = {
  NULL,        /* Optionally change directory before executing command */
  CONFIRM,     /* Optionally confirm command with: CONFIRM             */
  OUTS,        /* Optionally suppress command output with NO_OUT       */
};

/* CUSTOM COMMAND CONFIG */
static const Command commandkeys[] = {
/*  KEY        COMMAND       WAIT_OPTION     PRE_ANIMATION,        POST ANIMATION   ARGS    */
//{ 'a',      test_cmd,             WAIT,     neon_reverse,                  neon,   {"/home/hakirot/dls", NOCONFIRM, OUTS} },
  { 'r',         r_cmd,             WAIT,     neon_reverse,                  neon,                             defaults },
  { 'R',         R_cmd,             WAIT,     neon_reverse,                  neon,   {"/home/hakirot/dls", NOCONFIRM, OUTS} },
  { 'w',         w_cmd,             WAIT,     neon_reverse,                  neon,                             defaults },
  { 't',         t_cmd,             WAIT,      glitch_full,                  neon,              {"/home/hakirot/dox/.notes", NOCONFIRM, OUTS} },
//{ 'n',   newlook_cmd,             EXEC,      glitch_full,                  neon,                             defaults },
  { 'v',         v_cmd,             WAIT,      glitch_full,                  neon,                             defaults },
  { 'V',         V_cmd,             WAIT,     neon_reverse,                  neon,                             confirms },
  { 'O',         O_cmd,             WAIT,     neon_reverse,                  neon,                             confirms },
//{ 'F',         F_cmd,             EXEC,             none,                  neon,                             confirms },
  { 'y',         y_cmd,      STOP_ON_ERR,     neon_reverse,                  neon,                             {NULL, CONFIRM, OUTS} },
  { 'm',         m_cmd,      STOP_ON_ERR,             none,    shutter_slide_neon,                             defaults },
  { 'M',         M_cmd,             WAIT,     neon_reverse,            pixel_fill,                             defaults },
  { 'Q',         Q_cmd,             EXEC,             none,                  none,                             defaults },
  { 'X',         X_cmd,             EXEC,             none,                  neon,            {NULL, NOCONFIRM, NO_OUT} },
  { 'n',         n_cmd,             WAIT,     neon_reverse,                  none,                             confirms },
  { 'x',         x_cmd,             STOP,             none,                  neon,                             defaults },
  { 'N',         N_cmd,             STOP,             none,                  neon,                             defaults },
  { 'G',         G_cmd,             WAIT,             none,                  neon,                             defaults },
  { ':',         Reload_cmd,        EXEC,             none,                  neon,                             defaults },
};


/* BUILTIN FUNCTIONS */
static const Builtin builtinkeys[] = {
  { 'I', randomize_colors },
  { 'i', invert_colors },
  { 'H', rave },
  { 'p', pshd },
  { 'q', quit },
//{ 'C', check },
  { 'C', colors },
};



/* MENU b: BLUETOOTH/BACKLIGHT) */
static const char *connect_xm5_cmd[] = { "bluetoothctl", "connect", "AC:80:0A:19:89:A8", NULL };
static const Command connect_xm5 = { 0, connect_xm5_cmd, STOP_ON_ERR, none, shutter_slide_neon, defaults };

static const char *connect_acoustic_cmd[] = { "bluetoothctl", "connect", "FC:58:FA:9B:D7:3D", NULL };
static const Command connect_acoustic = { 0, connect_acoustic_cmd, STOP_ON_ERR, none, shutter_slide_neon, defaults };

static const char *connect_onforu_cmd[] = { "bluetoothctl", "connect", "31:51:27:F9:1D:62", NULL };
static const Command connect_onforu = { 0, connect_onforu_cmd, STOP_ON_ERR, none, shutter_slide_neon, defaults };

static const char *disconnect_xm5_cmd[] = { "bluetoothctl", "disconnect", "AC:80:0A:19:89:A8", NULL };
static const Command disconnect_xm5 = { 0, disconnect_xm5_cmd, STOP_ON_ERR, none, shutter_slide_neon, defaults };

static const char *disconnect_acoustic_cmd[] = { "bluetoothctl", "disconnect", "FC:58:FA:9B:D7:3D", NULL };
static const Command disconnect_acoustic = { 0, disconnect_acoustic_cmd, STOP_ON_ERR, none, shutter_slide_neon, defaults };

static const char *disconnect_onforu_cmd[] = { "bluetoothctl", "disconnect", "31:51:27:F9:1D:62", NULL };
static const Command disconnect_onforu = { 0, disconnect_onforu_cmd, STOP_ON_ERR, none, shutter_slide_neon, defaults };

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

static const char *boot_spawns_cmd[] = { "boot_spawns", NULL };
static const Command boot_spawns = { 0, boot_spawns_cmd, EXEC, glitch_full, down_wipes, confirms };

static const Menu b_menu[] = {
  { "BLUETOOTH", SUBMENU, { .submenu = connect_options } },
  { "boot_spawns", COMMAND, { .command = boot_spawns } },
  END_OF_MENU
};

/* MENU S: SHUTDOWN COMMANDS */
static const char *shutdown_cmd[] = { "/home/hakirot/skps/shutdown.sh", NULL };
static const Command shutdown = { 0, shutdown_cmd, EXEC, none, none, {NULL, CONFIRM, NO_OUT} };

static const char *reboot_cmd[] = { "shutdown", "-r", "now", NULL };
static const Command reboot = { 0, reboot_cmd, EXEC, none, none, {NULL, CONFIRM, NO_OUT} };

static const Menu S_menu[] = {
  { "SHUTDOWN", COMMAND, { .command = shutdown } },
  { "REBOOT",   COMMAND, { .command = reboot   } },
  END_OF_MENU
};

static const char *firefox_cmd[] = { "nohup", "firefox", NULL };
static const Command firefox = { 0, firefox_cmd, NOWAIT, glitch, down_wipes, {NULL, NOCONFIRM, NO_OUT}  };

static const char *slippi_cmd[] = { "nohup", "slippi", NULL };
static const Command slippi = { 0, slippi_cmd, NOWAIT, glitch_full, down_wipes, {NULL, NOCONFIRM, NO_OUT}  };

static const char *discord_cmd[] = { "discord", NULL };
static const Command discord = { 0, discord_cmd, NOWAIT, glitch_full, down_wipes, {NULL, NOCONFIRM, NO_OUT}  };

static const char *st_cmd[] = { "st", NULL };
static const Command st = { 0, st_cmd, NOWAIT, glitch_full, glitch_full, {NULL, NOCONFIRM, NO_OUT}  };

static const char *kitty_cmd[] = { "nohup", "kitty", NULL };
static const Command kitty = { 0, kitty_cmd, NOWAIT, glitch_full, glitch_full, {NULL, NOCONFIRM, NO_OUT}  };

static const char *steam_cmd[] = { "nohup", "steam", NULL };
static const Command steam = { 0, steam_cmd, NOWAIT, glitch_full, glitch_full, {NULL, NOCONFIRM, NO_OUT}  };

static const Menu F_menu[] = {
  { "FIREFOX", COMMAND, { .command = firefox } },
  { "SLIPPI", COMMAND, { .command = slippi } },
  { "DISCORD", COMMAND, { .command = discord } },
  { "ST", COMMAND, { .command = st } },
  { "KITTY", COMMAND, { .command = kitty } },
  { "STEAM", COMMAND, { .command = steam } },
  END_OF_MENU
};

static const char *journalctl_cmd[] = { "journalctl", "-f", NULL };
static const Command journalctl = { 0, journalctl_cmd, WAIT, neon_reverse, neon, defaults  };

static const char *dmesg_cmd[] = { "sudo", "dmesg", "-w", NULL };
static const Command dmesg = { 0, dmesg_cmd, WAIT, neon_reverse, neon, defaults  };

static const char *btm_cmd[] = { "btm", NULL };
static const Command btm = { 0, btm_cmd, WAIT, neon_reverse, neon, defaults  };

static const char *dysk_cmd[] = { "dysk", NULL };
static const Command dysk = { 0, dysk_cmd, STOP, neon_reverse, neon, defaults  };

static const char *ping_cmd[] = { "ping", "9.9.9.9",  NULL };
static const Command ping = { 0, ping_cmd, WAIT, neon_reverse, neon, defaults  };

static const Menu L_menu[] = {
  { "journalctl -f", COMMAND, { .command = journalctl } },
  { "dmesg -w", COMMAND, { .command = dmesg } },
  { "btm", COMMAND, { .command = btm } },
  { "dysk", COMMAND, { .command = dysk } },
  { "ping", COMMAND, { .command = ping } },
  END_OF_MENU
};

static const char *gss_cmd[] = { "git", "status", "-s", NULL };
static const Command gss = { 0, gss_cmd, STOP, none, none, defaults  };

static const char *gs_cmd[] = { "git", "status", NULL };
static const Command gs = { 0, gs_cmd, STOP, none, none, defaults  };

static const char *gaa_cmd[] = { "git", "add", "--all", NULL };
static const Command gaa = { 0, gaa_cmd, WAIT, none, neon, defaults  };

static const char *gc_cmd[] = { "git", "commit", NULL };
static const Command gc = { 0, gc_cmd, WAIT, glitch_full, neon, defaults  };

static const char *gdv_cmd[] = { "/home/hakirot/skps/gdv", NULL };
static const Command gdv = { 0, gdv_cmd, EXEC, none, none, defaults  };

static const char *gp_cmd[] = { "git", "push", NULL };
static const Command gp = { 0, gp_cmd, STOP, none, neon, defaults  };

static const Menu git_menu[] = {
  { "s", COMMAND, { .command = gss } },
  { "Status", COMMAND, { .command = gs } },
  { "gdv", COMMAND, { .command = gdv } },
  { "Add -A", COMMAND, { .command = gaa } },
  { "Commit", COMMAND, { .command = gc } },
  { "Push", COMMAND, { .command = gp } },
  END_OF_MENU
};

static const char *pix_cmd[] = { "NO_COMMAND", NULL };
static const Command pix = { 0, pix_cmd, STOP_ON_ERR, none, blink, { "/home/hakirot/pix", NOCONFIRM, OUTS } };

static const char *dls_cmd[] = { "NO_COMMAND", NULL };
static const Command dls = { 0, dls_cmd, STOP_ON_ERR, none, blink, { "/home/hakirot/dls", NOCONFIRM, OUTS } };

static const char *Downloads_cmd[] = { "NO_COMMAND", NULL };
static const Command Downloads = { 0, Downloads_cmd, STOP_ON_ERR, none, blink, { "/home/hakirot/Downloads", NOCONFIRM, OUTS } };

static const char *git_cmd[] = { "NO_COMMAND", NULL };
static const Command git = { 0, git_cmd, STOP_ON_ERR, none, blink, { "/home/hakirot/git", NOCONFIRM, OUTS } };

static const char *gmz_cmd[] = { "NO_COMMAND", NULL };
static const Command gmz = { 0, gmz_cmd, STOP_ON_ERR, none, blink, { "/home/hakirot/gmz", NOCONFIRM, OUTS } };

static const char *lib_cmd[] = { "NO_COMMAND", NULL };
static const Command lib = { 0, lib_cmd, STOP_ON_ERR, none, blink, { "/home/hakirot/lib", NOCONFIRM, OUTS } };

static const char *mnt_cmd[] = { "NO_COMMAND", NULL };
static const Command mnt = { 0, mnt_cmd, STOP_ON_ERR, none, blink, { "/home/hakirot/mnt", NOCONFIRM, OUTS } };

static const char *slash_mnt_cmd[] = { "NO_COMMAND", NULL };
static const Command slash_mnt = { 0, slash_mnt_cmd, STOP_ON_ERR, none, blink, { "/mnt", NOCONFIRM, OUTS } };

static const char *mzk_cmd[] = { "NO_COMMAND", NULL };
static const Command mzk = { 0, mzk_cmd, STOP_ON_ERR, none, blink, { "/home/hakirot/mzk", NOCONFIRM, OUTS } };

static const char *skps_cmd[] = { "NO_COMMAND", NULL };
static const Command skps = { 0, skps_cmd, STOP_ON_ERR, none, blink, { "/home/hakirot/skps", NOCONFIRM, OUTS } };

static const char *chtest_cmd[] = { "NO_COMMAND", NULL };
static const Command test = { 0, chtest_cmd, STOP_ON_ERR, none, blink, { "/home/hakirot/test", NOCONFIRM, OUTS } };

static const char *vidz_cmd[] = { "NO_COMMAND", NULL };
static const Command vidz = { 0, vidz_cmd, STOP_ON_ERR, none, blink, { "/home/hakirot/vidz", NOCONFIRM, OUTS } };

static const char *dox_cmd[] = { "NO_COMMAND", NULL };
static const Command dox = { 0, dox_cmd, STOP_ON_ERR, none, blink, { "/home/hakirot/dox", NOCONFIRM, OUTS } };

static const char *home_cmd[] = { "NO_COMMAND", NULL };
static const Command home = { 0, home_cmd, STOP_ON_ERR, none, blink, { "/home/hakirot/", NOCONFIRM, OUTS } };

static const Menu chdir_menu[] = {
  { "~",         COMMAND, { .command = home } },
  { "dls",       COMMAND, { .command = dls } },
  { "Downloads", COMMAND, { .command = Downloads } },
  { "dox",       COMMAND, { .command = dox } },
  { "git",       COMMAND, { .command = git } },
  { "gmz",       COMMAND, { .command = gmz } },
  { "lib",       COMMAND, { .command = lib } },
  { "mnt",       COMMAND, { .command = mnt } },
  { "/mnt",      COMMAND, { .command = slash_mnt } },
  { "mzk",       COMMAND, { .command = mzk } },
  { "pix",       COMMAND, { .command = pix } },
  { "skps",      COMMAND, { .command = skps } },
  { "test",      COMMAND, { .command = test } },
  { "vidz",      COMMAND, { .command = vidz } },
  END_OF_MENU
};

static const Menu killme_menu[] = {
  { "test",         COMMAND, { .command = home } },
  END_OF_MENU
};

static const char *polybar_cmd[] = { "nohup", "bash", "-c", "bar.sh", NULL};
static const Command polybar = { 0, polybar_cmd, STOP_ON_ERR, none, blink, { "/home/hakirot/", NOCONFIRM, OUTS } };

static const char *mpassport_cmd[] = { "sudo", "mount", "/dev/sda1", "/mnt/passport", NULL};
static const Command mpassport = { 0, mpassport_cmd, STOP_ON_ERR, none, blink, defaults };

static const char *umpassport_cmd[] = { "sudo", "umount", "/mnt/passport", NULL};
static const Command umpassport = { 0, umpassport_cmd, STOP_ON_ERR, none, blink, defaults };

static const Menu P_menu[] = {
  { "polybar",         COMMAND, { .command = polybar } },
  { "mount passport",         COMMAND, { .command = mpassport } },
  { "umount passport",         COMMAND, { .command = umpassport } },
  END_OF_MENU
};

/* menukey assignments */
static const MenuKey menukeys[] = {
  {'b', b_menu },
  {'S', S_menu },
  {'F', F_menu },
  {'D', L_menu },
  {'g', git_menu },
  {'c', chdir_menu },
  {'A', killme_menu },
  {'P', P_menu },
};

#endif
