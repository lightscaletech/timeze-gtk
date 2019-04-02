#ifndef _TIMEZE_COUNTRIES_FILES_H_
#define _TIMEZE_COUNTRIES_FILES_H_

#include <glib.h>

GList * timeze_countries_file_load();
void timeze_countries_file_save(GList *);

#endif
