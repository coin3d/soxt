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

#ifndef SOXT_MOUSE_H
#define SOXT_MOUSE_H

#include <Inventor/events/SoButtonEvent.h>

#include <Inventor/Xt/devices/SoXtDevice.h>

class SoLocation2Event;
class SoMouseButtonEvent;

// *************************************************************************

class SoXtMouse : public SoXtDevice {
  typedef SoXtDevice inherited;

public:
  enum MouseEvents {
    BUTTON_PRESS = ButtonPressMask, // 0x01
    BUTTON_RELEASE = ButtonReleaseMask, // 0x02
    POINTER_MOTION = PointerMotionMask, // 0x04
    BUTTON_MOTION = ButtonMotionMask, // 0x08
    ALL_EVENTS = BUTTON_PRESS | BUTTON_RELEASE | POINTER_MOTION | BUTTON_MOTION
  };

  SoXtMouse( int events = SoXtMouse::ALL_EVENTS );
  virtual ~SoXtMouse(void);

  virtual void enable( Widget widget,
      XtEventHandler func, XtPointer data, Window window = (Window) NULL );
  virtual void disable( Widget widget,
      XtEventHandler func, XtPointer data );

  virtual const SoEvent * translateEvent( XAnyEvent * xevent );

private:
  int events;

  SoLocation2Event * locationEvent;
  SoMouseButtonEvent * buttonEvent;

  SoLocation2Event * makeLocationEvent( XMotionEvent * event );
  SoMouseButtonEvent * makeButtonEvent( XButtonEvent * event,
                                        SoButtonEvent::State state );

}; // class SoXtMouse

#define SO_XT_ALL_MOUSE_EVENTS SoXtMouse::ALL_EVENTS

// *************************************************************************

#endif // ! SOXT_MOUSE_H
