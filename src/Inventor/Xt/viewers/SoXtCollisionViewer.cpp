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

#if SOXT_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOXT_DEBUG

#include <assert.h>

#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/viewers/SoXtCollisionViewer.h>

// *************************************************************************

/*!
*/

SoXtCollisionViewer::SoXtCollisionViewer(
  SoXtViewer * const _viewer )
: viewer( _viewer )
{
  assert( viewer != NULL );
  this->numsteps = 3;
  this->distance = 1.0f;
  this->collisiondetection = TRUE;
  this->zbufferoptimize = FALSE;
} // SoXtCollisionViewer()

/*!
*/

SoXtCollisionViewer::~SoXtCollisionViewer(
  void )
{
} // ~SoXtCollisionViewer()

// *************************************************************************

/*!
*/

SoXtViewer *
SoXtCollisionViewer::getViewer(
  void ) const
{
  return this->viewer;
} // getViewer()

/*!
*/

void
SoXtCollisionViewer::setDistance(
  const float distance )
{
  this->distance = distance;
} // setDistance()

/*!
*/

float
SoXtCollisionViewer::getDistance(
  void ) const
{
  return this->distance;
} // getDistance()

/*!
*/

void
SoXtCollisionViewer::setNumSteps(
  const int steps )
{
  this->numsteps = steps;
} // setNumSteps()

/*!
*/

int
SoXtCollisionViewer::getNumSteps(
  void ) const
{
  return this->numsteps;
} // getNumSteps()

/*!
*/

void
SoXtCollisionViewer::setCollisionDetection(
  const SbBool enable )
{
  this->collisiondetection = enable;
} // setCollisionDetection()

/*!
*/

SbBool
SoXtCollisionViewer::getCollisionDetection(
  void ) const
{
  return this->collisiondetection;
} // getCollisionDetection()

/*!
*/

void
SoXtCollisionViewer::setZBufferOptimization(
  const SbBool enable )
{
  this->zbufferoptimize = enable;
} // setZBufferOptimization()

/*!
*/

SbBool
SoXtCollisionViewer::getZBufferOptimization(
  void ) const
{
  return this->zbufferoptimize;
} // getZBufferOptimisation()

// *************************************************************************

/*!
*/

void
SoXtCollisionViewer::addCallback(
  SoXtCollisionViewerCB * func,
  void * user )
{
} // addCallback()

/*!
*/

void
SoXtCollisionViewer::removeCallback(
  SoXtCollisionViewerCB * func,
  void * user )
{
} // removeCallback()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtCollisionViewerRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

