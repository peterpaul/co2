MAINTAINERCLEANFILES = Makefile.in ${pkgconfig_DATA}

ACLOCAL_AMFLAGS = -I m4

# README.md/CHANGELOG.md aren't among automake's automatically-distributed
# standard filenames (only the plain README/ChangeLog are) -- list them
# explicitly so make dist doesn't silently drop them.
EXTRA_DIST = README.md CHANGELOG.md

SUBDIRS = src test
pkgconfigdir = ${libdir}/pkgconfig
pkgconfig_DATA = libco2-1.0.pc
