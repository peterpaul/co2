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

libmy_object_carbon_1_0_la_SOURCES=\
$(libmy_object_carbon_1_0_la_CO2SOURCES)\
$(GENERATED_CSOURCES)

libmy_object_carbon_1_0_la_HEADERS=\
$(libmy_object_carbon_1_0_la_CO2SOURCES)\
$(GENERATED_CHEADERS)

libmy_object_carbon_1_0_la_CPPFLAGS=-O0 -g3 -DO_DEBUG

CARBON_FLAGS=-I $(srcdir)

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
	-rm -f $(bin_PROGRAMS)
if CARBON_EXISTS
	-rm -f $(GENERATED_CO2DEPEND)
endif

if CARBON_EXISTS
include $(GENERATED_CO2DEPEND)
endif
