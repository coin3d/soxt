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

# libSoXtEditors_la_SOURCES =

nodist_libSoXtEditors_la_SOURCES = \
	$(SoGui_src_Inventor_Gui_editors_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_editors_BuiltMetaFiles)

############################################################################

SoXtEditorsincdir = $(includedir)/Inventor/@Gui@/editors

SoXtEditorsinc_HEADERS =

nodist_SoXtEditorsinc_HEADERS = \
	So@Gui@ColorEditor.h \
	So@Gui@MaterialEditor.h

############################################################################
