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

#include <Inventor/Xt/SoXtObject.h>
#include <Inventor/Xt/devices/SoXtDevice.h>
#include <Inventor/Xt/SoXtComponent.h>

/*!
  \class SoXtObject Inventor/Xt/SoXtObject.h
  \brief The SoXtObject class is for run-time type checking of SoXt
  objects.

  This class is an extension to the original InventorXt API.
*/

// *************************************************************************

SoType SoXtObject::classTypeId; // static

void
SoXtObject::init( // static
  void )
{
  SoXtObject::initClass();
  SoXtDevice::initClasses();
  SoXtComponent::initClasses();
} // init()

void
SoXtObject::initClass( // static
  void )
{
  assert( SoXtObject::classTypeId == SoType::badType() );
  SoXtObject::classTypeId =
    SoType::createType( SoType::badType(), "SoXtObject");
} // initClass()

SbBool
SoXtObject::isOfType(
  SoType type ) const
{
  return this->getTypeId().isDerivedFrom(type);
} // isOfType()

SoType
SoXtObject::getClassTypeId( // static
  void ) 
{
  return SoXtObject::classTypeId;
} // getClassTypeId()

// *************************************************************************
