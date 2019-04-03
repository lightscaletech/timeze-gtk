#include "timeze_utils.h"
#include "../config.h"

#include <unistd.h>

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
    const char * test = "/usr/share/zoneinfo/";
    size_t testlen = strlen(test);
    size_t limit = 100;
    size_t linklen = 0;
    char path[limit];

    if(g_file_test("/etc/localtime", G_FILE_TEST_IS_REGULAR)){
        linklen = readlink("/etc/localtime", path, limit);

        if(linklen >= limit) return timezone;

        path[linklen] = '\0';

        if(!g_str_has_prefix(path, test)) return timezone;

        timezone = g_strdup(&path[testlen]);
        g_debug("%s", timezone);
    }

    if(g_file_test("/etc/timezone", G_FILE_TEST_IS_REGULAR)) {
        g_file_get_contents("/etc/timezone", &timezone, NULL, NULL);
    }

#endif

    return timezone;
}
