
INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src
LIBS =

noinst_LTLIBRARIES = libSoXtWidgets.la

libSoXtWidgets_la_SOURCES = \
	SoXtThumbWheel.cpp \
	SoXtThumbWheelP.h \
	$(top_srcdir)/src/Inventor/Xt/common/ThumbWheel.cpp \
	$(top_srcdir)/src/Inventor/Xt/common/ThumbWheel.h \
	SoXtThumbWheel.h

libSoXtWidgetsincdir = $(includedir)/Inventor/Xt/widgets

libSoXtWidgetsinc_HEADERS = \
	SoXtThumbWheelP.h \
	SoXtThumbWheel.h

############################################################################

ThumbWheel.lo: $(top_srcdir)/src/Inventor/Xt/common/ThumbWheel.cpp
	source='$^' object='$@' libtool=yes \
	depfile='$(DEPDIR)/$*.Plo' tmpdepfile='$(DEPDIR)/$*.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LTCXXCOMPILE) -c -o $@ $^
	
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
