############################################################################
## SoXt/src/Inventor/Xt/Makefile.am
## $Id$

lib_LTLIBRARIES = libSoXt.la

SUBDIRS = devices widgets viewers

INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src -I$(top_builddir)/data

BUILT_SOURCES = \
	So@Gui@Basic.h \
	So@Gui@RenderArea.h \
	So@Gui@RenderArea.cpp \
	SoAnyMaterialList.h \
	SoAnyMaterialList.cpp \
	SoAnyMaterialListBuiltins.cpp

noinst_DATA = $(BUILT_SOURCES)

############################################################################
##

libSoXt_la_SOURCES = \
	$(BUILT_SOURCES) \
	So@Gui@.h \
	So@Gui@.cpp \
	So@Gui@Clipboard.h \
	So@Gui@Clipboard.cpp \
	So@Gui@Object.h \
	So@Gui@Object.cpp \
	So@Gui@Component.h \
	So@Gui@Component.cpp \
	So@Gui@Cursors.h \
	So@Gui@Cursors.cpp \
	So@Gui@DirectionalLightEditor.h \
	So@Gui@DirectionalLightEditor.cpp \
	So@Gui@GLWidget.h \
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

libSoXt_la_LIBADD = \
	devices/libSoXtDevices.la \
	widgets/libSoXtWidgets.la \
	viewers/libSoXtViewers.la

libSoXt_la_LDFLAGS = \
	-no-undefined \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

############################################################################
##

So@Gui@Basic.h: $(srcdir)/common/SoGuiBasic.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

So@Gui@RenderArea.h: $(srcdir)/common/SoGuiRenderArea.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

So@Gui@RenderArea.cpp: $(srcdir)/common/SoGuiRenderArea.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyMaterialList.h: $(srcdir)/common/SoAnyMaterialList.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyMaterialList.cpp: $(srcdir)/common/SoAnyMaterialList.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyMaterialListBuiltins.cpp: $(srcdir)/common/SoAnyMaterialListBuiltins.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

############################################################################
##

libSoXtincdir = $(includedir)/Inventor/@Gui@

libSoXtinc_HEADERS = \
	So@Gui@.h \
	So@Gui@Clipboard.h \
	So@Gui@Object.h \
	So@Gui@Component.h \
	So@Gui@Cursors.h \
	So@Gui@DirectionalLightEditor.h \
	So@Gui@GLWidget.h \
	So@Gui@Icons.h \
	So@Gui@LightSliderSet.h \
	So@Gui@MaterialEditor.h \
	So@Gui@MaterialList.h \
	So@Gui@MaterialSliderSet.h \
	So@Gui@PrintDialog.h \
	So@Gui@Resource.h \
	So@Gui@SliderSet.h \
	So@Gui@SliderSetBase.h \
	So@Gui@TransformSliderSet.h \
	$(top_builddir)/src/Inventor/@Gui@/So@Gui@Basic.h \
	$(top_builddir)/src/Inventor/@Gui@/So@Gui@RenderArea.h

############################################################################
