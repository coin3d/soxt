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

#ifndef SOXT_CONSTRAINEDVIEWER_H
#define SOXT_CONSTRAINEDVIEWER_H

#include <Inventor/Xt/viewers/SoXtFullViewer.h>

class SoAnyConstrainedViewer;

// *************************************************************************

class SOXT_DLL_EXPORT SoXtConstrainedViewer : public SoXtFullViewer {
  typedef SoXtFullViewer inherited;
  friend class SoAnyConstrainedViewer;

public:
  void setUpDirection( const SbVec3f & vector );
  const SbVec3f & getUpDirection(void) const;

  virtual void setCamera( SoCamera * camera );
  virtual void saveHomePosition(void);
  virtual void resetToHomePosition(void);
  virtual void recomputeSceneSize(void);

protected:
  SoXtConstrainedViewer(
    Widget parent,
    const char * const name,
    SbBool embed,
    SoXtFullViewer::BuildFlag flag,
    SoXtViewer::Type type,
    SbBool build );
  ~SoXtConstrainedViewer(void);

  SbVec3f upDirection;
  float sceneHeight;

  virtual void tiltCamera( float delta );
  virtual void bottomWheelMotion( float value );
  virtual void leftWheelMotion( float value );

  virtual void changeCameraValues( SoCamera * camera );
  void findUpDirection( SbVec2s mousepos );
  void checkForCameraUpConstrain(void);
  virtual void computeSeekFinalOrientation(void);

private:
  SbVec3f prevUpDirection;

  SoAnyConstrainedViewer * const common;

}; // class SoXtConstrainedViewer

// *************************************************************************

#endif // ! SOXT_CONSTRAINEDVIEWER_H
