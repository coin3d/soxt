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

#if SOXT_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOXT_DEBUG

// *************************************************************************

/*!
  \class SoXtCollisionViewer Inventor/Xt/viewers/SoXtCollisionViewer.h
  \brief The SoXtCollisionViewer class is yet to be documented.
  \ingroup viewers
*/

// *************************************************************************

#include <assert.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/viewers/SoXtCollisionViewer.h>

// *************************************************************************

/*!
  \fn SbBool SoXtCollisionViewer::getZBufferOptimistation(void) const

  This is a function name with a couple of typos in it, included purely for
  OIV compatibility reasons.
*/

// *************************************************************************

/*!
  Public constructor.
*/

SoXtCollisionViewer::SoXtCollisionViewer(
  SoXtViewer * const _viewer)
: viewer(_viewer)
{
  assert(viewer != NULL);
  this->numsteps = 3;
  this->distance = 1.0f;
  this->collisiondetection = TRUE;
  this->zbufferoptimize = FALSE;
} // SoXtCollisionViewer()

/*!
  Destructor.
*/

SoXtCollisionViewer::~SoXtCollisionViewer(
  void)
{
} // ~SoXtCollisionViewer()

// *************************************************************************

/*!
  Returns the viewer.
*/

SoXtViewer *
SoXtCollisionViewer::getViewer(
  void) const
{
  return this->viewer;
} // getViewer()

/*!
  Sets the collision distance.
*/

void
SoXtCollisionViewer::setDistance(
  const float distance)
{
  this->distance = distance;
} // setDistance()

/*!
  Returns the collision distance.
*/

float
SoXtCollisionViewer::getDistance(
  void) const
{
  return this->distance;
} // getDistance()

/*!
  Sets the number of steps?
*/

void
SoXtCollisionViewer::setNumSteps(
  const int steps)
{
  this->numsteps = steps;
} // setNumSteps()

/*!
  Returns the number of steps.
*/

int
SoXtCollisionViewer::getNumSteps(
  void) const
{
  return this->numsteps;
} // getNumSteps()

/*!
  Enables or disables collision detection.
*/

void
SoXtCollisionViewer::setCollisionDetection(
  const SbBool enable)
{
  this->collisiondetection = enable;
} // setCollisionDetection()

/*!
  Returns whether collision detection is enabled or not.
*/

SbBool
SoXtCollisionViewer::getCollisionDetection(
  void) const
{
  return this->collisiondetection;
} // getCollisionDetection()

/*!
  Sets whether the Z buffer should be used to optimize or not.
*/

void
SoXtCollisionViewer::setZBufferOptimization(
  const SbBool enable)
{
  this->zbufferoptimize = enable;
} // setZBufferOptimization()

/*!
  Returns whether the Z buffer is used to optimize or not.
*/

SbBool
SoXtCollisionViewer::getZBufferOptimization(
  void) const
{
  return this->zbufferoptimize;
} // getZBufferOptimisation()

// *************************************************************************

/*!
  Adds a callback.
*/

void
SoXtCollisionViewer::addCallback(
  SoXtCollisionViewerCB * func,
  void * user)
{
} // addCallback()

/*!
  Removes a callback.
*/

void
SoXtCollisionViewer::removeCallback(
  SoXtCollisionViewerCB * func,
  void * user)
{
} // removeCallback()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtCollisionViewerRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

