#include "timeze_main_window.h"
#include "timeze_time_widget.h"
#include "timeze_add_time_window.h"
#include "timeze_countries_loader.h"
#include "timeze_countries_file.h"
#include "timeze_utils.h"


struct _TimezeMainWindow {
    GtkWindow parent;
    TimezeCountriesLoader * loader;
    GList * times;
    GtkWidget * timesBox, * localTime;
};

G_DEFINE_TYPE(TimezeMainWindow, timeze_main_window, GTK_TYPE_WINDOW);

static void
save_times(TimezeMainWindow * main) {
    GList * keys, * cur;
    struct TimezeCountry * country;

    keys = NULL;
    cur = g_list_first(main->times);
    while(cur != NULL) {
        country = timeze_time_widget_get_country(cur->data);
        keys = g_list_append(keys, timeze_country_hash_key(country));
        cur = g_list_next(cur);
    }

    timeze_countries_file_save(keys);

    g_list_free_full(keys, timeze_string_list_free);
}

static void
remove_time(TimezeTimeWidget * widg, gpointer ud) {
    TimezeMainWindow * main = TIMEZE_MAIN_WINDOW(ud);

    GList * list_time = g_list_find(main->times, widg);
    if(list_time == NULL) {
        g_debug("List item to be removed was not found");
        return;
    }

    main->times = g_list_delete_link(main->times, list_time);

    gtk_widget_destroy(GTK_WIDGET(widg));

    save_times(main);
}

static void
add_time(TimezeMainWindow * main, struct TimezeCountry * country) {
    GtkWidget * timewidg = NULL;
    timewidg = timeze_time_widget_new(country, TRUE);
    main->times = g_list_append(main->times, timewidg);
    gtk_box_pack_start(GTK_BOX(main->timesBox), timewidg, FALSE, TRUE, 0);
    gtk_widget_show_all(timewidg);

    g_signal_connect(timewidg, "remove-time", G_CALLBACK(remove_time), main);
}

static void
add_time_response(GtkDialog * diag, gchar * country_key, gpointer ud) {
    TimezeMainWindow * main = TIMEZE_MAIN_WINDOW(ud);
    TimezeCountriesLoader * loader = main->loader;

    g_debug("%s", country_key);

    struct TimezeCountry * country =
        timeze_countries_loader_get_country(loader, country_key);

    if(country == NULL) return;

    add_time(main, country);
    save_times(main);
}

static void
add_btn_clicked(GtkButton * btn, gpointer parent) {
    g_debug("open add time dialog");
    TimezeMainWindow * main = TIMEZE_MAIN_WINDOW(parent);

    TimezeAddTimeWindow * diag =
        timeze_add_time_window_new(GTK_WINDOW(parent), main->loader);
    g_signal_connect(G_OBJECT(diag), "add-time", G_CALLBACK(add_time_response), parent);
}


static gboolean
tick(gpointer ud) {
    TimezeMainWindow * win = TIMEZE_MAIN_WINDOW(ud);

    timeze_time_widget_tick(TIMEZE_TIME_WIDGET(win->localTime));

    GList * times = g_list_first(win->times);
    while(times != NULL) {
        timeze_time_widget_tick(TIMEZE_TIME_WIDGET(times->data));
        times = g_list_next(times);
    }

    return G_SOURCE_CONTINUE;
}

static void
load_times(TimezeMainWindow * win) {
    GList * keys = timeze_countries_file_load();
    GList * cur = NULL;
    struct TimezeCountry * country = NULL;

    if(keys == NULL) return;

    cur = g_list_first(keys);
    while(cur != NULL) {
        country = timeze_countries_loader_get_country(win->loader, cur->data);
        add_time(win, country);
        cur = g_list_next(cur);
    }

    g_list_free_full(keys, timeze_string_list_free);
}

static void
set_geometry(TimezeMainWindow * win) {
    GdkGeometry geom;
    geom.min_width = 275;
    geom.min_height = 350;
    gtk_window_set_geometry_hints(GTK_WINDOW(win), NULL, &geom, GDK_HINT_MIN_SIZE);
}

static struct TimezeCountry *
get_local_country(TimezeMainWindow * win) {
    struct TimezeCountry * country = NULL;
    gchar * zone = timeze_get_local_timezone();

    country = timeze_countries_loader_get_by_tz(
        win->loader, zone);

    g_free(zone);
    return country;
}

static void
timeze_main_window_init(TimezeMainWindow * win) {
    GtkWidget * btn, * box, * viewport, * scrollwin;

    set_geometry(win);

    timeze_countries_file_load();

    win->times = NULL;
    win->loader = timeze_countries_loader_new();;

    scrollwin = gtk_scrolled_window_new(NULL, NULL);
    viewport = gtk_viewport_new(NULL, NULL);

    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollwin),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    win->timesBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    btn = gtk_button_new_with_label("Add");
    win->localTime = timeze_time_widget_new(get_local_country(win), FALSE);

    gtk_box_pack_start(GTK_BOX(box), win->localTime, FALSE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(scrollwin), viewport);
    gtk_box_pack_start(GTK_BOX(box), scrollwin, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(box), btn);
    gtk_container_add(GTK_CONTAINER(win), box);

    gtk_container_add(GTK_CONTAINER(viewport), win->timesBox);

    gtk_widget_show_all(GTK_WIDGET(win));

    g_timeout_add_seconds(3, tick, win);

    load_times(win);

    g_signal_connect(btn, "clicked", G_CALLBACK(add_btn_clicked), win);
}

static void
timeze_main_window_class_init(TimezeMainWindowClass * class) {

}

TimezeMainWindow *
timeze_main_window_new(GtkApplication * app) {
    return g_object_new(TIMEZE_MAIN_WINDOW_TYPE,
                        "application", app, NULL);
}
