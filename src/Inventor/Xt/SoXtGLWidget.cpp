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

#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/DrawingA.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xmu/StdCmap.h>

#define __GLX_MOTIF
#include <GL/GLwDrawA.h>

#include <Inventor/errors/SoDebugError.h>

#include <Inventor/Xt/SoXt.h>

#include <Inventor/Xt/SoXtGLWidget.h>

// *************************************************************************

SoXtGLWidget::SoXtGLWidget( // protected
  Widget parent,
  const char * name,
  SbBool inParent,
  int glModes,
  SbBool buildNow )
: inherited( parent, name, inParent )
{
  this->borderwidth = 2;
  this->doubleBuffer = TRUE;
  if ( buildNow ) {
    Widget widget = this->buildWidget( parent );
    this->setBaseWidget( widget );
  }
}

SoXtGLWidget::~SoXtGLWidget( // virtual, protected
  void )
{
}

// *************************************************************************

unsigned long
SoXtGLWidget::getOverlayTransparentPixel(
  void )
{
}

int
SoXtGLWidget::getOverlayColorMapSize(
  void )
{
}

int
SoXtGLWidget::getColorMapSize(
  void )
{
}

Window
SoXtGLWidget::getNormalWindow(
  void )
{
}

Window
SoXtGLWidget::getOverlayWindow(
  void )
{
}

GLXContext
SoXtGLWidget::getNormalContext(
  void )
{
}

GLXContext
SoXtGLWidget::getOverlayContext(
  void )
{
}

Widget
SoXtGLWidget::getNormalWidget(
  void )
{
}

Widget
SoXtGLWidget::getOverlayWidget(
  void )
{
}

void
SoXtGLWidget::setNormalVisual( // virtual
  XVisualInfo * visual )
{
}

XVisualInfo *
SoXtGLWidget::getNormalVisual(
  void )
{
}

void
SoXtGLWidget::setOverlayVisual( // virtual
  XVisualInfo * visual )
{
}

XVisualInfo *
SoXtGLWidget::getOverlayVisual(
  void )
{
}

void
SoXtGLWidget::setDoubleBuffer( // virtual
  SbBool enable )
{
}

SbBool
SoXtGLWidget::isDoubleBuffer(
  void )
{
}

void
SoXtGLWidget::setBorder(
  SbBool enable )
{
}

SbBool
SoXtGLWidget::isBorder(
  void ) const
{
}

void
SoXtGLWidget::setDrawToFrontBufferEnable(
  SbBool enable )
{
}

SbBool
SoXtGLWidget::isDrawToFrontBufferEnable(
  void ) const
{
}

void
SoXtGLWidget::redrawOverlay( // virtual, protected
  void )
{
}

void
SoXtGLWidget::processEvent( // virtual, protected
  XAnyEvent * event )
{
}

void
SoXtGLWidget::initGraphic( // virtual, protected
  void )
{
}

void
SoXtGLWidget::initOverlayGraphic( // virtual, protected
  void )
{
}

void
SoXtGLWidget::sizeChanged( // virtual, protected
  const SbVec2s size )
{
}

void
SoXtGLWidget::widgetChanged( // virtual, protected
  Widget widget )
{
}

void
SoXtGLWidget::setGlxSize( // protected
  const SbVec2s size )
{
}

const SbVec2s
SoXtGLWidget::getGlxSize( // protected
  void ) const
{
}

void
SoXtGLWidget::eventHandler( // static, protected
  Widget widget,
  SoXtGLWidget * component,
  XAnyEvent * event,
  Boolean * flag )
{
}

void
SoXtGLWidget::setStereoBuffer( // protected
  SbBool enable )
{
}

SbBool
SoXtGLWidget::isStereoBuffer( // protected
  void )
{
}

SbBool
SoXtGLWidget::isRGBMode( // protected
  void )
{
}

int
SoXtGLWidget::getDisplayListShareGroup( // protected
  GLXContext context )
{
}

Widget
SoXtGLWidget::buildWidget( // protected
  Widget parent )
{
  Display * display = SoXt::getDisplay();
  XVisualInfo * visual;
  Colormap colors;

  static int single_attrs[] = {
    GLX_RGBA, GLX_DEPTH_SIZE, 16, None
  };

  static int double_attrs[] = {
    GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None
  };

  visual = glXChooseVisual( display, DefaultScreen(display), double_attrs );
  if ( visual == NULL ) {
    this->doubleBuffer = FALSE;
    visual = glXChooseVisual( display, DefaultScreen(display), single_attrs );
    if ( visual == NULL ) {
      SoDebugError::post( "SoXtGLWidget::buildWidget",
        "could not get visual with RGB and ZBUFFER capabilities" );
      XtAppError( SoXt::getAppContext(), "SoXtGLWidget::buildWidget()" );
    }
  }

  this->glxManager = XtVaCreateManagedWidget(
    "GLBorderWidget", xmFormWidgetClass, parent, NULL );

// requires Mesa's (or IRIX's) Xm/
  colors = getShareableColormap( display, visual );
  this->glxWidget = XtVaCreateManagedWidget(
    "GLWidget", glwMDrawingAreaWidgetClass, this->glxManager,
    GLwNvisualInfo, visual,
    XtNcolormap, colors,
    XmNleftAttachment,   XmATTACH_FORM,
    XmNleftOffset,       this->borderwidth,
    XmNtopAttachment,    XmATTACH_FORM,
    XmNtopOffset,        this->borderwidth,
    XmNrightAttachment,  XmATTACH_FORM,
    XmNrightOffset,      this->borderwidth,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNbottomOffset,     this->borderwidth,
    NULL );

//  XtAddCallback( this->glxWidget, GLwNginitCallback,  initCB, this );
//  XtAddCallback( this->glxWidget, GLwNexposeCallback, exposeCB, this );

  return this->glxManager;
} // buildWidget()

Widget
SoXtGLWidget::getGlxMgrWidget( // protected
  void )
{
}

// *************************************************************************

Colormap
SoXtGLWidget::getShareableColormap (Display *dpy, XVisualInfo *vi)
{
  Status status;
  XStandardColormap *standardCmaps;
  Colormap cmap;
  int i, numCmaps;
  if (vi->c_class != TrueColor && vi->c_class != PseudoColor) {
    fprintf(stderr, "No support for non-TrueColor visual [0x%02x].\n",
            (unsigned) vi->visualid);
    exit(1);
  }
 
  // If no standard colormap but TrueColor, just make an unshared one
  status = XmuLookupStandardColormap(dpy, vi->screen,
                                     vi->visualid, vi->depth,
                                     XA_RGB_DEFAULT_MAP,
                                     False, True);
  if (status == 1) {
    status = XGetRGBColormaps(dpy, RootWindow(dpy, vi->screen),
                              &standardCmaps, &numCmaps, XA_RGB_DEFAULT_MAP);
    if (status == 1)
      for (i = 0; i < numCmaps; i++) {
        if (standardCmaps[i].visualid == vi->visualid) {
          cmap = standardCmaps[i].colormap;
          XFree(standardCmaps);
          return cmap;
        }
      }
  }
  cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen),
                         vi->visual, AllocNone);
  return cmap;
}

// *************************************************************************
