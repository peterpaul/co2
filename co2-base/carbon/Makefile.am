MAINTAINERCLEANFILES = Makefile.in
ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libco2-base-1.0.la
libco2_base_1_0_ladir=$(includedir)/co2-base-1.0/co2

libco2_base_1_0_la_SOURCES = \
co2/Array.co2\
co2/ArrayIterator.co2\
co2/ArrayList.co2\
co2/BaseObject.co2\
co2/Collection.co2\
co2/Comparable.co2\
co2/ConsoleHandler.co2\
co2/Enum.co2\
co2/Exception.co2\
co2/HashMap.co2\
co2/HashMapIterator.co2\
co2/HashSet.co2\
co2/HashSetIterator.co2\
co2/IndexOutOfBoundsException.co2\
co2/Iterable.co2\
co2/Iterator.co2\
co2/KeyValuePair.co2\
co2/Level.co2\
co2/List.co2\
co2/ListIterator.co2\
co2/LogFormatter.co2\
co2/Logger.co2\
co2/LogHandler.co2\
co2/LogRecord.co2\
co2/Map.co2\
co2/OutOfMemoryException.co2\
co2/RefObject.co2\
co2/ReleasePool.co2\
co2/ReleasePoolItem.co2\
co2/Set.co2\
co2/SimpleFormatter.co2\
co2/String.co2\
co2/Throwable.co2\
co2/Array.c\
co2/ArrayIterator.c\
co2/ArrayList.c\
co2/BaseObject.c\
co2/Collection.c\
co2/Comparable.c\
co2/ConsoleHandler.c\
co2/Enum.c\
co2/Exception.c\
co2/HashMap.c\
co2/HashMapIterator.c\
co2/HashSet.c\
co2/HashSetIterator.c\
co2/IndexOutOfBoundsException.c\
co2/Iterable.c\
co2/Iterator.c\
co2/KeyValuePair.c\
co2/Level.c\
co2/List.c\
co2/ListIterator.c\
co2/LogFormatter.c\
co2/Logger.c\
co2/LogHandler.c\
co2/LogRecord.c\
co2/Map.c\
co2/OutOfMemoryException.c\
co2/RefObject.c\
co2/ReleasePool.c\
co2/ReleasePoolItem.c\
co2/Set.c\
co2/SimpleFormatter.c\
co2/String.c\
co2/Throwable.c

libco2_base_1_0_la_HEADERS = \
co2/Array.h\
co2/ArrayIterator.h\
co2/ArrayList.h\
co2/BaseObject.h\
co2/Collection.h\
co2/Comparable.h\
co2/ConsoleHandler.h\
co2/Enum.h\
co2/Exception.h\
co2/HashMap.h\
co2/HashMapIterator.h\
co2/HashSet.h\
co2/HashSetIterator.h\
co2/IndexOutOfBoundsException.h\
co2/Iterable.h\
co2/Iterator.h\
co2/KeyValuePair.h\
co2/Level.h\
co2/List.h\
co2/ListIterator.h\
co2/LogFormatter.h\
co2/Logger.h\
co2/LogHandler.h\
co2/LogRecord.h\
co2/Map.h\
co2/OutOfMemoryException.h\
co2/RefObject.h\
co2/ReleasePool.h\
co2/ReleasePoolItem.h\
co2/Set.h\
co2/SimpleFormatter.h\
co2/String.h\
co2/Throwable.h

%.c: %.co2
	-rm $@
	carbon -P . $< $@

%.h: %.c

libco2_base_1_0_la_CPPFLAGS = -I$(top_srcdir)/include

bin_PROGRAMS = TestRefObject TestLogger TestMap TestSet

TestRefObject_SOURCES = co2/TestRefObject.c
TestRefObject_CPPFLAGS = -I$(top_srcdir)/include
TestRefObject_LDFLAGS = $(CO2_LIBS) -lco2-base-1.0

TestLogger_SOURCES = co2/TestLogger.c
TestLogger_CPPFLAGS = -I$(top_srcdir)/include
TestLogger_LDFLAGS = $(CO2_LIBS) -lco2-base-1.0

TestMap_SOURCES = co2/TestMap.c
TestMap_CPPFLAGS = -I$(top_srcdir)/include
TestMap_LDFLAGS = $(CO2_LIBS) -lco2-base-1.0

TestSet_SOURCES = co2/TestSet.c
TestSet_CPPFLAGS = -I$(top_srcdir)/include
TestSet_LDFLAGS = $(CO2_LIBS) -lco2-base-1.0

distclean-local:
	-rm -r ${bin_PROGRAMS}
