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

#ifndef SOXT_CONSTRAINEDVIEWER_H
#define SOXT_CONSTRAINEDVIEWER_H

#include <Inventor/Xt/viewers/SoXtFullViewer.h>

class SoAnyConstrainedViewer;

// *************************************************************************

class SoXtConstrainedViewer : public SoXtFullViewer {
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
