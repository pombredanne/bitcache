AC_INIT([bitcache], [0.0.1], [bitcache@googlegroups.com])
AC_CONFIG_SRCDIR([src/bitcache.h])
AC_CONFIG_AUX_DIR([etc/aclocal])
AC_CONFIG_MACRO_DIR([etc/aclocal])
AM_INIT_AUTOMAKE([foreign -Wall -Werror dist-bzip2])
AM_SILENT_RULES([yes])
LT_INIT
AC_PROG_CC
AM_PATH_GLIB_2_0([2.24.0], [], [], [gthread-2.0])
AC_CHECK_HEADERS(ev.h)
AC_CHECK_LIB(ev, ev_loop)
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile doc/Makefile doc/man/Makefile])
AC_OUTPUT
