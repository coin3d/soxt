/**************************************************************************\
 *
 *  This file is part of the Coin family of 3D visualization libraries.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and / or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use this library in software that is incompatible
 *  with the GNU GPL, and / or you would like to take advantage of the
 *  additional benefits with regard to our support services, please
 *  contact Systems in Motion about acquiring a Coin Professional
 *  Edition License.  See <URL:http://www.coin3d.org> for more
 *  information.
 *
 *  Systems in Motion, Abels gate 5, Teknobyen, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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
