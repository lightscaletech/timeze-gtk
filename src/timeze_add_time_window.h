#ifndef _TYPEZE_ADD_TIME_WINDOW_H_
#define _TYPEZE_ADD_TIME_WINDOW_H_

#include "timeze_countries_loader.h"

#include <gtk/gtk.h>

#define TIMEZE_ADD_TIME_WINDOW_TYPE timeze_add_time_window_get_type()
G_DECLARE_FINAL_TYPE(TimezeAddTimeWindow, timeze_add_time_window,
                     TIMEZE, ADD_TIME_WINDOW, GtkDialog);

TimezeAddTimeWindow * timeze_add_time_window_new(
    GtkWindow *, TimezeCountriesLoader *);

#endif
