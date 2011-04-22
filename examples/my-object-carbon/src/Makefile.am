MAINTAINERCLEANFILES = Makefile.in
ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libmy-object-carbon-1.0.la
libmy_object_carbon_1_0_ladir=$(includedir)/co2-1.0/co2

libmy_object_carbon_1_0_la_SOURCES = MyObject.c MyObject.co2
libmy_object_carbon_1_0_la_HEADERS = MyObject.h

libmy_object_carbon_1_0_la_CPPFLAGS = -I$(top_srcdir)/include

%.c: %.co2
	-rm $@
	carbon -P . $< $@

%.h: %.c

