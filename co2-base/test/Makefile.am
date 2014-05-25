MAINTAINERCLEANFILES=Makefile.in
ACLOCAL_AMFLAGS=-I m4

CO2SOURCES=\
co2/TestDoubleLinkedList.co2\
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

BUILT_SOURCES=\
$(GENERATED_CSOURCES)\
$(GENERATED_CHEADERS)

EXTRA_DIST=\
$(CO2SOURCES)\
$(BUILT_SOURCES)

# TODO: re-enable TestList and fix problem.
#noinst_PROGRAMS=TestRefObject TestLogger TestMap TestSet TestList
noinst_PROGRAMS=TestDoubleLinkedList TestRefObject TestLogger TestMap TestSet
TESTS=$(noinst_PROGRAMS)

TestRefObject_SOURCES=co2/TestRefObject.c
TestRefObject_LDADD=$(top_builddir)/src/libco2-base-1.0.la
TestRefObject_LDFLAGS=$(CO2_LIBS)
TestRefObject_CPPFLAGS=-O0 -g3 -DO_DEBUG -I$(top_srcdir)/src

TestDoubleLinkedList_SOURCES=co2/TestDoubleLinkedList.c
TestDoubleLinkedList_LDADD=$(top_builddir)/src/libco2-base-1.0.la
TestDoubleLinkedList_LDFLAGS=$(CO2_LIBS)
TestDoubleLinkedList_CPPFLAGS=-O0 -g3 -DO_DEBUG -I$(top_srcdir)/src

TestLogger_SOURCES=co2/TestLogger.c
TestLogger_LDADD=$(top_builddir)/src/libco2-base-1.0.la
TestLogger_LDFLAGS=$(CO2_LIBS)
TestLogger_CPPFLAGS=-O0 -g3 -DO_DEBUG -I$(top_srcdir)/src

TestMap_SOURCES=co2/TestMap.c
TestMap_LDADD=$(top_builddir)/src/libco2-base-1.0.la
TestMap_LDFLAGS=$(CO2_LIBS)
TestMap_CPPFLAGS=-O0 -g3 -DO_DEBUG -I$(top_srcdir)/src

TestSet_SOURCES=co2/TestSet.c
TestSet_LDADD=$(top_builddir)/src/libco2-base-1.0.la
TestSet_LDFLAGS=$(CO2_LIBS)
TestSet_CPPFLAGS=-O0 -g3 -DO_DEBUG -I$(top_srcdir)/src

TestList_SOURCES=co2/TestList.c
TestList_LDADD=$(top_builddir)/src/libco2-base-1.0.la
TestList_LDFLAGS=$(CO2_LIBS)
TestList_CPPFLAGS=-O0 -g3 -DO_DEBUG -I$(top_srcdir)/src

CARBON_FLAGS=-I $(srcdir) -I $(top_srcdir)/src

%.c: %.co2
if CARBON_EXISTS
	-rm -f $(builddir)/$@
	$(CARBON) $(CARBON_FLAGS) $< $(builddir)/$@;
else
	echo "please install carbon first";
endif

%.h: %.c

if CARBON_EXISTS
%.d: %.co2
	-rm -f $(builddir)/$@
	mkdir -p `dirname $(builddir)/$@`
	echo -n "`echo $(builddir)/$@ | sed 's/\.d/.c/g'` " > $(builddir)/$@
	$(CARBON) --depend $(CARBON_FLAGS) $< $(builddir)/$@ >> $(builddir)/$@ || rm -f $(builddir)/$@;
endif

clean-local:
if CARBON_EXISTS
	-rm -f $(GENERATED_CHEADERS) $(GENERATED_CSOURCES);
endif

distclean-local:
	-rm -f $(noinst_PROGRAMS)
if CARBON_EXISTS
	-rm -f $(GENERATED_CO2DEPEND)
endif

if CARBON_EXISTS
include $(GENERATED_CO2DEPEND)
endif
