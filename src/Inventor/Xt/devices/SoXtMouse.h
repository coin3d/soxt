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

#ifndef SOXT_MOUSE_H
#define SOXT_MOUSE_H

#include <Inventor/events/SoButtonEvent.h>

#include <Inventor/Xt/devices/SoXtDevice.h>

class SoLocation2Event;
class SoMouseButtonEvent;

// *************************************************************************

class SOXT_DLL_API SoXtMouse : public SoXtDevice {
  SOXT_OBJECT_HEADER(SoXtMouse, SoXtDevice);

public:
  enum MouseEvents {
    BUTTON_PRESS = ButtonPressMask, // 0x01
    BUTTON_RELEASE = ButtonReleaseMask, // 0x02
    POINTER_MOTION = PointerMotionMask, // 0x04
    BUTTON_MOTION = ButtonMotionMask, // 0x08
    ALL_EVENTS = BUTTON_PRESS | BUTTON_RELEASE | POINTER_MOTION | BUTTON_MOTION
  };

  SoXtMouse(int events = SoXtMouse::ALL_EVENTS);
  virtual ~SoXtMouse(void);

  virtual void enable(Widget widget, XtEventHandler handler,
      XtPointer closure, Window window = (Window) NULL);
  virtual void disable(Widget widget, XtEventHandler handler,
      XtPointer closure);

  virtual const SoEvent * translateEvent(XAnyEvent * xevent);

private:
  int events;

  SoLocation2Event * locationEvent;
  SoMouseButtonEvent * buttonEvent;

  SoLocation2Event * makeLocationEvent(XMotionEvent * event);
  SoMouseButtonEvent * makeButtonEvent(XButtonEvent * event,
      SoButtonEvent::State state);

}; // class SoXtMouse

#define SO_XT_ALL_MOUSE_EVENTS SoXtMouse::ALL_EVENTS

// *************************************************************************

#endif // ! SOXT_MOUSE_H
