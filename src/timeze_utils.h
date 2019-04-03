#ifndef _TIMEZE_UTILS_H_
#define _TIMEZE_UTILS_H_

#include <glib.h>

void
timeze_string_list_free(gpointer);

gchar *
timeze_get_local_timezone();

gchar *
timeze_remove_last_char(const gchar *);

#endif
