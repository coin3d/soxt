############################################################################
## SoXt/src/Inventor/Xt/widgets/Makefile.am
## $Id$

include ../common/Makefile.common

noinst_LTLIBRARIES = libSoXtWidgets.la

DEFAULT_INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src
LIBS =

BUILT_SOURCES = \
	$(SoGuiAllBuiltSources)

CLEANFILES = \
	$(SoGui_src_Inventor_Gui_widgets_BuiltMetaFiles)

DISTCLEANFILES = \
	$(SoGui_src_Inventor_Gui_widgets_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_widgets_BuiltMetaFiles)

############################################################################

RegularSources = \
	SoXtGLArea.h \
	SoXtGLAreaP.h \
	SoXtGLArea.cpp \
	XtNativePopupMenu.h \
	XtNativePopupMenu.cpp \
	SoXtThumbWheel.h \
	SoXtThumbWheelP.h \
	SoXtThumbWheel.cpp \
	SoXtSlider.h \
	SoXtSlider.cpp

LinkHackSources = \
	all-widgets-cpp.cpp

BuiltSources = \
	$(SoGui_src_Inventor_Gui_widgets_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_widgets_BuiltMetaFiles)

if HACKING_COMPACT_BUILD
BuildSources = $(LinkHackSources)
NoDistSources =
ExtraSources = $(RegularSources)
else
BuildSources = $(RegularSources)
NoDistSources = $(BuiltSources)
ExtraSources = $(LinkHackSources)
endif

libSoXtWidgets_la_SOURCES = \
	$(BuildSources)

nodist_libSoXtWidgets_la_SOURCES = \
	$(NoDistSources)

############################################################################

SoXtWidgetsincdir = $(includedir)/Inventor/Xt/widgets
SoXtWidgetsinc_HEADERS =
nodist_SoXtWidgetsinc_HEADERS = SoXtPopupMenu.h

############################################################################
