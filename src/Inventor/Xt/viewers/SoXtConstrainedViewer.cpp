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

static const char rcsid[] =
  "$Id$";

#include <Inventor/errors/SoDebugError.h>

#include <Inventor/Xt/viewers/SoAnyConstrainedViewer.h>
#include <Inventor/Xt/viewers/SoXtConstrainedViewer.h>

// *************************************************************************

/*!
  Protected constructor, used by derived components.
*/

SoXtConstrainedViewer::SoXtConstrainedViewer( // protected
  Widget parent,
  const char * const name,
  SbBool inParent,
  SoXtFullViewer::BuildFlag flag,
  SoXtViewer::Type type,
  SbBool build )
: inherited( parent, name, inParent, flag, type, FALSE )
, common( new SoAnyConstrainedViewer( this ) )
{
  this->upDirection.setValue( 0, 0, 1 );
} // SoXtConstrainedViewer()

/*!
  Destructor.
*/

SoXtConstrainedViewer::~SoXtConstrainedViewer(
  void )
{
} // ~SoXtConstrainedViewer()

// *************************************************************************

/*!
*/

void
SoXtConstrainedViewer::setUpDirection(
  const SbVec3f & vector )
{
  // adjust camera
  this->upDirection = vector;
} // setUpDirection()

/*!
  This method returns the up direction of the viewer.
*/

const SbVec3f &
SoXtConstrainedViewer::getUpDirection(
  void ) const
{
  return this->upDirection;
} // getUpDirection()

// *************************************************************************

/*!
*/

void
SoXtConstrainedViewer::setCamera( // virtual
  SoCamera * camera )
{
  inherited::setCamera( camera );
} // setCamera()

/*!
*/

void
SoXtConstrainedViewer::saveHomePosition( // virtual
  void )
{
  // store upDirection
  inherited::saveHomePosition();
} // saveHomePosition()

/*!
*/

void
SoXtConstrainedViewer::resetToHomePosition( // virtual
  void )
{
  // restore upDirection
  inherited::resetToHomePosition();
} // resetToHomePosition()

/*!
*/

void
SoXtConstrainedViewer::recomputeSceneSize( // virtual
  void )
{
  // do anything?
  inherited::recomputeSceneSize();
} // recomputeSceneSize()

// *************************************************************************

/*!
*/

void
SoXtConstrainedViewer::tiltCamera( // virtual, protected
  float delta )
{
} // tiltCamera()

/*!
*/

void
SoXtConstrainedViewer::bottomWheelMotion( // virtual, protected
  float value )
{
  inherited::bottomWheelMotion( value );
} // bottomWheelMotion()

/*!
*/

void
SoXtConstrainedViewer::leftWheelMotion( // virtual, protected
  float value )
{
  inherited::leftWheelMotion( value );
} // leftWheelMotion()

/*!
*/

void
SoXtConstrainedViewer::changeCameraValues( // virtual, protected
  SoCamera * camera )
{
} // changeCameraValues()

/*!
*/

void
SoXtConstrainedViewer::findUpDirection( // protected
  SbVec2s mousepos )
{
} // findUpDirection()

/*!
*/

void
SoXtConstrainedViewer::checkForCameraUpConstrain( // protected
  void )
{
} // checkForCamerUpConstrain()

/*!
*/

void
SoXtConstrainedViewer::computeSeekFinalOrientation( // virtual, protected
  void )
{
} // computeSeekFinalOrientation()

// *************************************************************************
