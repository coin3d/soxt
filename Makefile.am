############################################################################
## $Id$

include src/Inventor/Xt/common/Makefile.common

ACLOCAL_AMFLAGS = -I cfg/m4

############################################################################

if BUILD_LIBRARY
LIBDIRS = data src
else
LIBDIRS =
endif

if BUILD_HTMLPAGES
HTMLDIR = html
else
HTMLDIR =
endif

SUBDIRS = . $(LIBDIRS) man $(HTMLDIR)

############################################################################

BUILT_SOURCES = \
	$(SoGuiAllBuiltFiles)

CLEANFILES = \
	$(SoGui_BuiltMetaFiles)

DISTCLEANFILES = \
	$(SoGui_BuiltFiles) \
	$(SoGui_BuiltMetaFiles) \
	build/so@gui@.doxygen

EXTRA_DIST = \
	cfg/m4/doxy4win.pl \
	build/so@gui@.spec.in \
	build/so@gui@.doxygen.in \
	$(SoGuiCommonDistFiles)

if BUILD_LIBRARY

acdatadir = $(datadir)/aclocal
# Don't replace "xt" with "@gui@" here, as it will pøkk up
# for ``make dist''.
dist_acdata_DATA = soxt.m4

cfgdatadir = $(datadir)/Coin/conf
cfgdata_DATA = so@gui@-@CONFIG@.cfg

bin_SCRIPTS = so@gui@-config

endif

built-sources: $(BUILT_SOURCES)
all-local: built-sources

############################################################################
