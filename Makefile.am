## Process this file with automake to generate Makefile.in.

if BUILD_TESTS
TESTDIR = test-code
else
TESTDIR =
endif

if BUILD_EXAMPLES
EXAMPLEDIR = examples
else
EXAMPLEDIR =
endif

SUBDIRS = src/Inventor/Xt $(TESTDIR) $(EXAMPLEDIR)

bin_SCRIPTS = soxt-config

BUILT_SOURCES = \
	soxt-config \
	src/Inventor/Xt/SoXtRenderArea.h \
	src/Inventor/Xt/SoXtRenderArea.cpp \
	src/Inventor/Xt/viewers/SoXtViewer.h \
	src/Inventor/Xt/viewers/SoXtViewer.cpp

soxt-config: soxt-config.in

EXTRA_DIST = conf-macros/soxt.m4 conf-macros/modifierlist.m4

m4datadir = $(datadir)/aclocal
m4data_DATA = conf-macros/soxt.m4 conf-macros/modifierlist.m4

############################################################################

src/Inventor/Xt/SoXtRenderArea.h: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyRenderArea.h.in
	@CONFIG_HEADERS= CONFIG_FILES=$@:src/Inventor/Xt/common/SoAnyRenderArea.h.in ./config.status

src/Inventor/Xt/SoXtRenderArea.cpp: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyRenderArea.cpp.in
	@CONFIG_HEADERS= CONFIG_FILES=$@:src/Inventor/Xt/common/SoAnyRenderArea.cpp.in ./config.status

src/Inventor/Xt/viewers/SoXtViewer.h: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyViewer.h.in
	@CONFIG_HEADERS= CONFIG_FILES=$@:src/Inventor/Xt/common/SoAnyViewer.h.in ./config.status

src/Inventor/Xt/viewers/SoXtViewer.cpp: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyViewer.cpp.in
	@CONFIG_HEADERS= CONFIG_FILES=$@:src/Inventor/Xt/common/SoAnyViewer.cpp.in $(top_builddir)/config.status

############################################################################
