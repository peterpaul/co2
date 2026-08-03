#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([libco2], [0.3.3], [pp_kl_h@hotmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE
AM_SILENT_RULES([yes])
AC_CONFIG_SRCDIR([src/co2/Object.c])
AC_CONFIG_HEADERS([config.h])

# AC_PROG_CC only supplies its own "-g -O2" default when CFLAGS is entirely
# unset -- CI (translate.sh/build-and-test.sh) always passes an explicit
# CFLAGS (currently -std=gnu17, for bootstrap-tarball compatibility -- see
# TODO.md), so this default only matters for a plain
# autogen.sh/configure/make/install run with no CFLAGS override. Without it,
# that path silently inherited whatever -std= the local compiler defaulted
# to (gnu89 on some systems, as new as gnu23 on others), which is exactly
# what caused several codegen bugs to go unnoticed for years (see the recent
# "modern C" audit fixes in carbon/co2-base). Pin a known-working modern
# standard instead of leaving it to chance; CI's own explicit CFLAGS is
# untouched either way.
: ${CFLAGS="-g -O2 -std=gnu17"}

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_LIBTOOL
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
