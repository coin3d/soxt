/**************************************************************************
 *
 *  This file is part of the Coin SoXt GUI binding library.
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License version
 *  2.1 as published by the Free Software Foundation.  See the file
 *  LICENSE.LGPL at the root directory of the distribution for all the
 *  details.
 *
 *  If you want to use Coin SoXt for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

// $Id$

#ifndef SOXT_GLWIDGET_H
#define SOXT_GLWIDGET_H

#include <GL/glx.h>

#include <Inventor/Xt/SoXtComponent.h>

enum GLModes {           // should we keep these?
  SO_GL_RGB      = 0x01, SO_GLX_RGB      = SO_GL_RGB,
  SO_GL_DOUBLE   = 0x02, SO_GLX_DOUBLE   = SO_GL_DOUBLE,
  SO_GL_ZBUFFER  = 0x04, SO_GLX_ZBUFFER  = SO_GL_ZBUFFER,
  SO_GL_OVERLAY  = 0x08, SO_GLX_OVERLAY  = SO_GL_OVERLAY,
  SO_GL_STEREO   = 0x10, SO_GLX_STEREO   = SO_GL_STEREO
};

// *************************************************************************

class SOXT_DLL_API SoXtGLWidget : public SoXtComponent {
  SOXT_OBJECT_ABSTRACT_HEADER(SoXtGLWidget, SoXtComponent);

public:
  unsigned long getOverlayTransparentPixel(void);
  int getOverlayColorMapSize(void);
  int getColorMapSize(void);

  Window getNormalWindow(void);
  Window getOverlayWindow(void);
  GLXContext getNormalContext(void);
  GLXContext getOverlayContext(void);
  Widget getNormalWidget(void);
  Widget getGLWidget(void);
  Widget getOverlayWidget(void);
  virtual void setNormalVisual(XVisualInfo * visual);
  XVisualInfo * getNormalVisual(void);
  virtual void setOverlayVisual(XVisualInfo * visual);
  XVisualInfo * getOverlayVisual(void);
  virtual void setDoubleBuffer(SbBool enable);
  SbBool isDoubleBuffer(void);
  void setBorder(SbBool enable);
  SbBool isBorder(void) const;
  void setDrawToFrontBufferEnable(SbBool enable);
  SbBool isDrawToFrontBufferEnable(void) const;
  void setQuadBufferStereo(const SbBool enable);
  SbBool isQuadBufferStereo(void) const;

  SbBool hasOverlayGLArea(void) const;
  SbBool hasNormalGLArea(void) const;

protected:
  SoXtGLWidget(
    Widget parent = NULL,
    const char * name = NULL,
    SbBool embed = TRUE,
    int glModes = SO_GLX_RGB,
    SbBool build = TRUE);
  virtual ~SoXtGLWidget(void);
  virtual void redraw(void) = 0;
  virtual void redrawOverlay(void);
  virtual void processEvent(XAnyEvent * event);
  virtual void initGraphic(void);
  virtual void initOverlayGraphic(void);
  virtual void sizeChanged(const SbVec2s size);
  virtual void widgetChanged(Widget widget);

  void setGLSize(const SbVec2s size);
  const SbVec2s getGLSize(void) const;
  float getGLAspectRatio(void) const;

  // old aliases
  void setGlxSize(const SbVec2s size) { this->setGLSize(size); }
  const SbVec2s getGlxSize(void) const { return this->getGLSize(); }
  float getGlxAspectRatio(void) const { return this->getGLAspectRatio(); }
  void setStereoBuffer(SbBool flag) {
    this->setQuadBufferStereo(flag);
  }
  SbBool isStereoBuffer(void) const {
    return this->isQuadBufferStereo();
  }

  SbBool isRGBMode(void);
  int getDisplayListShareGroup(GLXContext context);

  Colormap normalColorMap;
  Colormap overlayColorMap;
  int normalColorMapSize;
  int overlayColorMapSize;

  SbBool waitForExpose;
  SbBool drawToFrontBuffer;
  Widget buildWidget(Widget parent);
  Widget getGlxMgrWidget(void);

  unsigned long transparentPixel;

  SbBool border;
  int borderwidth;

  Widget glxWidget;
  Widget glxManager;
  GLXContext normalContext;
  GLXContext overlayContext;
  XVisualInfo * normalVisual;
  XVisualInfo * overlayVisual;

  SbBool doubleBuffer;

  static void eventHandler(Widget, void *, XEvent *, Boolean *);

protected:

  void glLockNormal(void);
  void glUnlockNormal(void);
  void glLockOverlay(void);
  void glUnlockOverlay(void);

  void glSwapBuffers(void);
  void glFlushBuffer(void);

  virtual SbBool glScheduleRedraw(void);

private:
  void initNormalContext(void);
  SbVec2s glSize; // cached GL widget size

}; // class SoXtGLWidget

// *************************************************************************

#endif // ! SOXT_GLWIDGET_H
