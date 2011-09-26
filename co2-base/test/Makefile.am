MAINTAINERCLEANFILES=Makefile.in
ACLOCAL_AMFLAGS=-I m4

CO2SOURCES=\
co2/TestRefObject.co2\
co2/TestLogger.co2\
co2/TestMap.co2\
co2/TestSet.co2\
co2/TestList.co2

DEPENDENCIES=\
$(CO2SOURCES:.co2=.d)

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

CARBON_FLAGS=-I . -I ../carbon

%.c: %.co2
	-rm -f $@
	carbon $(CARBON_FLAGS) $< $@
	indent $@

%.h: %.c

%.d: %.co2
	-rm -f $@
	echo -n "`echo $@ | sed 's/\.d/.c/g'` " > $@
	carbon --depend $(CARBON_FLAGS) $< $@ >> $@

distclean-local:
	-rm -r $(bin_PROGRAMS)

clean-local:
	-rm -f $(DEPENDENCIES)

include $(DEPENDENCIES)