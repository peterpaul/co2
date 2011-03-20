#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([my-object-carbon], [0.0.1], [pp_kl_h@hotmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE
AM_SILENT_RULES([yes])
AC_CONFIG_SRCDIR([src/MyObject.co2])
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
AC_PROG_MKDIR_P

# Checks for libraries.
PKG_CHECK_MODULES([CO2], [co2-1.0 >= 0.0.1],,AC_MSG_ERROR([required module missing]))
CFLAGS="$CFLAGS $CO2_CFLAGS"
LDFLAGS="$LDFLAGS $CO2_LIBS"

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 test/Makefile
                 my-object-carbon-1.0.pc])

AC_OUTPUT
