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
	src/Inventor/Xt/viewers/SoXtViewer.cpp \
	examples/components/renderarea.cpp \
	examples/components/examinerviewer.cpp \
	examples/components/planeviewer.cpp \
	examples/components/walkviewer.cpp \
	examples/components/flyviewer.cpp \
	examples/nodes/selection.cpp

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

examples/components/renderarea.cpp: \
  $(top_srcdir)/examples/components/renderarea.cpp.in
	@CONFIG_HEADERS= CONFIG_FILES=$@ ./config.status

examples/components/examinerviewer.cpp: \
  $(top_srcdir)/examples/components/examinerviewer.cpp.in
	@CONFIG_HEADERS= CONFIG_FILES=$@ ./config.status

examples/components/planeviewer.cpp: \
  $(top_srcdir)/examples/components/planeviewer.cpp.in
	@CONFIG_HEADERS= CONFIG_FILES=$@ ./config.status

examples/components/walkviewer.cpp: \
  $(top_srcdir)/examples/components/walkviewer.cpp.in
	@CONFIG_HEADERS= CONFIG_FILES=$@ ./config.status

examples/components/flyviewer.cpp: \
  $(top_srcdir)/examples/components/flyviewer.cpp.in
	@CONFIG_HEADERS= CONFIG_FILES=$@ ./config.status

examples/nodes/selection.cpp: \
  $(top_srcdir)/examples/nodes/selection.cpp.in
	@CONFIG_HEADERS= CONFIG_FILES=$@ ./config.status

############################################################################
