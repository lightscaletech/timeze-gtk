#include "timeze_countries_loader.h"

#include <gio/gio.h>

struct _TimezeCountriesLoader {
    GObject parent;
    GHashTable * countries;
    GHashTable * zone_lookup;
    gsize countries_count;
};

G_DEFINE_TYPE(TimezeCountriesLoader, timeze_countries_loader, G_TYPE_OBJECT);

static struct TimezeCountry tmp_country;

static void
xml_start_elem(GMarkupParseContext *context,
               const gchar         *element_name,
               const gchar        **attribute_names,
               const gchar        **attribute_values,
               gpointer             user_data,
               GError             **error) {
    if(g_strcmp0(element_name, "zone") != 0) return;

    tmp_country.name = NULL;
    tmp_country.code = NULL;
    tmp_country.tz = NULL;
}


static void
xml_text(GMarkupParseContext *context,
         const gchar         *text,
         gsize                text_len,
         gpointer             user_data,
         GError             **error) {
    const gchar * elem = g_markup_parse_context_get_element(context);

    if(g_strcmp0(elem, "countryName") == 0) {
        tmp_country.name = g_strdup(text);
    }

    if(g_strcmp0(elem, "countryCode") == 0) {
        tmp_country.code = g_strdup(text);
    }

    if(g_strcmp0(elem, "zoneName") == 0) {
        tmp_country.tz = g_strdup(text);
    }
}

gchar *
timeze_country_hash_key(struct TimezeCountry * country) {
    return g_strdup_printf("%s_%s", country->code, country->tz);
}

static void
xml_end_elem(GMarkupParseContext *context,
             const gchar         *element_name,
             gpointer             user_data,
             GError             **error) {
    TimezeCountriesLoader * loader = NULL;
    struct TimezeCountry * country = NULL;
    gchar * key;

    if(g_strcmp0(element_name, "zone") != 0) return;

    loader = TIMEZE_COUNTRIES_LOADER(user_data);

    country = g_memdup(&tmp_country, sizeof(struct TimezeCountry));

    key = timeze_country_hash_key(country);
    g_hash_table_insert(loader->countries, key, country);
    g_hash_table_insert(loader->zone_lookup, country->tz, country);

    loader->countries_count++;
}

static void
parse_xml(TimezeCountriesLoader * loader, const gchar * str, gsize size) {
    GMarkupParseContext * ctx;
    GMarkupParser parser;
    GError * err;

    parser.start_element = xml_start_elem;
    parser.end_element = xml_end_elem;
    parser.text = xml_text;
    parser.error = NULL; //xml_error;
    parser.passthrough = NULL; //xml_passthrough;

    ctx = g_markup_parse_context_new(&parser, 0, loader, NULL);
    g_markup_parse_context_parse(ctx, str, size, &err);

}

static void
timeze_countries_loader_init(TimezeCountriesLoader * loader) {
    loader->countries = g_hash_table_new(g_str_hash, g_str_equal);
    loader->zone_lookup = g_hash_table_new(g_str_hash, g_str_equal);

    GBytes * countries_data;

    countries_data = g_resources_lookup_data(
        "/org/lightscale/timezeapp/countries.xml",
        G_RESOURCE_LOOKUP_FLAGS_NONE, NULL);

    gsize size;
    const gchar * countries_str;
    countries_str = g_bytes_get_data(countries_data, &size);

    parse_xml(loader, countries_str, size);
}

static void
timeze_countries_loader_class_init(TimezeCountriesLoaderClass * class) {}

TimezeCountriesLoader *
timeze_countries_loader_new() {
    return (TimezeCountriesLoader *) g_object_new(
        TIMEZE_COUNTRIES_LOADER_TYPE,
        NULL);
}

void
timeze_countries_loader_free(TimezeCountriesLoader * loader) {
    g_clear_object(&loader);
}

gsize
timeze_countries_loader_count(TimezeCountriesLoader * loader) {
    return loader->countries_count;
}

GHashTable *
timeze_countries_loader_get(TimezeCountriesLoader * loader) {
    return loader->countries;
}

gboolean
timeze_countries_loader_get_has_code(TimezeCountriesLoader * loader, gpointer key) {
    return g_hash_table_contains(loader->countries, key);
}

struct TimezeCountry *
timeze_countries_loader_get_country(TimezeCountriesLoader * loader,
                                    const gchar * key) {
    return g_hash_table_lookup(loader->countries, key);
}

struct TimezeCountry *
timeze_countries_loader_get_by_tz(TimezeCountriesLoader * loader,
                                  const gchar * tz) {
    return g_hash_table_lookup(loader->zone_lookup, tz);
}
