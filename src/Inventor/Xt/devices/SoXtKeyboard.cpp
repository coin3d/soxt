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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <ctype.h> // toupper()

#include <X11/X.h>
#include <X11/keysym.h>

#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/errors/SoDebugError.h>

#include <Inventor/Xt/devices/SoXtKeyboard.h>
#include <Inventor/Xt/devices/SoGuiKeyboardP.h>

#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->pub)

// *************************************************************************

class SoXtKeyboardP : public SoGuiKeyboardP {
};

// *************************************************************************

SoXtKeyboard::SoXtKeyboard(int events)
{
  PRIVATE(this) = new SoXtKeyboardP;
  PRIVATE(this)->eventmask = events;
}

SoXtKeyboard::~SoXtKeyboard()
{
  delete PRIVATE(this);
}

// *************************************************************************

// Doc in superclass.
void
SoXtKeyboard::enable(Widget widget, SoXtEventHandler * handler, void * closure)
{
  // FIXME: should explicitly convert eventmask to bitmask with X11/Xt
  // bitflag values, just in case either our or X11's enum values
  // should ever change (yeah, I know, slim chance, but still.. that'd
  // be better design). 20020625 mortene.
  XtAddEventHandler(widget, PRIVATE(this)->eventmask, FALSE, handler, closure);
}

// Doc in superclass.
void
SoXtKeyboard::disable(Widget widget, SoXtEventHandler * handler, void * closure)
{
  // FIXME: should explicitly convert eventmask to bitmask with X11/Xt
  // bitflag values, just in case either our or X11's enum values
  // should ever change (yeah, I know, slim chance, but still.. that'd
  // be better design). 20020625 mortene.
  XtRemoveEventHandler(widget, PRIVATE(this)->eventmask, FALSE, handler, closure);
}

// *************************************************************************

// Creates an SoKeyboardEvent from an X event.
static void
makeKeyboardEvent(XKeyEvent * event, SoKeyboardEvent * sokeybevent)
{
  char keybuf[8];
  KeySym keysym;

  SoKeyboardEvent::Key key = SoKeyboardEvent::ANY;

  int keybufusage = XLookupString(event, keybuf, 8, &keysym, NULL);
  
  // check these first or they will be set to incorrect values
  switch (keysym) {
  case XK_KP_0:         key = SoKeyboardEvent::PAD_0;          break;
  case XK_KP_1:         key = SoKeyboardEvent::PAD_1;          break;
  case XK_KP_2:         key = SoKeyboardEvent::PAD_2;          break;
  case XK_KP_3:         key = SoKeyboardEvent::PAD_3;          break;
  case XK_KP_4:         key = SoKeyboardEvent::PAD_4;          break;
  case XK_KP_5:         key = SoKeyboardEvent::PAD_5;          break;
  case XK_KP_6:         key = SoKeyboardEvent::PAD_6;          break;
  case XK_KP_7:         key = SoKeyboardEvent::PAD_7;          break;
  case XK_KP_8:         key = SoKeyboardEvent::PAD_8;          break;
  case XK_KP_9:         key = SoKeyboardEvent::PAD_9;          break;
  default:
    key = SoKeyboardEvent::ANY;
  }
  
  if (key == SoKeyboardEvent::ANY && keybufusage == 1) {
    switch (keybuf[0]) {
    case 'a': case 'A':   key = SoKeyboardEvent::A;              break;
    case 'b': case 'B':   key = SoKeyboardEvent::B;              break;
    case 'c': case 'C':   key = SoKeyboardEvent::C;              break;
    case 'd': case 'D':   key = SoKeyboardEvent::D;              break;
    case 'e': case 'E':   key = SoKeyboardEvent::E;              break;
    case 'f': case 'F':   key = SoKeyboardEvent::F;              break;
    case 'g': case 'G':   key = SoKeyboardEvent::G;              break;
    case 'h': case 'H':   key = SoKeyboardEvent::H;              break;
    case 'i': case 'I':   key = SoKeyboardEvent::I;              break;
    case 'j': case 'J':   key = SoKeyboardEvent::J;              break;
    case 'k': case 'K':   key = SoKeyboardEvent::K;              break;
    case 'l': case 'L':   key = SoKeyboardEvent::L;              break;
    case 'm': case 'M':   key = SoKeyboardEvent::M;              break;
    case 'n': case 'N':   key = SoKeyboardEvent::N;              break;
    case 'o': case 'O':   key = SoKeyboardEvent::O;              break;
    case 'p': case 'P':   key = SoKeyboardEvent::P;              break;
    case 'q': case 'Q':   key = SoKeyboardEvent::Q;              break;
    case 'r': case 'R':   key = SoKeyboardEvent::R;              break;
    case 's': case 'S':   key = SoKeyboardEvent::S;              break;
    case 't': case 'T':   key = SoKeyboardEvent::T;              break;
    case 'u': case 'U':   key = SoKeyboardEvent::U;              break;
    case 'v': case 'V':   key = SoKeyboardEvent::V;              break;
    case 'w': case 'W':   key = SoKeyboardEvent::W;              break;
    case 'x': case 'X':   key = SoKeyboardEvent::X;              break;
    case 'y': case 'Y':   key = SoKeyboardEvent::Y;              break;
    case 'z': case 'Z':   key = SoKeyboardEvent::Z;              break;
    case '\\': case '|':  key = SoKeyboardEvent::BACKSLASH;      break;
    case '[': case '{':   key = SoKeyboardEvent::BRACKETLEFT;    break;
    case ']': case '}':   key = SoKeyboardEvent::BRACKETRIGHT;   break;
    case '0': case ')':   key = SoKeyboardEvent::NUMBER_0;       break;
    case '1': case '!':   key = SoKeyboardEvent::NUMBER_1;       break;
    case '2': case '@':   key = SoKeyboardEvent::NUMBER_2;       break;
    case '3': case '#':   key = SoKeyboardEvent::NUMBER_3;       break;
    case '4': case '$':   key = SoKeyboardEvent::NUMBER_4;       break;
    case '5': case '%':   key = SoKeyboardEvent::NUMBER_5;       break;
    case '6': case '^':   key = SoKeyboardEvent::NUMBER_6;       break;
    case '7': case '&':   key = SoKeyboardEvent::NUMBER_7;       break;
    case '8': case '*':   key = SoKeyboardEvent::NUMBER_8;       break;
    case '9': case '(':   key = SoKeyboardEvent::NUMBER_9;       break;
    default:
      break;
    }
  } 

  if (key == SoKeyboardEvent::ANY) {
    switch (keysym) {
    case XK_Shift_L:      key = SoKeyboardEvent::LEFT_SHIFT;     break;
    case XK_Shift_R:      key = SoKeyboardEvent::RIGHT_SHIFT;    break;
    case XK_Control_L:    key = SoKeyboardEvent::LEFT_CONTROL;   break;
    case XK_Control_R:    key = SoKeyboardEvent::RIGHT_CONTROL;  break;
    case XK_Alt_L:        key = SoKeyboardEvent::LEFT_ALT;       break;
    case XK_Alt_R:        key = SoKeyboardEvent::RIGHT_ALT;      break;
    case XK_Home:         key = SoKeyboardEvent::HOME;           break;
    case XK_Left:         key = SoKeyboardEvent::LEFT_ARROW;     break;
    case XK_Up:           key = SoKeyboardEvent::UP_ARROW;       break;
    case XK_Right:        key = SoKeyboardEvent::RIGHT_ARROW;    break;
    case XK_Down:         key = SoKeyboardEvent::DOWN_ARROW;     break;
    case XK_Page_Up:      key = SoKeyboardEvent::PAGE_UP;        break;
    case XK_Page_Down:    key = SoKeyboardEvent::PAGE_DOWN;      break;

// X11 defines XK_Prior and XK_Next to the same as XK_Page_Up and XK_Page_Down
//  case XK_Prior:
//  case XK_Next:

    case XK_End:          key = SoKeyboardEvent::END;            break;
    case XK_KP_Enter:     key = SoKeyboardEvent::PAD_ENTER;      break;
    case XK_KP_F1:        key = SoKeyboardEvent::PAD_F1;         break;
    case XK_KP_F2:        key = SoKeyboardEvent::PAD_F2;         break;
    case XK_KP_F3:        key = SoKeyboardEvent::PAD_F3;         break;
    case XK_KP_F4:        key = SoKeyboardEvent::PAD_F4;         break;
    case XK_KP_Add:       key = SoKeyboardEvent::PAD_SUBTRACT;   break;
    case XK_KP_Subtract:  key = SoKeyboardEvent::PAD_ADD;        break;
    case XK_KP_Multiply:  key = SoKeyboardEvent::PAD_MULTIPLY;   break;
    case XK_KP_Divide:    key = SoKeyboardEvent::PAD_DIVIDE;     break;
    case XK_KP_Space:     key = SoKeyboardEvent::PAD_SPACE;      break;
    case XK_KP_Tab:       key = SoKeyboardEvent::PAD_TAB;        break;
    case XK_KP_Insert:    key = SoKeyboardEvent::PAD_INSERT;     break;
    case XK_KP_Delete:    key = SoKeyboardEvent::PAD_DELETE;     break;
    case XK_KP_Decimal:   key = SoKeyboardEvent::PAD_PERIOD;     break;
    case XK_F1:           key = SoKeyboardEvent::F1;             break;
    case XK_F2:           key = SoKeyboardEvent::F2;             break;
    case XK_F3:           key = SoKeyboardEvent::F3;             break;
    case XK_F4:           key = SoKeyboardEvent::F4;             break;
    case XK_F5:           key = SoKeyboardEvent::F5;             break;
    case XK_F6:           key = SoKeyboardEvent::F6;             break;
    case XK_F7:           key = SoKeyboardEvent::F7;             break;
    case XK_F8:           key = SoKeyboardEvent::F8;             break;
    case XK_F9:           key = SoKeyboardEvent::F9;             break;
    case XK_F10:          key = SoKeyboardEvent::F10;            break;
    case XK_F11:          key = SoKeyboardEvent::F11;            break;
    case XK_F12:          key = SoKeyboardEvent::F12;            break;
    case XK_BackSpace:    key = SoKeyboardEvent::BACKSPACE;      break;
    case XK_Tab:          key = SoKeyboardEvent::TAB;            break;
    case XK_Return:       key = SoKeyboardEvent::RETURN;         break;
    case XK_Linefeed:     key = SoKeyboardEvent::ENTER;          break;
    case XK_Pause:        key = SoKeyboardEvent::PAUSE;          break;
    case XK_Scroll_Lock:  key = SoKeyboardEvent::SCROLL_LOCK;    break;
    case XK_Escape:       key = SoKeyboardEvent::ESCAPE;         break;
#ifdef HAVE_SOKEYBOARDEVENT_DELETE
    case XK_Delete:       key = SoKeyboardEvent::DELETE;         break;
#else /* very strange */
    case XK_Delete:       key = SoKeyboardEvent::KEY_DELETE;     break;
#endif
    case XK_Print:        key = SoKeyboardEvent::PRINT;          break;
    case XK_Insert:       key = SoKeyboardEvent::INSERT;         break;
    case XK_Num_Lock:     key = SoKeyboardEvent::NUM_LOCK;       break;
    case XK_Caps_Lock:    key = SoKeyboardEvent::CAPS_LOCK;      break;
    case XK_Shift_Lock:   key = SoKeyboardEvent::SHIFT_LOCK;     break;
    case XK_space:        key = SoKeyboardEvent::SPACE;          break;
    case XK_apostrophe: case XK_quotedbl: key = SoKeyboardEvent::APOSTROPHE;     break;
    case XK_comma: case XK_less: key = SoKeyboardEvent::COMMA;          break;
    case XK_minus: case XK_underscore: key = SoKeyboardEvent::MINUS;          break;
    case XK_period: case XK_greater: key = SoKeyboardEvent::PERIOD;         break;
    case XK_slash: case XK_question: key = SoKeyboardEvent::SLASH;          break;
    case XK_semicolon: case XK_colon: key = SoKeyboardEvent::SEMICOLON;      break;
    case XK_equal: case XK_plus: key = SoKeyboardEvent::EQUAL;          break;
    case XK_bracketleft: case XK_braceleft: key = SoKeyboardEvent::BRACKETLEFT;    break;
    case XK_bracketright: case XK_braceright: key = SoKeyboardEvent::BRACKETRIGHT;   break;
    case XK_grave: case XK_asciitilde: key = SoKeyboardEvent::GRAVE;          break;

    default:
#if SOXT_DEBUG && 0
      SoDebugError::postWarning("SoXtKeyboard::makeKeyboardEvent",
                                "keysym 0x%04x isn't handled", keysym);
#endif // SOXT_DEBUG
      break;
    }
  }

  sokeybevent->setKey(key);

  // modifiers:
  sokeybevent->setShiftDown((event->state & ShiftMask) ? TRUE : FALSE);
  sokeybevent->setCtrlDown((event->state & ControlMask) ? TRUE : FALSE);
  sokeybevent->setAltDown((event->state & Mod1Mask) ? TRUE : FALSE);
}

// *************************************************************************

const SoEvent *
SoXtKeyboard::translateEvent(XAnyEvent * event)
{
  XKeyEvent * ke;

  // We don't check versus the current eventmask, as those flags has
  // been passed on to X11 -- so we shouldn't receive any other events
  // here than those requested.

  switch (event->type) {
  case KeyPress:
  case KeyRelease:
    ke = (XKeyEvent *)event;
    PRIVATE(this)->kbdevent->setState(event->type == KeyPress ?
                                      SoButtonEvent::DOWN : SoButtonEvent::UP);
    this->setEventPosition(PRIVATE(this)->kbdevent, ke->x, ke->y);
    makeKeyboardEvent(ke, PRIVATE(this)->kbdevent);
    break;
  default:
    return NULL;
  }
  return PRIVATE(this)->kbdevent;
}

// *************************************************************************

#undef PRIVATE
#undef PUBLIC

