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

#ifndef SOXTGLWIDGETP_H
#define SOXTGLWIDGETP_H

#include <Inventor/Xt/SoGuiGLWidgetP.h>

// ************************************************************************

// This class contains private data and methods used within the
// SoXtGLWidget class.

class SoXtGLWidgetP : public SoGuiGLWidgetP
{
public:
  SoXtGLWidgetP(SoXtGLWidget * publ);
  ~SoXtGLWidgetP();

  void initNormalContext(void);
  static void eventHandler(Widget, void *, XEvent *, Boolean *);

  SbVec2s glsize; // cached GL widget size

  // FIXME: none of these are really supported, and the initialization
  // values are probably wrong. 20011012 mortene.
  int normalcolormapsize;
  int overlaycolormapsize;
  unsigned long transparentpixel;

  SbBool border;
  int borderwidth;

  Widget glxwidget;
  Widget glxmanager;
  GLXContext normalcontext;
  XVisualInfo * normalvisual;
  GLXContext overlaycontext;
  XVisualInfo * overlayvisual;
  SbBool doublebuffer;
  SbBool enablealphachannel;

protected:
  virtual SbBool isDirectRendering(void);
};

// ************************************************************************

#endif // !SOXTGLWIDGETP_H
