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
  \class SoXtConstrainedViewer Inventor/Xt/viewers/SoXtConstrainedViewer.h
  \brief The SoXtConstrainedViewer is yet to be documented.
  \ingroup components viewers
*/

// *************************************************************************

#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/viewers/SoAnyConstrainedViewer.h>
#include <Inventor/Xt/viewers/SoXtConstrainedViewer.h>

// *************************************************************************

/*!
  \var SbVec3f SoXtConstrainedViewer::upDirection
  Vector controlling the up direction.
*/

/*!
  \var float SoXtConstrainedViewer::sceneHeight
  FIXME: write doc
*/

// *************************************************************************

/*!
  Protected constructor, used by derived components.
*/

SoXtConstrainedViewer::SoXtConstrainedViewer( // protected
  Widget parent,
  const char * const name,
  SbBool embed,
  SoXtFullViewer::BuildFlag flag,
  SoXtViewer::Type type,
  SbBool build )
: inherited( parent, name, embed, flag, type, FALSE )
, common( new SoAnyConstrainedViewer( this ) )
{
  this->upDirection.setValue( 0, 1, 0 );
  this->prevUpDirection.setValue( 0, 1, 0 );
//  this->sceneHeight = 1.0f;
  this->setLeftWheelString( "Tilt" );
  this->setBottomWheelString( "Rotate" );
  this->setRightWheelString( "Dolly" );
} // SoXtConstrainedViewer()

/*!
  Destructor.
*/

SoXtConstrainedViewer::~SoXtConstrainedViewer(
  void )
{
  delete this->common;
} // ~SoXtConstrainedViewer()

// *************************************************************************

/*!
  FIXME: write doc
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
  FIXME: write doc
*/

void
SoXtConstrainedViewer::setCamera( // virtual
  SoCamera * camera )
{
  // enforce camera up direction? or get camera up direction?
  inherited::setCamera( camera );
} // setCamera()

/*!
  FIXME: write doc
*/

void
SoXtConstrainedViewer::saveHomePosition( // virtual
  void )
{
  this->prevUpDirection = this->upDirection;
  inherited::saveHomePosition();
} // saveHomePosition()

/*!
  FIXME: write doc
*/

void
SoXtConstrainedViewer::resetToHomePosition( // virtual
  void )
{
  // restore upDirection in camera
  this->upDirection = this->prevUpDirection;
  inherited::resetToHomePosition();
} // resetToHomePosition()

/*!
  FIXME: write doc
*/

void
SoXtConstrainedViewer::recomputeSceneSize( // virtual
  void )
{
  // do anything?
  SOXT_STUB();
  inherited::recomputeSceneSize();
} // recomputeSceneSize()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtConstrainedViewer::tiltCamera( // virtual, protected
  float delta )
{
  common->tiltCamera( delta );
} // tiltCamera()

/*!
  FIXME: write doc
*/

void
SoXtConstrainedViewer::bottomWheelMotion( // virtual, protected
  float value )
{
  common->rotateCamera( value - this->getBottomWheelValue() );
  inherited::bottomWheelMotion( value );
} // bottomWheelMotion()

/*!
  FIXME: write doc
*/

void
SoXtConstrainedViewer::leftWheelMotion( // virtual, protected
  float value )
{
  this->tiltCamera( value - this->getLeftWheelValue() );
  inherited::leftWheelMotion( value );
} // leftWheelMotion()

/*!
  FIXME: write doc
*/

void
SoXtConstrainedViewer::changeCameraValues( // virtual, protected
  SoCamera * camera )
{
  SOXT_STUB();
} // changeCameraValues()

/*!
  FIXME: write doc
*/

void
SoXtConstrainedViewer::findUpDirection( // protected
  SbVec2s mousepos )
{
  SOXT_STUB();
} // findUpDirection()

/*!
  FIXME: write doc
*/

void
SoXtConstrainedViewer::checkForCameraUpConstrain( // protected
  void )
{
  SOXT_STUB();
} // checkForCamerUpConstrain()

/*!
  FIXME: write doc
*/

void
SoXtConstrainedViewer::computeSeekFinalOrientation( // virtual, protected
  void )
{
  SOXT_STUB();
} // computeSeekFinalOrientation()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtConstrainedViewerRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

