
INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src

noinst_LTLIBRARIES = libSoXtViewers.la

libSoXtViewers_la_SOURCES = \
	$(top_builddir)/src/Inventor/Xt/viewers/SoXtViewer.cpp \
	$(top_builddir)/src/Inventor/Xt/viewers/SoXtViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtViewerExtra.cpp \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtFullViewer.cpp \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtFullViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtExaminerViewer.cpp \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtExaminerViewer.h

libSoXtViewersincdir = $(includedir)/Inventor/Xt/viewers

libSoXtViewersinc_HEADERS = \
	$(top_builddir)/src/Inventor/Xt/viewers/SoXtViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtFullViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtExaminerViewer.h

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
