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

#include <assert.h>

#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/DrawingA.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xmu/StdCmap.h>

#ifndef HAVE_X11_GLW_GLWMDRAWA_H
#include <GL/GLwMDrawA.h>
#else // HAVE_X11_GLW_GLWMDRAWA_H
#include <X11/GLw/GLwMDrawA.h>
#endif // HAVE_X11_GLW_GLWMDRAWA_H

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoBasic.h>
#include <Inventor/SbViewportRegion.h>

#include <Inventor/Xt/SoXt.h>

#include <Inventor/Xt/SoXtGLWidget.h>

static const int SOXT_BORDER_WIDTH = 2;

// *************************************************************************

SoXtGLWidget::SoXtGLWidget( // protected
  Widget parent,
  const char * name,
  SbBool inParent,
  int glModes,
  SbBool buildNow )
: inherited( parent, name, inParent )
{
  this->glLockLevel = 0;
  this->borderwidth = 0; // SOXT_BORDER_WIDTH;
  this->doubleBuffer = TRUE;
  if ( buildNow ) {
    Widget widget = this->buildWidget( parent );
    this->setBaseWidget( widget );
  }
} // SoXtGLWidget()

SoXtGLWidget::~SoXtGLWidget( // virtual, protected
  void )
{
} // ~SoXtGLWidget()

// *************************************************************************

unsigned long
SoXtGLWidget::getOverlayTransparentPixel(
  void )
{
  return this->transparentPixel;
} // getOverlayTransparentPixel()

int
SoXtGLWidget::getOverlayColorMapSize(
  void )
{
  return this->overlayColorMapSize;
} // getOverlayColorMapSize()

int
SoXtGLWidget::getColorMapSize(
  void )
{
  return this->normalColorMapSize;
} // getColorMapSize()

Window
SoXtGLWidget::getNormalWindow(
  void )
{
  assert( this->glxWidget != (Widget) NULL );
  return XtWindow( this->glxWidget );
} // getNormalWindow()

Window
SoXtGLWidget::getOverlayWindow(
  void )
{
  COIN_STUB();
  return (Window) NULL;
} // getOverlayWindow()

GLXContext
SoXtGLWidget::getNormalContext(
  void )
{
  return this->normalContext;
} // getNormalContext()

GLXContext
SoXtGLWidget::getOverlayContext(
  void )
{
  return this->overlayContext;
} // getOverlayContext()

Widget
SoXtGLWidget::getNormalWidget(
  void )
{
  return this->glxWidget;
} // getNormalWidget()

Widget
SoXtGLWidget::getOverlayWidget(
  void )
{
  COIN_STUB();
  return (Widget) NULL;
} // getOverlayWidget()

void
SoXtGLWidget::setNormalVisual( // virtual
  XVisualInfo * visual )
{
  COIN_STUB();
} // setNormalVisual()

XVisualInfo *
SoXtGLWidget::getNormalVisual(
  void )
{
  COIN_STUB();
  return (XVisualInfo *) NULL;
} // setNormalVisual()

void
SoXtGLWidget::setOverlayVisual( // virtual
  XVisualInfo * visual )
{
  COIN_STUB();
} // setOverlayVisual()

XVisualInfo *
SoXtGLWidget::getOverlayVisual(
  void )
{
  COIN_STUB();
  return (XVisualInfo *) NULL;
} // getOverlayVisual()

void
SoXtGLWidget::setDoubleBuffer( // virtual
  SbBool enable )
{
  this->doubleBuffer = enable;
} // setDoubleBuffer()

SbBool
SoXtGLWidget::isDoubleBuffer(
  void )
{
  return this->doubleBuffer;
} // isDoubleBuffer()

void
SoXtGLWidget::setBorder(
  SbBool enable )
{
  this->borderwidth = enable ? SOXT_BORDER_WIDTH : 0;
  if ( this->glxWidget != (Widget) NULL ) {
    XtVaSetValues( this->glxWidget,
        XmNleftOffset, this->borderwidth,
        XmNtopOffset, this->borderwidth,
        XmNrightOffset, this->borderwidth,
        XmNbottomOffset, this->borderwidth,
        NULL );
  }
} // setBorder()

SbBool
SoXtGLWidget::isBorder(
  void ) const
{
  return (this->borderwidth != 0) ? TRUE : FALSE;
} // isBorder()

void
SoXtGLWidget::setDrawToFrontBufferEnable(
  SbBool enable )
{
  COIN_STUB();
} // setDrawToFrontBufferEnable()

SbBool
SoXtGLWidget::isDrawToFrontBufferEnable(
  void ) const
{
  COIN_STUB();
  return FALSE;
} // isDrawToFrontBufferEnable()

void
SoXtGLWidget::redrawOverlay( // virtual, protected
  void )
{
  COIN_STUB();
} // redrawOverlay()

void
SoXtGLWidget::processEvent( // virtual, protected
  XAnyEvent * event )
{
  switch ( event->type ) {
  case MapNotify:
    this->glInit();
    break;

  case Expose:
    this->glRender();
    break;

  case ConfigureNotify:
    if ( this->glxWidget != (Widget) NULL ) {
      Dimension width, height;
      XtVaGetValues( this->glxWidget,
          XmNwidth, &width, XmNheight, &height, NULL );
      this->glReshape( width, height );
    }
    break;

  // ignore warnings when we're not handling the following harmless events:
  case KeyPress:
  case KeyRelease:
  case ButtonPress:
  case ButtonRelease:
  case EnterNotify:
  case LeaveNotify:
  case MotionNotify:
  case FocusIn:
  case FocusOut:
    break;

  default:
    static int warnings = 10; // number of warnings we give before we shut up
    if ( warnings ) {
      SoDebugError::postInfo( "SoXtGLWidget::processEvent",
        "Don't know how to handle type %d XAnyEvents", event->type );
      warnings--;
    }
    break;

  } // switch ( event->type )
} // processEvent()

void
SoXtGLWidget::initGraphic( // virtual, protected
  void )
{
  COIN_STUB();
} // initGraphic()

void
SoXtGLWidget::initOverlayGraphic( // virtual, protected
  void )
{
  COIN_STUB();
} // initOverlayGraphic()

void
SoXtGLWidget::sizeChanged( // virtual, protected
  const SbVec2s size )
{
  // nothing to do
} // sizeChanged()

void
SoXtGLWidget::widgetChanged( // virtual, protected
  Widget widget )
{
  COIN_STUB();
} // widgetChanged()

// *************************************************************************

/*!
*/

void
SoXtGLWidget::setGlxSize( // protected
  const SbVec2s size )
{
  this->setGLSize( size );
} // setGlxSize()

/*!
*/

void
SoXtGLWidget::setGLSize( // protected
  const SbVec2s size )
{
  assert( this->glxWidget != (Widget) NULL );
  Dimension width = size[0];
  Dimension height = size[1];
  XtVaSetValues( this->glxWidget, XmNwidth, width, XmNheight, height, NULL );
} // setGLSize()

/*!
*/

const SbVec2s
SoXtGLWidget::getGlxSize( // protected
  void ) const
{
  return this->getGLSize();
} // getGlxSize()

/*!
*/

const SbVec2s
SoXtGLWidget::getGLSize( // protected
  void ) const
{
  assert( this->glxWidget != (Widget) NULL );
  Dimension width, height;
  XtVaGetValues( this->glxWidget, XmNwidth, &width, XmNheight, &height, NULL );
  return SbVec2s( width, height );
} // getGLSize()

float
SoXtGLWidget::getGLAspectRatio(
  void ) const
{
  assert( this->glxWidget != (Widget) NULL );
  Dimension width, height;
  XtVaGetValues( this->glxWidget, XmNwidth, &width, XmNheight, &height, NULL );
  return float(width) / float(height);
} // getGLAspectRatio()

// *************************************************************************

void
SoXtGLWidget::eventHandler( // static, protected
  Widget widget,
  SoXtGLWidget * component,
  XAnyEvent * event,
  Boolean * flag )
{
  COIN_STUB();
} // eventHandler()

void
SoXtGLWidget::setStereoBuffer( // protected
  SbBool enable )
{
  COIN_STUB();
} // setStereoBuffer()

SbBool
SoXtGLWidget::isStereoBuffer( // protected
  void )
{
  COIN_STUB();
  return FALSE;
} // isStereoBuffer()

SbBool
SoXtGLWidget::isRGBMode( // protected
  void )
{
  COIN_STUB();
  return TRUE;
} // isRGBMode()

int
SoXtGLWidget::getDisplayListShareGroup( // protected
  GLXContext context )
{
  COIN_STUB();
  return 0;
} // getDisplayListShareGroup()

Widget
SoXtGLWidget::buildWidget( // protected
  Widget parent )
{
  static int single_attrs[] = {
    GLX_RGBA, GLX_DEPTH_SIZE, 16, None };
  static int double_attrs[] = {
    GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None };

  Display * display = SoXt::getDisplay();
  XVisualInfo * visual =
    glXChooseVisual( display, DefaultScreen(display), double_attrs );

  if ( visual == NULL ) {
#if SOXT_DEBUG
    SoDebugError::postInfo( "SoXtGLWidget::buildWidget",
      "could not double-buffer - trying single-buffering" );
#endif // SOXT_DEBUG
    this->doubleBuffer = FALSE;
    visual = glXChooseVisual( display, DefaultScreen( display ), single_attrs );
    if ( visual == NULL ) {
      SoDebugError::post( "SoXtGLWidget::buildWidget",
        "could not get visual with RGB and ZBUFFER capabilities" );
      XtAppError( SoXt::getAppContext(), "SoXtGLWidget::buildWidget()" );
    }
  }

  this->glxManager = XtVaCreateManagedWidget(
    "GLBorderWidget", xmFormWidgetClass, parent, NULL );

  if ( ! (visual->c_class & (TrueColor | PseudoColor)) ) {
    SoDebugError::post( "SoXtGLWidget::buildWidget",
        "Visual hasn't the necessary color capabilities" );
    XtAppError( SoXt::getAppContext(), "SoXtGLWidget::buildWidget()" );
  }

  Colormap colors;
  XStandardColormap * cmaps;
  int nmaps;

  if ( XmuLookupStandardColormap(
           display, visual->screen, visual->visualid,
           visual->depth, XA_RGB_DEFAULT_MAP, False, True ) &&
       XGetRGBColormaps( display, RootWindow( display, visual->screen ),
           &cmaps, &nmaps, XA_RGB_DEFAULT_MAP ) )
  {
    SbBool found = FALSE;
    for ( int i = 0; i < nmaps && ! found; i++ ) {
      if ( cmaps[i].visualid == visual->visualid ) {
        colors = cmaps[i].colormap;
        XFree( cmaps );
        found = TRUE;
      }
    }
    if ( ! found )
      colors = XCreateColormap( display, RootWindow( display, visual->screen ),
                                visual->visual, AllocNone );
  } else {
    colors = XCreateColormap( display, RootWindow( display, visual->screen ),
                              visual->visual, AllocNone );
  }

  this->glxWidget = XtVaCreateManagedWidget(
    "GLWidget", glwMDrawingAreaWidgetClass, this->glxManager,
    GLwNvisualInfo, visual,
    XtNcolormap, colors,
    XmNleftAttachment,   XmATTACH_FORM,
    XmNtopAttachment,    XmATTACH_FORM,
    XmNrightAttachment,  XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    NULL );

  this->setBorder( this->isBorder() ); // set the widget parent-form offsets

  XtAddEventHandler( this->glxWidget,
      ExposureMask | StructureNotifyMask | ButtonPressMask | ButtonReleaseMask
      | PointerMotionMask | KeyPressMask | KeyReleaseMask,
      False, SoXtGLWidget::glWidgetEventHandler, this );

  this->setBaseWidget( this->glxManager );
  return this->glxManager;
} // buildWidget()

Widget
SoXtGLWidget::getGlxMgrWidget( // protected
  void )
{
  return this->glxManager;
} // getGlxMgrWidget()

Widget
SoXtGLWidget::getGLWidget( // protected
  void )
{
  return this->glxWidget;
} // getGLWidget()

// *************************************************************************

void
SoXtGLWidget::glWidgetEventHandler( // static, protected
  Widget widget,
  XtPointer user,
  XEvent * event,
  Boolean * dispatch )
{
  ((SoXtGLWidget *) user)->processEvent( (XAnyEvent *) event );
  *dispatch = True;
} // glWidgetEventHandler()

// *************************************************************************

/*!
*/

void
SoXtGLWidget::glLock(
  void )
{
  assert( this->glxWidget != (Widget) NULL );
  this->glLockLevel++;
  assert( this->glLockLevel < 10 && "must be programming error" );
  glXMakeCurrent( SoXt::getDisplay(), XtWindow( this->glxWidget ),
                  this->normalContext );
} // glLock()

/*!
*/

void
SoXtGLWidget::glUnlock(
  void )
{
  assert( this->glxWidget != (Widget) NULL );
  this->glLockLevel--;
  assert( this->glLockLevel >= 0 && "programming error" );
} // glUnlock()

/*!
*/

void
SoXtGLWidget::glSwapBuffers(
  void )
{
  assert( this->doubleBuffer != FALSE );
  assert( this->glLockLevel > 0 );
#if 0 // SOXT_DEBUG
  SoDebugError::postInfo( "SoXtGLWidget::glSwapBuffers", "called" );
#endif // 0 was SOXT_DEBUG
  glXSwapBuffers( SoXt::getDisplay(), XtWindow( this->glxWidget ) );
} // glSwapBuffers()

/*!
*/

void
SoXtGLWidget::glFlushBuffer(
  void )
{
  assert( this->glxWidget != (Widget) NULL );
  assert( this->glLockLevel > 0 );
  COIN_STUB();
} // glFlushBuffer()

// *************************************************************************

/*!
*/

void
SoXtGLWidget::glInit( // virtual
  void )
{
  assert( this->glxWidget != (Widget) NULL );
  XVisualInfo * visual;
  Display * display = SoXt::getDisplay();
  XtVaGetValues( this->glxWidget, GLwNvisualInfo, &visual, NULL );

  this->normalContext = glXCreateContext( display, visual, None, GL_TRUE );
  if ( ! this->normalContext ) {
    SoDebugError::postInfo( "SoXtGLWidget::glInit",
      "glXCreateContext() returned NULL" );
    XtAppError( SoXt::getAppContext(), "no context" );
  }

  Dimension width, height;
  XtVaGetValues( this->glxWidget, XmNwidth, &width, XmNheight, &height, NULL );
  this->glReshape( width, height );

  glLock();
  glEnable( GL_DEPTH_TEST );
  glUnlock();
} // glInit()

/*!
*/

void
SoXtGLWidget::glReshape( // virtual
  int width,
  int height )
{
  this->sizeChanged( SbVec2s( width, height ) );
#if 0 // SOXT_DEBUG
  SoDebugError::postInfo( "SoXtGLWidget::glReshape",
      "new size: %dx%d", width, height );
#endif // 0 was SOXT_DEBUG
} // glReshape()

/*!
*/

void
SoXtGLWidget::glRender( // virtual
  void )
{
  // do nothing - SoXtRenderArea overloads
} // glRender()

// *************************************************************************
