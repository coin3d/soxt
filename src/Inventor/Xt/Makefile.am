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
	SoXtInternal.cpp SoXtInternal.h \
	SoXt.cpp \
	SoXtComponent.cpp \
	SoXtGLWidget.cpp \
	SoXtLightSliderSet.cpp SoXtLightSliderSet.h \
	SoXtMaterialSliderSet.cpp SoXtMaterialSliderSet.h \
	SoXtSliderSet.cpp SoXtSliderSet.h \
	SoXtSliderSetBase.cpp SoXtSliderSetBase.h \
	SoXtTransformSliderSet.cpp SoXtTransformSliderSet.h \
	SoXtResource.cpp SoXtResource.h \
	SoXtMaterialList.cpp

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

SoXtincdir = $(includedir)/Inventor/Xt

SoXtinc_HEADERS = \
	SoXtLightSliderSet.h \
	SoXtMaterialSliderSet.h \
	SoXtResource.h \
	SoXtSliderSet.h \
	SoXtSliderSetBase.h \
	SoXtTransformSliderSet.h

nodist_SoXtinc_HEADERS = \
	SoXt.h \
	SoXtBasic.h \
	SoXtObject.h \
	SoXtCursor.h \
	SoXtComponent.h \
	SoXtGLWidget.h \
	SoXtRenderArea.h \
	SoXtMaterialList.h


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
