
INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src
LIBS =

noinst_LTLIBRARIES = libSoXtViewers.la

libSoXtViewers_la_SOURCES = \
	$(top_builddir)/src/Inventor/Xt/viewers/SoXtViewer.cpp \
	$(top_builddir)/src/Inventor/Xt/viewers/SoXtViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtViewerExtra.cpp \
	$(top_srcdir)/src/Inventor/Xt/common/SoAnyFullViewer.cpp \
	$(top_srcdir)/src/Inventor/Xt/common/SoAnyFullViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtFullViewer.cpp \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtFullViewer.h \
	$(top_srcdir)/src/Inventor/Xt/common/SoAnyExaminerViewer.cpp \
	$(top_srcdir)/src/Inventor/Xt/common/SoAnyExaminerViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtExaminerViewer.cpp \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtExaminerViewer.h

libSoXtViewersincdir = $(includedir)/Inventor/Xt/viewers

libSoXtViewersinc_HEADERS = \
	$(top_builddir)/src/Inventor/Xt/viewers/SoXtViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtFullViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtExaminerViewer.h

############################################################################

SoXtViewer.h: $(top_srcdir)/src/Inventor/Xt/common/SoAnyViewer.h.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Xt/viewers/SoXtViewer.h )

SoXtViewer.cpp: $(top_srcdir)/src/Inventor/Xt/common/SoAnyViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Xt/viewers/SoXtViewer.cpp )

############################################################################

SoAnyFullViewer.lo: $(top_srcdir)/src/Inventor/Xt/common/SoAnyFullViewer.cpp
	source='$(top_srcdir)/src/Inventor/Xt/common/SoAnyFullViewer.cpp' \
	object='$@' libtool=yes \
	depfile='$(DEPDIR)/$*.Plo' tmpdepfile='$(DEPDIR)/$*.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LTCXXCOMPILE) -c -o $@ \
	$(top_srcdir)/src/Inventor/Xt/common/SoAnyFullViewer.cpp

SoAnyExaminerViewer.lo: $(top_srcdir)/src/Inventor/Xt/common/SoAnyExaminerViewer.cpp
	source='$(top_srcdir)/src/Inventor/Xt/common/SoAnyExaminerViewer.cpp' \
	object='$@' libtool=yes \
	depfile='$(DEPDIR)/$*.Plo' tmpdepfile='$(DEPDIR)/$*.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LTCXXCOMPILE) -c -o $@ \
	$(top_srcdir)/src/Inventor/Xt/common/SoAnyExaminerViewer.cpp

############################################################################

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
