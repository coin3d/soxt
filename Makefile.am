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

SUBDIRS = . src/Inventor/Xt $(TESTDIR) $(EXAMPLEDIR)
SUFFIXES = .h .cpp .in .lo .o

bin_SCRIPTS = soxt-config

BUILT_SOURCES = \
	soxt-config \
	src/Inventor/Xt/SoXtRenderArea.h \
	src/Inventor/Xt/SoXtRenderArea.cpp \
	src/Inventor/Xt/viewers/SoXtViewer.h \
	src/Inventor/Xt/viewers/SoXtViewer.cpp \
	src/Inventor/Xt/viewers/SoAnyFullViewer.h \
	src/Inventor/Xt/viewers/SoAnyFullViewer.cpp \
	src/Inventor/Xt/viewers/SoAnyExaminerViewer.h \
	src/Inventor/Xt/viewers/SoAnyExaminerViewer.cpp

soxt-config: soxt-config.in

EXTRA_DIST = conf-macros/soxt.m4 conf-macros/modifierlist.m4

m4datadir = $(datadir)/aclocal
m4data_DATA = conf-macros/soxt.m4 conf-macros/modifierlist.m4

############################################################################

src/Inventor/Xt/SoXtRenderArea.h: \
  $(top_srcdir)/src/Inventor/Xt/common/SoGuiRenderArea.h.in
	@CONFIG_HEADERS= CONFIG_FILES=$@:src/Inventor/Xt/common/SoGuiRenderArea.h.in ./config.status

src/Inventor/Xt/SoXtRenderArea.cpp: \
  $(top_srcdir)/src/Inventor/Xt/common/SoGuiRenderArea.cpp.in
	@CONFIG_HEADERS= CONFIG_FILES=$@:src/Inventor/Xt/common/SoGuiRenderArea.cpp.in ./config.status

src/Inventor/Xt/viewers/SoXtViewer.h: \
  $(top_srcdir)/src/Inventor/Xt/common/SoGuiViewer.h.in
	@CONFIG_HEADERS= CONFIG_FILES=$@:src/Inventor/Xt/common/SoGuiViewer.h.in ./config.status

src/Inventor/Xt/viewers/SoXtViewer.cpp: \
  $(top_srcdir)/src/Inventor/Xt/common/SoGuiViewer.cpp.in
	@CONFIG_HEADERS= CONFIG_FILES=$@:src/Inventor/Xt/common/SoGuiViewer.cpp.in $(top_builddir)/config.status

src/Inventor/Xt/viewers/SoAnyFullViewer.h: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyFullViewer.h.in
	@CONFIG_HEADERS= CONFIG_FILES=$@:src/Inventor/Xt/common/SoAnyFullViewer.h.in ./config.status

src/Inventor/Xt/viewers/SoAnyFullViewer.cpp: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyFullViewer.cpp.in
	@CONFIG_HEADERS= CONFIG_FILES=$@:src/Inventor/Xt/common/SoAnyFullViewer.cpp.in $(top_builddir)/config.status

src/Inventor/Xt/viewers/SoAnyExaminerViewer.h: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyExaminerViewer.h.in
	@CONFIG_HEADERS= CONFIG_FILES=$@:src/Inventor/Xt/common/SoAnyExaminerViewer.h.in ./config.status

src/Inventor/Xt/viewers/SoAnyExaminerViewer.cpp: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyExaminerViewer.cpp.in
	@CONFIG_HEADERS= CONFIG_FILES=$@:src/Inventor/Xt/common/SoAnyExaminerViewer.cpp.in $(top_builddir)/config.status

############################################################################
