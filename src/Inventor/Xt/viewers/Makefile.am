############################################################################
## SoXt/src/Inventor/Xt/viewers/Makefile.am
## $Id$

include ../common/Makefile.common

noinst_LTLIBRARIES = libSoXtViewers.la

INCLUDES = -I$(top_srcdir)/src
LIBS =

BUILT_SOURCES = \
	$(SoGuiAllBuiltFiles)

CLEANFILES = \
	$(SoGui_src_Inventor_Gui_viewers_BuiltMetaFiles)

DISTCLEANFILES = \
	$(SoGui_src_Inventor_Gui_viewers_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_viewers_BuiltMetaFiles)

############################################################################

libSoXtViewers_la_SOURCES = \
	So@Gui@FullViewer.cpp \
	So@Gui@FullViewer.h \
	So@Gui@ExaminerViewer.cpp \
	So@Gui@ExaminerViewer.h \
	So@Gui@PlaneViewer.cpp \
	So@Gui@PlaneViewer.h \
	So@Gui@WalkViewer.cpp \
	So@Gui@WalkViewer.h

nodist_libSoXtViewers_la_SOURCES = \
	$(SoGui_src_Inventor_Gui_viewers_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_viewers_BuiltMetaFiles)

EXTRA_libSoXtViewers_la_SOURCES = \
	SoNativeFlyViewer.cpp

############################################################################

SoXtViewersincdir = $(includedir)/Inventor/@Gui@/viewers

SoXtViewersinc_HEADERS = \
	So@Gui@FullViewer.h \
	So@Gui@ExaminerViewer.h \
	So@Gui@PlaneViewer.h \
	So@Gui@WalkViewer.h

nodist_SoXtViewersinc_HEADERS = \
	So@Gui@Viewer.h \
	So@Gui@ConstrainedViewer.h \
	So@Gui@FlyViewer.h

############################################################################
