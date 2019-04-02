bin_PROGRAMS = timeze

timeze_SOURCES = \
	timeze_app.c \
	timeze_utils.c \
	timeze_countries_file.c \
	timeze_countries_loader.c \
	timeze_countries_dd_store.c \
	timeze_add_time_window.c \
	timeze_time_widget.c \
	timeze_main_window.c \
	resources.c \
	timeze.c

## Only our GUI program uses GTK+ at the moment.
timeze_CPPFLAGS = @GTK_CFLAGS@
timeze_LDADD = @GTK_LIBS@

#resource_files = $(shell glib-compile-resources --sourcedir=./resources/ --generate-dependencies=/timeze.gresources.xml)

resources.h: timeze.gresources.xml #$(resource_files)
	$(AM_V_GEN) glib-compile-resources --target $@ \
		--generate-header --sourcedir=./resources/ \
		--c-name=timeze $<

resources.c: timeze.gresources.xml resources.h $(resource_files)
	$(AM_V_GEN) glib-compile-resources --target $@ \
		--generate-source --sourcedir=./resources/ \
		--c-name=timeze $<


#clean-local:
#rm -f resources.h resources.c
