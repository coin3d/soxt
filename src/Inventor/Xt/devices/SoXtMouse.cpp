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

#include <X11/X.h>

#if SOXT_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // SOXT_DEBUG
#include <Inventor/misc/SoBasic.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMouseButtonEvent.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtBasic.h>

#include <Inventor/Xt/devices/SoXtMouse.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

// *************************************************************************

/*!
  \class SoXtMouse Inventor/Xt/devices/SoXtMouse.h
  \brief The SoXtMouse class is the glue between Xt/Motif mouse handling
  and mouse interaction in the Inventor scene graph.
  \ingroup devices
*/

// *************************************************************************

SOXT_OBJECT_SOURCE(SoXtMouse);

// *************************************************************************

/*
  FIXME: implement BUTTON_MOTION filtering
*/

// *************************************************************************

/*!
  \enum SoXtMouse::MouseEvents

  Enumeration over supported mouse events.
*/

// *************************************************************************

/*!
  Public constructor.
*/

SoXtMouse::SoXtMouse(
  int events)
{
  this->eventmask = events;
  this->locationevent = NULL;
  this->buttonevent = NULL;
} // SoXtMouse()

/*!
  Destructor.
*/

SoXtMouse::~SoXtMouse(// virtual
  void)
{
  delete this->locationevent;
  delete this->buttonevent;
} // ~SoXtMouse()

// *************************************************************************

// Doc in superclass.
void
SoXtMouse::enable(Widget widget, SoXtEventHandler * handler,
                     XtPointer closure, Window window)
{
  XtAddEventHandler(widget, this->eventmask, FALSE, handler, closure);
}

// Doc in superclass.
void
SoXtMouse::disable(Widget widget, SoXtEventHandler * handler,
                   XtPointer closure)
{
  XtRemoveEventHandler(widget, this->eventmask, FALSE, handler, closure);
}

// *************************************************************************

// FIXME: there's better doc in SoQtMouse::translateEvent(). 20011220 mortene.
/*!
  This method translates from X mouse events to Open Inventor events.

  If \a event is an X pointer motion event, an SoLocation2Event object is
  returned.
  If \a event is an X mouse button event, an SoMouseButtonEvent object is
  returned.
  If \a event is not an X mouse event, NULL is returned.
*/

const SoEvent *
SoXtMouse::translateEvent(// virtual
  XAnyEvent * event)
{
  SoEvent * soevent = (SoEvent *) NULL;
  SoButtonEvent::State state = SoButtonEvent::UNKNOWN;

  switch (event->type) {

  // events we should catch:
  case ButtonPress:
    if (! (this->eventmask & SoXtMouse::BUTTON_PRESS)) break;
    state = SoButtonEvent::DOWN;
    soevent = this->makeButtonEvent((XButtonEvent *) event, state);
    break;

  case ButtonRelease:
    if (! (this->eventmask & SoXtMouse::BUTTON_RELEASE)) break;
    state = SoButtonEvent::UP;
    soevent = this->makeButtonEvent((XButtonEvent *) event, state);
    break;

  case MotionNotify:
    if (! (this->eventmask & SoXtMouse::POINTER_MOTION)) break;
    soevent = this->makeLocationEvent((XMotionEvent *) event);
    break;

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

  } // switch (event->type)

  return (SoEvent *) soevent;
} // translateEvent()

// *************************************************************************

/*!
  This method translates from X motion events to Open Inventor
  SoLocation2Event events.
*/

SoLocation2Event *
SoXtMouse::makeLocationEvent(// private
  XMotionEvent * event)
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtMouse::makeLocationEvent",
    "pointer at (%d, %d)", event->x, this->getWindowSize()[1] - event->y);
#endif // 0 was SOXT_DEBUG
  delete this->locationevent;
  this->locationevent = new SoLocation2Event;
  this->setEventPosition(this->locationevent, event->x, event->y);

  this->locationevent->setShiftDown(
    (event->state & ShiftMask) ? TRUE : FALSE);
  this->locationevent->setCtrlDown(
    (event->state & ControlMask) ? TRUE : FALSE);
  this->locationevent->setAltDown(
    (event->state & Mod1Mask) ? TRUE : FALSE);

  SbTime stamp;
  stamp.setMsecValue(event->time);
  this->locationevent->setTime(stamp);

  return this->locationevent;
} // makeLocationEvent()

/*!
  This method translates from X button events (mouse/pointer) to
  Open Inventor SoMouseButtonEvent events.
*/

SoMouseButtonEvent *
SoXtMouse::makeButtonEvent(// private
  XButtonEvent * event,
  SoButtonEvent::State state)
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
  } // switch (event->button)

  this->buttonevent->setButton(button);

  this->setEventPosition(this->buttonevent, event->x, event->y);

  this->buttonevent->setShiftDown(
    (event->state & ShiftMask) ? TRUE : FALSE);
  this->buttonevent->setCtrlDown(
    (event->state & ControlMask) ? TRUE : FALSE);
  this->buttonevent->setAltDown(
    (event->state & Mod1Mask) ? TRUE : FALSE);

  SbTime stamp;
  stamp.setMsecValue(event->time);
  this->buttonevent->setTime(stamp);

  return this->buttonevent;
} // makeButtonEvent()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtMouseRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

