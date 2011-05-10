#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([libco2], [0.1.0], [pp_kl_h@hotmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE
AM_SILENT_RULES([yes])
AC_CONFIG_SRCDIR([src/co2/Object.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_LIBTOOL
DOLT
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memmove memset strdup strstr])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 test/Makefile
                 libco2-1.0.pc])

AC_OUTPUT
