MAINTAINERCLEANFILES = Makefile.in
ACLOCAL_AMFLAGS = -I m4

AM_YFLAGS = -d

lib_LTLIBRARIES = libtest-1.0.la
libtest_1_0_ladir=$(includedir)/test-1.0/co2
libtest_1_0_la_HEADERS = co2/Array.h\
co2/ArrayList.h\
co2/IList.h\
co2/LinkedList.h\
co2/ListList.h\
co2/MyInterface.h\
co2/MyObject.h\
co2/MySingleton.h\
co2/RefArray.h\
co2/TestInterface.h

libtest_1_0_la_SOURCES = co2/Array.c\
co2/ArrayList.c\
co2/IList.c\
co2/LinkedList.c\
co2/ListList.c\
co2/MyInterface.c\
co2/MyObject.c\
co2/MySingleton.c\
co2/RefArray.c\
co2/TestInterface.c

libtest_1_0_la_CPPFLAGS = -I$(top_srcdir)/include

bin_PROGRAMS = TestObject TestIterator

TestObject_SOURCES = co2/TestObject.c
TestObject_CPPFLAGS = -I$(top_srcdir)/include
TestObject_LDFLAGS = $(CO2_LIBS) -ltest-1.0

TestIterator_SOURCES = co2/TestIterator.c
TestIterator_CPPFLAGS = -I$(top_srcdir)/include
TestIterator_LDFLAGS = $(CO2_LIBS) -ltest-1.0

distclean-local:
	-rm -r ${bin_PROGRAMS}
