############################################################################

SUBDIRS = devices viewers widgets

INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src

lib_LTLIBRARIES = libSoXt.la

BUILT_SOURCES = \
	SoXtRenderArea.h \
	SoXtRenderArea.cpp

libSoXt_la_SOURCES = \
	SoXt.h \
	SoXt.cpp \
	SoXtComponent.h \
	SoXtComponent.cpp \
	SoXtGLWidget.h \
	SoXtGLWidget.cpp \
	$(BUILT_SOURCES) \
	SoXtRenderAreaExtra.cpp

libSoXt_la_LIBADD = \
	devices/libSoXtDevices.la \
	viewers/libSoXtViewers.la \
	widgets/libSoXtWidgets.la

libSoXtincdir = $(includedir)/Inventor/Xt

libSoXtinc_HEADERS = \
	SoXt.h \
	SoXtComponent.h \
	SoXtGLWidget.h \
	$(top_builddir)/src/Inventor/Xt/SoXtRenderArea.h

############################################################################

SoXtRenderArea.h: $(top_srcdir)/src/Inventor/Xt/common/SoGuiRenderArea.h.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Xt/SoXtRenderArea.h )

SoXtRenderArea.cpp: $(top_srcdir)/src/Inventor/Xt/common/SoGuiRenderArea.cpp.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Xt/SoXtRenderArea.cpp )

############################################################################

install-libSoXtincHEADERS: $(libSoXtinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libSoXtincdir)
	@list='$(libSoXtinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libSoXtincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libSoXtincdir)/$$f; \
	done

uninstall-libSoXtincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libSoXtinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libSoXtincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libSoXtincdir)/$$f; \
	done

############################################################################
