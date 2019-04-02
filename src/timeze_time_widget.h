#ifndef _TIMEZE_TIME_WIDGET_H_
#define _TIMEZE_TIME_WIDGET_H_

#include "timeze_countries.h"

#include <gtk/gtk.h>

#define TIMEZE_TIME_WIDGET_TYPE timeze_time_widget_get_type()
G_DECLARE_FINAL_TYPE(TimezeTimeWidget, timeze_time_widget,
                     TIMEZE, TIME_WIDGET, GtkBox);

GtkWidget * timeze_time_widget_new(struct TimezeCountry *, gboolean);
void timeze_time_widget_tick(TimezeTimeWidget *);
struct TimezeCountry * timeze_time_widget_get_country(TimezeTimeWidget *);


#endif
