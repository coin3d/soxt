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

nodist_libSoGuiNodes_la_SOURCES = \
	$(SoGui_src_Inventor_Gui_nodes_BuiltFiles)

############################################################################
## Header file installation information.

# These nodes are kept internal for now.  We know people will ask for them
# to be made public, but until then we'll let them mature.  Besides, the
# nodes can be used indirectly already through getTypeByName(),
# createInstance(), getFieldByName() and such indirect methods, which will
# allow us to change the nodes without any consequences for others.

# SoGuiNodesincdir = $(includedir)/Inventor/@Gui@/nodes
# 
# SoGuiNodesinc_HEADERS = \
# 	SoGuiNode.h \
# 	SoGuiSlider1.h

############################################################################
