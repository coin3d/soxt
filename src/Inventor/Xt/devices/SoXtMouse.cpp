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

static const char rcsid[] =
  "$Id$";

#include <X11/X.h>

#if SOXT_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // SOXT_DEBUG

#include <Inventor/misc/SoBasic.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMouseButtonEvent.h>

#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/devices/SoXtMouse.h>

/*
  FIXME: implement BUTTON_MOTION filtering
*/

// *************************************************************************

/*!
*/

SoXtMouse::SoXtMouse(
  int events )
{
  this->events = events;
  this->locationEvent = NULL;
  this->buttonEvent = NULL;
} // SoXtMouse()

/*!
*/

SoXtMouse::~SoXtMouse(
  void )
{
  delete this->locationEvent;
  delete this->buttonEvent;
} // ~SoXtMouse()

// *************************************************************************

/*!
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

  return this->locationEvent;
} // makeLocationEvent()

/*!
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
  switch ( event->button ) {
  case 1: // left button
    this->buttonEvent->setButton( SoMouseButtonEvent::BUTTON1 );
    break;
  case 2: // midbutton
    this->buttonEvent->setButton( SoMouseButtonEvent::BUTTON2 );
    break;
  case 3: // right button
    this->buttonEvent->setButton( SoMouseButtonEvent::BUTTON3 );
    break;
#if 0 // FIXME: disabled until it's enabled again through autoconf test
  case 4: // wheel up
    this->buttonEvent->setButton( SoMouseButtonEvent::BUTTON4 );
    break;
  case 5: // wheel down
    this->buttonEvent->setButton( SoMouseButtonEvent::BUTTON5 );
    break;
#endif
  default:
    this->buttonEvent->setButton( SoMouseButtonEvent::ANY );
    break;
  } // switch ( event->button )
  this->setEventPosition( this->buttonEvent, event->x, event->y );

  this->buttonEvent->setShiftDown(
    (event->state & ShiftMask) ? TRUE : FALSE );
  this->buttonEvent->setCtrlDown(
    (event->state & ControlMask) ? TRUE : FALSE );
  this->buttonEvent->setAltDown(
    (event->state & Mod1Mask) ? TRUE : FALSE );

  return this->buttonEvent;
} // makeButtonEvent()

// *************************************************************************

// To shut up nagging compilers...
static const char * getSoXtMouseRCSId(void) { return rcsid; }

