#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([carbon], [0.3.1], [pp_kl_h@hotmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE
AM_SILENT_RULES([yes])
AC_CONFIG_SRCDIR([src/co2/Compiler.co2])
AC_CONFIG_HEADERS([src/config.h])

AC_DEFUN([AC_PROG_CARBON],
	[AC_PATH_PROG(CARBON, carbon, no)])

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
AC_PROG_LEX
AC_PROG_MKDIR_P
AC_PROG_YACC
AC_PROG_CARBON

AM_CONDITIONAL([CARBON_EXISTS], [test ${CARBON} != no])

# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_create])
PKG_CHECK_MODULES([CO2], [libco2-1.0 >= 0.2.0],,AC_MSG_ERROR([required module missing]))
CFLAGS="$CFLAGS $CO2_CFLAGS"
LDFLAGS="$LDFLAGS $CO2_LIBS"
PKG_CHECK_MODULES([CO2BASE], [libco2-base-1.0 >= 0.1.0],,AC_MSG_ERROR([required module missing]))
CFLAGS="$CFLAGS $CO2BASE_CFLAGS"
LDFLAGS="$LDFLAGS $CO2BASE_LIBS"

# Checks for header files.
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([inttypes.h libintl.h limits.h malloc.h stddef.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([getcwd memset realpath sqrt strdup strrchr])

AC_CONFIG_FILES([Makefile
                 test/Makefile
                 src/Makefile
                 carbon-1.0.pc
		 ])

AC_OUTPUT
