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

#ifndef SOXT_LINUXJOYSTICK_H
#define SOXT_LINUXJOYSTICK_H

#include <Inventor/SbBasic.h>

#include <Inventor/Xt/devices/SoXtDevice.h>

// *************************************************************************

class SOXT_DLL_EXPORT SoXtLinuxJoystick : public SoXtDevice {
  typedef SoXtDevice inherited;

public:
  enum LinuxJoystickEvents {
    ALL_EVENTS = 0
  };

  SoXtLinuxJoystick( int events = SoXtLinuxJoystick::ALL_EVENTS );
  virtual ~SoXtLinuxJoystick(void);

  virtual void enable( Widget widget, XtEventHandler handler,
      XtPointer closure, Window window = (Window) NULL );
  virtual void disable( Widget widget, XtEventHandler handler,
      XtPointer closure );

  virtual const SoEvent * translateEvent( XAnyEvent * xevent );

  static SbBool exists(void);

  void setFocusToWindow( SbBool enable );
  SbBool isFocusToWindow(void) const;

private:
  void input( int * source, XtInputId * id );
  static void input_cb( XtPointer closure, int * source, XtInputId * id );

private:
  int events;
  int joydev;

}; // class SoXtLinuxJoystick

#define SO_XT_ALL_LINUX_JOYSTICK_EVENTS SoXtLinuxJoystick::ALL_EVENTS

// *************************************************************************

#endif // ! SOXT_LINUXJOYSTICK_H
