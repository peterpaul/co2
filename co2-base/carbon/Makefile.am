MAINTAINERCLEANFILES = Makefile.in
ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libco2-base-1.0.la
libco2_base_1_0_ladir=$(includedir)/co2-base-1.0/co2

libco2_base_1_0_la_CO2SOURCES = Array.co2 ArrayList.co2 Exception.co2 IndexOutOfBoundsException.co2 Iterator.co2 List.co2 ListIterator.co2 Object.co2 OutOfMemoryException.co2 RefObject.co2 ReleasePool.co2 ReleasePoolItem.co2 Singleton.co2 String.co2
libco2_base_1_0_la_SOURCES = Array.c ArrayList.c Exception.c IndexOutOfBoundsException.c Iterator.c List.c ListIterator.c Object.c OutOfMemoryException.c RefObject.c ReleasePool.c ReleasePoolItem.c Singleton.c String.c
libco2_base_1_0_la_HEADERS = Array.h ArrayList.h Exception.h IndexOutOfBoundsException.h Iterator.h List.h ListIterator.h Object.h OutOfMemoryException.h RefObject.h ReleasePool.h ReleasePoolItem.h Singleton.h String.h

%.c: %.co2
	carbon $< $@

libco2_base_1_0_la_CPPFLAGS = -I$(top_srcdir)/include

bin_PROGRAMS = TestRefObject
carbon_SOURCES = TestRefObject.c
carbon_CPPFLAGS = -I$(top_srcdir)/include
carbon_LDFLAGS = $(CO2_LIBS) -lco2-base-1.0

distclean-local:
	-rm -r ${bin_PROGRAMS}
