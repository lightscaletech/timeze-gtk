#include "timeze_countries_dd_store.h"

struct _TimezeCountriesDDStore {
    GtkListStore parent;
};

enum {
    COLUMN_TITLE,
    COLUMN_KEY,
    N_COLUMNS
};

G_DEFINE_TYPE(TimezeCountriesDDStore, timeze_countries_dd_store,
              GTK_TYPE_LIST_STORE);

static void
timeze_countries_dd_store_init(TimezeCountriesDDStore * store) {
    GType types[] = {G_TYPE_STRING, G_TYPE_STRING};
    gtk_list_store_set_column_types(
        GTK_LIST_STORE(store), N_COLUMNS, types);
}

static void
hash_foreach(gpointer keyp, gpointer countryp, gpointer ud) {
    GtkTreeIter treeiter;
    TimezeCountriesDDStore * store = TIMEZE_COUNTRIES_DD_STORE(ud);
    gchar * col, * key;
    struct TimezeCountry * country;

    key = (gchar *) keyp;
    country = (struct TimezeCountry *) countryp;

    col = g_strdup_printf("%s - %s", country->name, country->tz);
    gtk_list_store_append(GTK_LIST_STORE(store), &treeiter);
    gtk_list_store_set(
        GTK_LIST_STORE(store), &treeiter,
        COLUMN_TITLE, col,
        COLUMN_KEY, key,
        -1);

    g_free(col);
}

void
timeze_countries_dd_store_load(TimezeCountriesDDStore * store,
                               TimezeCountriesLoader * loader) {

    GHashTable * countries = timeze_countries_loader_get(loader);

    g_hash_table_foreach(countries, hash_foreach, store);


}

static void
timeze_countries_dd_store_class_init(TimezeCountriesDDStoreClass * class) {

}

TimezeCountriesDDStore *
timeze_countries_dd_store_new() {
    TimezeCountriesDDStore * store =  g_object_new(
        TIMEZE_COUNTRIES_DD_STORE_TYPE, NULL);

    return store;
}
