/**************************************************************************
 *
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
 **************************************************************************/

#if SOXT_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOXT_DEBUG

// *************************************************************************

/*!
  \class SoXtMouse Inventor/Xt/devices/SoXtMouse.h
  \brief The SoXtMouse class is the glue between Xt/Motif mouse handling
  and mouse interaction in the Inventor scene graph.
  \ingroup devices
*/

// *************************************************************************

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
  int events )
{
  this->events = events;
  this->locationEvent = NULL;
  this->buttonEvent = NULL;
} // SoXtMouse()

/*!
  Destructor.
*/

SoXtMouse::~SoXtMouse( // virtual
  void )
{
  delete this->locationEvent;
  delete this->buttonEvent;
} // ~SoXtMouse()

// *************************************************************************

/*!
  This method is not implemented yet.
*/

void
SoXtMouse::enable( // virtual
  Widget, // widget,
  XtEventHandler, // func,
  XtPointer, // data,
  Window ) // window )
{
  SOXT_STUB();
} // enable()

/*!
  This method is not implemented yet.
*/

void
SoXtMouse::disable( // virtual
  Widget, // widget,
  XtEventHandler, // func,
  XtPointer ) // data )
{
  SOXT_STUB();
} // disable()

// *************************************************************************

/*!
  This method translates from X mouse events to Open Inventor events.

  If \a event is an X pointer motion event, an SoLocation2Event object is
  returned.
  If \a event is an X mouse button event, an SoMouseButtonEvent object is
  returned.
  If \a event is not an X mouse event, NULL is returned.
*/

const SoEvent *
SoXtMouse::translateEvent( // virtual
  XAnyEvent * event )
{
  SoEvent * soevent = (SoEvent *) NULL;
  SoButtonEvent::State state = SoButtonEvent::UNKNOWN;

  switch ( event->type ) {

  // events we should catch:
  case ButtonPress:
    if ( ! (this->events & SoXtMouse::BUTTON_PRESS) ) break;
    state = SoButtonEvent::DOWN;
    soevent = this->makeButtonEvent( (XButtonEvent *) event, state );
    break;

  case ButtonRelease:
    if ( ! (this->events & SoXtMouse::BUTTON_RELEASE) ) break;
    state = SoButtonEvent::UP;
    soevent = this->makeButtonEvent( (XButtonEvent *) event, state );
    break;

  case MotionNotify:
    if ( ! (this->events & SoXtMouse::POINTER_MOTION) ) break;
    soevent = this->makeLocationEvent( (XMotionEvent *) event );
    break;

  case EnterNotify:
  case LeaveNotify:
    // should we make location-events for these?
    do {
      SOXT_STUB();
    } while ( FALSE );
    break;

  // events we should ignore:
  default:
    break;

  } // switch ( event->type )

  return (SoEvent *) soevent;
} // translateEvent()

// *************************************************************************

/*!
  This method translates from X motion events to Open Inventor
  SoLocation2Event events.
*/

SoLocation2Event *
SoXtMouse::makeLocationEvent( // private
  XMotionEvent * event )
{
#if 0 // SOXT_DEBUG
  SoDebugError::postInfo( "SoXtMouse::makeLocationEvent",
    "pointer at (%d, %d)", event->x, this->getWindowSize()[1] - event->y );
#endif // 0 was SOXT_DEBUG
  delete this->locationEvent;
  this->locationEvent = new SoLocation2Event;
  this->setEventPosition( this->locationEvent, event->x, event->y );

  this->locationEvent->setShiftDown(
    (event->state & ShiftMask) ? TRUE : FALSE );
  this->locationEvent->setCtrlDown(
    (event->state & ControlMask) ? TRUE : FALSE );
  this->locationEvent->setAltDown(
    (event->state & Mod1Mask) ? TRUE : FALSE );

  SbTime stamp;
  stamp.setMsecValue( event->time );
  this->locationEvent->setTime( stamp );

  return this->locationEvent;
} // makeLocationEvent()

/*!
  This method translates from X button events (mouse/pointer) to
  Open Inventor SoMouseButtonEvent events.
*/

SoMouseButtonEvent *
SoXtMouse::makeButtonEvent( // private
  XButtonEvent * event,
  SoButtonEvent::State state )
{
#if 0 // SOXT_DEBUG
  SoDebugError::postInfo( "SoXtMouse::makeButtonEvent",
    "button %d, state %d", event->button, (int) state );
#endif // 0 was SOXT_DEBUG
  delete this->buttonEvent;
  this->buttonEvent = new SoMouseButtonEvent;
  this->buttonEvent->setState( state );

  SoMouseButtonEvent::Button button = SoMouseButtonEvent::ANY;

  switch ( event->button ) {
  case 1: button = SoMouseButtonEvent::BUTTON1; break;
#if 0
  case 2: button = SoMouseButtonEvent::BUTTON2; break;
  case 3: button = SoMouseButtonEvent::BUTTON3; break;
#else
  case 3: button = SoMouseButtonEvent::BUTTON2; break;
  case 2: button = SoMouseButtonEvent::BUTTON3; break;
#endif
#ifdef HAVE_SOMOUSEBUTTONEVENT_BUTTONS
  case 4: button = SoMouseButtonEvent::BUTTON4; break;
  case 5: button = SoMouseButtonEvent::BUTTON5; break;
#endif // HAVE_SOMOUSEBUTTONEVENT_BUTTONS
  default:
    break;
  } // switch ( event->button )

  this->buttonEvent->setButton( button );

  this->setEventPosition( this->buttonEvent, event->x, event->y );

  this->buttonEvent->setShiftDown(
    (event->state & ShiftMask) ? TRUE : FALSE );
  this->buttonEvent->setCtrlDown(
    (event->state & ControlMask) ? TRUE : FALSE );
  this->buttonEvent->setAltDown(
    (event->state & Mod1Mask) ? TRUE : FALSE );

  SbTime stamp;
  stamp.setMsecValue( event->time );
  this->buttonEvent->setTime( stamp );

  return this->buttonEvent;
} // makeButtonEvent()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtMouseRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

