#include "config.h"
#include <vte/vte.h>

// the fallback window title
#define WINDOW_TITLE "Faffy"

// the font scale value for dynamically adjusting the font size
static double font_scale = 1.0;

// issue window close when shell session ended
static void childExitedCallback(VteTerminal *terminal, gint status, gpointer user_data) {
  GtkWidget *window = GTK_WIDGET(user_data);
  gtk_widget_destroy(window);
}

// updating the window title to the title outputed by the shell if it's valid and enabled in the config
#ifdef UPDATE_WINDOW_TITLE
static void windowTitleChangedCallback(VteTerminal *terminal, gpointer user_data) {
  GtkWindow *window = GTK_WINDOW(user_data);
  const char *vte_title = vte_terminal_get_window_title(terminal);
  if(vte_title && strlen(vte_title) > 0) gtk_window_set_title(window, vte_title);
  else gtk_window_set_title(window, WINDOW_TITLE);
}
#endif

// handle all keybinds
gboolean keyInputCallback(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
  VteTerminal *terminal = VTE_TERMINAL(user_data);
  guint filtered_state = event->state & (GDK_CONTROL_MASK | GDK_SHIFT_MASK | GDK_MOD1_MASK);

  size_t num_binds = sizeof(KEYBINDS) / sizeof(KEYBINDS[0]);
  for(size_t i = 0; i < num_binds; i++) {
    if(filtered_state != KEYBINDS[i].mask || gdk_keyval_to_lower(event->keyval) != gdk_keyval_to_lower(KEYBINDS[i].keyval))
      continue;

    switch(KEYBINDS[i].action) {

      case ACTION_ZOOM_IN:
        font_scale += 0.1;
        vte_terminal_set_font_scale(terminal, font_scale);
        return TRUE;

      case ACTION_ZOOM_OUT:
        if(font_scale > 0.3) {
          font_scale -= 0.1;
          vte_terminal_set_font_scale(terminal, font_scale);
        }
        return TRUE;

      case ACTION_ZOOM_RESET:
        font_scale = 1.0;
        vte_terminal_set_font_scale(terminal, font_scale);
        return TRUE;

      case ACTION_COPY:
        if(vte_terminal_get_has_selection(terminal)) {
          vte_terminal_copy_clipboard_format(terminal, VTE_FORMAT_TEXT);
          return TRUE;
        }
        break;

      case ACTION_PASTE:
        vte_terminal_paste_clipboard(terminal);
        return TRUE;

      case ACTION_COMMAND:
        vte_terminal_feed_child(terminal, KEYBINDS[i].cmd, -1);
        return TRUE;

      case ACTION_EXIT:
        gtk_main_quit();
        return TRUE;

    }
  }

  return FALSE;
}

int main(int argc, char *argv[]) {
  gtk_init(&argc, &argv);

  // window init
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), WINDOW_TITLE);
  gtk_window_set_icon_name(GTK_WINDOW(window), "utilities-terminal");
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  // create the terminal
  GtkWidget *terminal = vte_terminal_new();

  // setup the font
  PangoFontDescription *font_desc = pango_font_description_new();
  pango_font_description_set_family(font_desc, FONT_NAME);
  pango_font_description_set_size(font_desc, FONT_SIZE * PANGO_SCALE);
  vte_terminal_set_font(VTE_TERMINAL(terminal), font_desc);
  pango_font_description_free(font_desc);

  // setup the colors
  GdkRGBA fg_color, bg_color;
  gdk_rgba_parse(&fg_color, FG_COLOR);
  gdk_rgba_parse(&bg_color, BG_COLOR);

  GdkRGBA palette[16];
  for(int i = 0; i < 16; i++) gdk_rgba_parse(&palette[i], ANSI_PALETTE[i]);
  
  vte_terminal_set_colors(VTE_TERMINAL(terminal), &fg_color, &bg_color, palette, 16);

  // register callback handlers
  g_signal_connect(window, "key-press-event", G_CALLBACK(keyInputCallback), terminal);
  g_signal_connect(terminal, "child_exited", G_CALLBACK(childExitedCallback), window);

  // only register window title handler if title changing is set in the config
  #ifdef UPDATE_WINDOW_TITLE
  g_signal_connect(terminal, "window-title-changed", G_CALLBACK(windowTitleChangedCallback), window);
  #endif

  // init the shell and handle the -e option to immediately execute a command in that session
  char **argv_shell = NULL;
  if(argc >= 3 && strcmp(argv[1], "-e") == 0) {
    argv_shell = g_new0(char*, 4);
    argv_shell[0] = g_strdup(SHELL);
    argv_shell[1] = g_strdup("-c");
    argv_shell[2] = g_strdup(argv[2]);
  } else {
    argv_shell = g_new0(char*, 2);
    argv_shell[0] = g_strdup(SHELL);
  }

  // get the environment
  char **envv = g_get_environ();

  // init the terminal
  vte_terminal_spawn_async(
    VTE_TERMINAL(terminal),
    VTE_PTY_DEFAULT,
    NULL,
    argv_shell,
    envv,
    G_SPAWN_DEFAULT,
    NULL, NULL,
    NULL,
    -1,
    NULL,
    NULL, NULL
  );

  // clean up garbage
  g_strfreev(envv);
  g_strfreev(argv_shell);

  // add terminal instance to window
  gtk_container_add(GTK_CONTAINER(window), terminal);
  gtk_widget_show_all(window);

  // call the main window loop
  gtk_main();
  return 0;
}
