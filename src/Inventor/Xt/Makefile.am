############################################################################
## SoXt/src/Inventor/Xt/Makefile.am
## $Id$

include common/Makefile.common

SUBDIRS = devices editors engines nodes viewers widgets .

DEFAULT_INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src -I$(top_builddir)/data

BUILT_SOURCES = \
	$(SoGuiAllBuiltFiles)

CLEANFILES = \
	$(SoGui_src_Inventor_Gui_BuiltMetaFiles)

DISTCLEANFILES = \
	$(SoGui_src_Inventor_Gui_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_BuiltMetaFiles)

############################################################################

lib_LTLIBRARIES = libSoXt.la

RegularSources = \
	SoXtInternal.cpp \
	SoXt.cpp \
	SoXtComponent.cpp \
	SoXtGLWidget.cpp \
	SoXtResource.cpp \
	SoXtClipboard.cpp
BuiltSources = \
	$(SoGui_src_Inventor_Gui_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_BuiltMetaFiles)

LinkHackSources = \
	all-soxt-cpp.cpp

if HACKING_COMPACT_BUILD
BuildSources = $(LinkHackSources)
NoDistSources =
ExtraSources = $(RegularSources)
else
BuildSources = $(RegularSources)
NoDistSources = $(BuiltSources)
ExtraSources = $(LinkHackSources)
endif

libSoXt_la_SOURCES = \
	$(BuildSources)

nodist_libSoXt_la_SOURCES = \
	$(NoDistSources)

libSoXt_la_LIBADD = \
	devices/libSoXtDevices.la \
	editors/libSoXtEditors.la \
	engines/libSoGuiEngines.la \
	nodes/libSoGuiNodes.la \
	viewers/libSoXtViewers.la \
	widgets/libSoXtWidgets.la

libSoXt_la_LDFLAGS = \
	-no-undefined \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

############################################################################

SoXtincdir = $(includedir)/Inventor/Xt

SoXtinc_HEADERS = \
	SoXtClipboard.h \
	SoXtResource.h

nodist_SoXtinc_HEADERS = \
	SoXt.h \
	SoXtBasic.h \
	SoXtObject.h \
	SoXtCursor.h \
	SoXtComponent.h \
	SoXtGLWidget.h \
	SoXtRenderArea.h \
	SoXtColorEditor.h \
	SoXtMaterialEditor.h


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
	  "$$baseincdir/Xt/editors" \
	  "$$baseincdir/Xt/nodes" \
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
