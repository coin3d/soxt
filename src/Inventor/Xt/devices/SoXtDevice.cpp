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

#include <assert.h>

#if SOXT_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // SOXT_DEBUG

#include <Inventor/misc/SoBasic.h>
#include <Inventor/events/SoEvent.h>

#include <Inventor/Xt/devices/SoXtDevice.h>

// *************************************************************************

SoXtDevice::SoXtDevice(
  void )
: size( 0, 0 )
{
} // SoXtDevice()

// *************************************************************************

void
SoXtDevice::setWindowSize(
  const SbVec2s size )
{
  this->size = size;
} // setWindowSize()

const SbVec2s
SoXtDevice::getWindowSize(
  void ) const
{
  return this->size;
} // getWindowSize()

// *************************************************************************

void
SoXtDevice::setEventPosition(
  SoEvent * event,
  int x,
  int y ) const
{
  assert( event != NULL );
  SbVec2s position( x, this->size[1] - y - 1 );
  event->setPosition( position );
#if 0 // SOXT_DEBUG
  SoDebugError::postInfo( "SoXtDevice::setEventPosition",
    "position = (%d, %d)", position[0], position[1] );
#endif // 0 was SOXT_DEBUG
} // setEventPosition()

// *************************************************************************
