#pragma once
#include "config_defs.h"

// user shell, set this to the shell you want to use
#define SHELL "/usr/bin/fish"

// text foreground and background color
#define FG_COLOR "#ffffff"
#define BG_COLOR "#1a1a1a"

// the font for the terminal
#define FONT_NAME "Monospace"
#define FONT_SIZE 17

// if this is uncommented the window title will update to the current shell's title, otherwise it will stay Faffy
#define UPDATE_WINDOW_TITLE

// all keybinds
static const CommandKeybind KEYBINDS[] = {
  // Zooming
  { GDK_CONTROL_MASK, GDK_KEY_plus, ACTION_ZOOM_IN, NULL},
  { GDK_CONTROL_MASK, GDK_KEY_equal, ACTION_ZOOM_IN, NULL},
  { GDK_CONTROL_MASK, GDK_KEY_minus, ACTION_ZOOM_OUT, NULL},
  { GDK_CONTROL_MASK, GDK_KEY_0, ACTION_ZOOM_RESET, NULL},

  // Clipboard
  { GDK_CONTROL_MASK, GDK_KEY_c, ACTION_COPY, NULL},
  { GDK_CONTROL_MASK, GDK_KEY_v, ACTION_PASTE, NULL},

  // Interrupt and Exit
  { GDK_CONTROL_MASK, GDK_KEY_i, ACTION_COMMAND, "\003"}, // keyboard interrupt signal
  { GDK_CONTROL_MASK, GDK_KEY_e, ACTION_EXIT, NULL},

  // Custom Shell Macros 
  // (without a \n the command will just be pasted in the shell, adding a \n at the end automatically executes it)
  { GDK_CONTROL_MASK | GDK_SHIFT_MASK, GDK_KEY_c, ACTION_COMMAND, "clear\n" },
};

// ansi color codes
const char *ANSI_PALETTE[16] = {
  "#363333", // 0 black
  "#c51e3a", // 1 red
  "#1db169", // 2 green
  "#f3ad16", // 3 yellow
  "#00c4ff", // 4 blue
  "#901d6c", // 5 magenta
  "#36bfa8", // 6 cyan
  "#f7f2e1", // 7 white

  "#5f5f6e", // 8 bright black (so basically gray)
  "#ff2929", // 9 bright red
  "#84df5a", // 10 bright green
  "#eeee2e", // 11 bright yellow
  "#9fd7ff", // 12 bright blue
  "#c87992", // 13 bright magenta
  "#4bf0fc", // 14 bright cyan
  "#f9feff"  // 15 bright white
};
