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

#if SOXT_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOXT_DEBUG

#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#include <X11/Intrinsic.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SbPList.h>
#include <Inventor/events/SoSpaceballButtonEvent.h>
#include <Inventor/events/SoMotion3Event.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXt.h>

#include <Inventor/Xt/devices/SoXtLinuxJoystick.h>
#include <Inventor/Xt/SoAny.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef HAVE_JOYSTICK_LINUX
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#endif // HAVE_JOYSTICK_LINUX

#ifndef M_PI
#define M_PI 3.141592653579
#endif

// *************************************************************************

/*!
  \class SoXtLinuxJoystick Inventor/Xt/devices/SoXtLinuxJoystick.h
  \brief The SoXtLinuxJoystick class is for enabling use of joysticks with
  SoXt through the Linux Joystick driver.
  \ingroup devices

  This class is an SoXt-specific extension.  It is not included with
  Open Inventor.
*/

// *************************************************************************

SOXT_OBJECT_SOURCE(SoXtLinuxJoystick);

// *************************************************************************

SbBool SoXtLinuxJoystick::enabled = FALSE;

/*!
  \enum SoXtLinuxJoystick::LinuxJoystickEvents

  Enumeration over supported event types.
*/

// *************************************************************************

/*!
  Public constructor.
*/

SoXtLinuxJoystick::SoXtLinuxJoystick(
  int events)
{
  this->events = events;
  this->joydev = 0;
  this->joyid = (XtInputId) 0;
  this->numaxes = 0;
  this->axisvalues = NULL;
  this->numbuttons = 0;
  this->buttonvalues = NULL;
  this->name = NULL;
  this->rotationScaleFactor = M_PI / float(3*0x10000);
  this->translationScaleFactor = M_PI / float(0x10000);
  this->motion3Event = NULL;
  this->buttonEvent = NULL;
} // SoXtLinuxJoystick()

/*!
  Destructor.
*/

SoXtLinuxJoystick::~SoXtLinuxJoystick(// virtual
  void)
{
  if (this->joydev != 0)
    close(this->joydev);

  delete [] this->name;
  delete [] this->axisvalues;
  delete [] this->buttonvalues;

  delete this->motion3Event;
  delete this->buttonEvent;
} // ~SoXtLinuxJoystick()

// *************************************************************************

/*!
  This method enables the Linux Joystick driver, and sets up \a handler
  to handle the Linux Joystick events.
*/

void
SoXtLinuxJoystick::enable(// virtual
  Widget widget,
  XtEventHandler handler,
  XtPointer closure,
  Window window)
{
#ifdef HAVE_JOYSTICK_LINUX
  if (! SoXtLinuxJoystick::enabled) {
    const char * devpathname = SoXtLinuxJoystick::getDevicePathName();
    this->joydev = open(devpathname, O_RDONLY | O_NONBLOCK);
    if (joydev <= 0) {
      SoDebugError::post("SoXtLinuxJoystick::enable",
        "failed to open device '%s'...", devpathname);
      this->joydev = 0;
      return;
    }
    char char_return;
    int int_return;

    if (ioctl(this->joydev, JSIOCGAXES, &char_return) >= 0) {
      this->numaxes = char_return;
    } else {
      SoDebugError::post("SoXtLinuxJoystick::enable",
        "ioctl(JSIOCGAXES) failed");
    }

    if (ioctl(this->joydev, JSIOCGBUTTONS, &char_return) >= 0) {
      this->numbuttons = char_return;
    } else {
      SoDebugError::post("SoXtLinuxJoystick::enable",
        "ioctl(JSIOCGBUTTONS) failed");
    }

    char name[128];
    if (ioctl(this->joydev, JSIOCGNAME(sizeof(name)), name) >= 0) {
      this->name = strcpy(new char [strlen(name)+1], name);
    } else {
      SoDebugError::post("SoXtLinuxJoystick::enable",
        "ioctl(JSIOCGNAME) failed");
      this->name = strcpy(new char [sizeof("Unknown")+1], "Unknown");
    }

#if SOXT_DEBUG && 0
    SoDebugError::post("SoXtLinuxJoystick::enable",
      "successfully opened \"%s\" device with %d axes and %d buttons",
      this->name, this->numaxes, this->numbuttons);
#endif // SOXT_DEBUG

    int i;
    this->axisvalues = new int32_t [this->numaxes];
    for (i = 0; i < this->numaxes; i++)
      this->axisvalues[i] = 0;
    
    this->buttonvalues = new int32_t [this->numbuttons];
    for (i = 0; i < this->numbuttons; i++)
      this->buttonvalues[i] = FALSE;

    XtAppAddInput(SoXt::getAppContext(), this->joydev,
      (XtPointer) XtInputReadMask,
      SoXtLinuxJoystick::input_cb, (XtPointer) this);
  }

  this->addEventHandler(widget, handler, closure, window);
#endif // HAVE_JOYSTICK_LINUX
} // enable()

/*!
  This method removes \a handler from the list of device handlers.
*/

void
SoXtLinuxJoystick::disable(// virtual
  Widget widget,
  XtEventHandler handler,
  XtPointer closure)
{
#ifdef HAVE_JOYSTICK_LINUX
  this->removeEventHandler(widget, handler, closure);
#endif // HAVE_JOYSTICK_LINUX
} // disable()

// *************************************************************************

/*!
  This method translates X events into Open Inventor events suitable for
  propagating into the scene graph.

  NULL is returned if the event can't
  be translated (wrong event type for this device).
*/

const SoEvent *
SoXtLinuxJoystick::translateEvent(
  XAnyEvent * xevent)
{
#ifdef HAVE_JOYSTICK_LINUX
  switch (xevent->type) {
  case soxt6dofDeviceButtonPressedEvent:
    return this->makeButtonEvent((SoXt6dofDeviceButtonEvent *) xevent,
      SoButtonEvent::DOWN);
  case soxt6dofDeviceButtonReleasedEvent:
    return this->makeButtonEvent((SoXt6dofDeviceButtonEvent *) xevent,
      SoButtonEvent::UP);
  case soxt6dofDevicePressureEvent:
    return this->makeMotion3Event((SoXt6dofDevicePressureEvent *) xevent);
  default:
    break;
  }
#endif // HAVE_JOYSTICK_LINUX
  return (SoEvent *) NULL;
} // translateEvent()

// *************************************************************************

/*!
  This method sets the scale factor for the Linux Joystick driver rotation
  vectors.
*/

void
SoXtLinuxJoystick::setRotationScaleFactor(
  const float factor)
{
  this->rotationScaleFactor = factor;
} // setRotationScaleFactor()

/*!
  This method returns the scale factor used on the Linux Joystick driver
  rotation vectors.
*/

float
SoXtLinuxJoystick::getRotationScaleFactor(
  void) const
{
  return this->rotationScaleFactor;
} // getRotationScaleFactor()

/*!
  This method sets the scale factor used on the Linux Joystick driver
  translation vectors.
*/

void
SoXtLinuxJoystick::setTranslationScaleFactor(
  const float factor)
{
  this->translationScaleFactor = factor;
} // setTranslationScaleFactor()

/*!
  This method returns the scale factor used on the Linux Joystick driver
  translation vectors.
*/

float
SoXtLinuxJoystick::getTranslationScaleFactor(
  void) const
{
  return this->translationScaleFactor;
} // getTranslationScaleFactor()

// *************************************************************************

/*!
  This function returns wether there is a Linux Joystick device available
  on the application server.
*/

SbBool
SoXtLinuxJoystick::exists(// static
  void)
{
#ifdef HAVE_JOYSTICK_LINUX
  if (SoXtLinuxJoystick::enabled)
    return TRUE;
  const char * jsdevicepath = SoXtLinuxJoystick::getDevicePathName();
  int joydev = open(jsdevicepath, O_RDONLY);
  if (joydev <= 0)
    return FALSE;
  close(joydev);
  return TRUE;
#else // ! HAVE_JOYSTICK_LINUX
  return FALSE;
#endif // ! HAVE_JOYSTICK_LINUX
} // exists()

// *************************************************************************

/*!
  Return the path name of the device file to try to connect with.
*/

const char *
SoXtLinuxJoystick::getDevicePathName(// static, private
  void)
{
  const char * devicepath = SoAny::si()->getenv("SOXT_JOYSTICK_DEVICE");
#ifdef SOXT_JOYSTICK_LINUX_DEVICE
  static const char configured[] = SOXT_JOYSTICK_LINUX_DEVICE;
  if (devicepath == NULL)
    devicepath = configured;
#endif // SOXT_JOYSTICK_LINUX_DEVICE
  static const char hardcoded[] = "/dev/js0";
  if (devicepath == NULL)
    devicepath = hardcoded;
  return devicepath;
} // getDevicePathName()

// *************************************************************************

/*!
  Returns the number of buttons on the device attached to the Linux Joystick
  driver.
*/

int
SoXtLinuxJoystick::getNumButtons(
  void) const
{
  return this->numbuttons;
} // getNumButtons()

/*!
  Returns whether the given button is pressed or not.
*/

SbBool
SoXtLinuxJoystick::getButtonValue(
  const int button) const
{
  if (button < 0 || button >= this->numbuttons) {
#if SOXT_DEBUG
    SoDebugError::post("SoXtLinuxJoystick::getButtonValue",
      "invalid button %d", button);
#endif // SOXT_DEBUG
    return FALSE;
  }
  return this->buttonvalues[button];
} // getButtonValue()

/*!
  Returns the number of axes on the input device.
*/

int
SoXtLinuxJoystick::getNumAxes(
  void) const
{
  return this->numaxes;
} // getNumAxes()

/*!
  Returns the current value of the given axis on the input device.
*/

float
SoXtLinuxJoystick::getAxisValue(
  const int axis) const
{
  if (axis < 0 || axis >= this->numaxes) {
#if SOXT_DEBUG
    SoDebugError::post("SoXtLinuxJoystick::getButtonValue",
      "invalid axis %d", axis);
#endif // SOXT_DEBUG
    return 0.0f;
  }
  return float(this->axisvalues[axis]) * this->translationScaleFactor;
} // getAxisValue()

// *************************************************************************

/*!
  This method is not implemented.
*/

void
SoXtLinuxJoystick::setFocusToWindow(
  SbBool enable)
{
  SOXT_STUB();
} // setFocusToWindow()

/*!
  This method is not implemented.
*/

SbBool
SoXtLinuxJoystick::isFocusToWindow(
  void) const
{
  SOXT_STUB();
  return FALSE;
} // isFocusToWindow()

// *************************************************************************

/*!
  This method is invoked when the joystick is enabled and there are joystick
  events in the queue.

  See linux/Documentation/joystick-api.txt for programming information.
*/

void
SoXtLinuxJoystick::input(// private
  int * source,
  XtInputId * id)
{
#ifdef HAVE_JOYSTICK_LINUX
  struct js_event event;

  int button = -1;
  SbBool motion = FALSE;

  int bytes = read(this->joydev, &event, sizeof(struct js_event));
  while (bytes == sizeof(struct js_event)) {
    if ((event.type & JS_EVENT_INIT) != 0) {
      if ((event.type & JS_EVENT_BUTTON) != 0) {
        assert(event.number < this->numbuttons);
        this->buttonvalues[event.number] = event.value ? TRUE : FALSE;
      } else if ((event.type & JS_EVENT_AXIS) != 0) {
        assert(event.number < this->numaxes);
        long value = event.value;
        SbBool negative = FALSE;
        if (value < 0) negative = TRUE;
        value = (value * value) / 0x8000;
        if (negative) value = 0 - value; 
        assert(value >= -32768 && value <= 32767);
        this->axisvalues[event.number] = value;
      } else {
        SoDebugError::post("SoXtLinuxJoystick::input",
          "event [initial] not supported");
      }
      bytes = read(this->joydev, &event, sizeof(struct js_event));
      continue;
    }

    if ((event.type & JS_EVENT_BUTTON) != 0) {
      assert(event.number < this->numbuttons);
      this->buttonvalues[event.number] = event.value ? TRUE : FALSE;
      button = event.number;
    } else if ((event.type & JS_EVENT_AXIS) != 0) {
      assert(event.number < this->numaxes);
      long value = event.value;
      SbBool negative = FALSE;
      if (value < 0) negative = TRUE;
      value = (value * value) / 0x8000;
      if (negative) value = 0 - value; 
      assert(value >= -32768 && value <= 32767);
      this->axisvalues[event.number] = value;
      motion = TRUE;
    }

    bytes = read(this->joydev, &event, sizeof(struct js_event));
  }

  if (button != -1) {
    SoXt6dofDeviceButtonEvent xevent;
    if (event.value == 0)
      xevent.type = soxt6dofDeviceButtonReleasedEvent;
    else
      xevent.type = soxt6dofDeviceButtonPressedEvent;
    xevent.serial = 0;
    xevent.send_event = False;
    xevent.display = SoXt::getDisplay();
    xevent.window = (Window) NULL;
    xevent.time = event.time;
    xevent.state = 0;
    for (int i = 0; i < this->numbuttons; i++) {
      if (this->buttonvalues[i] != FALSE)
        xevent.state |= (1 << i);
    }
    xevent.button = button;
    xevent.buttons = this->numbuttons;

    this->invokeHandlers((XEvent *) &xevent);
  }

  if (motion != FALSE) {
    SoXt6dofDevicePressureEvent xevent;
    xevent.type = soxt6dofDevicePressureEvent;
    xevent.serial = 0;
    xevent.send_event = False;
    xevent.display = SoXt::getDisplay();
    xevent.window = (Window) NULL;
    xevent.time = event.time;

    xevent.trans_x = 0.0f;
    xevent.trans_y = 0.0f;
    xevent.trans_z = 0.0f;
    xevent.rot_x = 0.0f;
    xevent.rot_y = 0.0f;
    xevent.rot_z = 0.0f;

    do {
      if (this->numaxes < 1) break;
      xevent.trans_x =
        this->translationScaleFactor * float(this->axisvalues[0]);
      if (this->numaxes < 2) break;
      xevent.trans_y =
        this->translationScaleFactor * float(this->axisvalues[1]);
      if (this->numaxes < 3) break;
      xevent.trans_z =
        this->translationScaleFactor * float(this->axisvalues[2]);
      if (this->numaxes < 4) break;
      xevent.rot_x =
        this->rotationScaleFactor * float(this->axisvalues[3]);
      if (this->numaxes < 5) break;
      xevent.rot_y =
        this->rotationScaleFactor * float(this->axisvalues[4]);
      if (this->numaxes < 6) break;
      xevent.rot_z =
        this->rotationScaleFactor * float(this->axisvalues[5]);
    } while (FALSE);

    this->invokeHandlers((XEvent *) &xevent);
  }
#endif // HAVE_JOYSTICK_LINUX
} // input()

/*!
  This callback just jumps to the input handler in the device object.
*/

void
SoXtLinuxJoystick::input_cb(// static, private
  XtPointer closure,
  int * source,
  XtInputId * id)
{
  assert(closure != NULL);
  SoXtLinuxJoystick * device = (SoXtLinuxJoystick *) closure;
  device->input(source, id);
} // input_cb()

// *************************************************************************

/*!
  This method translates between custom input device pressure events
  and Open Inventor SoMotion3Event events.
*/

SoMotion3Event *
SoXtLinuxJoystick::makeMotion3Event(// private
  SoXt6dofDevicePressureEvent * event)
{
  if (this->motion3Event == NULL)
    this->motion3Event = new SoMotion3Event;

  this->motion3Event->setTranslation(
    SbVec3f(event->trans_x,
             event->trans_y,
             0 - event->trans_z));

  SbRotation xrot(SbVec3f(1, 0, 0), event->rot_x);
  SbRotation yrot(SbVec3f(0, 1, 0), event->rot_y);
  SbRotation zrot(SbVec3f(0, 0, 1), 0 - event->rot_z);

  this->motion3Event->setRotation(xrot * yrot * zrot);

  return this->motion3Event;
} // makeMotion3Event()

/*!
  This method translates between custom device button events and
  Open Inventor SoSpaceballButtonEvent events.
*/

SoSpaceballButtonEvent *
SoXtLinuxJoystick::makeButtonEvent(
  SoXt6dofDeviceButtonEvent * event,
  SoButtonEvent::State state)
{
  if (this->buttonEvent == NULL)
    this->buttonEvent = new SoSpaceballButtonEvent;

  this->buttonEvent->setButton(SoSpaceballButtonEvent::ANY);

  switch (event->button) {
  case 0:
    this->buttonEvent->setButton(SoSpaceballButtonEvent::BUTTON1);
    break;
  case 1:
    this->buttonEvent->setButton(SoSpaceballButtonEvent::BUTTON2);
    break;
  case 2:
    this->buttonEvent->setButton(SoSpaceballButtonEvent::BUTTON3);
    break;
  case 3:
    this->buttonEvent->setButton(SoSpaceballButtonEvent::BUTTON4);
    break;
  case 4:
    this->buttonEvent->setButton(SoSpaceballButtonEvent::BUTTON5);
    break;
  case 5:
    this->buttonEvent->setButton(SoSpaceballButtonEvent::BUTTON6);
    break;
  case 6:
    this->buttonEvent->setButton(SoSpaceballButtonEvent::BUTTON7);
    break;
  case 7:
    this->buttonEvent->setButton(SoSpaceballButtonEvent::BUTTON8);
    break;
  // FIXME: which one should it be?
  case 8: this->buttonEvent->setButton(SoSpaceballButtonEvent::PICK); break;
  default: break;
  } // switch (event->button)

  this->buttonEvent->setState(state);

  return this->buttonEvent;
} // makeButtonEvent()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtLinuxJoystickRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

