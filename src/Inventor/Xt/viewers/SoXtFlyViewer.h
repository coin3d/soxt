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

#ifndef SOXT_FLYVIEWER_H
#define SOXT_FLYVIEWER_H

#include <Inventor/Xt/viewers/SoXtConstrainedViewer.h>

class SoAnyFlyViewer;

// *************************************************************************

class SoXtFlyViewer : public SoXtConstrainedViewer {
  typedef SoXtConstrainedViewer inherited;
  friend class SoAnyFlyViewer;

public:
  SoXtFlyViewer(
    Widget parent = NULL,
    const char * const name = NULL,
    SbBool inParent = TRUE,
    SoXtFullViewer::BuildFlag flags = BUILD_ALL,
    SoXtViewer::Type type = BROWSER );
  ~SoXtFlyViewer(void);

  virtual void setViewing( SbBool enable );
  virtual void resetToHomePosition(void);
  virtual void setCamera( SoCamera * camera );
  virtual void setCursorEnabled( SbBool enable );
  virtual void setCameraType( SoType type );

protected:
  SoXtFlyViewer(
    Widget parent,
    const char * const name,
    SbBool inParent,
    SoXtFullViewer::BuildFlag flag,
    SoXtViewer::Type type,
    SbBool build );

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  virtual void processEvent( XAnyEvent * event );
  virtual void setSeekMode( SbBool enable );
  virtual void actualRedraw(void);
  virtual void rightWheelMotion( float value );
  virtual void createPrefSheet(void);
  virtual void openViewerHelpCard(void);

private:
  void constructor( SbBool build );

  SoAnyFlyViewer * const common;

}; // class SoXtFlyViewer

// *************************************************************************

#endif // ! SOXT_FLYVIEWER_H
