/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

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

#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->pub)

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

#undef PRIVATE
#undef PUBLIC

