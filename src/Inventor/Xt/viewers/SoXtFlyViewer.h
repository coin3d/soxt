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

#ifndef SOXT_FLYVIEWER_H
#define SOXT_FLYVIEWER_H

#include <Inventor/Xt/viewers/SoXtConstrainedViewer.h>

class SoAnyFlyViewer;

// *************************************************************************

class SOXT_DLL_EXPORT SoXtFlyViewer : public SoXtConstrainedViewer {
  SOXT_OBJECT_HEADER(SoXtFlyViewer, SoXtConstrainedViewer);
  friend class SoAnyFlyViewer;

public:
  SoXtFlyViewer(
    Widget parent = NULL,
    const char * const name = NULL,
    SbBool embed = TRUE,
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
    SbBool embed,
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

  Widget prefshell, prefsheet, * prefparts;
  int numprefparts;

}; // class SoXtFlyViewer

// *************************************************************************

#endif // ! SOXT_FLYVIEWER_H
