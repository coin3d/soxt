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
#include <Inventor/errors/SoDebugError.h>
#endif // SOXT_DEBUG

#include <Inventor/SbPList.h>
#include <Inventor/misc/SoBasic.h>
#include <Inventor/events/SoEvent.h>

#include <soxtdefs.h>

#include <Inventor/Xt/devices/SoXtDevice.h>
#include <Inventor/Xt/devices/SoXtInputFocus.h>
#include <Inventor/Xt/devices/SoXtKeyboard.h>
#include <Inventor/Xt/devices/SoXtMouse.h>
#include <Inventor/Xt/devices/SoXtSpaceball.h>
#include <Inventor/Xt/devices/SoXtLinuxJoystick.h>

// *************************************************************************

/*!
  \class SoXtDevice Inventor/Xt/SoXtDevice.h
  \brief The SoXtDevice class is the base class for the SoXt device classes.
  \ingroup devices

  The SoXt device classes provide glue functionality for translating GUI
  Xt/Motif events to Inventor Scene Graph SoEvent events.
*/

// *************************************************************************

SOXT_OBJECT_ABSTRACT_SOURCE(SoXtDevice);

/*!
  \internal
  This function initializes the type system for all the device classes.
  It is called indirectly when calling SoXt::init().
*/

void
SoXtDevice::initClasses(void)
{
  SoXtDevice::initClass();
  SoXtInputFocus::initClass();
  SoXtKeyboard::initClass();
  SoXtMouse::initClass();
  SoXtSpaceball::initClass();
  SoXtLinuxJoystick::initClass();
}

// *************************************************************************

struct SoXtDeviceHandlerInfo {
  Widget widget;
  SoXtEventHandler * handler;
  XtPointer closure;
  Window window;
};

// *************************************************************************

/*!
  Constructor.  Protected to only enable invocation from derived device
  classes.
*/

SoXtDevice::SoXtDevice(void)
  : size(0, 0), handlers(NULL)
{
}

/*!
  Public virtual destructor.
*/

SoXtDevice::~SoXtDevice()
{
  if (this->handlers) {
    for (int i = 0; i < this->handlers->getLength(); i++) {
      SoXtDeviceHandlerInfo * info =
        (SoXtDeviceHandlerInfo *) (*this->handlers)[i];
      delete info;
    }
    delete this->handlers;
  }
}

// *************************************************************************

/*!
  \fn void SoXtDevice::enable(Widget widget, SoXtEventHandler * handler, XtPointer closure, Window window = (Window) NULL) = 0

  This method will enable the device for the widget.

  \a handler is invoked with the \a closure argument when an event occur
  in \a widget.  The \a window argument is needed because GLX widgets can
  have more than one window (normal, overlay).
*/

/*!
  \fn void SoXtDevice::disable(Widget widget, SoXtEventHandler * handler, XtPointer closure) = 0

  This method will disable the handler for the device.
*/

/*!
  \fn SoEvent * SoXtDevice::translateEvent(XAnyEvent * event) = 0

  This method translates from X events to Open Inventor SoEvent events.
*/

// *************************************************************************

/*!
  This method sets the cached size of the window the device is "attached"
  to.
*/

void
SoXtDevice::setWindowSize(const SbVec2s size)
{
  this->size = size;
}

/*!
  This method returns the cached window size.
*/

const SbVec2s
SoXtDevice::getWindowSize(void) const
{
  return this->size;
}

// *************************************************************************

/*!
  This method fills in the position information of \a event, translating the
  coordinates into the correct coordinate system.
*/

void
SoXtDevice::setEventPosition(SoEvent * event,
                             int x,
                             int y) const
{
  assert(event != NULL);
  SbVec2s position(x, this->size[1] - y - 1);
  event->setPosition(position);
#if 0 // SOXT_DEBUG
  SoDebugError::postInfo("SoXtDevice::setEventPosition",
    "position = (%d, %d)", position[0], position[1]);
#endif // 0 was SOXT_DEBUG
}

// *************************************************************************

/*!
  This method adds an event handler.
*/

void
SoXtDevice::addEventHandler(Widget widget,
                            SoXtEventHandler * handler,
                            XtPointer closure,
                            Window window)
{
  if (this->handlers == NULL)
    this->handlers = new SbPList;
  SoXtDeviceHandlerInfo * info = new SoXtDeviceHandlerInfo;
  info->widget = widget;
  info->handler = handler;
  info->closure = closure;
  info->window = window;
  this->handlers->append(info);
}

/*!
  This method removes an event handler.
*/

void
SoXtDevice::removeEventHandler(Widget widget,
                               SoXtEventHandler * handler,
                               XtPointer closure)
{
  if (this->handlers) {
    for (int i = 0; i < this->handlers->getLength(); i++) {
      SoXtDeviceHandlerInfo * info =
        (SoXtDeviceHandlerInfo *) (*this->handlers)[i];
      if ((info->widget == widget) && (info->handler == handler) &&
           (info->closure == closure)) {
        delete info;
        this->handlers->remove(i);
        return;
      }
    }
  }
#if SOXT_DEBUG
  SoDebugError::post("SoXtDevice::removeEventHandler",
                     "tried to remove nonexisting handler");
#endif // SOXT_DEBUG
}

/*!
  This method invokes all the event handlers.
*/

void
SoXtDevice::invokeHandlers(XEvent * const event)
{
  if (this->handlers) {
    Boolean dispatch = False;
    for (int i = 0; i < this->handlers->getLength(); i++) {
      SoXtDeviceHandlerInfo * info =
        (SoXtDeviceHandlerInfo *) (*this->handlers)[i];
      info->handler(info->widget, info->closure, event, &dispatch);
    }
  }
}

// *************************************************************************
