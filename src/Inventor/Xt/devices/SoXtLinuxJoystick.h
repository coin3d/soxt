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

#include <X11/Intrinsic.h>

#include <Inventor/SbBasic.h>

#include <Inventor/Xt/devices/SoXtDevice.h>

class SbPList;
class SoMotion3Event;
class SoSpaceballButtonEvent;

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

  void setRotationScaleFactor( const float factor );
  float getRotationScaleFactor(void) const;
  void setTranslationScaleFactor( const float factor );
  float getTranslationScaleFactor(void) const;

  static SbBool exists(void);

  void setFocusToWindow( SbBool enable );
  SbBool isFocusToWindow(void) const;

  int getNumButtons(void) const;
  SbBool getButtonValue( const int button ) const;

  int getNumAxes(void) const;
  float getAxisValue( const int axis ) const;

private:
  static const char * getDevicePathName(void);

  void input( int * source, XtInputId * id );
  static void input_cb( XtPointer closure, int * source, XtInputId * id );

private:
  int events;
  int joydev;
  XtInputId joyid;

  char * name;
  int numaxes;
  int32_t * axisvalues;
  int numbuttons;
  SbBool * buttonvalues;

  float rotationScaleFactor;
  float translationScaleFactor;

  SoMotion3Event * motion3Event;
  SoSpaceballButtonEvent * buttonEvent;

  SoMotion3Event * makeMotion3Event( SoXt6dofDevicePressureEvent * event );
  SoSpaceballButtonEvent * makeButtonEvent( SoXt6dofDeviceButtonEvent * event,
      SoButtonEvent::State state );

  static SbBool enabled;
}; // class SoXtLinuxJoystick

#define SO_XT_ALL_LINUX_JOYSTICK_EVENTS SoXtLinuxJoystick::ALL_EVENTS

// *************************************************************************

#endif // ! SOXT_LINUXJOYSTICK_H
