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

// Class documentation in common code file.

// *************************************************************************

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <X11/X.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMouseButtonEvent.h>

#include <Inventor/Xt/devices/SoXtMouse.h>
#include <Inventor/Xt/devices/SoGuiMouseP.h>
#include <soxtdefs.h>

#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->pub)

// *************************************************************************

class SoXtMouseP : public SoGuiMouseP {
public:
  SoXtMouseP(SoXtMouse * p) : SoGuiMouseP(p) { }

  SoLocation2Event * makeLocationEvent(XMotionEvent * event);
  SoMouseButtonEvent * makeButtonEvent(XButtonEvent * event, SoButtonEvent::State state);
};

// *************************************************************************

// Doc in common code file.
SoXtMouse::SoXtMouse(int events)
{
  PRIVATE(this) = new SoXtMouseP(this);
  PRIVATE(this)->eventmask = events;
}

// Doc in common code file.
SoXtMouse::~SoXtMouse()
{
  delete PRIVATE(this);
}

// *************************************************************************

// Doc in superclass.
void
SoXtMouse::enable(Widget widget, SoXtEventHandler * handler, void * closure)
{
  // FIXME: should explicitly convert eventmask to bitmask with X11/Xt
  // bitflag values, just in case either our or X11's enum values
  // should ever change (yeah, I know, slim chance, but still.. that'd
  // be better design). 20020625 mortene.
  XtAddEventHandler(widget, PRIVATE(this)->eventmask, FALSE, handler, closure);
}

// Doc in superclass.
void
SoXtMouse::disable(Widget widget, SoXtEventHandler * handler, void * closure)
{
  XtRemoveEventHandler(widget, PRIVATE(this)->eventmask, FALSE, handler, closure);
}

// *************************************************************************

// Doc in common code file.
const SoEvent *
SoXtMouse::translateEvent(XAnyEvent * event)
{
  SoEvent * soevent = (SoEvent *) NULL;
  SoButtonEvent::State state = SoButtonEvent::UNKNOWN;

  switch (event->type) {

  // events we should catch:
  case ButtonPress:
    if (! (PRIVATE(this)->eventmask & SoXtMouse::BUTTON_PRESS)) break;
    state = SoButtonEvent::DOWN;
    soevent = PRIVATE(this)->makeButtonEvent((XButtonEvent *) event, state);
    break;

  case ButtonRelease:
    if (! (PRIVATE(this)->eventmask & SoXtMouse::BUTTON_RELEASE)) break;
    state = SoButtonEvent::UP;
    soevent = PRIVATE(this)->makeButtonEvent((XButtonEvent *) event, state);
    break;

  case MotionNotify:
    if (! (PRIVATE(this)->eventmask & SoXtMouse::POINTER_MOTION)) break;
    soevent = PRIVATE(this)->makeLocationEvent((XMotionEvent *) event);
    break;

    //  FIXME: implement BUTTON_MOTION filtering. larsa.

  case EnterNotify:
  case LeaveNotify:
    // should we make location-events for these?
    do {
      SOXT_STUB();
    } while (FALSE);
    break;

  // events we should ignore:
  default:
    break;
  }

  return (SoEvent *) soevent;
}

// *************************************************************************

// This method translates from X motion events to Open Inventor
// SoLocation2Event events.
SoLocation2Event *
SoXtMouseP::makeLocationEvent(XMotionEvent * event)
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtMouse::makeLocationEvent",
                         "pointer at (%d, %d)", event->x, PUBLIC(this)->getWindowSize()[1] - event->y);
#endif // 0 was SOXT_DEBUG
  delete this->locationevent;
  this->locationevent = new SoLocation2Event;
  PUBLIC(this)->setEventPosition(this->locationevent, event->x, event->y);

  this->locationevent->setShiftDown((event->state & ShiftMask) ? TRUE : FALSE);
  this->locationevent->setCtrlDown((event->state & ControlMask) ? TRUE : FALSE);
  this->locationevent->setAltDown((event->state & Mod1Mask) ? TRUE : FALSE);

  SbTime stamp;
  stamp.setMsecValue(event->time);
  this->locationevent->setTime(stamp);

  return this->locationevent;
}

// This method translates from X button events (mouse/pointer) to
// Open Inventor SoMouseButtonEvent events.
SoMouseButtonEvent *
SoXtMouseP::makeButtonEvent(XButtonEvent * event, SoButtonEvent::State state)
{
#if 0 // SOXT_DEBUG
  SoDebugError::postInfo("SoXtMouse::makeButtonEvent",
                         "button %d, state %d", event->button, (int) state);
#endif // 0 was SOXT_DEBUG
  delete this->buttonevent;
  this->buttonevent = new SoMouseButtonEvent;
  this->buttonevent->setState(state);

  SoMouseButtonEvent::Button button = SoMouseButtonEvent::ANY;

  switch (event->button) {
  case 1: button = SoMouseButtonEvent::BUTTON1; break;
  case 3: button = SoMouseButtonEvent::BUTTON2; break;
  case 2: button = SoMouseButtonEvent::BUTTON3; break;
#ifdef HAVE_SOMOUSEBUTTONEVENT_BUTTON5
  case 4: button = SoMouseButtonEvent::BUTTON4; break;
  case 5: button = SoMouseButtonEvent::BUTTON5; break;
#endif // HAVE_SOMOUSEBUTTONEVENT_BUTTON5
  default:
    break;
  }

  this->buttonevent->setButton(button);

  PUBLIC(this)->setEventPosition(this->buttonevent, event->x, event->y);

  this->buttonevent->setShiftDown((event->state & ShiftMask) ? TRUE : FALSE);
  this->buttonevent->setCtrlDown((event->state & ControlMask) ? TRUE : FALSE);
  this->buttonevent->setAltDown((event->state & Mod1Mask) ? TRUE : FALSE);

  SbTime stamp;
  stamp.setMsecValue(event->time);
  this->buttonevent->setTime(stamp);

  return this->buttonevent;
}

// *************************************************************************

#undef PRIVATE
#undef PUBLIC

