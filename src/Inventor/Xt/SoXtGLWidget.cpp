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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <assert.h>

#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/DrawingA.h>
#include <X11/IntrinsicP.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xmu/StdCmap.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoBasic.h>
#include <Inventor/SbViewportRegion.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtResource.h>
#include <Inventor/Xt/widgets/SoXtGLArea.h>

#include <Inventor/Xt/SoXtGLWidget.h>
#include <Inventor/Xt/SoAny.h>

// *************************************************************************

/*!
  \class SoXtGLWidget Inventor/Xt/SoXtGLWidget.h
  \brief The SoXtGLWidget class manages GL contexts.
  \ingroup components
*/

SOXT_OBJECT_ABSTRACT_SOURCE(SoXtGLWidget);

// *************************************************************************

/*!
  \fn void SoXtGLWidget::redraw(void) = 0

  This method is invokedwhen the GL buffer needs to be redrawn.
*/

/*!
  \var Widget SoXtGLWidget::glxManager

  The widget managing the GL widget, providing it with a border.
*/

/*!
  \var Widget SoXtGLWidget::glxWidget

  The GL area widget.
*/

/*!
  \var Visual * SoXtGLWidget::normalVisual

  Visual for normal graphics.
*/

/*!
  \var GLXcontext SoXtGLWidget::normalContext

  GL context for normal graphics.
*/

/*!
  \var GLXcontext SoXtGLWidget::normalColorMapSize

  Size of colormap for normal graphics.
*/

/*!
  \var SbColor * SoXtGLWidget::normalColorMap

  Colormap for normal graphics.
*/

/*!
  \var Visual * SoXtGLWidget::overlayVisual

  Visual for overlay graphics.
*/

/*!
  \var GLXcontext SoXtGLWidget::overlayContext

  GL context for overlay graphics.
*/

/*!
  \var GLXcontext SoXtGLWidget::overlayColorMapSize

  Size of colormap for overlay graphics.
*/

/*!
  \var SbColor * SoXtGLWidget::overlayColorMap

  Colormap for overlay graphics.
*/

/*!
  \var SbBool SoXtGLWidget::waitForExpose

  Flag telling whether to wait for expose or not..
*/

/*!
  \var SbBool SoXtGLWidget::border

  Whether there should be a border around the GL area or not.
*/

/*!
  \var int SoXtGLWidget::borderwidth

  Width of the border around the GL area.
*/

/*!
  \var unsigned long SoXtGLWidget::transparentPixel

  Which pixel value for transparent pixels in the overlay planes.
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

SoXtGLWidget::SoXtGLWidget( // protected
  Widget parent,
  const char * name,
  SbBool embed,
  int glModes,
  SbBool build )
: inherited( parent, name, embed )
, waitForExpose( TRUE )
, drawToFrontBuffer( FALSE )
, glxWidget( NULL )
{
  this->doubleBuffer = TRUE;
  this->normalContext = NULL;
  this->overlayContext = NULL;
  this->normalVisual = NULL;
  this->overlayVisual = NULL;

  this->border = FALSE;
  this->borderwidth = 2;
  this->glSize = SbVec2s( -1, -1 );

  if ( build ) {
    Widget glarea = this->buildWidget( this->getParentWidget() );
    this->setBaseWidget( glarea );
    XtVaSetValues( glarea,
      XmNleftAttachment, XmATTACH_FORM,
      XmNtopAttachment, XmATTACH_FORM,
      XmNrightAttachment, XmATTACH_FORM,
      XmNbottomAttachment, XmATTACH_FORM,
      NULL );
  }
} // SoXtGLWidget()

/*!
  The destructor.
*/

SoXtGLWidget::~SoXtGLWidget( // virtual, protected
  void )
{
  if (this->normalContext) SoAny::si()->unregisterGLContext((void*) this);
} // ~SoXtGLWidget()

// *************************************************************************

/*!
  This method returns the transparent pixel for the overlay planes.
*/

unsigned long
SoXtGLWidget::getOverlayTransparentPixel(
  void )
{
  return this->transparentPixel;
} // getOverlayTransparentPixel()

/*!
  This method returns the size of the colormap for the overlay planes.
*/

int
SoXtGLWidget::getOverlayColorMapSize(
  void )
{
  return this->overlayColorMapSize;
} // getOverlayColorMapSize()

/*!
  This methos returns the size of the colormap for the normal planes.
*/

int
SoXtGLWidget::getColorMapSize(
  void )
{
  return this->normalColorMapSize;
} // getColorMapSize()

/*!
  This method returns the Window associated with the normal GL context.
*/

Window
SoXtGLWidget::getNormalWindow(
  void )
{
  assert( this->glxWidget != (Widget) NULL );
  return XtWindow( this->glxWidget );
} // getNormalWindow()

/*!
  This method returns the Window associated with the overlay GL context.
*/

Window
SoXtGLWidget::getOverlayWindow(
  void )
{
  return (Window) NULL;
} // getOverlayWindow()

/*!
  This method returns the normal GL context.
*/

GLXContext
SoXtGLWidget::getNormalContext(
  void )
{
  return this->normalContext;
} // getNormalContext()

/*!
  This method returns the GL context for the overlay planes.
*/

GLXContext
SoXtGLWidget::getOverlayContext(
  void )
{
  return this->overlayContext;
} // getOverlayContext()

/*!
  This method returns the Widget associated with the normal GL context.
*/

Widget
SoXtGLWidget::getNormalWidget(
  void )
{
  return this->glxWidget;
} // getNormalWidget()

/*!
  This method returns the Widget associated with the overlay GL context.
*/

Widget
SoXtGLWidget::getOverlayWidget(
  void )
{
  return (Widget) NULL;
} // getOverlayWidget()

/*!
  This method sets the visual for the normal GL context.
*/

void
SoXtGLWidget::setNormalVisual( // virtual
  XVisualInfo * visual )
{
  this->normalVisual = visual;
} // setNormalVisual()

/*!
  This method returns the visual for the normal GL context.
*/

XVisualInfo *
SoXtGLWidget::getNormalVisual(
  void )
{
  return this->normalVisual;
} // setNormalVisual()

/*!
  This method sets the visual for the overlay GL context.
*/

void
SoXtGLWidget::setOverlayVisual( // virtual
  XVisualInfo * visual )
{
  this->overlayVisual = visual;
} // setOverlayVisual()

/*!
  This method returns the visual for the overlay GL context.
*/

XVisualInfo *
SoXtGLWidget::getOverlayVisual(
  void )
{
  return this->overlayVisual;
} // getOverlayVisual()

/*!
  This method sets whether double buffering is to be used or not.
*/

void
SoXtGLWidget::setDoubleBuffer( // virtual
  SbBool enable )
{
  this->doubleBuffer = enable;
} // setDoubleBuffer()

/*!
  This method returns whether double buffering is used or not.
*/

SbBool
SoXtGLWidget::isDoubleBuffer(
  void )
{
  return this->doubleBuffer;
} // isDoubleBuffer()

/*!
  This method sets whether the GL area should be framed by a border or not.
*/

void
SoXtGLWidget::setBorder(
  SbBool enable )
{
  this->border = enable;
  if ( this->glxWidget != (Widget) NULL ) {
    if ( this->border == FALSE )
      XtVaSetValues( this->glxWidget,
        XmNleftOffset, 0,
        XmNtopOffset, 0,
        XmNrightOffset, 0,
        XmNbottomOffset, 0,
        NULL );
    else
      XtVaSetValues( this->glxWidget,
        XmNleftOffset, this->borderwidth,
        XmNtopOffset, this->borderwidth,
        XmNrightOffset, this->borderwidth,
        XmNbottomOffset, this->borderwidth,
        NULL );
  }
} // setBorder()

/*!
  This method returns whether the GL area is framed by a border or not.
*/

SbBool
SoXtGLWidget::isBorder(
  void ) const
{
  return this->border;
} // isBorder()

/*!
  This method sets whether rendering will be done to the front buffer
  or not.
*/

void
SoXtGLWidget::setDrawToFrontBufferEnable(
  SbBool enable )
{
  this->drawToFrontBuffer = enable;
} // setDrawToFrontBufferEnable()

/*!
  This method returns whether rendering will be done to the front buffer
  or not.
*/

SbBool
SoXtGLWidget::isDrawToFrontBufferEnable(
  void ) const
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
SoXtGLWidget::redrawOverlay( // virtual, protected
  void )
{
  SOXT_STUB();
} // redrawOverlay()

/*!
  This method handles X events.
*/

void
SoXtGLWidget::processEvent( // virtual, protected
  XAnyEvent * event )
{
  switch ( event->type ) {
  case MapNotify:
    this->initNormalContext();
    this->initGraphic();
    break;

  case Expose:
    this->waitForExpose = FALSE; // Gets flipped from TRUE on first expose.
    if (!this->glScheduleRedraw()) {
      this->redraw();
    }
    break;

  case ConfigureNotify:
    if ( this->glxWidget != (Widget) NULL ) {
      Dimension width, height;
      XtVaGetValues( this->glxWidget,
          XmNwidth, &width, XmNheight, &height, NULL );
      this->glSize = SbVec2s( width, height );
    }
    break;

  } // switch ( event->type )

} // processEvent()

/*!
  This method initializes the graphics.
*/

void
SoXtGLWidget::initGraphic( // virtual, protected
  void )
{
  assert( this->glxWidget != (Widget) NULL );

  glLockNormal();
  Dimension width, height;
  XtVaGetValues( this->glxWidget, XmNwidth, &width, XmNheight, &height, NULL );
  this->glSize = SbVec2s(width, height);
  glEnable( GL_DEPTH_TEST );
  glUnlockNormal();
} // initGraphic()


// private method that initializes the normal GL context  
void 
SoXtGLWidget::initNormalContext(void)
{
  assert( this->glxWidget != (Widget) NULL );
  XVisualInfo * visual;
  Display * display = SoXt::getDisplay();
  XtVaGetValues( this->glxWidget, SoXtNvisualInfo, &visual, NULL );
  int screen = DefaultScreen(display);

  SoXtGLWidget * share = (SoXtGLWidget*) SoAny::si()->getSharedGLContext((void*) display, 
                                                                         (void*) screen);
  
  this->normalContext = glXCreateContext(display, visual, 
                                         share ? share->normalContext : None, 
                                         GL_TRUE);
  if ( ! this->normalContext ) {
    SoDebugError::postInfo( "SoXtGLWidget::glInit",
      "glXCreateContext() returned NULL" );
    XtAppError( SoXt::getAppContext(), "no context" );
  }
  else {
    SoAny::si()->registerGLContext((void*) this, (void*) display, (void*) screen);
  }
}


/*!
  This method initializes the overlay graphics.
*/

void
SoXtGLWidget::initOverlayGraphic( // virtual, protected
  void )
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
 
/*!
  This method is invoked when the GL widget component changes size.
*/

void
SoXtGLWidget::sizeChanged( // virtual, protected
  const SbVec2s size )
{
//  SoDebugError::postInfo( "SoXtGLWidget::sizeChanged", "[invoked (%d, %d)]",
//    size[0], size[1] );
  if ( this->isBorder() ) {
    this->glSize[0] = size[0] - 2 * this->borderwidth;
    this->glSize[1] = size[1] - 2 * this->borderwidth;
  } else {
    this->glSize = size;
  }
  XtResizeWidget( this->glxWidget, this->glSize[0], this->glSize[1], 0 );
} // sizeChanged()

/*!
  This method is invoked when a widget Cin the component is changed.
*/

void
SoXtGLWidget::widgetChanged( // virtual, protected
  Widget widget )
{
  SOXT_STUB();
} // widgetChanged()

// *************************************************************************

/*!
  \fn void SoXtGLWidget::setGlxSize( const SbVec2s size )
  This method sets the Glx size.
  \sa setGLSize
*/

/*!
  This methid sets the size of the GL widget.
*/

void
SoXtGLWidget::setGLSize( // protected
  const SbVec2s size )
{
  this->glSize = size;
  if ( this->glxWidget != (Widget) NULL ) {
    Dimension width = size[0];
    Dimension height = size[1];
    XtVaSetValues( this->glxWidget,
      XmNwidth, width,
      XmNheight, height,
      NULL );
  }
} // setGLSize()

/*!
  \fn const SbVec2s SoXtGLWidget::getGlxSize( void ) const

  This method returns the size of the Glx area.

  \sa getGLSize
*/

/*!
  This method returns the size of the GL area.
*/

const SbVec2s
SoXtGLWidget::getGLSize( // protected
  void ) const
{
  return this->glSize;
} // getGLSize()

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
  void ) const
{
  assert( this->glxWidget != (Widget) NULL );
  Dimension width, height;
  XtVaGetValues( this->glxWidget, XmNwidth, &width, XmNheight, &height, NULL );
  return float(width) / float(height);
} // getGLAspectRatio()

// *************************************************************************

/*!
  This method returns whether the GL context is in RGB mode or not.
*/

SbBool
SoXtGLWidget::isRGBMode( // protected
  void )
{
  return TRUE;
} // isRGBMode()

/*!
  This method returns the share group for the display lists for the
  GL context.
*/

int
SoXtGLWidget::getDisplayListShareGroup( // protected
  GLXContext context )
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
SoXtGLWidget::eventHandler( // static, protected
  Widget widget,
  void * closure,
  XEvent * event,
  Boolean * dispatch )
{
  assert( closure != NULL );
  SoXtGLWidget * component = (SoXtGLWidget *) closure;
  component->processEvent( (XAnyEvent *) event );
  *dispatch = False;
} // event_handler()

static void
buildGLAttrs(int * attrs, int trynum)
{
  int pos = 0;
  attrs[pos++] = GLX_RGBA;
  attrs[pos++] = GLX_DEPTH_SIZE;
  attrs[pos++] = 1;
  if ( ! (trynum & 0x04) ) {
    attrs[pos++] = GLX_STENCIL_SIZE;
    attrs[pos++] = 1;
  }
  if ( ! (trynum & 0x02) ) {
    attrs[pos++] = GLX_DOUBLEBUFFER;
  }
  if ( ! (trynum & 0x01) ) {
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
  This method builds the GL widget inside \a parent.
  The returned widget is the widget managing the GL widget and providing it
  with a border.
*/

Widget
SoXtGLWidget::buildWidget( // protected
  Widget parent )
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtGLWidget::buildWidget", "[enter]" );
#endif // SOXT_DEBUG

  this->glxManager = XtVaCreateManagedWidget( "SoXtRenderArea",
    xmFormWidgetClass, parent,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    NULL );
  this->registerWidget( this->glxManager );

  SoXtResource rsc( this->glxManager );

  short width = 0;
  if ( rsc.getResource( "borderThickness", XmRShort, width ) )
    this->borderwidth = width;

  SbBool haveborder = FALSE;
  if ( rsc.getResource( "border", XmRBoolean, haveborder ) )
    this->border = haveborder;

  Display * const display = SoXt::getDisplay();
  int trynum = 0;
  int attrs[32];
  int screen = DefaultScreen(display);
  while ( this->normalVisual == NULL && trynum < 8 ) {
    buildGLAttrs( attrs, trynum );
    this->normalVisual = glXChooseVisual( display, screen, attrs );
    trynum++;
  }

  if ( this->normalVisual == NULL ) {
    SoDebugError::post( "SoXtGLWidget::buildWidget",
      "could not get satisfactory visual for GLX" );
    XtAppError( SoXt::getAppContext(), "SoXtGLWidget::buildWidget()" );
  }

  this->doubleBuffer = ((trynum-1) & 0x02) ? FALSE : TRUE;

  if ( (this->normalVisual->c_class != TrueColor) &&
       (this->normalVisual->c_class != PseudoColor) ) {
    SoDebugError::post( "SoXtGLWidget::buildWidget",
        "Visual hasn't the necessary color capabilities" );
    XtAppError( SoXt::getAppContext(), "SoXtGLWidget::buildWidget()" );
  }

  Colormap colors = (Colormap) NULL;
  XStandardColormap * cmaps = NULL;
  int nmaps = 0;

  if ( XmuLookupStandardColormap(
         display, this->normalVisual->screen, this->normalVisual->visualid,
         this->normalVisual->depth, XA_RGB_DEFAULT_MAP, False, True ) &&
       XGetRGBColormaps( display,
         RootWindow( display, this->normalVisual->screen ), &cmaps, &nmaps,
         XA_RGB_DEFAULT_MAP ) )
  {
    SbBool found = FALSE;
    for ( int i = 0; i < nmaps && ! found; i++ ) {
      if ( cmaps[i].visualid == this->normalVisual->visualid ) {
#if SOXT_DEBUG && 0
        SoDebugError::postInfo( "SoXtGLWidget::buildWidget",
          "got shared color map" );
#endif // SOXT_DEBUG
        colors = cmaps[i].colormap;
        XFree( cmaps );
        found = TRUE;
      }
    }
    if ( ! found )
      colors = XCreateColormap( display,
                   RootWindow( display, this->normalVisual->screen ),
                   this->normalVisual->visual, AllocNone );
  } else {
    colors = XCreateColormap( display,
                 RootWindow( display, this->normalVisual->screen ),
                 this->normalVisual->visual, AllocNone );
  }
  
  this->glxWidget = XtVaCreateManagedWidget( "SoXtGLWidget",
                                             soxtGLAreaWidgetClass, this->glxManager,
                                             SoXtNvisualInfo, this->normalVisual,
                                             XmNcolormap, colors,
                                             SoXtNstencilSize, 1,
                                             XmNleftAttachment, XmATTACH_FORM,
                                             XmNtopAttachment, XmATTACH_FORM,
                                             XmNrightAttachment, XmATTACH_FORM,
                                             XmNbottomAttachment, XmATTACH_FORM,
                                             NULL );
  this->registerWidget( this->glxWidget );  
  
  this->setBorder( this->isBorder() ); // "refresh" the widget offsets

  // Our callback has this signature:
  // (void (*)(_WidgetRec *, SoXtGLWidget *, XAnyEvent *, char *))
  // ..so we need to cast to avoid a compiler warning or error.
  XtAddEventHandler( this->glxWidget,
      ExposureMask | StructureNotifyMask | ButtonPressMask |
      ButtonReleaseMask | PointerMotionMask | KeyPressMask | KeyReleaseMask,
      False,
      (void (*)(_WidgetRec *, void *, _XEvent *, char *))
        SoXtGLWidget::eventHandler, this );

#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtGLWidget::buildWidget", "[exit]" );
#endif // SOXT_DEBUG
  return this->glxManager;
} // buildWidget()

/*!
  This method returns the widget that is managing the GL widget.
*/

Widget
SoXtGLWidget::getGlxMgrWidget( // protected
  void )
{
  return this->glxManager;
} // getGlxMgrWidget()

/*!
  This method returns the actual GL widget.
*/

Widget
SoXtGLWidget::getGLWidget( // protected
  void )
{
  return this->glxWidget;
} // getGLWidget()

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
  assert( this->glxWidget != (Widget) NULL );
  glXMakeCurrent( SoXt::getDisplay(), XtWindow( this->glxWidget ),
                  this->normalContext );
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
  void )
{
  assert( this->doubleBuffer != FALSE );
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtGLWidget::glSwapBuffers", "called" );
#endif // SOXT_DEBUG
  glXSwapBuffers( SoXt::getDisplay(), XtWindow( this->glxWidget ) );
} // glSwapBuffers()

/*!
  This methid flushes the GL context.
*/

void
SoXtGLWidget::glFlushBuffer(
  void )
{
  assert( this->glxWidget != (Widget) NULL );
  // nothing to do...
  glFlush();
} // glFlushBuffer()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtGLWidgetRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

