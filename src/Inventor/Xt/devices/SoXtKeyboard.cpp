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

#include <X11/X.h>

#include <Inventor/misc/SoBasic.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/errors/SoDebugError.h>

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

SoXtKeyboard::~SoXtKeyboard( // virtual
  void )
{
  delete this->keyboardEvent;
} // ~SoXtKeyboard()

// *************************************************************************

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

// *************************************************************************

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
  SoButtonEvent::State state )
{
  delete this->keyboardEvent;
  this->keyboardEvent = new SoKeyboardEvent;
  this->setEventPosition( this->keyboardEvent, event->x, event->y );
  this->keyboardEvent->setState( state );

  char keybuf[8];
  KeySym keysym;

  SoKeyboardEvent::Key key = SoKeyboardEvent::ANY;

  int keybufusage = XLookupString( event, keybuf, 8, &keysym, NULL );

  if ( keybufusage == 1 ) {
    char cap = keybuf[0];
    if ( cap >= 'A' && cap <= 'Z' ) {
      key = (SoKeyboardEvent::Key)
        ((int) SoKeyboardEvent::A + (cap - 'A'));
    } else if ( cap >= 'a' && cap <= 'z' ) {
      key = (SoKeyboardEvent::Key)
        ((int) SoKeyboardEvent::A + (cap - 'a'));
    } else if ( cap >= '0' && cap <= '9' ) {
      key = (SoKeyboardEvent::Key)
        ((int) SoKeyboardEvent::NUMBER_0 + (cap - '0'));
    }
  } 

  if ( key == SoKeyboardEvent::ANY ) {
    int offset = 0;
    switch ( keysym ) {
    case XK_Shift_L:    offset++;
    case XK_Shift_R:    offset++;
    case XK_Control_R:  offset++;
    case XK_Control_L:  offset++;
    case XK_Alt_L:      offset++;
    case XK_Alt_R:
      key = (SoKeyboardEvent::Key) ((int) SoKeyboardEvent::RIGHT_ALT - offset);
      break;

    case XK_Home:      offset++;
    case XK_Left:      offset++;
    case XK_Up:        offset++;
    case XK_Right:     offset++;
    case XK_Down:      offset++;
    case XK_Page_Up:   offset++;
    case XK_Page_Down:
      key = (SoKeyboardEvent::Key) ((int) SoKeyboardEvent::PAGE_DOWN - offset);
      break;

// X11 defines XK_Prior and XK_Next to the same as XK_Page_Up and XK_Page_Down
//  case XK_Prior:     offset++;
//  case XK_Next:      offset++;

    case XK_End:
      key = (SoKeyboardEvent::Key) ((int) SoKeyboardEvent::END - offset);
      break;

    case XK_KP_Enter:    offset++;
    case XK_KP_F1:       offset++;
    case XK_KP_F2:       offset++;
    case XK_KP_F3:       offset++;
    case XK_KP_F4:       offset++;
    case XK_KP_0:        offset++;
    case XK_KP_1:        offset++;
    case XK_KP_2:        offset++;
    case XK_KP_3:        offset++;
    case XK_KP_4:        offset++;
    case XK_KP_5:        offset++;
    case XK_KP_6:        offset++;
    case XK_KP_7:        offset++;
    case XK_KP_8:        offset++;
    case XK_KP_9:        offset++;
    case XK_KP_Add:      offset++;
    case XK_KP_Subtract: offset++;
    case XK_KP_Multiply: offset++;
    case XK_KP_Divide:   offset++;
    case XK_KP_Space:    offset++;
    case XK_KP_Tab:      offset++;
    case XK_KP_Insert:   offset++;
    case XK_KP_Delete:   offset++;
    case XK_KP_Decimal:
      key = (SoKeyboardEvent::Key) ((int) SoKeyboardEvent::PAD_PERIOD - offset);
      break;

    case XK_F1:     offset++;
    case XK_F2:     offset++;
    case XK_F3:     offset++;
    case XK_F4:     offset++;
    case XK_F5:     offset++;
    case XK_F6:     offset++;
    case XK_F7:     offset++;
    case XK_F8:     offset++;
    case XK_F9:     offset++;
    case XK_F10:    offset++;
    case XK_F11:    offset++;
    case XK_F12:
      key = (SoKeyboardEvent::Key) ((int) SoKeyboardEvent::F12 - offset);
      break;

    case XK_BackSpace:     offset++;
    case XK_Tab:           offset++;
    case XK_Return:        offset++;
    case XK_Linefeed:      offset++;
    case XK_Pause:         offset++;
    case XK_Scroll_Lock:   offset++;
    case XK_Escape:        offset++;
    case XK_Delete:        offset++;
    case XK_Print:         offset++;
    case XK_Insert:        offset++;
    case XK_Num_Lock:      offset++;
    case XK_Caps_Lock:     offset++;
    case XK_Shift_Lock:    offset++;
    case XK_space:         offset++;
    case XK_apostrophe:    offset++;
    case XK_comma:         offset++;
    case XK_minus:         offset++;
    case XK_period:        offset++;
    case XK_slash:         offset++;
    case XK_semicolon:     offset++;
    case XK_equal:         offset++;
    case XK_bracketleft:   offset++;
    case XK_bracketright:  offset++;
    case XK_grave:
      key = (SoKeyboardEvent::Key) ((int) SoKeyboardEvent::GRAVE - offset);
      break;

    default:
#if SOXT_DEBUG && 0
      SoDebugError::postWarning( "SoXtKeyboard::makeKeyboardEvent",
        "keysym 0x%04x isn't handled", keysym );
#endif // SOXT_DEBUG
      break;
    }
  }

  this->keyboardEvent->setKey( key );

#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "", "shift: %s, control: %s, alt: %s",
    (event->state & ShiftMask) ? "on" : "off",
    (event->state & ControlMask) ? "on" : "off",
    (event->state & Mod1Mask) ? "on" : "off" );
#endif // SOXT_DEBUG

  // modifiers:
  this->keyboardEvent->setShiftDown(
    (event->state & ShiftMask) ? TRUE : FALSE );
  this->keyboardEvent->setCtrlDown(
    (event->state & ControlMask) ? TRUE : FALSE );
  this->keyboardEvent->setAltDown(
    (event->state & Mod1Mask) ? TRUE : FALSE );

  return this->keyboardEvent;
} // makeKeyboardEvent()

// *************************************************************************

// To shut up nagging compilers...
#if SOXT_DEBUG
static const char * getSoXtKeyboardRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

