############################################################################
##

noinst_LTLIBRARIES = libSoXtWidgets.la

INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src
LIBS =
VPATH = .:@srcdir@
SHELL = /bin/sh

BUILT_SOURCES = \
	SoAnyPopupMenu.h \
	SoAnyPopupMenu.cpp \
	SoAnyThumbWheel.h \
	SoAnyThumbWheel.cpp

############################################################################
## 

libSoXtWidgetsVPATH = . $(srcdir)

.cpp.lo:
	@( source='$<'; \
	for dir in $(libSoXtWidgetsVPATH); do \
	  test ! -f "$$source" && test -f "$$dir/$<" && source="$$dir/$<"; \
	done; \
	eval "$(CXXDEPMODE)"; \
	: $${depmode=none}; \
	source="$$source" \
	object='$@' libtool=no \
	depfile='$(DEPDIR)/$*.Plo' tmpdepfile='$(DEPDIR)/$*.TPlo' \
	depmode=$$depmode $(depcomp) $(LTCXXCOMPILE) -c -o $@ "$$source"; )

libSoXtWidgets_la_SOURCES = \
	$(BUILT_SOURCES) \
	SoXtPopupMenu.h \
	SoXtPopupMenu.cpp \
	SoXtThumbWheel.h \
	SoXtThumbWheelP.h \
	SoXtThumbWheel.cpp

############################################################################

SoAnyPopupMenu.h: $(top_srcdir)/src/Inventor/Xt/common/SoAnyPopupMenu.h.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Xt/widgets/SoAnyPopupMenu.h )

SoAnyPopupMenu.cpp: $(top_srcdir)/src/Inventor/Xt/common/SoAnyPopupMenu.cpp.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Xt/widgets/SoAnyPopupMenu.cpp )

SoAnyThumbWheel.h: $(top_srcdir)/src/Inventor/Xt/common/SoAnyThumbWheel.h.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Xt/widgets/SoAnyThumbWheel.h )

SoAnyThumbWheel.cpp: $(top_srcdir)/src/Inventor/Xt/common/SoAnyThumbWheel.cpp.in
	@( cd $(top_builddir); \
	   $(MAKE) src/Inventor/Xt/widgets/SoAnyThumbWheel.cpp )

############################################################################

libSoXtWidgetsincdir = $(includedir)/Inventor/Xt/widgets

libSoXtWidgetsinc_HEADERS = \
	SoXtThumbWheelP.h \
	SoXtThumbWheel.h

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
