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

#include <Inventor/misc/SoBasic.h>

#include <Inventor/Xt/devices/SoXtDevice.h>

// *************************************************************************

void
SoXtDevice::setWindowSize(
  const SbVec2s size )
{
  this->size = size;
}

const SbVec2s
SoXtDevice::getWindowSize(
  void ) const
{
  return this->size;
}

// *************************************************************************

void
SoXtDevice::setEventPosition(
  SoEvent * event,
  int x,
  int y ) const
{
  COIN_STUB();
} // setEventPosition()

// *************************************************************************
