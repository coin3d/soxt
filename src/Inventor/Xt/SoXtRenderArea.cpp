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

#include <Inventor/errors/SoDebugError.h>

#include <Inventor/Xt/SoXtRenderArea.h>

#define DEFAULT_GL_MODE (SO_GLX_RGB | SO_GLX_ZBUFFER | SO_GLX_DOUBLE)

// *************************************************************************

SoXtRenderArea::SoXtRenderArea(
  Widget parent,
  const char * name,
  SbBool inParent,
  SbBool mouseInput,
  SbBool keyboardInput )
: inherited( parent, name, inParent, DEFAULT_GL_MODE, FALSE )
{
  this->constructor( mouseInput, keyboardInput );
  Widget widget = this->buildWidget( inParent ? parent : NULL );
  this->setBaseWidget( widget );
//  SoDebugError::postInfo( "SoXtRenderArea",
//    "size = SbVec2s( %d, %d )", this->getSize()[0], this->getSize()[1] );
  if ( this->getSize()[0] == -1 )
    this->setSize( SbVec2s(400, 400) );
} // SoXtRenderArea()

SoXtRenderArea::SoXtRenderArea( // protected
  Widget parent,
  const char * name,
  SbBool inParent,
  SbBool mouseInput,
  SbBool keyboardInput,
  SbBool build )
: inherited( parent, name, inParent, DEFAULT_GL_MODE, FALSE )
{
} // SoXtRenderArea()

void
SoXtRenderArea::constructor( // private
  SbBool mouseInput,
  SbBool keyboardInput )
{
} // constructor()
  
SoXtRenderArea::~SoXtRenderArea(
  void )
{
} // ~SoXtRenderArea()

// *************************************************************************

void
SoXtRenderArea::setSceneGraph( // virtual
  SoNode * scene )
{
}

SoNode *
SoXtRenderArea::getSceneGraph( // virtual
  void )
{
}

void
SoXtRenderArea::setOverlaySceneGraph(
  SoNode * scene )
{
}

SoNode *
SoXtRenderArea::getOverlaySceneGraph(
  void )
{
}

void
SoXtRenderArea::registerDevice(
  SoXtDevice * device )
{
}

void
SoXtRenderArea::unregisterDevice(
  SoXtDevice * device )
{
}

void
SoXtRenderArea::setBackgroundColor(
  const SbColor & color )
{
}

const SbColor &
SoXtRenderArea::getBackgroundColor(
  void ) const
{
}

void
SoXtRenderArea::setBackgroundIndex(
  int idx )
{
}

int
SoXtRenderArea::getBackgroundIndex(
  void ) const
{
}

void
SoXtRenderArea::setOverlayBackgroundIndex(
  int idx )
{
}

int
SoXtRenderArea::getOverlayBackgroundIndex(
  void ) const
{
}

void
SoXtRenderArea::setColorMap(
  int start,
  int num,
  const SbColor * colors )
{
}

void
SoXtRenderArea::setOverlayColorMap(
  int start,
  int num,
  const SbColor * colors )
{
}

void
SoXtRenderArea::setViewportRegion(
  const SbViewportRegion & region )
{
}

const SbViewportRegion &
SoXtRenderArea::getViewportRegion(
  void ) const
{
}

void
SoXtRenderArea::setTransparencyType(
  SoGLRenderAction::TransparencyType type )
{
}

SoGLRenderAction::TransparencyType
SoXtRenderArea::getTransparencyType(
  void ) const
{
}

void
SoXtRenderArea::setAntialiasing(
  SbBool smoothing,
  int numPasses )
{
}

void
SoXtRenderArea::getAntialiasing(
  SbBool & smoothing,
  int & numPasses) const
{
}

void
SoXtRenderArea::setClearBeforeRender(
  SbBool trueOrFalse,
  SbBool zbTrueOrFalse = TRUE )
{
}

SbBool
SoXtRenderArea::isClearBeforeRender(
  void ) const
{
}

SbBool
SoXtRenderArea::isClearZBufferBeforeRender(
  void ) const
{
}

void
SoXtRenderArea::setClearBeforeOverlayRender(
  SbBool enable )
{
}

SbBool
SoXtRenderArea::isClearBeforeOverlayRender(
  void ) const
{
}

void
SoXtRenderArea::setAutoRedraw(
  SbBool enable )
{
}

SbBool
SoXtRenderArea::isAutoRedraw(
  void ) const
{
}

void
SoXtRenderArea::setRedrawPriority(
  uint32_t priority )
{
}

uint32_t
SoXtRenderArea::getRedrawPriority(
  void ) const
{
}

uint32_t
SoXtRenderArea::getDefaultRedrawPriority( // static
  void )
{
}

void
SoXtRenderArea::render(
  void )
{
}

void
SoXtRenderArea::renderOverlay(
  void )
{
}

void
SoXtRenderArea::scheduleRedraw(
  void )
{
}

void
SoXtRenderArea::scheduleOverlayRedraw(
  void )
{
}

void
SoXtRenderArea::redrawOnSelectionChange(
  SoSelection * selection )
{
}

void
SoXtRenderArea::redrawOverlayOnSelectionChange(
  SoSelection * selection )
{
}

void
SoXtRenderArea::setEventCallback(
  SoXtRenderAreaEventCB * func,
  void * user )
{
}

void
SoXtRenderArea::setSceneManager(
  SoSceneManager * manager )
{
}

SoSceneManager *
SoXtRenderArea::getSceneManager(
  void ) const
{
}

void
SoXtRenderArea::setOverlaySceneManager(
  SoSceneManager * manager )
{
}

SoSceneManager *
SoXtRenderArea::getOverlaySceneManager(
  void ) const
{
}

void
SoXtRenderArea::setGLRenderAction(
  SoGLRenderAction * action )
{
}

SoGLRenderAction *
SoXtRenderArea::getGLRenderAction(
  void ) const
{
}

void
SoXtRenderArea::setOverlayGLRenderAction(
  SoGLRenderAction * action )
{
}

SoGLRenderAction *
SoXtRenderArea::getOverlayGLRenderAction(
  void ) const
{
}

void
SoXtRenderArea::redraw( // virtual, protected
  void )
{
}

void
SoXtRenderArea::actualRedraw( // virtual, protected
  void )
{
}

void
SoXtRenderArea::redrawOverlay( // virtual, protected
  void )
{
}

void
SoXtRenderArea::actualOverlayRedraw( // virtual, protected
  void )
{
}

void
SoXtRenderArea::processEvent( // virtual, protected
 XAnyEvent * event )
{
}

void
SoXtRenderArea::initGraphic( // virtual, protected
  void )
{
}

void
SoXtRenderArea::initOverlayGraphic( // virtual, protected
  void )
{
}

void
SoXtRenderArea::sizeChanged( // virtual, protected
  const SbVec2s size )
{
}

void
SoXtRenderArea::widgetChanged( // virtual, protected
 Widget widget )
{
}

Widget
SoXtRenderArea::buildWidget( // protected
  Widget parent )
{
  Widget widget = inherited::buildWidget( parent );
  return widget;
}

const char *
SoXtRenderArea::getDefaultWidgetName( // virtual, protected
  void ) const
{
}

const char *
SoXtRenderArea::getDefaultTitle( // virtual, protected
  void ) const
{
}

const char *
SoXtRenderArea::getDefaultIconTitle( // virtual, protected
  void ) const
{
}

SbBool
SoXtRenderArea::invokeAppCB( // protected
  XAnyEvent * event )
{
}

// *************************************************************************
