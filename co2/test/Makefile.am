MAINTAINERCLEANFILES=Makefile.in
ACLOCAL_AMFLAGS=-I m4

AM_YFLAGS=-d

noinst_LIBRARIES=libtest.a
libtest_a_SOURCES=\
co2/MyInterface.c\
co2/MyObject.c\
co2/MySingleton.c\
co2/TestInterface.c\
co2/MyInterface.h\
co2/MyObject.h\
co2/MySingleton.h\
co2/TestInterface.h

libtest_a_CPPFLAGS=-I$(top_srcdir)/src

noinst_PROGRAMS=TestObject
TESTS=TestObject 

TestObject_SOURCES=co2/TestObject.c
TestObject_CPPFLAGS=-I$(top_srcdir)/src
TestObject_LDADD=libtest.a ../src/libco2-1.0.la

distclean-local:
	-rm -r ${bin_PROGRAMS}
