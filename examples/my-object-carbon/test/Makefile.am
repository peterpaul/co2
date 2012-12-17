MAINTAINERCLEANFILES=Makefile.in
ACLOCAL_AMFLAGS=-I m4

CO2SOURCES=\
TestMyObject.co2

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

noinst_PROGRAMS=TestMyObject
TESTS=$(noinst_PROGRAMS)

TestMyObject_SOURCES=TestMyObject.c TestMyObject.h TestMyObject.co2
TestMyObject_CPPFLAGS=-O0 -g3 -DO_DEBUG -I$(top_srcdir)/src
TestMyObject_LDADD=$(top_builddir)/src/libmy-object-carbon-1.0.la
TestMyObject_LDFLAGS=$(CO2_LIBS)

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
