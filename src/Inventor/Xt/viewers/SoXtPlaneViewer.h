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

#ifndef SOXT_PLANEVIEWER_H
#define SOXT_PLANEVIEWER_H

class SbPlaneProjector;
class SoAnyPlaneViewer;

#include <Inventor/Xt/viewers/SoXtFullViewer.h>

// *************************************************************************

class SOXT_DLL_EXPORT SoXtPlaneViewer : public SoXtFullViewer {
  SOXT_OBJECT_HEADER(SoXtPlaneViewer, SoXtFullViewer);
  friend class SoAnyPlaneViewer;

public:
  SoXtPlaneViewer(
    Widget parent = NULL,
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

  virtual SbBool processSoEvent( const SoEvent * const event );
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

  void xbutton(void);
  static void xbuttonCB( Widget, XtPointer, XtPointer );
  void ybutton(void);
  static void ybuttonCB( Widget, XtPointer, XtPointer );
  void zbutton(void);
  static void zbuttonCB( Widget, XtPointer, XtPointer );
  void camerabutton(void);
  static void camerabuttonCB( Widget, XtPointer, XtPointer );

  void zoom( const float difference );

  SbVec2f prevMousePosition;

private:
  SoAnyPlaneViewer * const common;

}; // class SoXtPlaneViewer

// *************************************************************************

#endif // ! SOXT_PLANEVIEWER_H
