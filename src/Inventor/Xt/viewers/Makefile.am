############################################################################
## SoXt/src/Inventor/Xt/viewers/Makefile.am
## $Id$

noinst_LTLIBRARIES = libSoXtViewers.la

INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src
LIBS =

BUILT_SOURCES = \
	SoXtViewer.h \
	SoXtViewer.cpp \
	SoAnyFullViewer.h \
	SoAnyFullViewer.cpp \
	SoAnyExaminerViewer.h \
	SoAnyExaminerViewer.cpp \
	SoAnyPlaneViewer.h \
	SoAnyPlaneViewer.cpp \
	SoAnyConstrainedViewer.h \
	SoAnyConstrainedViewer.cpp \
	SoAnyFlyViewer.h \
	SoAnyFlyViewer.cpp \
	SoAnyWalkViewer.h \
	SoAnyWalkViewer.cpp

############################################################################
##

libSoXtViewers_la_SOURCES = \
	$(BUILT_SOURCES) \
	SoXtViewerExtra.cpp \
	SoXtFullViewer.cpp \
	SoXtFullViewer.h \
	SoXtExaminerViewer.cpp \
	SoXtExaminerViewer.h \
	SoXtPlaneViewer.cpp \
	SoXtPlaneViewer.h \
	SoXtConstrainedViewer.cpp \
	SoXtConstrainedViewer.h \
	SoXtFlyViewer.cpp \
	SoXtFlyViewer.h \
	SoXtWalkViewer.cpp \
	SoXtWalkViewer.h \
	SoXtCollisionViewer.cpp \
	SoXtCollisionViewer.h

############################################################################
#

SoXtViewer.h: $(top_srcdir)/src/Inventor/Xt/common/SoGuiViewer.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoXtViewer.cpp: $(top_srcdir)/src/Inventor/Xt/common/SoGuiViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyFullViewer.h: $(top_srcdir)/src/Inventor/Xt/common/SoAnyFullViewer.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyFullViewer.cpp: $(top_srcdir)/src/Inventor/Xt/common/SoAnyFullViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyExaminerViewer.h: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyExaminerViewer.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyExaminerViewer.cpp: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyExaminerViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyPlaneViewer.h: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyPlaneViewer.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyPlaneViewer.cpp: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyPlaneViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyConstrainedViewer.h: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyConstrainedViewer.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyConstrainedViewer.cpp: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyConstrainedViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyFlyViewer.h: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyFlyViewer.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyFlyViewer.cpp: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyFlyViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyWalkViewer.h: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyWalkViewer.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyWalkViewer.cpp: \
  $(top_srcdir)/src/Inventor/Xt/common/SoAnyWalkViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

############################################################################
##

libSoXtViewersincdir = $(includedir)/Inventor/Xt/viewers

libSoXtViewersinc_HEADERS = \
	$(top_builddir)/src/Inventor/Xt/viewers/SoXtViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtFullViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtExaminerViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtPlaneViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtConstrainedViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtFlyViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtWalkViewer.h \
	$(top_srcdir)/src/Inventor/Xt/viewers/SoXtCollisionViewer.h

############################################################################
