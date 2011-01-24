EXTRA_DIST = defs $(TESTS)
MAINTAINERCLEANFILES = Makefile.in

targetdir = target

TESTS_ENVIRONMENT = CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)"
TESTS=run_tests.sh

distclean-local:
	-rm -rf $(targetdir)
