############################################################################
## SoXt/src/Inventor/Xt/viewers/Makefile.am

include ../common/Makefile.common

noinst_LTLIBRARIES = libSoXtViewers.la

DEFAULT_INCLUDES = -I. -I$(srcdir) -I$(top_builddir)/src  -I$(top_srcdir)/src
LIBS =

BUILT_SOURCES = \
	$(SoGuiAllBuiltFiles)

CLEANFILES = \
	$(SoGui_src_Inventor_Gui_viewers_BuiltMetaFiles)

DISTCLEANFILES = \
	$(SoGui_src_Inventor_Gui_viewers_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_viewers_BuiltMetaFiles)

############################################################################

RegularSources = \
	So@Gui@FullViewerP.h \
	FullViewer.cpp \
	So@Gui@ExaminerViewerP.h \
	ExaminerViewer.cpp \
	So@Gui@PlaneViewerP.h \
	PlaneViewer.cpp

BuiltSources = \
	$(SoGui_src_Inventor_Gui_viewers_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_viewers_BuiltMetaFiles)

LinkHackSources = \
	all-viewers-cpp.cpp

if HACKING_COMPACT_BUILD
BuildSources = $(LinkHackSources)
NoDistSources =
ExtraSources = $(RegularSources)
else
BuildSources = $(RegularSources)
NoDistSources = $(BuiltSources)
ExtraSources = $(LinkHackSources)
endif

libSoXtViewers_la_SOURCES = \
	$(BuildSources)

nodist_libSoXtViewers_la_SOURCES = \
	$(NoDistSources)

############################################################################

SoXtViewersincdir = $(includedir)/Inventor/@Gui@/viewers

SoXtViewersinc_HEADERS =

nodist_SoXtViewersinc_HEADERS = \
	So@Gui@Viewer.h \
	So@Gui@ConstrainedViewer.h \
	So@Gui@FullViewer.h \
	So@Gui@ExaminerViewer.h \
	So@Gui@FlyViewer.h \
	So@Gui@PlaneViewer.h

############################################################################
