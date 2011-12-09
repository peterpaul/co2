MAINTAINERCLEANFILES=Makefile.in
ACLOCAL_AMFLAGS=-I m4

lib_LTLIBRARIES=libco2-1.0.la
libco2_1_0_ladir=$(includedir)/co2-1.0/co2

libco2_1_0_la_SOURCES=\
 co2/Object.c\
 co2/utils.c\
 co2/Interface.c\
 co2/Singleton.c\
 co2/exception.c

libco2_1_0_la_HEADERS=\
$(libco2_1_0_la_SOURCES:.c=.h)

libco2_1_0_la_CPPFLAGS=-DO_DEBUG
