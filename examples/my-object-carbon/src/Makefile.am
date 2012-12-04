MAINTAINERCLEANFILES=Makefile.in
ACLOCAL_AMFLAGS=-I m4

lib_LTLIBRARIES=libmy-object-carbon-1.0.la
libmy_object_carbon_1_0_ladir=$(includedir)/co2-1.0/co2

libmy_object_carbon_1_0_la_CO2SOURCES=\
MyObject.co2\
YourObject.co2

GENERATED_CSOURCES=\
$(libmy_object_carbon_1_0_la_CO2SOURCES:.co2=.c)

GENERATED_CHEADERS=\
$(libmy_object_carbon_1_0_la_CO2SOURCES:.co2=.h)

GENERATED_CO2DEPEND=\
$(libmy_object_carbon_1_0_la_CO2SOURCES:.co2=.d)

BUILT_SOURCES=\
$(GENERATED_CSOURCES)\
$(GENERATED_CHEADERS)

EXTRA_DIST=\
$(GENERATED_CO2DEPEND)

libmy_object_carbon_1_0_la_SOURCES=\
$(libmy_object_carbon_1_0_la_CO2SOURCES)\
$(GENERATED_CSOURCES)

libmy_object_carbon_1_0_la_HEADERS=\
$(libmy_object_carbon_1_0_la_CO2SOURCES)\
$(GENERATED_CHEADERS)

libmy_object_carbon_1_0_la_CPPFLAGS=-O0 -g3 -DO_DEBUG

CARBON_FLAGS=-I .

%.c: %.co2
	if test "$(CARBON)" == "no"; then echo "please install carbon first"; else rm -f $@ | true; $(CARBON) $(CARBON_FLAGS) $< $@; indent $@; fi

%.h: %.c

%.d: %.co2
	-rm -f $@
	echo -n "`echo $@ | sed 's/\.d/.c/g'` " > $@
	if test "$(CARBON)" == "no"; then echo ": " $< >> $@; else $(CARBON) --depend $(CARBON_FLAGS) $< $@ >> $@ | rm -f $@; fi

clean-local:
	if test "$(CARBON)" == "no"; then rm -f $(GENERATED_CO2DEPEND); else rm -f $(GENERATED_CHEADERS) $(GENERATED_CSOURCES) $(GENERATED_CO2DEPEND); fi

include $(GENERATED_CO2DEPEND)
