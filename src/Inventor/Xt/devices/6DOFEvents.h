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

#ifndef SOXT_6DOFEVENTS_H
#define SOXT_6DOFEVENTS_H

/*
  Customized XEvent-like structures
*/

#define soxt6dofDeviceButtonPressedEvent   (LASTEvent + 1)
#define soxt6dofDeviceButtonReleasedEvent  (LASTEvent + 2)
#define soxt6dofDevicePressureEvent        (LASTEvent + 3)

typedef struct {
  int type;
  unsigned long serial;
  Bool send_event;
  Display * display;
  Window window;
  Time time;
  unsigned int state;
  unsigned int button;
  unsigned int buttons;
} SoXt6dofDeviceButtonEvent;
typedef SoXt6dofDeviceButtonEvent SoXt6dofDeviceButtonPressedEvent;
typedef SoXt6dofDeviceButtonEvent SoXt6dofDeviceButtonReleasedEvent;

typedef struct {
  int type;
  unsigned long serial;
  Bool send_event;
  Display * display;
  Window window;
  Time time;
  float trans_x, trans_y, trans_z;
  float rot_x, rot_y, rot_z;
} SoXt6dofDevicePressureEvent;

#endif // !SOXT_6DOFEVENTS_H
