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

#ifndef SOXT_PLANEVIEWER_H
#define SOXT_PLANEVIEWER_H

class SbPlaneProjector;
class SoAnyPlaneViewer;

#include <Inventor/Xt/viewers/SoXtFullViewer.h>

// *************************************************************************

class SoXtPlaneViewer : public SoXtFullViewer {
  typedef SoXtFullViewer inherited;
  friend class SoAnyPlaneViewer;

public:
  SoXtPlaneViewer(
    Widget parent,
    const char * const name = NULL,
    SbBool embed = TRUE,
    SoXtFullViewer::BuildFlag flag = BUILD_ALL,
    SoXtViewer::Type type = BROWSER );
  ~SoXtPlaneViewer( void );

  virtual void setViewing( SbBool enable );
  virtual void setCamera( SoCamera * camera );
  virtual void setCursorEnabled( SbBool enable );

protected:
  SoXtPlaneViewer(
    Widget parent,
    const char * const name,
    SbBool embed,
    SoXtFullViewer::BuildFlag flag,
    SoXtViewer::Type type,
    SbBool buildNow );

  enum PlaneViewerMode {
    IDLE_MODE,

    DOLLY_MODE,
    TRANSLATE_MODE,

    ROTZ_WAIT_MODE,
    ROTZ_MODE,

    SEEK_WAIT_MODE,
    SEEK_MODE
  } mode;



  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  virtual void processEvent( XAnyEvent * event );
  virtual void setSeekMode( SbBool enable );
  virtual void actualRedraw(void);

  virtual void bottomWheelMotion( float value );
  virtual void leftWheelMotion( float value );
  virtual void rightWheelMotion( float value );

  virtual void createPrefSheet(void);
  virtual void createViewerButtons( Widget parent, SbPList * buttonlist );
  virtual void openViewerHelpCard(void);

private:
  void constructor( SbBool build );

  struct {
    Widget x, y, z, camera;
  } buttons;

  struct {
    Pixmap ortho, ortho_ins;
    Pixmap perspective, perspective_ins;
  } pixmaps;

  Widget prefshell, prefsheet, * prefparts;
  int numprefparts;

  SOXT_WIDGET_CALLBACK( xbutton );
  SOXT_WIDGET_CALLBACK( ybutton );
  SOXT_WIDGET_CALLBACK( zbutton );
  SOXT_WIDGET_CALLBACK( camerabutton );

  void zoom( const float difference );

  SbVec2f prevMousePosition;

private:
  SoAnyPlaneViewer * const common;

}; // class SoXtPlaneViewer

// *************************************************************************

#endif // ! SOXT_PLANEVIEWER_H
