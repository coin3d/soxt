############################################################################
## SoXt/src/Inventor/Xt/engines/Makefile.am

include ../common/Makefile.common

noinst_LTLIBRARIES = libSoGuiEngines.la

DEFAULT_INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src
LIBS =

BUILT_SOURCES = \
	$(SoGuiAllBuiltFiles)

CLEANFILES = \
	$(SoGui_src_Inventor_Gui_engines_BuiltMetaFiles)

DISTCLEANFILES = \
	$(SoGui_src_Inventor_Gui_engines_BuiltFiles) \
	$(SoGui_src_Inventor_Gui_engines_BuiltMetaFiles)

############################################################################

# libSoGuiEngines_la_SOURCES =

RegularSources = 
LinkHackSources = \
	all-engines-cpp.cpp
BuiltSources = \
	$(SoGui_src_Inventor_Gui_engines_BuiltFiles)

if HACKING_COMPACT_BUILD
BuildSources = $(LinkHackSources)
NoDistSources =
ExtraSources = $(RegularSources)
else
BuildSources = $(RegularSources)
NoDistSources = $(BuiltSources)
ExtraSources = $(LinkHackSources)
endif

libSoGuiEngines_la_SOURCES = \
	$(BuildSources)

nodist_libSoGuiEngines_la_SOURCES = \
	$(NoDistSources)

############################################################################
## Header file installation information.

# The engines are kept internal for now.  We know people will ask for them
# to be made public, but until then we'll let them mature.  Only headers
# that other public headers depend on are installed for now.

SoGuiEnginesincdir = $(includedir)/Inventor/@Gui@/engines

SoGuiEnginesinc_HEADERS =

############################################################################
