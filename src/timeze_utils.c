#include "timeze_utils.h"
#include "../config.h"

void
timeze_string_list_free(gpointer str) {
    g_free(str);
}

gchar *
timeze_get_local_timezone() {
    gchar * timezone = NULL;

#ifdef HAS_GLIB_GET_TIMEZONE
    GDateTime * time = g_date_time_new_now_local();
    GTimeZone * zone = g_date_time_get_timezone(time);
    timezone = g_strdup(g_time_zone_get_identifier(zone));
#else



#endif

    return timezone;
}
