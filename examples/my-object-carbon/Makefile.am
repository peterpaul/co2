MAINTAINERCLEANFILES = Makefile.in ${pkgconfig_DATA}
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src test
pkgconfigdir = ${libdir}/pkgconfig
pkgconfig_DATA = my-object-carbon-1.0.pc

distclean-local:
	-rm -f doltcompile doltlibtool
