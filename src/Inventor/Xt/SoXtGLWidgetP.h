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
  SbBool firstexpose;
  SbBool quadbuffer;
  SbBool alphachannel;
  SbBool stencilbuffer;
  SbBool accumbuffer;
  SbBool needrebuild;
  Colormap colormap;

  void createVisual(void);
  void cleanupVisual(void);

  void buildGLWidget(void);
  void cleanupGLWidget(void);

  static void exposeCB(Widget w, XtPointer closure, XtPointer call_data);

  int buildGLAttrs(int * attrs, int trynum);

  void buildContext(void);
  void cleanupContext(void);

protected:
  virtual SbBool isDirectRendering(void);


};

// ************************************************************************

#endif // !SOXTGLWIDGETP_H
