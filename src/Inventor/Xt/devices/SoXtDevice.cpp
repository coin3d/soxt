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

#if SOXT_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOXT_DEBUG

#include <assert.h>

#if SOXT_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // SOXT_DEBUG

#include <Inventor/SbPList.h>
#include <Inventor/misc/SoBasic.h>
#include <Inventor/events/SoEvent.h>

#include <soxtdefs.h>

#include <Inventor/Xt/devices/SoXtDevice.h>

/*!
  \class SoXtDevice Inventor/Xt/SoXtDevice.h
  \brief The SoXtDevice class system is for gluing GUI events to Inventor
  Scene Graph events.
*/

// *************************************************************************

struct SoXtDeviceHandlerInfo {
  Widget widget;
  XtEventHandler handler;
  XtPointer closure;
  Window window;
};

// *************************************************************************

/*!
*/

SoXtDevice::SoXtDevice(
  void )
: size( 0, 0 )
, handlers( NULL )
{
} // SoXtDevice()

/*!
*/

SoXtDevice::~SoXtDevice( // virtual
  void )
{
  if ( this->handlers ) {
    for ( int i = 0; i < this->handlers->getLength(); i++ ) {
      SoXtDeviceHandlerInfo * info =
        (SoXtDeviceHandlerInfo *) (*this->handlers)[i];
      delete info;
    }
    delete this->handlers;
  }
} // ~SoXtDevice()

// *************************************************************************

/*!
  \fn void SoXtDevice::enable( Widget widget, XtEventHandler handler, XtPointer closure, Window window = (Window) NULL ) = 0

  This method will enable the device for the widget.

  \a handler is invoked with the \a closure argument when an event occur
  in \a widget.  The \a window argument is needed because GLX widgets can
  have more than one window (normal, overlay).
*/

/*!
  \fn void SoXtDevice::disable( Widget widget, XtEventHandler handler, XtPointer closure ) = 0

  This method will disable the handler for the device.
*/

// *************************************************************************

/*!
*/

void
SoXtDevice::setWindowSize(
  const SbVec2s size )
{
  this->size = size;
} // setWindowSize()

/*!
*/

const SbVec2s
SoXtDevice::getWindowSize(
  void ) const
{
  return this->size;
} // getWindowSize()

// *************************************************************************

/*!
*/

void
SoXtDevice::setEventPosition(
  SoEvent * event,
  int x,
  int y ) const
{
  assert( event != NULL );
  SbVec2s position( x, this->size[1] - y - 1 );
  event->setPosition( position );
#if 0 // SOXT_DEBUG
  SoDebugError::postInfo( "SoXtDevice::setEventPosition",
    "position = (%d, %d)", position[0], position[1] );
#endif // 0 was SOXT_DEBUG
} // setEventPosition()

// *************************************************************************

/*!
*/

void
SoXtDevice::addEventHandler(
  Widget widget,
  XtEventHandler handler,
  XtPointer closure,
  Window window )
{
  if ( this->handlers == NULL )
    this->handlers = new SbPList;
  SoXtDeviceHandlerInfo * info = new SoXtDeviceHandlerInfo;
  info->widget = widget;
  info->handler = handler;
  info->closure = closure;
  info->window = window;
  this->handlers->append( info );
} // addEventHandler()

/*!
*/

void
SoXtDevice::removeEventHandler(
  Widget widget,
  XtEventHandler handler,
  XtPointer closure )
{
  if ( this->handlers ) {
    for ( int i = 0; i < this->handlers->getLength(); i++ ) {
      SoXtDeviceHandlerInfo * info =
        (SoXtDeviceHandlerInfo *) (*this->handlers)[i];
      if ( (info->widget == widget) && (info->handler == handler) &&
           (info->closure == closure) ) {
        delete info;
        this->handlers->remove(i);
        return;
      }
    }
  }
#if SOXT_DEBUG
  SoDebugError::post( "SoXtDevice::removeEventHandler",
    "tried to remove nonexisting handler" );
#endif // SOXT_DEBUG
} // removeEventHandler()

/*!
*/

void
SoXtDevice::invokeHandlers(
  XEvent * const event )
{
  if ( this->handlers ) {
    Boolean dispatch = False;
    for ( int i = 0; i < this->handlers->getLength(); i++ ) {
      SoXtDeviceHandlerInfo * info =
        (SoXtDeviceHandlerInfo *) (*this->handlers)[i];
      info->handler( info->widget, info->closure, event, &dispatch );
    }
  }
} // invokeEventHandlers()

// *************************************************************************

#if SOXT_DEBUG
const char * getSoXtDeviceRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

