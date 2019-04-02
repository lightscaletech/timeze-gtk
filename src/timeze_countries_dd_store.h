#ifndef _TYPEZE_COUNTRIES_DD_STORE_H_
#define _TYPEZE_COUNTRIES_DD_STORE_H_

#include <gtk/gtk.h>
#include "timeze_countries_loader.h"

#define TIMEZE_COUNTRIES_DD_STORE_TYPE timeze_countries_dd_store_get_type()
G_DECLARE_FINAL_TYPE(TimezeCountriesDDStore, timeze_countries_dd_store,
                     TIMEZE, COUNTRIES_DD_STORE, GtkListStore);

TimezeCountriesDDStore * timeze_countries_dd_store_new();
void timeze_countries_dd_store_load(TimezeCountriesDDStore *,
                                    TimezeCountriesLoader *);

#endif
