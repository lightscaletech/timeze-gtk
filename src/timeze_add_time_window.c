#include "timeze_add_time_window.h"
#include "timeze_countries_dd_store.h"

enum {
    PROP_COUNTRIES_STORE = 1,
    N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = {};

static guint signal_add;

struct _TimezeAddTimeWindow {
    GtkDialog parent;
    TimezeCountriesDDStore * countries_store;
    GtkWidget * combo;
};

G_DEFINE_TYPE(TimezeAddTimeWindow, timeze_add_time_window, GTK_TYPE_DIALOG);

static void
response(GtkDialog * dialog, gint id, gpointer ud) {
    g_debug("dialog response");
    TimezeAddTimeWindow * win = TIMEZE_ADD_TIME_WINDOW(ud);
    GtkTreeIter iter;
    gchar * result;

    switch(id) {
    case GTK_RESPONSE_OK:
        gtk_combo_box_get_active_iter(GTK_COMBO_BOX(win->combo), &iter);
        gtk_tree_model_get(GTK_TREE_MODEL(win->countries_store), &iter,
                           1, &result, -1);
        result = g_strdup(result);
        g_signal_emit(dialog, signal_add, 0, result);
        break;
    default: break;
    }

    gtk_widget_destroy(GTK_WIDGET(dialog));
}

static void
timeze_add_time_window_init(TimezeAddTimeWindow * win) {
    gtk_window_set_title(GTK_WINDOW(win), "Add time");
    gtk_dialog_add_button(
        GTK_DIALOG(win), "Cancel", GTK_RESPONSE_CANCEL);
    gtk_dialog_add_button(
        GTK_DIALOG(win), "Add", GTK_RESPONSE_OK);

    g_signal_connect(win, "response", G_CALLBACK(response), win);
}


static void
get_prop(GObject      *object,
         guint         property_id,
         GValue *value,
         GParamSpec   *pspec) {
    TimezeAddTimeWindow * self = TIMEZE_ADD_TIME_WINDOW(object);
    switch(property_id) {
    case PROP_COUNTRIES_STORE:
        g_value_set_pointer(value, self->countries_store);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
set_prop(GObject      *object,
         guint         property_id,
         const GValue *value,
         GParamSpec   *pspec) {
    TimezeAddTimeWindow * self = TIMEZE_ADD_TIME_WINDOW(object);

    switch(property_id) {
    case PROP_COUNTRIES_STORE:
        self->countries_store = g_value_get_pointer(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
constructed(GObject * obj) {
    G_OBJECT_CLASS (timeze_add_time_window_parent_class)->constructed(obj);

    GtkWidget * content_area, * label, * combo, * box;
    TimezeAddTimeWindow * win = TIMEZE_ADD_TIME_WINDOW(obj);
    GtkCellRenderer * cell;

    label = gtk_label_new("Select country");
    combo = gtk_combo_box_new_with_model(
        GTK_TREE_MODEL(win->countries_store));

    win->combo = combo;

    cell = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo), cell, TRUE);
    gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo), cell,
                                   "text", 0,
                                  NULL);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(win));

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(box), label);
    gtk_container_add(GTK_CONTAINER(box), combo);
    gtk_container_add(GTK_CONTAINER(content_area), box);

    gtk_widget_show_all(GTK_WIDGET(win));
}

static void
timeze_add_time_window_class_init(TimezeAddTimeWindowClass * class) {
    signal_add = g_signal_new(
        "add-time",
        TIMEZE_ADD_TIME_WINDOW_TYPE,
        G_SIGNAL_ACTION, 0, NULL, NULL, NULL,
        G_TYPE_NONE, 1, G_TYPE_STRING);

    GObjectClass * object_class = G_OBJECT_CLASS(class);

    object_class->constructed = constructed;
    object_class->get_property = get_prop;
    object_class->set_property = set_prop;

    obj_properties[PROP_COUNTRIES_STORE] = g_param_spec_pointer(
        "countries-store", "Countries Store",
        "A List store for the dropdown.",
        G_PARAM_CONSTRUCT | G_PARAM_READWRITE);

    g_object_class_install_properties(
        object_class, N_PROPERTIES, obj_properties);
}

TimezeAddTimeWindow *
timeze_add_time_window_new(GtkWindow * parent,
                           TimezeCountriesLoader * loader) {
    TimezeCountriesDDStore * store = timeze_countries_dd_store_new();
    timeze_countries_dd_store_load(store, loader);

    TimezeAddTimeWindow * diag =  g_object_new(
        TIMEZE_ADD_TIME_WINDOW_TYPE,
        "countries-store", store, NULL);


    gtk_window_set_transient_for(GTK_WINDOW(diag), parent);
    return diag;
}
