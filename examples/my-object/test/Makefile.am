MAINTAINERCLEANFILES=Makefile.in
ACLOCAL_AMFLAGS=-I m4

libtest_a_CPPFLAGS=-I$(top_srcdir)/src

noinst_PROGRAMS=TestMyObject TestMyObjectStatic
TESTS=TestMyObject TestMyObjectStatic

TestMyObject_SOURCES=TestMyObject.c
TestMyObject_CPPFLAGS=-I$(top_srcdir)/src
TestMyObject_LDADD=../src/libmy-object-1.0.la

TestMyObjectStatic_SOURCES=TestMyObjectStatic.c
TestMyObjectStatic_CPPFLAGS=-I$(top_srcdir)/src
TestMyObjectStatic_LDADD=../src/libmy-object-1.0.la

distclean-local:
	-rm -r ${bin_PROGRAMS}
