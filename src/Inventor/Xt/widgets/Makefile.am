############################################################################
## SoXt/src/Inventor/Xt/widgets/Makefile.am
## $Id$

# SUBDIRS = compound

noinst_LTLIBRARIES = libSoXtWidgets.la

INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src
LIBS =

BUILT_SOURCES = \
	SoAnyPopupMenu.h \
	SoAnyPopupMenu.cpp \
	SoAnyThumbWheel.h \
	SoAnyThumbWheel.cpp \
	SoAnyStateStack.h \
	SoAnyStateStack.cpp

############################################################################
## 

libSoXtWidgets_la_SOURCES = \
	$(BUILT_SOURCES) \
	SoXtGLArea.h \
	SoXtGLAreaP.h \
	SoXtGLArea.cpp \
	SoXtPopupMenu.h \
	SoXtPopupMenu.cpp \
	SoXtThumbWheel.h \
	SoXtThumbWheelP.h \
	SoXtThumbWheel.cpp \
	SoXtSlider.h \
	SoXtSlider.cpp

############################################################################

SoAnyPopupMenu.h: $(top_srcdir)/src/Inventor/Xt/common/SoAnyPopupMenu.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyPopupMenu.cpp: $(top_srcdir)/src/Inventor/Xt/common/SoAnyPopupMenu.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyThumbWheel.h: $(top_srcdir)/src/Inventor/Xt/common/SoAnyThumbWheel.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyThumbWheel.cpp: $(top_srcdir)/src/Inventor/Xt/common/SoAnyThumbWheel.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyStateStack.h: $(top_srcdir)/src/Inventor/Xt/common/SoAnyStateStack.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

SoAnyStateStack.cpp: $(top_srcdir)/src/Inventor/Xt/common/SoAnyStateStack.cpp.in
	@( cd $(top_builddir); $(MAKE) built-sources )

############################################################################

libSoXtWidgetsincdir = $(includedir)/Inventor/Xt/widgets

libSoXtWidgetsinc_HEADERS = \
	SoXtThumbWheel.h \
	SoXtThumbWheelP.h \
	SoXtGLArea.h \
	SoXtGLAreaP.h

############################################################################
