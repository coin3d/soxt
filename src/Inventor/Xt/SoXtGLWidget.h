/**************************************************************************
 *
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
 **************************************************************************/

//  $Id$

#ifndef SOXT_GLWIDGET_H
#define SOXT_GLWIDGET_H

#include <GL/glx.h>

#include <Inventor/Xt/SoXtComponent.h>

// *************************************************************************

enum GLModes {
  SO_GLX_RGB      = 0x01,
  SO_GLX_DOUBLE   = 0x02,
  SO_GLX_ZBUFFER  = 0x04,
  SO_GLX_OVERLAY  = 0x08,
  SO_GLX_STEREO   = 0x10
};

class SoXtGLWidget : public SoXtComponent {
  typedef SoXtComponent inherited;

public:
  unsigned long getOverlayTransparentPixel(void);
  int getOverlayColorMapSize(void);
  int getColorMapSize(void);

  Window getNormalWindow(void);
  Window getOverlayWindow(void);
  GLXContext getNormalContext(void);
  GLXContext getOverlayContext(void);
  Widget getNormalWidget(void);
  Widget getOverlayWidget(void);
  virtual void setNormalVisual( XVisualInfo * visual );
  XVisualInfo * getNormalVisual(void);
  virtual void setOverlayVisual( XVisualInfo * visual );
  XVisualInfo * getOverlayVisual(void);
  virtual void setDoubleBuffer( SbBool enable );
  SbBool isDoubleBuffer(void);
  void setBorder( SbBool enable );
  SbBool isBorder(void) const;
  void setDrawToFrontBufferEnable( SbBool enable );
  SbBool isDrawToFrontBufferEnable(void) const;

protected:
  SoXtGLWidget( Widget parent = NULL, const char * name = NULL,
      SbBool buildInsideParent = TRUE, int glModes = SO_GLX_RGB,
      SbBool buildNow = TRUE );
  virtual ~SoXtGLWidget(void);
  virtual void redraw(void) = 0;
  virtual void redrawOverlay(void);
  virtual void processEvent( XAnyEvent * event );
  virtual void initGraphic(void);
  virtual void initOverlayGraphic(void);
  virtual void sizeChanged( const SbVec2s size );
  virtual void widgetChanged( Widget widget );
  void setGlxSize( const SbVec2s size );
  const SbVec2s getGlxSize(void) const;
  static void eventHandler( Widget, SoXtGLWidget *, XAnyEvent *, Boolean *);
  void setStereoBuffer( SbBool enable );
  SbBool isStereoBuffer(void);
  SbBool isRGBMode(void);
  int getDisplayListShareGroup( GLXContext context );

  Colormap normalColorMap;
  Colormap overlayColorMap;
  int normalColorMapSize;
  int overlayColorMapSize;

  SbBool waitForExpose;
  SbBool drawToFrontBuffer;
  Widget buildWidget( Widget parent );
  Widget getGlxMgrWidget(void);

  unsigned long transparentPixel;

  int borderwidth;
  Widget glxWidget;
  Widget glxManager;

  SbBool doubleBuffer;

  Colormap getShareableColormap( Display *, XVisualInfo * );

}; // class SoXtGLWidget

// *************************************************************************

#endif // ! SOXT_GLWIDGET_H
