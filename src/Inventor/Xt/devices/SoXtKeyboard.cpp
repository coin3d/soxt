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

#include <Inventor/misc/SoBasic.h>
#include <Inventor/events/SoKeyboardEvent.h>

#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/devices/SoXtKeyboard.h>

#include <X11/keysym.h>

#include <ctype.h> // toupper()

// *************************************************************************

/*!
*/

SoXtKeyboard::SoXtKeyboard(
  int events )
{
  this->events = events;
  this->keyboardEvent = NULL;
} // SoXtKeyboard()

/*!
*/

SoXtKeyboard::~SoXtKeyboard(
  void )
{
  delete this->keyboardEvent;
} // ~SoXtKeyboard()

/*!
*/

void
SoXtKeyboard::enable( // virtual
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
SoXtKeyboard::disable( // virtual
  Widget, // widget,
  XtEventHandler, // func,
  XtPointer ) // data )
{
  SOXT_STUB();
} // disable()

/*!
*/

const SoEvent *
SoXtKeyboard::translateEvent( // virtual
  XAnyEvent * event )
{
  switch ( event->type ) {

  // events we should catch:
  case KeyPress:
    return this->makeKeyboardEvent( (XKeyEvent *) event, SoButtonEvent::DOWN);
    break;
  case KeyRelease:
    return this->makeKeyboardEvent( (XKeyEvent *) event, SoButtonEvent::UP );
    break;

  // events we should ignore:
  default:
    break;

  } // switch ( event->type )

  return (SoEvent *) NULL;
} // translateEvent()

/*!
*/

SoKeyboardEvent *
SoXtKeyboard::makeKeyboardEvent( // private
  XKeyEvent * event,
  SoButtonEvent::State state)
{
  delete this->keyboardEvent;
  this->keyboardEvent = new SoKeyboardEvent;
  this->setEventPosition( this->keyboardEvent, event->x, event->y );
  this->keyboardEvent->setState( state );

  char           buffer[1024], number;
  KeySym         keysym;
  SoKeyboardEvent::Key key = SoKeyboardEvent::ANY;

  // FIXME:
  //   do modifiers correctly
  //   0-9, HOME, prior, all pad keys, function keys
  //   backspace, tab, next, end,enter, pause, scroll lock, escape, delete,
  //   print insert, *lock, space, ', ,, -, ., /, ;, =, [, ],\, GRAVE (?)
  if (number = XLookupString(event, buffer, 1024, &keysym, 0)) {
    if (keysym  == XK_Return || keysym == XK_Linefeed)
      key = SoKeyboardEvent::RETURN;
    else {
      char cap = toupper(buffer[0]);
      if (cap >= 'A' && cap <= 'Z') {
        key = (SoKeyboardEvent::Key) ((int) SoKeyboardEvent::A +  (cap - 'A'));
      } 
    }
  }
  if (key == SoKeyboardEvent::ANY) {
    switch (keysym) {
      case XK_Shift_R: key = SoKeyboardEvent::RIGHT_SHIFT; break;
      case XK_Shift_L: key = SoKeyboardEvent::LEFT_SHIFT; break;
      case XK_Control_R: key = SoKeyboardEvent::RIGHT_CONTROL; break;
      case XK_Control_L: key = SoKeyboardEvent::LEFT_CONTROL; break;
      case XK_Left: key = SoKeyboardEvent::LEFT_ARROW; break;
      case XK_Right: key = SoKeyboardEvent::RIGHT_ARROW; break;
      case XK_Up: key = SoKeyboardEvent::UP_ARROW; break;
      case XK_Down: key = SoKeyboardEvent::DOWN_ARROW; break;
      case XK_Page_Up: key = SoKeyboardEvent::PAGE_UP; break;
      case XK_Page_Down: key = SoKeyboardEvent::PAGE_DOWN; break;

      default: SOXT_STUB();
    }
  }
  this->keyboardEvent->setKey(key);
  return this->keyboardEvent;
} // makeKeyboardEvent()

// *************************************************************************

// To shut up nagging compilers...
static const char * getSoXtKeyboardRCSId(void) { return rcsid; }

