#include "timeze_time_widget.h"

struct _TimezeTimeWidget {
    GtkBox parent;
    GtkWidget * labTime, * labCountry;
    struct TimezeCountry * country;
    gboolean show_remove;
};

static guint signal_remove;

G_DEFINE_TYPE(TimezeTimeWidget, timeze_time_widget, GTK_TYPE_BOX);

enum {
      PROP_COUNTRY = 1,
      PROP_SHOW_REMOVE_BTN,
      N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = {};

static void
timeze_time_widget_init(TimezeTimeWidget * widg) {
    gtk_orientable_set_orientation(GTK_ORIENTABLE(widg),
                                   GTK_ORIENTATION_HORIZONTAL);
}

static void
set_time(TimezeTimeWidget * widg) {
    GTimeZone * zone = g_time_zone_new(widg->country->tz);
    GDateTime * time = g_date_time_new_now(zone);
    gchar * timestr = g_date_time_format(time, "%H:%M");
    gtk_label_set_text(GTK_LABEL(widg->labTime), timestr);
}

static void
remove_clicked(GtkButton * btn, gpointer ud) {
    TimezeTimeWidget * widg = TIMEZE_TIME_WIDGET(ud);
    g_signal_emit(widg, signal_remove, 0);
}

static void
constructed(GObject * obj) {
    TimezeTimeWidget * widg = TIMEZE_TIME_WIDGET(obj);
    GtkBox * box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 10));
    GtkWidget * btn = NULL;

    widg->labTime = gtk_label_new("");
    widg->labCountry = gtk_label_new(widg->country->name);

    gtk_box_pack_start(box, widg->labTime, FALSE, FALSE, 0);
    gtk_box_pack_start(box, widg->labCountry, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(widg), GTK_WIDGET(box), TRUE, TRUE, 0);

    if(widg->show_remove){
        btn = gtk_button_new_from_icon_name("edit-clear", GTK_ICON_SIZE_BUTTON);
        gtk_box_pack_end(GTK_BOX(widg), btn, FALSE, TRUE, 0);

        g_signal_connect(btn, "clicked", G_CALLBACK(remove_clicked), widg);
    }

    set_time(widg);
}

static void
get_prop(GObject      *object,
         guint         property_id,
         GValue *value,
         GParamSpec   *pspec) {
    TimezeTimeWidget * self = TIMEZE_TIME_WIDGET(object);
    switch(property_id) {
    case PROP_COUNTRY:
        g_value_set_pointer(value, self->country);
        break;
    case PROP_SHOW_REMOVE_BTN:
        g_value_set_boolean(value, self->show_remove);
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
    TimezeTimeWidget * self = TIMEZE_TIME_WIDGET(object);

    switch(property_id) {
    case PROP_COUNTRY:
        self->country = g_value_get_pointer(value);
        break;
    case PROP_SHOW_REMOVE_BTN:
        self->show_remove = g_value_get_boolean(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
    }
}

static void
timeze_time_widget_class_init(TimezeTimeWidgetClass * class) {

    GObjectClass * object_class = G_OBJECT_CLASS(class);

    object_class->constructed = constructed;
    object_class->get_property = get_prop;
    object_class->set_property = set_prop;

    obj_properties[PROP_COUNTRY] = g_param_spec_pointer
        (
         "country", "Country",
         "Country to display time for",
         G_PARAM_CONSTRUCT | G_PARAM_READWRITE
         );

    obj_properties[PROP_SHOW_REMOVE_BTN] = g_param_spec_boolean
        (
         "show-remove-btn", "Show remove button",
         "Weather this should have a remove button",
         FALSE,
         G_PARAM_CONSTRUCT | G_PARAM_READWRITE
         );

    g_object_class_install_properties(object_class, N_PROPERTIES, obj_properties);

    signal_remove = g_signal_new("remove-time",
                                 TIMEZE_TIME_WIDGET_TYPE,
                                 G_SIGNAL_ACTION, 0, NULL, NULL, NULL,
                                 G_TYPE_NONE, 0);
}

GtkWidget *
timeze_time_widget_new(struct TimezeCountry * country, gboolean show_remove) {
    return g_object_new(TIMEZE_TIME_WIDGET_TYPE,
                        "country", country,
                        "show-remove-btn", show_remove,
                        NULL);
}

void timeze_time_widget_tick(TimezeTimeWidget * widg) {
    set_time(widg);
}

struct TimezeCountry *
timeze_time_widget_get_country(TimezeTimeWidget * widg) {
    return widg->country;
}
