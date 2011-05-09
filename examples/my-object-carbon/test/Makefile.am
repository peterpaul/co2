MAINTAINERCLEANFILES=Makefile.in
ACLOCAL_AMFLAGS=-I m4

libtest_a_CPPFLAGS=-I$(top_srcdir)/src -I$(srcdir)

noinst_PROGRAMS=TestMyObject
TESTS=TestMyObject

TestMyObject_SOURCES=TestMyObject.c TestMyObject.h TestMyObject.co2
TestMyObject_CPPFLAGS=-I$(top_srcdir)/src
TestMyObject_LDADD=../src/libmy-object-carbon-1.0.la

distclean-local:
	-rm -r ${bin_PROGRAMS}

%.c: %.co2
	-rm $@
	carbon -P $(top_srcdir)/src $< $@

%.h: %.c

