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

#ifndef SOXT_LINUXJOYSTICK_H
#define SOXT_LINUXJOYSTICK_H

#include <X11/Intrinsic.h>

#include <Inventor/SbBasic.h>

#include <Inventor/Xt/devices/SoXtDevice.h>

class SbPList;
class SoMotion3Event;
class SoSpaceballButtonEvent;

// *************************************************************************

class SOXT_DLL_API SoXtLinuxJoystick : public SoXtDevice {
  SOXT_OBJECT_HEADER(SoXtLinuxJoystick, SoXtDevice);

public:
  enum LinuxJoystickEvents {
    ALL_EVENTS = 0
  };

  SoXtLinuxJoystick(int events = SoXtLinuxJoystick::ALL_EVENTS);
  virtual ~SoXtLinuxJoystick(void);

  virtual void enable(Widget widget, SoXtEventHandler * handler,
                      XtPointer closure, Window window = (Window) NULL);
  virtual void disable(Widget widget, SoXtEventHandler * handler,
                       XtPointer closure);

  virtual const SoEvent * translateEvent(XAnyEvent * xevent);

  void setRotationScaleFactor(const float factor);
  float getRotationScaleFactor(void) const;
  void setTranslationScaleFactor(const float factor);
  float getTranslationScaleFactor(void) const;

  static SbBool exists(void);

  void setFocusToWindow(SbBool enable);
  SbBool isFocusToWindow(void) const;

  int getNumButtons(void) const;
  SbBool getButtonValue(const int button) const;

  int getNumAxes(void) const;
  float getAxisValue(const int axis) const;

private:
  class SoXtLinuxJoystickP * pimpl;
  friend class SoXtLinuxJoystickP;
};

#define SO_XT_ALL_LINUX_JOYSTICK_EVENTS SoXtLinuxJoystick::ALL_EVENTS

// *************************************************************************

#endif // ! SOXT_LINUXJOYSTICK_H
