MAINTAINERCLEANFILES=Makefile.in
ACLOCAL_AMFLAGS=-I m4

CO2SOURCES=\
co2/TestRefObject.co2\
co2/TestLogger.co2\
co2/TestMap.co2\
co2/TestSet.co2\
co2/TestList.co2

GENERATED_CSOURCES=\
$(CO2SOURCES:.co2=.c)

GENERATED_CHEADERS=\
$(CO2SOURCES:.co2=.h)

GENERATED_CO2DEPEND=\
$(CO2SOURCES:.co2=.d)

noinst_PROGRAMS=TestRefObject TestLogger TestMap TestSet TestList
TESTS=$(noinst_PROGRAMS)

TestRefObject_SOURCES=co2/TestRefObject.c
TestRefObject_LDADD=$(top_builddir)/carbon/libco2-base-1.0.la
TestRefObject_LDFLAGS=$(CO2_LIBS)
TestRefObject_CPPFLAGS=-O0 -g3 -DO_DEBUG -I$(top_srcdir)/carbon

TestLogger_SOURCES=co2/TestLogger.c
TestLogger_LDADD=$(top_builddir)/carbon/libco2-base-1.0.la
TestLogger_LDFLAGS=$(CO2_LIBS)
TestLogger_CPPFLAGS=-O0 -g3 -DO_DEBUG -I$(top_srcdir)/carbon

TestMap_SOURCES=co2/TestMap.c
TestMap_LDADD=$(top_builddir)/carbon/libco2-base-1.0.la
TestMap_LDFLAGS=$(CO2_LIBS)
TestMap_CPPFLAGS=-O0 -g3 -DO_DEBUG -I$(top_srcdir)/carbon

TestSet_SOURCES=co2/TestSet.c
TestSet_LDADD=$(top_builddir)/carbon/libco2-base-1.0.la
TestSet_LDFLAGS=$(CO2_LIBS)
TestSet_CPPFLAGS=-O0 -g3 -DO_DEBUG -I$(top_srcdir)/carbon

TestList_SOURCES=co2/TestList.c
TestList_LDADD=$(top_builddir)/carbon/libco2-base-1.0.la
TestList_LDFLAGS=$(CO2_LIBS)
TestList_CPPFLAGS=-O0 -g3 -DO_DEBUG -I$(top_srcdir)/carbon

CARBON_FLAGS=-I . -I $(top_srcdir)/carbon

%.c: %.co2
	if test "$(CARBON)" == "no"; then echo "please install carbon first"; else rm -f $@ | true; $(CARBON) $(CARBON_FLAGS) $< $@; indent $@; fi

%.h: %.c

%.d: %.co2
	-rm -f $@
	echo -n "`echo $@ | sed 's/\.d/.c/g'` " > $@
	if test "$(CARBON)" == "no"; then echo ": " $< >> $@; else $(CARBON) --depend $(CARBON_FLAGS) $< $@ >> $@; fi

distclean-local:
	-rm -r $(noinst_PROGRAMS)

clean-local:
	if test "$(CARBON)" == "no"; then rm -f $(GENERATED_CO2DEPEND); else rm -f $(GENERATED_CHEADERS) $(GENERATED_CSOURCES) $(GENERATED_CO2DEPEND); fi

include $(GENERATED_CO2DEPEND)