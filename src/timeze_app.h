#ifndef _TIMEZE_APP_H_
#define _TIMEZE_APP_H_

#include <gtk/gtk.h>

#define TIMEZE_APP_TYPE timeze_app_get_type ()
G_DECLARE_FINAL_TYPE (TimezeApp, timeze_app, TIMEZE,
                      APP, GtkApplication)

TimezeApp * timeze_app_new(void);

#endif
