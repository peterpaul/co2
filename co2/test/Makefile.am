MAINTAINERCLEANFILES=Makefile.in
ACLOCAL_AMFLAGS=-I m4

AM_YFLAGS=-d

noinst_LIBRARIES=libtest.a
libtest_a_SOURCES=\
co2/Array.c\
co2/ArrayList.c\
co2/IList.c\
co2/LinkedList.c\
co2/ListList.c\
co2/MyInterface.c\
co2/MyInterfaceImpl.c\
co2/MyObject.c\
co2/MySingleton.c\
co2/RefArray.c\
co2/Array.h\
co2/ArrayList.h\
co2/IList.h\
co2/LinkedList.h\
co2/ListList.h\
co2/MyInterface.h\
co2/MyInterfaceImpl.h\
co2/MyObject.h\
co2/MySingleton.h\
co2/RefArray.h

libtest_a_CPPFLAGS=-I$(top_srcdir)/src

noinst_PROGRAMS=TestObject TestIterator
TESTS=TestObject 
# TestIterator

TestObject_SOURCES=co2/TestObject.c
TestObject_CPPFLAGS=-I$(top_srcdir)/src
TestObject_LDADD=libtest.a ../src/libco2-1.0.la

TestIterator_SOURCES=co2/TestIterator.c
TestIterator_CPPFLAGS=-I$(top_srcdir)/src
TestIterator_LDADD=libtest.a ../src/libco2-1.0.la

distclean-local:
	-rm -r ${bin_PROGRAMS}
