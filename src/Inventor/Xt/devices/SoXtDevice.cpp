/**************************************************************************\
 *
 *  This file is part of the Coin family of 3D visualization libraries.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and / or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use this library in software that is incompatible
 *  with the GNU GPL, and / or you would like to take advantage of the
 *  additional benefits with regard to our support services, please
 *  contact Systems in Motion about acquiring a Coin Professional
 *  Edition License.  See <URL:http://www.coin3d.org> for more
 *  information.
 *
 *  Systems in Motion, Abels gate 5, Teknobyen, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#if SOXT_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // SOXT_DEBUG

#include <Inventor/SbPList.h>
#include <Inventor/misc/SoBasic.h>
#include <Inventor/events/SoEvent.h>

#include <soxtdefs.h>

#include <Inventor/Xt/devices/SoXtDevice.h>
#include <Inventor/Xt/devices/SoXtDeviceP.h>

#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->pub)

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

#undef PRIVATE
#undef PUBLIC

