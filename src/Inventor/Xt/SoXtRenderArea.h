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

#ifndef SOXT_RENDERAREA_H
#define SOXT_RENDERAREA_H

#include <Inventor/SbColor.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoGLRenderAction.h>

#include <Inventor/Xt/SoXtGLWidget.h>

class SoNode;
class SoSelection;
class SoSceneManager;
class SoXtDevice;

// *************************************************************************

typedef SbBool SoXtRenderAreaEventCB(void *userData, XAnyEvent *anyevent);

class SoXtRenderArea : public SoXtGLWidget {
  typedef SoXtGLWidget inherited;

public:
  SoXtRenderArea( Widget parent = NULL, const char *name = NULL,
      SbBool buildInsideParent = TRUE, SbBool getMouseInput = TRUE,
      SbBool getKeyboardInput = TRUE );
  ~SoXtRenderArea(void);

  virtual void setSceneGraph( SoNode * scene );
  virtual SoNode * getSceneGraph(void);
  void setOverlaySceneGraph( SoNode * scene );
  SoNode * getOverlaySceneGraph(void);
  void registerDevice( SoXtDevice * device );
  void unregisterDevice( SoXtDevice * device );
  void setBackgroundColor( const SbColor & color );
  const SbColor & getBackgroundColor(void) const;
  void setBackgroundIndex( int idx );
  int getBackgroundIndex(void) const;
  void setOverlayBackgroundIndex( int idx );
  int getOverlayBackgroundIndex(void) const;
  void setColorMap( int start, int num, const SbColor * colors );
  void setOverlayColorMap( int start, int num, const SbColor * colors );
  void setViewportRegion( const SbViewportRegion &newRegion );
  const SbViewportRegion & getViewportRegion(void) const;
  void setTransparencyType( SoGLRenderAction::TransparencyType type );
  SoGLRenderAction::TransparencyType getTransparencyType(void) const;
  void setAntialiasing( SbBool smoothing, int numPasses);
  void getAntialiasing( SbBool & smoothing, int & numPasses) const;
  void setClearBeforeRender( SbBool trueOrFalse, SbBool zbTrueOrFalse = TRUE );
  SbBool isClearBeforeRender(void) const;
  SbBool isClearZBufferBeforeRender(void) const;
  void setClearBeforeOverlayRender( SbBool enable );
  SbBool isClearBeforeOverlayRender(void) const;
  void setAutoRedraw( SbBool enable );
  SbBool isAutoRedraw(void) const;
  void setRedrawPriority( uint32_t priority );
  uint32_t getRedrawPriority(void) const;
  static uint32_t getDefaultRedrawPriority(void);
  void render(void);
  void renderOverlay(void);
  void scheduleRedraw(void);
  void scheduleOverlayRedraw(void);
  void redrawOnSelectionChange( SoSelection * selection );
  void redrawOverlayOnSelectionChange( SoSelection * selection );
  void setEventCallback( SoXtRenderAreaEventCB * func, void * user = NULL );
  void setSceneManager( SoSceneManager * manager );
  SoSceneManager * getSceneManager(void) const;
  void setOverlaySceneManager( SoSceneManager * manager );
  SoSceneManager * getOverlaySceneManager(void) const;
  void setGLRenderAction( SoGLRenderAction * action );
  SoGLRenderAction * getGLRenderAction(void) const;
  void setOverlayGLRenderAction( SoGLRenderAction * action );
  SoGLRenderAction * getOverlayGLRenderAction(void) const;

protected:
  SoXtRenderArea( Widget parent, const char * name, SbBool buildInsideParent,
      SbBool getMouseInput, SbBool getKeyboardInput, SbBool buildNow );
  virtual void redraw(void);
  virtual void actualRedraw(void);
  virtual void redrawOverlay(void);
  virtual void actualOverlayRedraw(void);

  virtual void processEvent( XAnyEvent * event );
  virtual void initGraphic(void);
  virtual void initOverlayGraphic(void);
  virtual void sizeChanged( const SbVec2s size );
  virtual void widgetChanged( Widget widget );

  Widget buildWidget( Widget parent );
  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;
  SbPList * deviceList;
  SoXtRenderAreaEventCB * appEventHandler;
  void * appEventHandlerData;
  SbBool invokeAppCB( XAnyEvent * event );

private:
  void constructor( SbBool mouseInput, SbBool keyboardInput );

}; // class SoXtRenderArea

// *************************************************************************

#endif // ! SOXT_RENDERAREA_H
