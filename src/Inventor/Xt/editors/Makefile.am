############################################################################
## SoXt/src/Inventor/Xt/editors/Makefile.am

include ../common/Makefile.common

noinst_LTLIBRARIES = libSoXtEditors.la

DEFAULT_INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src
LIBS =

BUILT_SOURCES = \
	$(SoGuiAllBuiltFiles)

CLEANFILES = \
	$(SoGui_src_Inventor_Gui_editors_BuiltMetaFiles)

DISTCLEANFILES = \
	$(SoGui_src_Inventor_Gui_editors_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_editors_BuiltMetaFiles)

############################################################################

RegularSources =
BuiltSources = \
	$(SoGui_src_Inventor_Gui_editors_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_editors_BuiltMetaFiles)
LinkHackSources = \
	all-editors-cpp.cpp

if HACKING_COMPACT_BUILD
BuildSources = $(LinkHackSources)
NoDistSources =
ExtraSources = $(RegularSources)
else
BuildSources = $(RegularSources)
NoDistSources = $(BuiltSources)
ExtraSources = $(LinkHackSources)
endif

libSoXtEditors_la_SOURCES = \
	$(BuildSources)

nodist_libSoXtEditors_la_SOURCES = \
	$(NoDistSources)


############################################################################

SoXtEditorsincdir = $(includedir)/Inventor/@Gui@/editors

SoXtEditorsinc_HEADERS =

nodist_SoXtEditorsinc_HEADERS = \
	So@Gui@ColorEditor.h \
	So@Gui@MaterialEditor.h

############################################################################
