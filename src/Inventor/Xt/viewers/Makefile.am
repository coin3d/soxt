############################################################################
##

noinst_LTLIBRARIES = libSoXtViewers.la

INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src
LIBS =

BUILT_SOURCES = \
	SoXtViewer.h \
	SoXtViewer.cpp \
	SoAnyFullViewer.h \
	SoAnyFullViewer.cpp \
	SoAnyExaminerViewer.h \
	SoAnyExaminerViewer.cpp \
	SoAnyPlaneViewer.h \
	SoAnyPlaneViewer.cpp \
	SoAnyConstrainedViewer.h \
	SoAnyConstrainedViewer.cpp \
	SoAnyFlyViewer.h \
	SoAnyFlyViewer.cpp \
	SoAnyWalkViewer.h \
	SoAnyWalkViewer.cpp

############################################################################
##

libSoXtViewersVPATH = . $(srcdir)

.cpp.lo:
	@( source='$<'; \
	for dir in $(libSoXtViewersVPATH); do \
	  test ! -f "$$source" && test -f "$$dir/$<" && source="$$dir/$<"; \
	done; \
	eval "$(CXXDEPMODE)"; \
	: $${depmode=none}; \
	source="$$source" \
	object='$@' libtool=no \
	depfile='$(DEPDIR)/$*.Plo' tmpdepfile='$(DEPDIR)/$*.TPlo' \
	depmode=$$depmode $(depcomp) $(LTCXXCOMPILE) -c -o $@ "$$source"; )

libSoXtViewers_la_SOURCES = \
	$(BUILT_SOURCES) \
	SoXtViewerExtra.cpp \
	SoXtFullViewer.cpp \
	SoXtFullViewer.h \
	SoXtExaminerViewer.cpp \
	SoXtExaminerViewer.h \
	SoXtPlaneViewer.cpp \
	SoXtPlaneViewer.h \
	SoXtConstrainedViewer.cpp \
	SoXtConstrainedViewer.h \
	SoXtFlyViewer.cpp \
	SoXtFlyViewer.h \
	SoXtWalkViewer.cpp \
	SoXtWalkViewer.h

############################################################################
##

SoXtViewer.h: $(top_srcdir)/src/Inventor/Xt/common/SoGuiViewer.h.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Xt/viewers/SoXtViewer.h )

SoXtViewer.cpp: $(top_srcdir)/src/Inventor/Xt/common/SoGuiViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Xt/viewers/SoXtViewer.cpp )

SoAnyFullViewer.h: $(top_srcdir)/src/Inventor/Xt/common/SoAnyFullViewer.h.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Xt/viewers/SoAnyFullViewer.h )

SoAnyFullViewer.cpp: $(top_srcdir)/src/Inventor/Xt/common/SoAnyFullViewer.cpp.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Xt/viewers/SoAnyFullViewer.cpp )

SoAnyExaminerViewer.h: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyExaminerViewer.h.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Xt/viewers/SoAnyExaminerViewer.h )

SoAnyExaminerViewer.cpp: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyExaminerViewer.cpp.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Xt/viewers/SoAnyExaminerViewer.cpp )

SoAnyPlaneViewer.h: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyPlaneViewer.h.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Xt/viewers/SoAnyPlaneViewer.h )

SoAnyPlaneViewer.cpp: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyPlaneViewer.cpp.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Xt/viewers/SoAnyPlaneViewer.cpp )

SoAnyConstrainedViewer.h: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyConstrainedViewer.h.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Xt/viewers/SoAnyConstrainedViewer.h )

SoAnyConstrainedViewer.cpp: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyConstrainedViewer.cpp.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Xt/viewers/SoAnyConstrainedViewer.cpp )

SoAnyFlyViewer.h: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyFlyViewer.h.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Xt/viewers/SoAnyFlyViewer.h )

SoAnyFlyViewer.cpp: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyFlyViewer.cpp.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Xt/viewers/SoAnyFlyViewer.cpp )

SoAnyWalkViewer.h: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyWalkViewer.h.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Xt/viewers/SoAnyWalkViewer.h )

SoAnyWalkViewer.cpp: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyWalkViewer.cpp.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Xt/viewers/SoAnyWalkViewer.cpp )

############################################################################
##

libSoXtViewersincdir = $(includedir)/Inventor/Xt/viewers

libSoXtViewersinc_HEADERS = \
	$(top_builddir)/src/Inventor/Xt/viewers/SoXtViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtFullViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtExaminerViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtPlaneViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtConstrainedViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtFlyViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtWalkViewer.h

install-libSoXtViewersincHEADERS: $(libSoXtViewersinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libSoXtViewersincdir)
	@list='$(libSoXtViewersinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libSoXtViewersincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libSoXtViewersincdir)/$$f; \
	done

uninstall-libSoXtViewersincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libSoXtViewersinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libSoXtViewersincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libSoXtViewersincdir)/$$f; \
	done

############################################################################
