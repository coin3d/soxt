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
