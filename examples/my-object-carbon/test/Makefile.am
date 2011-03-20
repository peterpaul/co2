MAINTAINERCLEANFILES=Makefile.in
ACLOCAL_AMFLAGS=-I m4

libtest_a_CPPFLAGS=-I$(top_srcdir)/src

noinst_PROGRAMS=TestMyObject
TESTS=TestMyObject

TestMyObject_SOURCES=TestMyObject.c
TestMyObject_CPPFLAGS=-I$(top_srcdir)/src
TestMyObject_LDADD=../src/libmy-object-carbon-1.0.la

distclean-local:
	-rm -r ${bin_PROGRAMS}

%.c: %.co2
	-rm $@
	carbon -P . -P ../src $< $@

%.h: %.c

