
DEFAULT_INCLUDES = -I$(top_builddir)/src -I$(top_srcdir)/src

if USE_LIBXPM
XpmApps = button
else
XpmApps = 
endif

noinst_PROGRAMS = materialeditor coloreditor examinerviewer $(XpmApps)

materialeditor_SOURCES = materialeditor.cpp
materialeditor_LDADD = ../src/Inventor/Xt/libSoXt.la

coloreditor_SOURCES = coloreditor.cpp
coloreditor_LDADD = ../src/Inventor/Xt/libSoXt.la

examinerviewer_SOURCES = examinerviewer.cpp
examinerviewer_LDADD = ../src/Inventor/Xt/libSoXt.la

button_SOURCES = button.cpp
button_LDADD =

