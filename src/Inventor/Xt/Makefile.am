############################################################################
## SoXt/src/Inventor/Xt/Makefile.am
## $Id$

include common/Makefile.common

SUBDIRS = devices widgets viewers .

INCLUDES = -I$(top_srcdir)/src -I$(top_builddir)/data

BUILT_SOURCES = \
	$(SoGuiAllBuiltSources)

CLEANFILES = \
	$(SoGui_src_Inventor_Gui_BuiltMetaFiles)
	
DISTCLEANFILES = \
	$(SoGui_src_Inventor_Gui_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_BuiltMetaFiles)
	
############################################################################

lib_LTLIBRARIES = libSoXt.la

libSoXt_la_SOURCES = \
	So@Gui@.cpp \
	So@Gui@Clipboard.h \
	So@Gui@Clipboard.cpp \
	So@Gui@Component.cpp \
	So@Gui@DirectionalLightEditor.h \
	So@Gui@DirectionalLightEditor.cpp \
	So@Gui@GLWidget.cpp \
	So@Gui@Icons.h \
	So@Gui@Icons.cpp \
	So@Gui@LightSliderSet.h \
	So@Gui@LightSliderSet.cpp \
	So@Gui@MaterialEditor.h \
	So@Gui@MaterialEditor.cpp \
	So@Gui@MaterialList.h \
	So@Gui@MaterialList.cpp \
	So@Gui@MaterialSliderSet.h \
	So@Gui@MaterialSliderSet.cpp \
	So@Gui@PrintDialog.h \
	So@Gui@PrintDialog.cpp \
	So@Gui@Resource.h \
	So@Gui@Resource.cpp \
	So@Gui@SliderSet.h \
	So@Gui@SliderSet.cpp \
	So@Gui@SliderSetBase.h \
	So@Gui@SliderSetBase.cpp \
	So@Gui@TransformSliderSet.h \
	So@Gui@TransformSliderSet.cpp

nodist_libSoXt_la_SOURCES = \
	$(SoGui_src_Inventor_Gui_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_BuiltMetaFiles)

libSoXt_la_LIBADD = \
	devices/libSoXtDevices.la \
	widgets/libSoXtWidgets.la \
	viewers/libSoXtViewers.la

libSoXt_la_LDFLAGS = \
	-no-undefined \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

############################################################################

SoXtincdir = $(includedir)/Inventor/@Gui@

SoXtinc_HEADERS = \
	So@Gui@Clipboard.h \
	So@Gui@DirectionalLightEditor.h \
	So@Gui@Icons.h \
	So@Gui@LightSliderSet.h \
	So@Gui@MaterialEditor.h \
	So@Gui@MaterialList.h \
	So@Gui@MaterialSliderSet.h \
	So@Gui@PrintDialog.h \
	So@Gui@Resource.h \
	So@Gui@SliderSet.h \
	So@Gui@SliderSetBase.h \
	So@Gui@TransformSliderSet.h

nodist_SoXtinc_HEADERS = \
	So@Gui@.h \
	So@Gui@Basic.h \
	So@Gui@Cursor.h \
	So@Gui@Cursors.h \
	So@Gui@Component.h \
	So@Gui@Object.h \
	So@Gui@GLWidget.h \
	So@Gui@RenderArea.h

install-SoXtincHEADERS: $(SoXtinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(SoXtincdir)
	@list='$(SoXtinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  src="$$d$$p"; dst="$(DESTDIR)$(SoXtincdir)/$$f"; \
	  if cmp -s "$$src" "$$dst"; then :; else \
	    echo " $(INSTALL_HEADER) $$src $$dst"; \
	    $(INSTALL_HEADER) "$$src" "$$dst"; \
	  fi \
	done

install-nodist_SoXtincHEADERS: $(nodist_SoXtinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(SoXtincdir)
	@list='$(nodist_SoXtinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  src="$$d$$p"; dst="$(DESTDIR)$(SoXtincdir)/$$f"; \
	  if cmp -s "$$src" "$$dst"; then :; else \
	    echo " $(INSTALL_HEADER) $$src $$dst"; \
	    $(INSTALL_HEADER) "$$src" "$$dst"; \
	  fi \
	done

############################################################################

uninstall-local: uninstall-SoXtincHEADERS uninstall-nodist_SoXtincHEADERS
	@baseincdir="$(DESTDIR)$(includedir)/Inventor"; \
	for dir in \
	  "$$baseincdir/Xt/devices" \
	  "$$baseincdir/Xt/viewers" \
	  "$$baseincdir/Xt/widgets" \
	  "$$baseincdir/Xt" \
	  "$$baseincdir"; do \
	  if test -d "$$dir"; then \
	    echo " rmdir" "$$dir"; \
	    rmdir "$$dir" &>/dev/null; \
	  fi \
	done; \
	:

############################################################################
