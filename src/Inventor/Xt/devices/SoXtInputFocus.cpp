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

#include <X11/X.h>

#include <soxtdefs.h>
#include <Inventor/Xt/devices/SoXtInputFocus.h>
#include <Inventor/Xt/devices/SoGuiInputFocusP.h>

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
