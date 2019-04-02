#ifndef _TYPEZE_MAIN_WINDOW_H_
#define _TYPEZE_MAIN_WINDOW_H_

#include <gtk/gtk.h>

#define TIMEZE_MAIN_WINDOW_TYPE timeze_main_window_get_type()
G_DECLARE_FINAL_TYPE(TimezeMainWindow, timeze_main_window,
                     TIMEZE, MAIN_WINDOW, GtkWindow);

TimezeMainWindow * timeze_main_window_new(GtkApplication *);

#endif
