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

#ifndef SOXT_COLLISIONVIEWER_H
#define SOXT_COLLISIONVIEWER_H

#include <Inventor/SbBasic.h>

#include <Inventor/Xt/SoXtBasic.h>

class SoXtViewer;
class SoXtCollisionViewer;

// *************************************************************************

typedef void SoXtCollisionViewerCB(
  void * userdata, SoXtCollisionViewer * viewer );

class SOXT_DLL_EXPORT SoXtCollisionViewer {
public:
  SoXtCollisionViewer( SoXtViewer * const viewer );
  ~SoXtCollisionViewer(void);

  SoXtViewer * getViewer(void) const;

  void setDistance( const float distance );
  float getDistance(void) const;

  void setNumSteps( const int steps );
  int getNumSteps(void) const;

  void setCollisionDetection( const SbBool value = TRUE );
  SbBool getCollisionDetection(void) const;

  void setZBufferOptimization( const SbBool value = TRUE );
  SbBool getZBufferOptimization(void) const;
  SbBool getZBufferOptimistation(void) const // eh?
    { return this->getZBufferOptimization(); }

  void addCallback( SoXtCollisionViewerCB * func, void * user = NULL );
  void removeCallback( SoXtCollisionViewerCB * func, void * user = NULL );

private:
  SoXtViewer * const viewer;

  int numsteps;
  float distance;
  SbBool collisiondetection;
  SbBool zbufferoptimize;

}; // class SoXtCollisionViewer

// *************************************************************************

#endif // ! SOXT_COLLISIONVIEWER_H
