############################################################################
## $Id$

include src/Inventor/Xt/common/Makefile.common

############################################################################

if BUILD_LIBRARY
# Material editor(s) not part of the public API yet, so there's
# no point yet in installing the material data.
#LIBDIRS = data src
LIBDIRS = src
else
LIBDIRS =
endif

if BUILD_HTMLPAGES
HTMLDIR = html
else
HTMLDIR =
endif

SUBDIRS = . $(LIBDIRS) man $(HTMLDIR) # test-code

############################################################################

BUILT_SOURCES = \
	$(SoGuiAllBuiltFiles)

CLEANFILES = \
	$(SoGui_BuiltMetaFiles)

DISTCLEANFILES = \
	$(SoGui_BuiltFiles) \
	$(SoGui_BuiltMetaFiles)

EXTRA_DIST = \
	LICENSE.GPL \
	build/so@gui@.spec.in \
	cfg/soxt.m4 \
	docs/announcement-1_1_0.txt \
	$(SoGuiCommonDistFiles)

if BUILD_LIBRARY

acdatadir = $(datadir)/aclocal
# Don't replace "xt" with "@gui@" here, as it will pøkk up
# for ``make dist''.
dist_acdata_DATA = cfg/soxt.m4

cfgdatadir = $(datadir)/Coin/conf
cfgdata_DATA = so@gui@-@CONFIG@.cfg

bin_SCRIPTS = so@gui@-config

endif

built-sources: $(BUILT_SOURCES)
all-local: built-sources

############################################################################
