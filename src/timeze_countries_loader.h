#ifndef _TIMEZE_COUNTRIES_LOADER_H_
#define _TIMEZE_COUNTRIES_LOADER_H_

#include "timeze_countries.h"

#include <glib-object.h>

G_BEGIN_DECLS

#define TIMEZE_COUNTRIES_LOADER_TYPE timeze_countries_loader_get_type()
G_DECLARE_FINAL_TYPE(TimezeCountriesLoader, timeze_countries_loader,
                     TIMEZE, COUNTRIES_LOADER, GObject);

TimezeCountriesLoader * timeze_countries_loader_new();
void timeze_countries_loader_free(TimezeCountriesLoader *);

gsize timeze_countries_loader_count(TimezeCountriesLoader *);
GHashTable * timeze_countries_loader_get(TimezeCountriesLoader *);
struct TimezeCountry * timeze_countries_loader_get_country (
    TimezeCountriesLoader *, const gchar *);
struct TimezeCountry * timeze_countries_loader_get_by_tz(
    TimezeCountriesLoader *, const gchar *);

gboolean timeze_countries_loader_get_has_code(TimezeCountriesLoader *, gpointer);
gchar * timeze_country_hash_key(struct TimezeCountry *);

G_END_DECLS

#endif
