/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifndef SOXTCOMPONENTP_H
#define SOXTCOMPONENTP_H

#include <Inventor/Xt/SoGuiComponentP.h>
#include <Inventor/Xt/SoXtCursor.h>

// ************************************************************************

// This class contains private data and methods used within the
// SoXtComponent class.

class SoXtComponentP : public SoGuiComponentP
{
public:
  SoXtComponentP(SoXtComponent * publ);
  ~SoXtComponentP();

  static Cursor getNativeCursor(Display * d, const SoXtCursor::CustomCursor * cc);

  static void wmDeleteWindowCB(Widget w, XtPointer user, XtPointer call);
  static void widgetDestroyedCB(Widget w, XtPointer user, XtPointer call);

  static void structureNotifyOnWidgetCB(Widget w, XtPointer user, XEvent * e, Boolean *);
  static void structureNotifyOnShellCB(Widget w, XtPointer user, XEvent * e, Boolean *);

  void checkVisibilityChange(void);

  Widget parent;
  Widget widget;
  char * widgetname;
  char * widgetclass;
  char * title;
  char * icontitle;

  SbBool embedded;

  SbBool widgetmappedstatus;
  SbBool shellmappedstatus;

  SbVec2s size;

  SbPList * closecbs;
  SbPList * visibilitycbs;
  SbBool visibilitystate;
    
  SbBool fullscreen;

private:
  static SbDict * cursordict;
};

// ************************************************************************

#endif // !SOXTCOMPONENTP_H
