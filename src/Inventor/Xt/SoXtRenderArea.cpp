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
#include <Inventor/misc/SoBasic.h>
#include <Inventor/SoSceneManager.h>

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
  this->clear = TRUE;
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
  this->manager = new SoSceneManager;
  this->manager->setRenderCallback( SoXtRenderArea::renderCB, this );
  this->manager->activate();
} // constructor()
  
SoXtRenderArea::~SoXtRenderArea(
  void )
{
  delete this->manager;
} // ~SoXtRenderArea()

// *************************************************************************

void
SoXtRenderArea::setSceneGraph( // virtual
  SoNode * scene )
{
  this->manager->setSceneGraph( scene );
}

SoNode *
SoXtRenderArea::getSceneGraph( // virtual
  void )
{
  return this->manager->getSceneGraph();
}

void
SoXtRenderArea::setOverlaySceneGraph(
  SoNode * scene )
{
  COIN_STUB();
}

SoNode *
SoXtRenderArea::getOverlaySceneGraph(
  void )
{
  COIN_STUB();
}

void
SoXtRenderArea::registerDevice(
  SoXtDevice * device )
{
  COIN_STUB();
}

void
SoXtRenderArea::unregisterDevice(
  SoXtDevice * device )
{
  COIN_STUB();
}

void
SoXtRenderArea::setBackgroundColor(
  const SbColor & color )
{
  COIN_STUB();
}

const SbColor &
SoXtRenderArea::getBackgroundColor(
  void ) const
{
  COIN_STUB();
}

void
SoXtRenderArea::setBackgroundIndex(
  int idx )
{
  COIN_STUB();
}

int
SoXtRenderArea::getBackgroundIndex(
  void ) const
{
  COIN_STUB();
}

void
SoXtRenderArea::setOverlayBackgroundIndex(
  int idx )
{
  COIN_STUB();
}

int
SoXtRenderArea::getOverlayBackgroundIndex(
  void ) const
{
  COIN_STUB();
}

void
SoXtRenderArea::setColorMap(
  int start,
  int num,
  const SbColor * colors )
{
  COIN_STUB();
}

void
SoXtRenderArea::setOverlayColorMap(
  int start,
  int num,
  const SbColor * colors )
{
  COIN_STUB();
}

void
SoXtRenderArea::setViewportRegion(
  const SbViewportRegion & region )
{
  this->manager->getGLRenderAction()->setViewportRegion( region );
}

const SbViewportRegion &
SoXtRenderArea::getViewportRegion(
  void ) const
{
  return this->manager->getGLRenderAction()->getViewportRegion();
}

void
SoXtRenderArea::setTransparencyType(
  SoGLRenderAction::TransparencyType type )
{
  COIN_STUB();
}

SoGLRenderAction::TransparencyType
SoXtRenderArea::getTransparencyType(
  void ) const
{
  COIN_STUB();
}

void
SoXtRenderArea::setAntialiasing(
  SbBool smoothing,
  int numPasses )
{
  COIN_STUB();
}

void
SoXtRenderArea::getAntialiasing(
  SbBool & smoothing,
  int & numPasses) const
{
  COIN_STUB();
}

void
SoXtRenderArea::setClearBeforeRender(
  SbBool trueOrFalse,
  SbBool zbTrueOrFalse )
{
  COIN_STUB();
}

SbBool
SoXtRenderArea::isClearBeforeRender(
  void ) const
{
  COIN_STUB();
}

SbBool
SoXtRenderArea::isClearZBufferBeforeRender(
  void ) const
{
  COIN_STUB();
}

void
SoXtRenderArea::setClearBeforeOverlayRender(
  SbBool enable )
{
  COIN_STUB();
}

SbBool
SoXtRenderArea::isClearBeforeOverlayRender(
  void ) const
{
  COIN_STUB();
}

void
SoXtRenderArea::setAutoRedraw(
  SbBool enable )
{
  COIN_STUB();
}

SbBool
SoXtRenderArea::isAutoRedraw(
  void ) const
{
  COIN_STUB();
}

void
SoXtRenderArea::setRedrawPriority(
  uint32_t priority )
{
  COIN_STUB();
}

uint32_t
SoXtRenderArea::getRedrawPriority(
  void ) const
{
  COIN_STUB();
}

uint32_t
SoXtRenderArea::getDefaultRedrawPriority( // static
  void )
{
  COIN_STUB();
}

void
SoXtRenderArea::render(
  void )
{
  this->glLock();
  this->redraw();
  if ( this->isDoubleBuffer() )
    this->glSwapBuffers();
  this->glUnlock();
}

void
SoXtRenderArea::renderOverlay(
  void )
{
  COIN_STUB();
}

void
SoXtRenderArea::scheduleRedraw(
  void )
{
  COIN_STUB();
}

void
SoXtRenderArea::scheduleOverlayRedraw(
  void )
{
  COIN_STUB();
}

void
SoXtRenderArea::redrawOnSelectionChange(
  SoSelection * selection )
{
  COIN_STUB();
}

void
SoXtRenderArea::redrawOverlayOnSelectionChange(
  SoSelection * selection )
{
  COIN_STUB();
}

void
SoXtRenderArea::setEventCallback(
  SoXtRenderAreaEventCB * func,
  void * user )
{
  COIN_STUB();
}

void
SoXtRenderArea::setSceneManager(
  SoSceneManager * manager )
{
  this->manager = manager;
}

SoSceneManager *
SoXtRenderArea::getSceneManager(
  void ) const
{
  return this->manager;
}

void
SoXtRenderArea::setOverlaySceneManager(
  SoSceneManager * manager )
{
  COIN_STUB();
}

SoSceneManager *
SoXtRenderArea::getOverlaySceneManager(
  void ) const
{
  COIN_STUB();
}

void
SoXtRenderArea::setGLRenderAction(
  SoGLRenderAction * action )
{
  COIN_STUB();
}

SoGLRenderAction *
SoXtRenderArea::getGLRenderAction(
  void ) const
{
  COIN_STUB();
}

void
SoXtRenderArea::setOverlayGLRenderAction(
  SoGLRenderAction * action )
{
  COIN_STUB();
}

SoGLRenderAction *
SoXtRenderArea::getOverlayGLRenderAction(
  void ) const
{
  COIN_STUB();
}

void
SoXtRenderArea::redraw( // virtual, protected
  void )
{
  this->actualRedraw();
}

void
SoXtRenderArea::actualRedraw( // virtual, protected
  void )
{
  this->manager->render( this->clear, this->clear );
}

void
SoXtRenderArea::redrawOverlay( // virtual, protected
  void )
{
  COIN_STUB();
}

void
SoXtRenderArea::actualOverlayRedraw( // virtual, protected
  void )
{
  COIN_STUB();
}

void
SoXtRenderArea::processEvent( // virtual, protected
  XAnyEvent * event )
{
  switch ( event->type ) {
  default:
    inherited::processEvent( event );
  } // switch ( event->type )
} // processEvent()

void
SoXtRenderArea::initGraphic( // virtual, protected
  void )
{
  COIN_STUB();
}

void
SoXtRenderArea::initOverlayGraphic( // virtual, protected
  void )
{
  COIN_STUB();
}

void
SoXtRenderArea::sizeChanged( // virtual, protected
  const SbVec2s size )
{
  COIN_STUB();
}

void
SoXtRenderArea::widgetChanged( // virtual, protected
  Widget widget )
{
  COIN_STUB();
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
  COIN_STUB();
}

const char *
SoXtRenderArea::getDefaultTitle( // virtual, protected
  void ) const
{
  COIN_STUB();
}

const char *
SoXtRenderArea::getDefaultIconTitle( // virtual, protected
  void ) const
{
  COIN_STUB();
}

SbBool
SoXtRenderArea::invokeAppCB( // protected
  XAnyEvent * event )
{
  COIN_STUB();
}

// *************************************************************************

void
SoXtRenderArea::renderCB( // static
  void * user,
  SoSceneManager * manager )
{
  ((SoXtRenderArea *)user)->render();
}

// *************************************************************************
