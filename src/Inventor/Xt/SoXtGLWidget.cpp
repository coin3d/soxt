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

#if SOXT_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOXT_DEBUG

// *************************************************************************

/*!
  \class SoXtGLWidget Inventor/Xt/SoXtGLWidget.h
  \brief The SoXtGLWidget class manages GL contexts.
  \ingroup components
*/

// *************************************************************************

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoBasic.h>
#include <Inventor/SbViewportRegion.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtResource.h>
#include <Inventor/Xt/widgets/SoXtGLArea.h>

#include <Inventor/Xt/SoXtGLWidget.h>
#include <Inventor/Xt/SoAny.h>

#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/DrawingA.h>
#include <X11/IntrinsicP.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#ifdef HAVE_LIBXMU
#include <X11/Xmu/StdCmap.h>
#endif // HAVE_LIBXMU

#include <assert.h>
#include <stdlib.h>

// *************************************************************************

// The private data for the SoQtGLWidget.

class SoXtGLWidgetP {
public:
  SoXtGLWidgetP(const SoXtGLWidget * w) :
    glsize(SbVec2s(-1, -1)), normalcolormapsize(0), overlaycolormapsize(0),
    transparentpixel(0), border(FALSE), borderwidth(2),
    glxwidget(NULL), glxmanager(NULL),
    normalcontext(NULL), normalvisual(NULL),
    overlaycontext(NULL), overlayvisual(NULL),
    doublebuffer(TRUE)
  {
    this->owner = w;
  }

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

private:
  const SoXtGLWidget * owner;
};

#define PRIVATE(o) (o->pimpl)

// *************************************************************************

SOXT_OBJECT_ABSTRACT_SOURCE(SoXtGLWidget);

// *************************************************************************

/*!
  \fn void SoXtGLWidget::redraw(void) = 0

  This method is invokedwhen the GL buffer needs to be redrawn.
*/

/*!
  \var Visual * SoXtGLWidget::normalVisual

  Visual for normal graphics.
*/

/*!
  \var Visual * SoXtGLWidget::overlayVisual

  Visual for overlay graphics.
*/

/*!
  \var SbBool SoXtGLWidget::waitForExpose

  Flag telling whether to wait for expose or not..
*/

/*!
  \var SbBool SoXtGLWidget::drawToFrontBuffer

  Whether drawing should happen to the front buffer or not.
*/

/*!
  \var SbBool SoXtGLWidget::doubleBuffer

  Whether double buffering is used or not.
*/

/*!
  \fn void SoXtGLWidget::setStereoBuffer(SbBool flag)
  
  Sets whether OpenGL stereo buffers (quad buffer stereo) should be used.
*/

/*!
  \fn SbBool SoXtGLWidget::isStereoBuffer(void) const

  Returns whether OpenGL stereo buffers are being used.
*/


// *************************************************************************

/*!
  The constructor.
*/

SoXtGLWidget::SoXtGLWidget(// protected
  Widget parent,
  const char * name,
  SbBool embed,
  int glModes,
  SbBool build)
  : inherited(parent, name, embed),
    waitForExpose(TRUE), drawToFrontBuffer(FALSE)
{
  PRIVATE(this) = new SoXtGLWidgetP(this);

  if (build) {
    Widget glarea = this->buildWidget(this->getParentWidget());
    this->setBaseWidget(glarea);
    XtVaSetValues(glarea,
      XmNleftAttachment, XmATTACH_FORM,
      XmNtopAttachment, XmATTACH_FORM,
      XmNrightAttachment, XmATTACH_FORM,
      XmNbottomAttachment, XmATTACH_FORM,
      NULL);
  }
} // SoXtGLWidget()

/*!
  The destructor.
*/

SoXtGLWidget::~SoXtGLWidget(// virtual, protected
  void)
{
  if (PRIVATE(this)->normalcontext) {
    SoAny::si()->unregisterGLContext((void *)this);
  }
  delete PRIVATE(this);
} // ~SoXtGLWidget()

// *************************************************************************

/*!
  This method returns the transparent pixel for the overlay planes.
*/

unsigned long
SoXtGLWidget::getOverlayTransparentPixel(
  void)
{
  return PRIVATE(this)->transparentpixel;
} // getOverlayTransparentPixel()

/*!
  This method returns the size of the colormap for the overlay planes.
*/

int
SoXtGLWidget::getOverlayColorMapSize(
  void)
{
  return PRIVATE(this)->overlaycolormapsize;
} // getOverlayColorMapSize()

/*!
  This methos returns the size of the colormap for the normal planes.
*/

int
SoXtGLWidget::getColorMapSize(
  void)
{
  return PRIVATE(this)->normalcolormapsize;
} // getColorMapSize()

/*!
  This method returns the Window associated with the normal GL context.
*/
Window
SoXtGLWidget::getNormalWindow(void)
{
  assert(PRIVATE(this)->glxwidget != (Widget) NULL);
  return XtWindow(PRIVATE(this)->glxwidget);
}

/*!
  This method returns the Window associated with the overlay GL context.
*/
Window
SoXtGLWidget::getOverlayWindow(void)
{
  return (Window) NULL;
}

/*!
  This method returns the normal GL context.
*/
GLXContext
SoXtGLWidget::getNormalContext(void)
{
  return PRIVATE(this)->normalcontext;
}

/*!
  This method returns the GL context for the overlay planes.
*/

GLXContext
SoXtGLWidget::getOverlayContext(
  void)
{
  return PRIVATE(this)->overlaycontext;
} // getOverlayContext()

/*!
  This method returns the Widget associated with the normal GL
  context.
*/
Widget
SoXtGLWidget::getNormalWidget(void) const
{
  return PRIVATE(this)->glxwidget;
}

/*!
  This method returns the Widget associated with the overlay GL
  context.
*/
Widget
SoXtGLWidget::getOverlayWidget(void) const
{
  return (Widget) NULL;
}

/*!
  This method sets the visual for the normal GL context.
*/

void
SoXtGLWidget::setNormalVisual(// virtual
  XVisualInfo * visual)
{
  PRIVATE(this)->normalvisual = visual;
} // setNormalVisual()

/*!
  This method returns the visual for the normal GL context.
*/

XVisualInfo *
SoXtGLWidget::getNormalVisual(
  void)
{
  return PRIVATE(this)->normalvisual;
} // setNormalVisual()

/*!
  This method sets the visual for the overlay GL context.
*/

void
SoXtGLWidget::setOverlayVisual(// virtual
  XVisualInfo * visual)
{
  PRIVATE(this)->overlayvisual = visual;
} // setOverlayVisual()

/*!
  This method returns the visual for the overlay GL context.
*/

XVisualInfo *
SoXtGLWidget::getOverlayVisual(
  void)
{
  return PRIVATE(this)->overlayvisual;
} // getOverlayVisual()

// FIXME: make doc in SoQtGLWidget valid for this also. 20011116 mortene.
void
SoXtGLWidget::setOverlayRender(const SbBool onoff)
{
  SOXT_STUB();
}

// FIXME: make doc in SoQtGLWidget valid for this also. 20011116 mortene.
SbBool
SoXtGLWidget::isOverlayRender(void) const
{
  SOXT_STUB();
  return FALSE;
}

/*!
  This method sets whether double buffering is to be used or not.
*/
void        // virtual
SoXtGLWidget::setDoubleBuffer(SbBool enable)
{
  PRIVATE(this)->doublebuffer = enable;
}

/*!
  This method returns whether double buffering is used or not.
*/
SbBool
SoXtGLWidget::isDoubleBuffer(void) const
{
  return PRIVATE(this)->doublebuffer;
}

/*!
  This method sets whether the GL area should be framed by a border or not.
*/

void
SoXtGLWidget::setBorder(SbBool enable)
{
  PRIVATE(this)->border = enable;
  if (PRIVATE(this)->glxwidget != (Widget) NULL) {
    if (PRIVATE(this)->border == FALSE)
      XtVaSetValues(PRIVATE(this)->glxwidget,
        XmNleftOffset, 0,
        XmNtopOffset, 0,
        XmNrightOffset, 0,
        XmNbottomOffset, 0,
        NULL);
    else
      XtVaSetValues(PRIVATE(this)->glxwidget,
        XmNleftOffset, PRIVATE(this)->borderwidth,
        XmNtopOffset, PRIVATE(this)->borderwidth,
        XmNrightOffset, PRIVATE(this)->borderwidth,
        XmNbottomOffset, PRIVATE(this)->borderwidth,
        NULL);
  }
} // setBorder()

/*!
  This method returns whether the GL area is framed by a border or not.
*/

SbBool
SoXtGLWidget::isBorder(void) const
{
  return PRIVATE(this)->border;
} // isBorder()

/*!
  This method sets whether rendering will be done to the front buffer
  or not.
*/

void
SoXtGLWidget::setDrawToFrontBufferEnable(SbBool enable)
{
  this->drawToFrontBuffer = enable;
} // setDrawToFrontBufferEnable()

/*!
  This method returns whether rendering will be done to the front buffer
  or not.
*/

SbBool
SoXtGLWidget::isDrawToFrontBufferEnable(void) const
{
  return this->drawToFrontBuffer;
} // isDrawToFrontBufferEnable()

/*!
  Enables or disables quad buffer stereo.
*/

void 
SoXtGLWidget::setQuadBufferStereo(const SbBool enable)
{
  // FIXME: do proper implementation. 20001123 mortene.
}

/*!
  Returns \c TRUE if quad buffer stereo is enabled for this widget.
*/
SbBool 
SoXtGLWidget::isQuadBufferStereo(void) const
{
  // FIXME: do proper implementation. 20001123 mortene.
  return FALSE;
}

/*!
  This method is invoked when the overlay graphics should be redrawn.
*/

void
SoXtGLWidget::redrawOverlay(// virtual, protected
  void)
{
  SOXT_STUB();
} // redrawOverlay()

/*!
  Any events from the native window system that goes to the OpenGL
  canvas gets piped through this method.

  It is overloaded in the subclasses to catch user interaction with
  the render canvas in the viewers, aswell as forwarding relevant
  events to the scenegraph.
*/
void
SoXtGLWidget::processEvent(// virtual, protected
  XAnyEvent * event)
{
  switch (event->type) {
  case MapNotify:
    PRIVATE(this)->initNormalContext();
    this->initGraphic();
    break;

  case Expose:
    this->waitForExpose = FALSE; // Gets flipped from TRUE on first expose.
    if (!this->glScheduleRedraw()) {
      this->redraw();
    }
    break;

  case ConfigureNotify:
    if (PRIVATE(this)->glxwidget != (Widget) NULL) {
      Dimension width, height;
      XtVaGetValues(PRIVATE(this)->glxwidget,
          XmNwidth, &width, XmNheight, &height, NULL);
      PRIVATE(this)->glsize = SbVec2s(width, height);
    }
    break;

  } // switch (event->type)

} // processEvent()

/*!
  This method initializes the graphics.
*/

void
SoXtGLWidget::initGraphic(// virtual, protected
  void)
{
  assert(PRIVATE(this)->glxwidget != (Widget) NULL);

  glLockNormal();
  Dimension width, height;
  XtVaGetValues(PRIVATE(this)->glxwidget, XmNwidth, &width, XmNheight, &height, NULL);
  PRIVATE(this)->glsize = SbVec2s(width, height);
  glEnable(GL_DEPTH_TEST);
  glUnlockNormal();
} // initGraphic()


// private method that initializes the normal GL context  
void 
SoXtGLWidgetP::initNormalContext(void)
{
  assert(this->glxwidget != (Widget) NULL);
  XVisualInfo * visual;
  Display * display = SoXt::getDisplay();
  XtVaGetValues(this->glxwidget, SoXtNvisualInfo, &visual, NULL);
  int screen = DefaultScreen(display);

  SoXtGLWidget * share = (SoXtGLWidget *)
    SoAny::si()->getSharedGLContext((void *)display, (void *)screen);
  
  this->normalcontext =
    glXCreateContext(display, visual, 
                     share ? share->getNormalContext() : None, 
                     GL_TRUE);
  if (! this->normalcontext) {
    SoDebugError::postInfo("SoXtGLWidget::glInit",
                           "glXCreateContext() returned NULL");
    XtAppError(SoXt::getAppContext(), "no context");
  }
  else {
    SoAny::si()->registerGLContext((void *)this->owner, (void *)display, (void *)screen);
  }
}


/*!
  This method initializes the overlay graphics.
*/

void
SoXtGLWidget::initOverlayGraphic(// virtual, protected
  void)
{
  // should be empty
} // initOverlayGraphic()

/*!
  Will be called whenever scene graph needs to be redrawn().
  If this method return FALSE, redraw() will be called immediately.

  Default method simply returns FALSE. Overload this method to
  schedule a redraw and return TRUE if you're trying to do The Right
  Thing.  
*/
SbBool 
SoXtGLWidget::glScheduleRedraw(void)
{
  return FALSE;
}

/*!
  Should return TRUE if an overlay GL drawing area exists.
*/
SbBool 
SoXtGLWidget::hasOverlayGLArea(void) const 
{
  return ((SoXtGLWidget*)this)->getOverlayWidget() != 0;
}

/*!
  Should return TRUE if a normal GL drawing area exists.
*/
SbBool 
SoXtGLWidget::hasNormalGLArea(void) const 
{
  return ((SoXtGLWidget*)this)->getNormalWidget() != 0;
}
 
// doc in super
void    // virtual, protected
SoXtGLWidget::sizeChanged(const SbVec2s & size)
{
//  SoDebugError::postInfo("SoXtGLWidget::sizeChanged", "[invoked (%d, %d)]",
//    size[0], size[1]);
  if (this->isBorder()) {
    PRIVATE(this)->glsize[0] = size[0] - 2 * PRIVATE(this)->borderwidth;
    PRIVATE(this)->glsize[1] = size[1] - 2 * PRIVATE(this)->borderwidth;
  } else {
    PRIVATE(this)->glsize = size;
  }
  XtResizeWidget(PRIVATE(this)->glxwidget, PRIVATE(this)->glsize[0], PRIVATE(this)->glsize[1], 0);
}

/*!
  This method is invoked when a widget in the component is changed.
*/
void        // virtual, protected
SoXtGLWidget::widgetChanged(Widget widget)
{
  SOXT_STUB();
} // widgetChanged()

// *************************************************************************

/*!
  \fn void SoXtGLWidget::setGlxSize(const SbVec2s size)
  This method sets the Glx size.
  \sa setGLSize
*/

/*!
  This methid sets the size of the GL widget.
*/

void
SoXtGLWidget::setGLSize(// protected
  const SbVec2s size)
{
  PRIVATE(this)->glsize = size;
  if (PRIVATE(this)->glxwidget != (Widget) NULL) {
    Dimension width = size[0];
    Dimension height = size[1];
    XtVaSetValues(PRIVATE(this)->glxwidget,
      XmNwidth, width,
      XmNheight, height,
      NULL);
  }
} // setGLSize()

/*!
  \fn const SbVec2s SoXtGLWidget::getGlxSize(void) const

  This method returns the size of the Glx area.

  \sa getGLSize
*/

/*!
  This method returns the size of the GL area.
*/
SbVec2s // protected
SoXtGLWidget::getGLSize(void) const
{
  return PRIVATE(this)->glsize;
}

/*!
  \fn float SoXtGLWidget::getGlxAspectRatio(void) const

  This is the old name for getGLAspectRatio.
  
  \sa getGLAspectRatio
*/

/*!
  This method returns the aspect ratio of the GL area.
*/

float
SoXtGLWidget::getGLAspectRatio(
  void) const
{
  assert(PRIVATE(this)->glxwidget != (Widget) NULL);
  Dimension width, height;
  XtVaGetValues(PRIVATE(this)->glxwidget, XmNwidth, &width, XmNheight, &height, NULL);
  return float(width) / float(height);
} // getGLAspectRatio()

// *************************************************************************

/*!
  This method returns whether the GL context is in RGB mode or not.
*/

SbBool
SoXtGLWidget::isRGBMode(// protected
  void)
{
  return TRUE;
} // isRGBMode()

/*!
  This method returns the share group for the display lists for the
  GL context.
*/

int
SoXtGLWidget::getDisplayListShareGroup(// protected
  GLXContext context)
{
  SOXT_STUB();
  return 0;
} // getDisplayListShareGroup()

// *************************************************************************

/*!
  This method is a callback that dispatches events to processEvent().

  \sa processEvent
*/

void
SoXtGLWidget::eventHandler(// static, protected
  Widget widget,
  void * closure,
  XEvent * event,
  Boolean * dispatch)
{
  assert(closure != NULL);
  SoXtGLWidget * component = (SoXtGLWidget *) closure;
  component->processEvent((XAnyEvent *) event);
  *dispatch = False;
} // event_handler()

static void
buildGLAttrs(int * attrs, int trynum)
{
  int pos = 0;
  attrs[pos++] = GLX_RGBA;
  attrs[pos++] = GLX_DEPTH_SIZE;
  attrs[pos++] = 1;
  if (! (trynum & 0x04)) {
    attrs[pos++] = GLX_STENCIL_SIZE;
    attrs[pos++] = 1;
  }
  if (! (trynum & 0x02)) {
    attrs[pos++] = GLX_DOUBLEBUFFER;
  }
  if (! (trynum & 0x01)) {
    attrs[pos++] = GLX_RED_SIZE;
    attrs[pos++] = 4;
    attrs[pos++] = GLX_GREEN_SIZE;
    attrs[pos++] = 4;
    attrs[pos++] = GLX_BLUE_SIZE;
    attrs[pos++] = 4;
  }
  attrs[pos] = None;
}

/*!
  This method builds the GL widget inside \a parent.  The returned
  widget is the widget managing the GL widget and providing it with a
  border.
*/

Widget      // protected
SoXtGLWidget::buildWidget(Widget parent)
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtGLWidget::buildWidget", "[enter]");
#endif // SOXT_DEBUG

  PRIVATE(this)->glxmanager =
    XtVaCreateManagedWidget("SoXtRenderArea",
                            xmFormWidgetClass, parent,
                            XmNleftAttachment, XmATTACH_FORM,
                            XmNtopAttachment, XmATTACH_FORM,
                            XmNrightAttachment, XmATTACH_FORM,
                            XmNbottomAttachment, XmATTACH_FORM,
                            NULL);
  this->registerWidget(PRIVATE(this)->glxmanager);

  SoXtResource rsc(PRIVATE(this)->glxmanager);

  short width = 0;
  if (rsc.getResource("borderThickness", XmRShort, width))
    PRIVATE(this)->borderwidth = width;

  SbBool haveborder = FALSE;
  if (rsc.getResource("border", XmRBoolean, haveborder))
    PRIVATE(this)->border = haveborder;

  Display * const display = SoXt::getDisplay();
  int trynum = 0;
  int attrs[32];
  int screen = DefaultScreen(display);
  while (PRIVATE(this)->normalvisual == NULL && trynum < 8) {
    buildGLAttrs(attrs, trynum);
    PRIVATE(this)->normalvisual = glXChooseVisual(display, screen, attrs);
    trynum++;
  }

  if (PRIVATE(this)->normalvisual == NULL) {
    SoDebugError::post("SoXtGLWidget::buildWidget",
      "could not get satisfactory visual for GLX");
    XtAppError(SoXt::getAppContext(), "SoXtGLWidget::buildWidget()");
  }

  PRIVATE(this)->doublebuffer = ((trynum-1) & 0x02) ? FALSE : TRUE;

  if ((PRIVATE(this)->normalvisual->c_class != TrueColor) &&
       (PRIVATE(this)->normalvisual->c_class != PseudoColor)) {
    SoDebugError::post("SoXtGLWidget::buildWidget",
        "Visual hasn't the necessary color capabilities");
    XtAppError(SoXt::getAppContext(), "SoXtGLWidget::buildWidget()");
  }

#ifndef HAVE_LIBXMU
  SoDebugError::post("SoXtGLWidget::buildWidget",
    "SoXt does not support detecting best visual/colormap without the Xmu library (yet)");
  exit(1);
#else // HAVE_LIBXMU

  Colormap colors = (Colormap) NULL;
  XStandardColormap * cmaps = NULL;
  int nmaps = 0;

  if (XmuLookupStandardColormap(
         display, PRIVATE(this)->normalvisual->screen, PRIVATE(this)->normalvisual->visualid,
         PRIVATE(this)->normalvisual->depth, XA_RGB_DEFAULT_MAP, False, True) &&
       XGetRGBColormaps(display,
         RootWindow(display, PRIVATE(this)->normalvisual->screen), &cmaps, &nmaps,
         XA_RGB_DEFAULT_MAP))
  {
    SbBool found = FALSE;
    for (int i = 0; i < nmaps && ! found; i++) {
      if (cmaps[i].visualid == PRIVATE(this)->normalvisual->visualid) {
#if SOXT_DEBUG && 0
        SoDebugError::postInfo("SoXtGLWidget::buildWidget",
          "got shared color map");
#endif // SOXT_DEBUG
        colors = cmaps[i].colormap;
        XFree(cmaps);
        found = TRUE;
      }
    }
    if (! found)
      colors = XCreateColormap(display,
                   RootWindow(display, PRIVATE(this)->normalvisual->screen),
                   PRIVATE(this)->normalvisual->visual, AllocNone);
  } else {
    colors = XCreateColormap(display,
                 RootWindow(display, PRIVATE(this)->normalvisual->screen),
                 PRIVATE(this)->normalvisual->visual, AllocNone);
  }
  
  PRIVATE(this)->glxwidget =
    XtVaCreateManagedWidget("SoXtGLWidget",
                            soxtGLAreaWidgetClass, PRIVATE(this)->glxmanager,
                            SoXtNvisualInfo, PRIVATE(this)->normalvisual,
                            XmNcolormap, colors,
                            SoXtNstencilSize, 1,
                            XmNleftAttachment, XmATTACH_FORM,
                            XmNtopAttachment, XmATTACH_FORM,
                            XmNrightAttachment, XmATTACH_FORM,
                            XmNbottomAttachment, XmATTACH_FORM,
                            NULL);
  this->registerWidget(PRIVATE(this)->glxwidget);  
  
  this->setBorder(this->isBorder()); // "refresh" the widget offsets

  // Our callback has this signature:
  // (void (*)(_WidgetRec *, SoXtGLWidget *, XAnyEvent *, char *))
  // ..so we need to cast to avoid a compiler warning or error.
  XtAddEventHandler(PRIVATE(this)->glxwidget,
      ExposureMask | StructureNotifyMask | ButtonPressMask |
      ButtonReleaseMask | PointerMotionMask | KeyPressMask | KeyReleaseMask,
      False,
      (void (*)(_WidgetRec *, void *, _XEvent *, char *))
        SoXtGLWidget::eventHandler, this);

#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtGLWidget::buildWidget", "[exit]");
#endif // SOXT_DEBUG
#endif // HAVE_LIBXMU
  return PRIVATE(this)->glxmanager;
} // buildWidget()

/*!
  This method returns the widget that is managing the GL widget.
*/

Widget
SoXtGLWidget::getGlxMgrWidget(// protected
  void)
{
  return PRIVATE(this)->glxmanager;
} // getGlxMgrWidget()

/*!
  This method returns the actual GL widget.
*/
Widget      // protected
SoXtGLWidget::getGLWidget(void) const
{
  return PRIVATE(this)->glxwidget;
}

// *************************************************************************

/*!
  This method locks the GL context.

  On systems that use GL context locking and unlocking, this method will
  lock the GL context.  On other systems, only makeCurrent will be run.

  This method is an SoXt extension.
*/

void
SoXtGLWidget::glLockNormal(void)
{
  assert(PRIVATE(this)->glxwidget != (Widget) NULL);
  glXMakeCurrent(SoXt::getDisplay(), XtWindow(PRIVATE(this)->glxwidget),
                 PRIVATE(this)->normalcontext);
} // glLockNormal()

/*!
  This method unlocks the GL context.
*/

void
SoXtGLWidget::glUnlockNormal(void)
{
} // glUnlockNormal()

/*!
  This method locks the GL context.

  On systems that use GL context locking and unlocking, this method will
  lock the GL context.  On other systems, only makeCurrent will be run.

  This method is an SoXt extension.
*/

void
SoXtGLWidget::glLockOverlay(void)
{
} // glLockOverlay()

/*!
  This method unlocks the GL context.
*/

void
SoXtGLWidget::glUnlockOverlay(void)
{
} // glUnlockOverlay()

/*!
  This method swaps the GL buffers.
*/

void
SoXtGLWidget::glSwapBuffers(
  void)
{
  assert(PRIVATE(this)->doublebuffer != FALSE);
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtGLWidget::glSwapBuffers", "called");
#endif // SOXT_DEBUG
  glXSwapBuffers(SoXt::getDisplay(), XtWindow(PRIVATE(this)->glxwidget));
} // glSwapBuffers()

/*!
  This methid flushes the GL context.
*/

void
SoXtGLWidget::glFlushBuffer(
  void)
{
  assert(PRIVATE(this)->glxwidget != (Widget) NULL);
  // nothing to do...
  glFlush();
} // glFlushBuffer()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtGLWidgetRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

