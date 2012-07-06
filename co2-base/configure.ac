#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([co2-base], [0.0.1], [pp_kl_h@hotmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE
AM_SILENT_RULES([yes])
AC_CONFIG_SRCDIR([src/co2/RefObject.co2])
AC_CONFIG_HEADERS([config.h])

AC_DEFUN([AC_PROG_CARBON],
	[AC_PATH_PROG(CARBON, carbon, no)])

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_LIBTOOL
DOLT
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_PROG_CARBON

# Checks for libraries.
PKG_CHECK_MODULES([CO2], [libco2-1.0 >= 0.1.999],,AC_MSG_ERROR([required module missing]))
CFLAGS="$CFLAGS $CO2_CFLAGS"
LDFLAGS="$LDFLAGS $CO2_LIBS"

# Checks for header files.
AC_CHECK_HEADERS([unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([gettimeofday localtime_r strdup strstr tzset])

AC_CONFIG_FILES([Makefile
                 src/Makefile
		 test/Makefile
                 co2-base-1.0.pc])

AC_OUTPUT
