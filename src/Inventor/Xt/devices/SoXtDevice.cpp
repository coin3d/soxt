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
#include <Inventor/errors/SoDebugError.h>
#endif // SOXT_DEBUG

#include <Inventor/SbPList.h>
#include <Inventor/misc/SoBasic.h>
#include <Inventor/events/SoEvent.h>

#include <soxtdefs.h>

#include <Inventor/Xt/devices/SoXtDevice.h>
#include <Inventor/Xt/devices/SoXtDeviceP.h>

// *************************************************************************

SoXtDevice::SoXtDevice(void)
{
  PRIVATE(this) = new SoXtDeviceP(this);
}

SoXtDevice::~SoXtDevice()
{
  delete PRIVATE(this);
}

// *************************************************************************

void
SoXtDevice::invokeHandlers(XAnyEvent * event)
{
  PRIVATE(this)->invokeHandlers(SoXtDeviceP::invokeHandlerCB, event);
}

// *************************************************************************

#ifndef DOXYGEN_SKIP_THIS

// Code and data for the internal, private hidden implementation
// class.

SbBool
SoXtDeviceP::invokeHandlerCB(SoXtEventHandler * handler,
                             Widget widget,
                             XAnyEvent * event,
                             void * handlerclosure)
{
  Boolean dispatch = False;
  handler(widget, handlerclosure, (XEvent *)event, &dispatch);
  return TRUE;
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************
