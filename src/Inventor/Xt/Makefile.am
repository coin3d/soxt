############################################################################

lib_LTLIBRARIES = libSoXt.la

SUBDIRS = devices widgets viewers

INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src
VPATH = .:@srcdir@
SHELL = /bin/sh

BUILT_SOURCES = \
	SoXtBasic.h \
	SoXtRenderArea.h \
	SoXtRenderArea.cpp

############################################################################
##

libSoXtVPATH = . $(srcdir)

.cpp.lo:
	@( source='$<'; \
	for dir in $(libSoXtVPATH); do \
	  test ! -f "$$source" && test -f "$$dir/$<" && source="$$dir/$<"; \
	done; \
	eval "$(CXXDEPMODE)"; \
	: $${depmode=none}; \
	source="$$source" \
	object='$@' libtool=no \
	depfile='$(DEPDIR)/$*.Plo' tmpdepfile='$(DEPDIR)/$*.TPlo' \
	depmode=$$depmode $(depcomp) $(LTCXXCOMPILE) -c -o $@ "$$source"; )

libSoXt_la_SOURCES = \
	$(BUILT_SOURCES) \
	SoXt.h \
	SoXt.cpp \
	SoXtClipboard.h \
	SoXtClipboard.cpp \
	SoXtComponent.h \
	SoXtComponent.cpp \
	SoXtCursors.h \
	SoXtCursors.cpp \
	SoXtDirectionalLightEditor.h \
	SoXtDirectionalLightEditor.cpp \
	SoXtGLWidget.h \
	SoXtGLWidget.cpp \
	SoXtIcons.h \
	SoXtIcons.cpp \
	SoXtLightSliderSet.h \
	SoXtLightSliderSet.cpp \
	SoXtMaterialEditor.h \
	SoXtMaterialEditor.cpp \
	SoXtMaterialList.h \
	SoXtMaterialList.cpp \
	SoXtMaterialSliderSet.h \
	SoXtMaterialSliderSet.cpp \
	SoXtPrintDialog.h \
	SoXtPrintDialog.cpp \
	SoXtRenderAreaExtra.cpp \
	SoXtResource.h \
	SoXtResource.cpp \
	SoXtSliderSet.h \
	SoXtSliderSet.cpp \
	SoXtSliderSetBase.h \
	SoXtSliderSetBase.cpp \
	SoXtTransformSliderSet.h \
	SoXtTransformSliderSet.cpp

libSoXt_la_LIBADD = \
	devices/libSoXtDevices.la \
	viewers/libSoXtViewers.la \
	widgets/libSoXtWidgets.la

############################################################################
##

SoXtBasic.h: $(top_srcdir)/src/Inventor/Xt/common/SoGuiBasic.h.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Xt/SoXtBasic.h )

SoXtRenderArea.h: $(top_srcdir)/src/Inventor/Xt/common/SoGuiRenderArea.h.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Xt/SoXtRenderArea.h )

SoXtRenderArea.cpp: $(top_srcdir)/src/Inventor/Xt/common/SoGuiRenderArea.cpp.in
	@( cd $(top_builddir); $(MAKE) src/Inventor/Xt/SoXtRenderArea.cpp )

############################################################################
##

libSoXtincdir = $(includedir)/Inventor/Xt

libSoXtinc_HEADERS = \
	SoXt.h \
	SoXtClipboard.h \
	SoXtComponent.h \
	SoXtCursors.h \
	SoXtDirectionalLightEditor.h \
	SoXtGLWidget.h \
	SoXtIcons.h \
	SoXtLightSliderSet.h \
	SoXtMaterialEditor.h \
	SoXtMaterialList.h \
	SoXtMaterialSliderSet.h \
	SoXtPrintDialog.h \
	SoXtResource.h \
	SoXtSliderSet.h \
	SoXtSliderSetBase.h \
	SoXtTransformSliderSet.h \
	$(top_builddir)/src/Inventor/Xt/SoXtRenderArea.h

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
