############################################################################
## $Id$

include src/Inventor/Xt/common/Makefile.common

ACLOCAL_AMFLAGS = -I cfg/m4

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

SUBDIRS = . $(LIBDIRS) man $(HTMLDIR) test-code

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
	so@gui@.m4 \
	bootstrap \
	build/so@gui@.spec.in \
	cfg/m4/soxt.m4 \
	cfg/m4/check_dl.m4 \
	cfg/m4/check_inventor.m4 \
	cfg/m4/check_mac.m4 \
	cfg/m4/check_mathlib.m4 \
	cfg/m4/check_motif.m4 \
	cfg/m4/check_opengl.m4 \
	cfg/m4/check_pthread.m4 \
	cfg/m4/check_x11.m4 \
	cfg/m4/coin.m4 \
	cfg/m4/configuration_summary.m4 \
	cfg/m4/compile_debug.m4 \
	cfg/m4/compile_exceptions.m4 \
	cfg/m4/compile_functionname.m4 \
	cfg/m4/compile_profiling.m4 \
	cfg/m4/compile_rtti.m4 \
	cfg/m4/compile_symbols.m4 \
	cfg/m4/compile_warnings.m4 \
	cfg/m4/cpp_search_order.m4 \
	cfg/m4/cvschanges.m4 \
	cfg/m4/dates.m4 \
	cfg/m4/doxygen_detection.m4 \
	cfg/m4/errors.m4 \
	cfg/m4/expand_dir_vars.m4 \
	cfg/m4/linux_api.m4 \
	cfg/m4/misc_compiler.m4 \
	cfg/m4/path_conversion.m4 \
	cfg/m4/sogui-misc-common.m4 \
	cfg/m4/sogui.m4 \
	cfg/m4/uniqify.m4 \
	docs/announcement-1_1_0.txt \
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
