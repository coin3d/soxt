############################################################################

SUBDIRS = devices viewers widgets

############################################################################

INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src

lib_LTLIBRARIES = libSoXt.la

libSoXt_la_SOURCES = \
	SoXt.h \
	SoXt.cpp \
	SoXtComponent.h \
	SoXtComponent.cpp \
	SoXtGLWidget.h \
	SoXtGLWidget.cpp \
	SoXtRenderArea.h \
	SoXtRenderArea.cpp

libSoXt_la_LIBADD = \
	devices/libSoXtDevices.la

libSoXtincdir = $(includedir)/Inventor/Xt

libSoXtinc_HEADERS = \
	SoXt.h \
	SoXtComponent.h \
	SoXtGLWidget.h \
	SoXtRenderArea.h

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

