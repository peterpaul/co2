MAINTAINERCLEANFILES = Makefile.in
ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libco2-base-1.0.la
libco2_base_1_0_ladir=$(includedir)/co2-base-1.0/co2

libco2_base_1_0_la_CO2SOURCES = \
Array.co2\
ArrayIterator.co2\
ArrayList.co2\
Collection.co2\
Comparable.co2\
ConsoleHandler.co2\
Exception.co2\
Hash.co2\
IndexOutOfBoundsException.co2\
Iterable.co2\
Iterator.co2\
Level.co2\
List.co2\
ListIterator.co2\
LogFormatter.co2\
Logger.co2\
LogHandler.co2\
LogRecord.co2\
Object.co2\
OutOfMemoryException.co2\
RefObject.co2\
ReleasePool.co2\
ReleasePoolItem.co2\
SimpleFormatter.co2\
Singleton.co2\
String.co2

libco2_base_1_0_la_SOURCES = \
Array.c\
ArrayIterator.c\
ArrayList.c\
Collection.c\
Comparable.c\
ConsoleHandler.c\
Exception.c\
Hash.c\
IndexOutOfBoundsException.c\
Iterable.c\
Iterator.c\
Level.c\
List.c\
ListIterator.c\
LogFormatter.c\
Logger.c\
LogHandler.c\
LogRecord.c\
Object.c\
OutOfMemoryException.c\
RefObject.c\
ReleasePool.c\
ReleasePoolItem.c\
SimpleFormatter.c\
Singleton.c\
String.c

libco2_base_1_0_la_HEADERS = \
Array.h\
ArrayIterator.h\
ArrayList.h\
Collection.h\
Comparable.h\
ConsoleHandler.h\
Exception.h\
Hash.h\
IndexOutOfBoundsException.h\
Iterable.h\
Iterator.h\
Level.h\
List.h\
ListIterator.h\
LogFormatter.h\
Logger.h\
LogHandler.h\
LogRecord.h\
Object.h\
OutOfMemoryException.h\
RefObject.h\
ReleasePool.h\
ReleasePoolItem.h\
SimpleFormatter.h\
Singleton.h\
String.h

%.c: %.co2
	carbon $< $@

libco2_base_1_0_la_CPPFLAGS = -I$(top_srcdir)/include -O0 -g3

bin_PROGRAMS = TestRefObject TestLogger
TestRefObject_SOURCES = TestRefObject.c
TestRefObject_CPPFLAGS = -I$(top_srcdir)/include
TestRefObject_LDFLAGS = $(CO2_LIBS) -lco2-base-1.0

TestLogger_SOURCES = TestLogger.c
TestLogger_CPPFLAGS = -I$(top_srcdir)/include
TestLogger_LDFLAGS = $(CO2_LIBS) -lco2-base-1.0

distclean-local:
	-rm -r ${bin_PROGRAMS}
