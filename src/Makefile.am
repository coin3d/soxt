
SUBDIRS = . Inventor

BUILT_SOURCES = \
	soxtdefs.h

DISTCLEANFILES = \
	$(BUILT_SOURCES)

soxtdefs.h: $(top_builddir)/src/Inventor/Xt/common/soguidefs.h.in
	@( cd ..; $(MAKE) src/soxtdefs.h )

