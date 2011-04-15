MAINTAINERCLEANFILES = Makefile.in ${pkgconfig_DATA}
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src test
pkgconfigdir = ${libdir}/pkgconfig
pkgconfig_DATA = carbon-1.0.pc

PKG_NAME=carbon

deb-pkg:
	- rm -r debian
	dh_make -s -c gpl -p $(PKG_NAME)_$(VERSION) --createorig
	- rm debian/README.Debian debian/*.ex debian/*.EX
	- cp package/debian/* debian
	debuild -us -uc
