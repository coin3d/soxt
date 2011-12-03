/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

