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
                      XtPointer closure);
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
