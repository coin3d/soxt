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

// $Id$

#ifndef SOXT_KEYBOARD_H
#define SOXT_KEYBOARD_H

#include <Inventor/events/SoButtonEvent.h>

#include <Inventor/Xt/devices/SoXtDevice.h>

class SoKeyboardEvent;

// *************************************************************************

class SOXT_DLL_API SoXtKeyboard : public SoXtDevice {
  SOXT_OBJECT_HEADER(SoXtKeyboard, SoXtDevice);

public:
  enum KeyboardEvents {
    KEY_PRESS = KeyPressMask, // 0x01,
    KEY_RELEASE = KeyReleaseMask, // 0x02,
    ALL_EVENTS = KEY_PRESS | KEY_RELEASE
  };

  SoXtKeyboard( int events = SoXtKeyboard::ALL_EVENTS );
  virtual ~SoXtKeyboard(void);

  virtual void enable( Widget widget, XtEventHandler handler,
      XtPointer closure, Window window = (Window) NULL );
  virtual void disable( Widget widget, XtEventHandler handler,
      XtPointer closure );

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
