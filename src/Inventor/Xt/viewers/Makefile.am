############################################################################
## SoXt/src/Inventor/Xt/viewers/Makefile.am
## $Id$

noinst_LTLIBRARIES = libSoXtViewers.la

INCLUDES = -I$(top_srcdir)/src
LIBS =

BUILT_SOURCES = \
	So@Gui@Viewer.cpp \
	So@Gui@Viewer.h \
	SoAnyFullViewer.cpp \
	SoAnyFullViewer.h \
	SoAnyExaminerViewer.cpp \
	SoAnyExaminerViewer.h \
	SoAnyPlaneViewer.cpp \
	SoAnyPlaneViewer.h \
	SoAnyConstrainedViewer.cpp \
	SoAnyConstrainedViewer.h \
	SoAnyFlyViewer.cpp \
	SoAnyFlyViewer.h \
	SoAnyWalkViewer.cpp \
	SoAnyWalkViewer.h

DISTCLEANFILES = \
	$(BUILT_SOURCES)

############################################################################

libSoXtViewers_la_SOURCES = \
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

nodist_libSoXtViewers_la_SOURCES = \
	$(BUILT_SOURCES)

############################################################################

So@Gui@Viewer.h: $(srcdir)/../common/SoGuiViewer.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

So@Gui@Viewer.cpp: $(srcdir)/../common/SoGuiViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyFullViewer.h: $(srcdir)/../common/SoAnyFullViewer.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyFullViewer.cpp: $(srcdir)/../common/SoAnyFullViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyExaminerViewer.h: $(srcdir)/../common/SoAnyExaminerViewer.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyExaminerViewer.cpp: $(srcdir)/../common/SoAnyExaminerViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyPlaneViewer.h: $(srcdir)/../common/SoAnyPlaneViewer.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyPlaneViewer.cpp: $(srcdir)/../common/SoAnyPlaneViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyConstrainedViewer.h: $(srcdir)/../common/SoAnyConstrainedViewer.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyConstrainedViewer.cpp: $(srcdir)/../common/SoAnyConstrainedViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyFlyViewer.h: $(srcdir)/../common/SoAnyFlyViewer.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyFlyViewer.cpp: $(srcdir)/../common/SoAnyFlyViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyWalkViewer.h: $(srcdir)/../common/SoAnyWalkViewer.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyWalkViewer.cpp: $(srcdir)/../common/SoAnyWalkViewer.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

############################################################################

SoXtViewersincdir = $(includedir)/Inventor/@Gui@/viewers

SoXtViewersinc_HEADERS = \
	So@Gui@Viewer.h \
	So@Gui@FullViewer.h \
	So@Gui@ExaminerViewer.h \
	So@Gui@PlaneViewer.h \
	So@Gui@ConstrainedViewer.h \
	So@Gui@FlyViewer.h \
	So@Gui@WalkViewer.h \
	So@Gui@CollisionViewer.h

############################################################################
