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

#ifndef SOXT_EXAMINERVIEWER_H
#define SOXT_EXAMINERVIEWER_H

#include <Inventor/Xt/viewers/SoXtFullViewer.h>

class SbRotation;
class SoTimerSensor;
class SoAnyExaminerViewer;

// *************************************************************************

class SOXT_DLL_EXPORT SoXtExaminerViewer : public SoXtFullViewer {
  SOXT_OBJECT_HEADER(SoXtExaminerViewer, SoXtFullViewer);
  friend class SoAnyExaminerViewer;

public:
  SoXtExaminerViewer(
    Widget parent = (Widget) NULL,
    const char * name = NULL,
    SbBool embed = TRUE,
    SoXtFullViewer::BuildFlag flags = BUILD_ALL,
    SoXtViewer::Type type = BROWSER );
  ~SoXtExaminerViewer(void);

  virtual void setViewing( SbBool enable );

  void setAnimationEnabled( const SbBool enable );
  SbBool isAnimationEnabled(void) const;

  void stopAnimating(void);
  SbBool isAnimating(void) const;

  void setFeedbackVisibility( const SbBool enable );
  SbBool isFeedbackVisible(void) const;

  void setFeedbackSize( const int size );
  int getFeedbackSize(void) const;

  virtual void setCamera( SoCamera * camera );

protected:
  SoXtExaminerViewer(
    Widget parent,
    const char * name,
    SbBool embed,
    SoXtFullViewer::BuildFlag flags,
    SoXtViewer::Type type,
    SbBool build );

  virtual void actualRedraw( void );

  virtual void setSeekMode( SbBool enable );

  virtual void processEvent( XAnyEvent * event );

  virtual void afterRealizeHook(void);

  virtual SbBool processSoEvent( const SoEvent * const event );

  virtual void leftWheelStart( void );
  virtual void leftWheelMotion( float value );
  virtual void bottomWheelStart( void );
  virtual void bottomWheelMotion( float value );
  virtual void rightWheelMotion( float value );

  void openViewerHelpCard(void);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  virtual void createViewerButtons( Widget parent, SbPList * buttonlist );
  void camerabuttonClicked(void);
  static void camerabuttonCB( Widget, XtPointer, XtPointer );

  virtual void createPrefSheet( void );

  Widget createFramedSpinAnimPrefSheetGuts( Widget parent );
  Widget createSpinAnimPrefSheetGuts( Widget parent );
  Widget spinanimtoggle;
  void spinanimtoggled(void);
  static void spinanimtoggledCB( Widget, XtPointer, XtPointer );

  Widget createRotAxisPrefSheetGuts( Widget parent );
  Widget createFramedRotAxisPrefSheetGuts( Widget parent );
  Widget rotpointaxestoggle, rotaxesoverlaytoggle, axessizewheel, axessizefield;
  void rotpointtoggled(void);
  static void rotpointtoggledCB( Widget, XtPointer, XtPointer );
  void rotaxesoverlaytoggled(void);
  static void rotaxesoverlaytoggledCB( Widget, XtPointer, XtPointer );
  void axeswheelmoved( int ticks );
  static void axeswheelmovedCB( Widget, XtPointer, XtPointer );
  void axesfieldchanged(void);
  static void axesfieldchangedCB( Widget, XtPointer, XtPointer );

private:
  void constructor( const SbBool build );

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

  void setCursorRepresentation(const ViewerMode mode);
  void setMode( const ViewerMode mode );
  void setModeFromState( const unsigned int state );

  Widget camerabutton;
  struct {
    Pixmap ortho, ortho_ins;
    Pixmap perspective, perspective_ins;
    Pixmap nocam, nocam_ins;
  } camerapixmaps;

  Cursor cursor;
  SbBool mapped;

  Widget * prefparts;
  int numprefparts;

  SoAnyExaminerViewer * const common;

}; // class SoXtExaminerViewer

// *************************************************************************

#endif // ! SOXT_EXAMINERVIEWER_H
