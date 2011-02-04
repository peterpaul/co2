MAINTAINERCLEANFILES = Makefile.in
ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libco2-base-1.0.la
libco2_base_1_0_ladir=$(includedir)/co2-base-1.0/co2

libco2_base_1_0_la_CO2SOURCES = Object.co2 List.co2 String.co2 Singleton.co2 Iterator.co2 ListIterator.co2 RefObject.co2 ReleasePool.co2 ReleasePoolItem.co2
libco2_base_1_0_la_SOURCES = Object.c List.c String.c Singleton.c Iterator.c ListIterator.c RefObject.c ReleasePool.c ReleasePoolItem.c
libco2_base_1_0_la_HEADERS = Object.h List.h String.h Singleton.h Iterator.h ListIterator.h RefObject.h ReleasePool.h ReleasePoolItem.h

%.c: %.co2
	carbon $< $@

libco2_base_1_0_la_CPPFLAGS = -I$(top_srcdir)/include
