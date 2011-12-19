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

noinst_PROGRAMS=TestMyObject
TESTS=$(noinst_PROGRAMS)

TestMyObject_SOURCES=TestMyObject.c TestMyObject.h TestMyObject.co2
TestMyObject_CPPFLAGS=-O0 -g3 -DO_DEBUG -I$(top_srcdir)/src
TestMyObject_LDADD=$(top_builddir)/src/libmy-object-carbon-1.0.la
TestMyObject_LDFLAGS=$(CO2_LIBS)

CARBON_FLAGS=-I . -I $(top_srcdir)/src

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
