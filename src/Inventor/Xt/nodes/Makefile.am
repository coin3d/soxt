############################################################################
## SoXt/src/Inventor/Xt/nodes/Makefile.am

include ../common/Makefile.common

noinst_LTLIBRARIES = libSoGuiNodes.la

DEFAULT_INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src
LIBS =

BUILT_SOURCES = \
	$(SoGuiAllBuiltFiles)

CLEANFILES = \
	$(SoGui_src_Inventor_Gui_nodes_BuiltMetaFiles)

DISTCLEANFILES = \
	$(SoGui_src_Inventor_Gui_nodes_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_nodes_BuiltMetaFiles)

############################################################################

# libSoGuiNodes_la_SOURCES =

RegularSources =
BuiltSources = \
	$(SoGui_src_Inventor_Gui_nodes_BuiltFiles)
LinkHackSources = \
	all-nodes-cpp.cpp

if HACKING_COMPACT_BUILD
BuildSources = $(LinkHackSources)
NoDistSources =
ExtraSources = $(RegularSources)
else
BuildSources = $(RegularSources)
NoDistSources = $(BuiltSources)
ExtraSources = $(LinkHackSources)
endif

libSoGuiNodes_la_SOURCES = \
	$(BuildSources)
	
nodist_libSoGuiNodes_la_SOURCES = \
	$(NoDistSources)

############################################################################
## Header file installation information.

# Most nodes are kept internal for now.  We know people will ask for them
# to be made public, but until then we'll let them mature.  Only headers
# that other public headers depend on are installed for now.

SoGuiNodesincdir = $(includedir)/Inventor/@Gui@/nodes

nodist_SoGuiNodesinc_HEADERS = \
	SoGuiColorEditor.h \
	SoGuiMaterialEditor.h

############################################################################
