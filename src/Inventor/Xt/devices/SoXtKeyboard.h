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

//  $Id$

#ifndef SOXT_KEYBOARD_H
#define SOXT_KEYBOARD_H

#include <Inventor/events/SoButtonEvent.h>

#include <Inventor/Xt/devices/SoXtDevice.h>

class SoKeyboardEvent;

// *************************************************************************

class SOXT_DLL_EXPORT SoXtKeyboard : public SoXtDevice {
  typedef SoXtDevice inherited;

public:
  enum KeyboardEvents {
    KEY_PRESS = KeyPressMask, // 0x01,
    KEY_RELEASE = KeyReleaseMask, // 0x02,
    ALL_EVENTS = KEY_PRESS | KEY_RELEASE
  };

  SoXtKeyboard( int events = SoXtKeyboard::ALL_EVENTS );
  virtual ~SoXtKeyboard(void);

  virtual void enable( Widget widget,
      XtEventHandler func, XtPointer data, Window window = (Window) NULL );
  virtual void disable( Widget widget,
      XtEventHandler func, XtPointer data );

  virtual const SoEvent * translateEvent( XAnyEvent * xevent );

private:
  int events;
  SoKeyboardEvent * keyboardEvent;

  SoKeyboardEvent * makeKeyboardEvent( XKeyEvent * event,
                                       SoButtonEvent::State state );

}; // class SoXtKeyboard

#define SO_XT_ALL_KEYBOARD_EVENTS SoXtKeyboard::ALL_EVENTS

// *************************************************************************

#endif // ! SOXT_KEYBOARD_H
