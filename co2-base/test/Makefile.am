MAINTAINERCLEANFILES=Makefile.in
ACLOCAL_AMFLAGS=-I m4

%.c: %.co2
	-rm $@
	carbon -I . -I ../carbon $< $@

%.h: %.c

bin_PROGRAMS=TestRefObject TestLogger TestMap TestSet TestList
TESTS=$(bin_PROGRAMS)

TestRefObject_SOURCES=co2/TestRefObject.c
TestRefObject_LDADD=../carbon/libco2-base-1.0.la
TestRefObject_LDFLAGS=$(CO2_LIBS)
TestRefObject_CPPFLAGS=-O0 -g3 -DO_DEBUG -I../carbon

TestLogger_SOURCES=co2/TestLogger.c
TestLogger_LDADD=../carbon/libco2-base-1.0.la
TestLogger_LDFLAGS=$(CO2_LIBS)
TestLogger_CPPFLAGS=-O0 -g3 -DO_DEBUG -I../carbon

TestMap_SOURCES=co2/TestMap.c
TestMap_LDADD=../carbon/libco2-base-1.0.la
TestMap_LDFLAGS=$(CO2_LIBS)
TestMap_CPPFLAGS=-O0 -g3 -DO_DEBUG -I../carbon

TestSet_SOURCES=co2/TestSet.c
TestSet_LDADD=../carbon/libco2-base-1.0.la
TestSet_LDFLAGS=$(CO2_LIBS)
TestSet_CPPFLAGS=-O0 -g3 -DO_DEBUG -I../carbon

TestList_SOURCES=co2/TestList.c
TestList_LDADD=../carbon/libco2-base-1.0.la
TestList_LDFLAGS=$(CO2_LIBS)
TestList_CPPFLAGS=-O0 -g3 -DO_DEBUG -I../carbon

distclean-local:
	-rm -r ${bin_PROGRAMS}
