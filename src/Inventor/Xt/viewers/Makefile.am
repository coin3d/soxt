############################################################################
## SoXt/src/Inventor/Xt/viewers/Makefile.am
## $Id$

noinst_LTLIBRARIES = libSoXtViewers.la

INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src
LIBS =

BUILT_SOURCES = \
	So@Gui@Viewer.h \
	So@Gui@Viewer.cpp \
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
	So@Gui@ViewerExtra.cpp \
	So@Gui@FullViewer.cpp \
	So@Gui@FullViewer.h \
	So@Gui@ExaminerViewer.cpp \
	So@Gui@ExaminerViewer.h \
	So@Gui@PlaneViewer.cpp \
	So@Gui@PlaneViewer.h \
	So@Gui@ConstrainedViewer.cpp \
	So@Gui@ConstrainedViewer.h \
	So@Gui@FlyViewer.cpp \
	So@Gui@FlyViewer.h \
	So@Gui@WalkViewer.cpp \
	So@Gui@WalkViewer.h \
	So@Gui@CollisionViewer.cpp \
	So@Gui@CollisionViewer.h

############################################################################
#

So@Gui@Viewer.h: $(top_srcdir)/src/Inventor/@Gui@/common/SoGuiViewer.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

So@Gui@Viewer.cpp: $(top_srcdir)/src/Inventor/@Gui@/common/SoGuiViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyFullViewer.h: $(top_srcdir)/src/Inventor/@Gui@/common/SoAnyFullViewer.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyFullViewer.cpp: $(top_srcdir)/src/Inventor/@Gui@/common/SoAnyFullViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyExaminerViewer.h: \
  $(top_srcdir)/src/Inventor/@Gui@/common/SoAnyExaminerViewer.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyExaminerViewer.cpp: \
  $(top_srcdir)/src/Inventor/@Gui@/common/SoAnyExaminerViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyPlaneViewer.h: \
  $(top_srcdir)/src/Inventor/@Gui@/common/SoAnyPlaneViewer.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyPlaneViewer.cpp: \
  $(top_srcdir)/src/Inventor/@Gui@/common/SoAnyPlaneViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyConstrainedViewer.h: \
  $(top_srcdir)/src/Inventor/@Gui@/common/SoAnyConstrainedViewer.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyConstrainedViewer.cpp: \
  $(top_srcdir)/src/Inventor/@Gui@/common/SoAnyConstrainedViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyFlyViewer.h: \
  $(top_srcdir)/src/Inventor/@Gui@/common/SoAnyFlyViewer.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyFlyViewer.cpp: \
  $(top_srcdir)/src/Inventor/@Gui@/common/SoAnyFlyViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyWalkViewer.h: \
  $(top_srcdir)/src/Inventor/@Gui@/common/SoAnyWalkViewer.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyWalkViewer.cpp: \
  $(top_srcdir)/src/Inventor/@Gui@/common/SoAnyWalkViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

############################################################################
##

libSoXtViewersincdir = $(includedir)/Inventor/@Gui@/viewers

libSoXtViewersinc_HEADERS = \
	$(top_builddir)/src/Inventor/@Gui@/viewers/So@Gui@Viewer.h \
	$(top_srcdir)/src/Inventor/@Gui@/viewers/So@Gui@FullViewer.h \
	$(top_srcdir)/src/Inventor/@Gui@/viewers/So@Gui@ExaminerViewer.h \
	$(top_srcdir)/src/Inventor/@Gui@/viewers/So@Gui@PlaneViewer.h \
	$(top_srcdir)/src/Inventor/@Gui@/viewers/So@Gui@ConstrainedViewer.h \
	$(top_srcdir)/src/Inventor/@Gui@/viewers/So@Gui@FlyViewer.h \
	$(top_srcdir)/src/Inventor/@Gui@/viewers/So@Gui@WalkViewer.h \
	$(top_srcdir)/src/Inventor/@Gui@/viewers/So@Gui@CollisionViewer.h

############################################################################
