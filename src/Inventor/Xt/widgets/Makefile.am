
INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/src
LIBS =

noinst_LTLIBRARIES = libSoXtWidgets.la

libSoXtWidgets_la_SOURCES = \
	SoXtThumbWheel.cpp \
	SoXtThumbWheelP.h \
	SoXtThumbWheel.h

libSoXtWidgetsincdir = $(includedir)/Inventor/Xt/widgets

libSoXtWidgetsinc_HEADERS = \
	SoXtThumbWheelP.h \
	SoXtThumbWheel.h

############################################################################

install-libSoXtWidgetsincHEADERS: $(libSoXtWidgetsinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libSoXtWidgetsincdir)
	@list='$(libSoXtWidgetsinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libSoXtWidgetsincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libSoXtWidgetsincdir)/$$f; \
	done

uninstall-libSoXtWidgetsincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libSoXtWidgetsinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libSoXtWidgetsincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libSoXtWidgetsincdir)/$$f; \
	done

############################################################################
