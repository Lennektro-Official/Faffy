#pragma once

// these includes are also utilized by the config.h and faffy.c, i know, a bit confusing
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

// what actions a keybind can perform
typedef enum {
  ACTION_COMMAND,
  ACTION_ZOOM_IN,
  ACTION_ZOOM_OUT,
  ACTION_ZOOM_RESET,
  ACTION_COPY,
  ACTION_PASTE,
  ACTION_EXIT
} ActionType;

// the struct to define a keybind used in the config
typedef struct {
  guint mask;
  guint keyval;
  ActionType action;
  const char *cmd;
} CommandKeybind;
