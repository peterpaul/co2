#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([carbon], [0.0.1], [pp_kl_h@hotmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/co2/Compiler.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_YACC
AM_PROG_LEX

# Checks for libraries.
PKG_CHECK_MODULES([CO2], [co2-1.0 >= 0.0.1],,exit)
CFLAGS="$CFLAGS $CO2_CFLAGS"
LDFLAGS="$LDFLAGS $CO2_LIBS"

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memmove memset strdup strstr])

AC_CONFIG_FILES([Makefile
                 include/Makefile
                 src/Makefile
		 carbon-1.0.pc])

AC_OUTPUT
