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

#include <X11/extensions/XInput.h>

#include <Inventor/misc/SoBasic.h>

// FIXME: really ugly, but necessary to avoid XInput.h/Print.h conflict
#define _XpPrint_H_
#define _XP_PRINT_SERVER_
typedef void * XPContext;

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXt.h>

#include <Inventor/Xt/devices/SoXtSpaceball.h>
#include <Inventor/Xt/devices/SoXtSpaceballP.h>

// *************************************************************************

SoXtSpaceball::SoXtSpaceball(int events)
{
  PRIVATE(this) = new SoXtSpaceballP(this);
  PRIVATE(this)->events = events;
}

SoXtSpaceball::~SoXtSpaceball()
{
  delete PRIVATE(this);
}

// *************************************************************************

// Documented in superclass.
void
SoXtSpaceball::enable(Widget, // widget,
                      SoXtEventHandler *, // func,
                      XtPointer)
{
  SOXT_STUB();
}

// Documented in superclass.
void
SoXtSpaceball::disable(Widget, // widget,
                       SoXtEventHandler *, // func,
                       XtPointer) // data)
{
  SOXT_STUB();
}

// *************************************************************************

const SoEvent *
SoXtSpaceball::translateEvent(XAnyEvent * event)
{
  SOXT_STUB_ONCE();
  return (SoEvent *) NULL;
}

// *************************************************************************

SbBool
SoXtSpaceball::exists(void)
{
  Display * display = SoXt::getDisplay();
  assert(display != NULL);
  Atom SpaceballAtom = XInternAtom(display, XI_SPACEBALL, True);
  if (SpaceballAtom == None)
    return FALSE;
  int numDevices = 0;
  XDeviceInfo * devices = XListInputDevices(display, &numDevices);
  for (int i = 0; i < numDevices; i++) {
    if (devices[i].type == SpaceballAtom) {
      XFreeDeviceList(devices);
      return TRUE;
    }
  }
  XFreeDeviceList(devices);
  return FALSE;
}

// *************************************************************************

void
SoXtSpaceball::setFocusToWindow(SbBool)
{
  SOXT_STUB();
}

SbBool
SoXtSpaceball::isFocusToWindow(void) const
{
  SOXT_STUB();
  return FALSE;
}

// *************************************************************************
