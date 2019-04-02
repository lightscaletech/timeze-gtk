AC_PREREQ(2.60)
AC_INIT([timeze], [0.1], [sam@lightscale.co.uk])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.9.6 -Wall -Werror dist-bzip2])

AC_PROG_CC
# Compiling sources with per-target flags requires AM_PROG_CC_C_O
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_LIBTOOL

AM_PATH_GTK_3_0([3.10.0],,AC_MSG_ERROR([Gtk+ 2.10.0 or higher required.]))

AC_CHECK_LIB(
        [glib-2.0],
        [g_date_time_get_timezone],
        [AC_DEFINE(
                [HAS_GLIB_GET_TIMEZONE],
                [1],
                [Does glib have g_date_time_get_timezone])])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
Makefile
src/Makefile
])
AC_OUTPUT
