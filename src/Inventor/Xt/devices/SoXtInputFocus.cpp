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

#include <X11/X.h>

#include <soxtdefs.h>
#include <Inventor/Xt/devices/SoXtInputFocus.h>
#include <Inventor/Xt/devices/SoGuiInputFocusP.h>

#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->pub)

// *************************************************************************

class SoXtInputFocusP : public SoGuiInputFocusP {
public:
  SoXtInputFocusP(SoXtInputFocus * p) : SoGuiInputFocusP(p) { }
};

// *************************************************************************

SoXtInputFocus::SoXtInputFocus(int events)
{
  PRIVATE(this) = new SoXtInputFocusP(this);
  PRIVATE(this)->eventmask = events;
}

SoXtInputFocus::~SoXtInputFocus()
{
  delete PRIVATE(this);
}

// *************************************************************************

// Documented in superclass.
void
SoXtInputFocus::enable(Widget, // widget,
                       SoXtEventHandler *, // func,
                       XtPointer)
{
  SOXT_STUB();
}

// Documented in superclass.
void
SoXtInputFocus::disable(Widget, // widget,
                        SoXtEventHandler *, // func,
                        XtPointer) // data)
{
  SOXT_STUB();
}

// *************************************************************************

const SoEvent *
SoXtInputFocus::translateEvent(XAnyEvent * event)
{
  switch (event->type) {

  // events we should handle:
  case FocusIn:
  case FocusOut:
    do {
      SOXT_STUB();
    } while (FALSE);
    // return SoEvent here
    break;

  // events we should ignore:
  default:
    break;
  }

  return (SoEvent *) NULL;
}

// *************************************************************************

#undef PRIVATE
#undef PUBLIC

