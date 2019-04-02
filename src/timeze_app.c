#include "timeze_app.h"

#include "timeze_main_window.h"
#include "resources.h"

#include <stdio.h>

struct _TimezeApp {
    GtkApplication parent;
};

G_DEFINE_TYPE(TimezeApp, timeze_app, GTK_TYPE_APPLICATION);

static void
timeze_app_activate(GApplication * app) {
    TimezeMainWindow * win;
    win = timeze_main_window_new(GTK_APPLICATION(app));
    gtk_window_present(GTK_WINDOW(win));
}

static void
timeze_app_init(TimezeApp * app) {
    GResource * resource;
    resource = timeze_get_resource();
    g_resources_register(resource);
}

static void
timeze_app_class_init(TimezeAppClass * class) {
    G_APPLICATION_CLASS(class)->activate = timeze_app_activate;

    g_debug("%s", g_get_user_data_dir());
}

TimezeApp * timeze_app_new() {
    return (TimezeApp *) g_object_new(
        TIMEZE_APP_TYPE,
        "application-id", "org.lightscale.timezeapp",
        "flags", G_APPLICATION_HANDLES_OPEN,
        NULL
        );
}
