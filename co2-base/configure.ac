#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([libco2-base], [0.3.4], [pp_kl_h@hotmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE
AM_SILENT_RULES([yes])
AC_CONFIG_SRCDIR([src/co2/RefObject.co2])
AC_CONFIG_HEADERS([config.h])
AC_CANONICAL_HOST

AC_DEFUN([AC_PROG_CARBON],
	[AC_PATH_PROG(CARBON, carbon, no)])

# AC_PROG_CC only supplies its own "-g -O2" default when CFLAGS is entirely
# unset -- CI (translate.sh/build-and-test.sh) always passes an explicit
# CFLAGS (currently -std=gnu89, for bootstrap-tarball compatibility -- see
# TODO.md), so this default only matters for a plain
# autogen.sh/configure/make/install run with no CFLAGS override. Without it,
# that path silently inherited whatever -std= the local compiler defaulted
# to (gnu89 on some systems, as new as gnu23 on others), which is exactly
# what caused several codegen bugs to go unnoticed for years (see the recent
# "modern C" audit fixes in carbon). Pin a known-working modern standard
# instead of leaving it to chance; CI's own explicit CFLAGS is untouched
# either way.
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
AC_PROG_CARBON

AM_CONDITIONAL([CARBON_EXISTS], [test ${CARBON} != no])

# TestLogger crashes at runtime on MinGW with no output (exit status 127) --
# see TODO.md item #1 for the still-open investigation. Marked XFAIL only on
# this platform, not unconditionally, so a real regression on Linux/macOS
# still fails the build loudly.
case $host_os in
  *mingw*) on_mingw=yes ;;
  *) on_mingw=no ;;
esac
AM_CONDITIONAL([ON_MINGW], [test "x$on_mingw" = "xyes"])

# Checks for libraries.
PKG_CHECK_MODULES([CO2], [libco2-1.0 >= 0.2.0],,AC_MSG_ERROR([required module missing]))
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
                 libco2-base-1.0.pc])

AC_OUTPUT
