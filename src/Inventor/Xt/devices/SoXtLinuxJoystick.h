/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
