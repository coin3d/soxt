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
	SoAnyExaminerViewer.cpp

############################################################################
##

libSoXtViewersVPATH = . $(srcdir)

.cpp.lo:
	@( source='$<'; \
	for dir in $(libSoXtViewersVPATH); do \
	  test ! -f "$$source" && test -f "$$dir/$<" && source="$$dir/$<"; \
	done; \
	eval "$(CXXDEPMODE)"; \
	: ${depmode=none}; \
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
	SoXtExaminerViewer.h

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

############################################################################
##

libSoXtViewersincdir = $(includedir)/Inventor/Xt/viewers

libSoXtViewersinc_HEADERS = \
	$(top_builddir)/src/Inventor/Xt/viewers/SoXtViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtFullViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtExaminerViewer.h

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
