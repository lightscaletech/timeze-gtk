#include "timeze_countries_file.h"

#include <glib/gstdio.h>
#include <sys/stat.h>

static gchar *
file_path() {
    const gchar * data_dir = g_get_user_data_dir();
    const gchar * folder = "/timeze/";
    const gchar * file = "countries.csv";
    gchar * folder_path, * path;

    folder_path = g_strdup_printf("%s%s", data_dir, folder);

    if(g_mkdir_with_parents(
           folder_path,
           S_IRUSR | S_IWUSR | S_IXUSR |
           S_IRGRP | S_IXGRP |
           S_IROTH | S_IXOTH) != 0)
        return NULL;

    path = g_strdup_printf("%s%s", folder_path, file);

    g_free(folder_path);
    return path;
}

GList *
timeze_countries_file_load() {
    gsize limit = 60;
    gchar * path, * key;
    gchar cur[limit];
    FILE * fd;
    GList * res = NULL;
    size_t length;

    path = file_path();
    if(path == NULL) return res;

    if(g_file_test(path, G_FILE_TEST_IS_REGULAR) == FALSE) return NULL;

    fd = g_fopen(path, "r");
    while(fgets(cur, limit, fd) != NULL) {
        length = strlen(cur);
        key = g_strndup(cur, length - 1);
        res = g_list_append(res, key);
    }

    g_free(path);
    return res;
}

void
timeze_countries_file_save(GList * list) {
    gchar * path, * line;
    FILE * fd;
    GList * cur;

    path = file_path();
    if(path == NULL) return;

    fd = g_fopen(path, "w");

    cur = g_list_first(list);
    while(cur != NULL) {
        line = g_strdup_printf("%s\n", (gchar *) cur->data);

        fputs(line, fd);

        g_free(line);
        cur = g_list_next(cur);
    }

    g_free(path);
}
