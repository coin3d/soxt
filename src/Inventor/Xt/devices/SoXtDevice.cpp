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

struct SoXtDeviceHandlerInfo {
  Widget widget;
  SoXtEventHandler * handler;
  XtPointer closure;
};

// *************************************************************************

class SoXtDeviceP {
public:
  SoXtDeviceP(void);
  ~SoXtDeviceP();

  SbVec2s size;
  SbPList * handlers;
};

SoXtDeviceP::SoXtDeviceP(void)
{
  this->size = SbVec2s(0, 0);
  this->handlers = NULL;
}

SoXtDeviceP::~SoXtDeviceP()
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

#define PRIVATE(p) (p->pimpl)

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

/*!
  Constructor.  Protected to only enable invocation from derived device
  classes.
*/

SoXtDevice::SoXtDevice(void)
{
  PRIVATE(this) = new SoXtDeviceP;
}

/*!
  Public virtual destructor.
*/

SoXtDevice::~SoXtDevice()
{
  delete PRIVATE(this);
}

// *************************************************************************

/*!
  \fn void SoXtDevice::enable(Widget widget, SoXtEventHandler * handler, XtPointer closure) = 0

  This method will enable the device for the widget.

  \a handler is invoked with the \a closure argument when an event occur
  in \a widget.
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
  PRIVATE(this)->size = size;
}

/*!
  This method returns the cached window size.
*/

SbVec2s
SoXtDevice::getWindowSize(void) const
{
  return PRIVATE(this)->size;
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
  SbVec2s position(x, PRIVATE(this)->size[1] - y - 1);
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
                            XtPointer closure)
{
  if (PRIVATE(this)->handlers == NULL)
    PRIVATE(this)->handlers = new SbPList;
  SoXtDeviceHandlerInfo * info = new SoXtDeviceHandlerInfo;
  info->widget = widget;
  info->handler = handler;
  info->closure = closure;
  PRIVATE(this)->handlers->append(info);
}

/*!
  This method removes an event handler.
*/

void
SoXtDevice::removeEventHandler(Widget widget,
                               SoXtEventHandler * handler,
                               XtPointer closure)
{
  if (PRIVATE(this)->handlers) {
    for (int i = 0; i < PRIVATE(this)->handlers->getLength(); i++) {
      SoXtDeviceHandlerInfo * info =
        (SoXtDeviceHandlerInfo *) (*PRIVATE(this)->handlers)[i];
      if ((info->widget == widget) && (info->handler == handler) &&
           (info->closure == closure)) {
        delete info;
        PRIVATE(this)->handlers->remove(i);
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
SoXtDevice::invokeHandlers(XAnyEvent * event)
{
  if (PRIVATE(this)->handlers) {
    Boolean dispatch = False;
    for (int i = 0; i < PRIVATE(this)->handlers->getLength(); i++) {
      SoXtDeviceHandlerInfo * info =
        (SoXtDeviceHandlerInfo *) (*PRIVATE(this)->handlers)[i];
      info->handler(info->widget, info->closure, (XEvent *)event, &dispatch);
    }
  }
}

// *************************************************************************

SbVec2s
SoXtDevice::getLastEventPosition(void)
{
  SOXT_STUB();
  return SbVec2s(0, 0);
}

// *************************************************************************
