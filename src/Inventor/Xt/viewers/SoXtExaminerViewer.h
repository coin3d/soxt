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

#ifndef SOXT_EXAMINERVIEWER_H
#define SOXT_EXAMINERVIEWER_H

#include <Inventor/Xt/viewers/SoXtFullViewer.h>

class SbSphereSheetProjector;
class SbRotation;
class SoTimerSensor;

// *************************************************************************

class SoXtExaminerViewer : public SoXtFullViewer {
  typedef SoXtFullViewer inherited;

public:
  SoXtExaminerViewer( Widget parent = (Widget) NULL,
      const char * name = NULL, SbBool inParent = TRUE,
      SoXtFullViewer::BuildFlag flags = BUILD_ALL,
      SoXtViewer::Type type = BROWSER );
  ~SoXtExaminerViewer(void);

protected:
  SoXtExaminerViewer( Widget parent, const char * name,
      SbBool inParent, SoXtFullViewer::BuildFlag flags,
      SoXtViewer::Type type, SbBool build );

  virtual void processEvent( XAnyEvent * event );

  virtual void leftWheelMotion( float value );
  virtual void bottomWheelMotion( float value );
  virtual void rightWheelMotion( float value );

  void zoom( float diffvalue );

  void openViewerHelpCard(void);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  void setAnimationEnabled( SbBool enable );
  SbBool isAnimationEnabled(void) const;
  SbBool isAnimating(void) const;
  void stopAnimating(void);

  XtIntervalId spindetecttimerId;
  SbBool spindetecttimerActive;
  static void spindetecttimerCB( XtPointer user, XtIntervalId * id );

  SbBool animatingallowed, spinanimating;
  SbRotation spinincrement;
  int spinsamplecounter;
  SoTimerSensor * timertrigger;

  static void timertriggerCB( void * user, SoSensor * sensor );
  void timertriggerCallback( SoSensor * sensor );

private:
  void constructor( SbBool build );

  enum ViewerMode {
    IDLE,
    INTERACT,
    EXAMINE,
    DRAGGING,
    WAITING_FOR_SEEK,
    ZOOMING,
    WAITING_FOR_PAN,
    PANNING
  } mode;

  void reorientCamera( const SbRotation & rotation );
  void spin( const SbVec2f & mousepos );
  void pan( const SbVec2f & mousepos );
  void zoomByCursor( const SbVec2f & mousepos );

  void setMode( const ViewerMode mode );
  void setModeFromState( const unsigned int state );

  SbSphereSheetProjector * projector;
  SbVec2f lastmouseposition;
  SbVec2f spinsaveposition;
  SbPlane panningplane;

}; // class SoXtExaminerViewer

// *************************************************************************

#endif // ! SOXT_EXAMINERVIEWER_H
