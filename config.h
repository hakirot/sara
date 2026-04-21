/*
--   ██████╗ ██████╗ ███╗   ██╗███████╗██╗ ██████╗   --
--  ██╔════╝██╔═══██╗████╗  ██║██╔════╝██║██╔════╝   --
--  ██║     ██║   ██║██╔██╗ ██║█████╗  ██║██║  ███╗  --
--  ██║     ██║   ██║██║╚██╗██║██╔══╝  ██║██║   ██║  --
--  ╚██████╗╚██████╔╝██║ ╚████║██║     ██║╚██████╔╝  --
--   ╚═════╝ ╚═════╝ ╚═╝  ╚═══╝╚═╝     ╚═╝ ╚═════╝.h --
                                                             */

#include "globals.h"
#include <wchar.h>

#ifndef CONFIG_H
#define CONFIG_H

wchar_t FG_STR[] = L"█╔╗╚╝║═";
wchar_t BG_STR[] = L"`+so:-./";

int GLITCH_FRAME_TIME = 3500;
int FOREGROUND = RED;
int BACKGROUND = GREEN;

bool use_bold_for_bg = true;
bool use_bold_for_fg = false;

bool HOLOGRAPHIC = false;
bool FOLLOW = true;

#endif
