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
#include <unistd.h>
#include <fcntl.h>
#include <linux/joystick.h>

#include <X11/Intrinsic.h>

#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>

#include <Inventor/Xt/devices/SoXtLinuxJoystick.h>

/*!
  \class SoXtLinuxJoystick Inventor/Xt/devices/SoXtLinuxJoystick.h
  \brief The SoXtLinuxJoystick class is for enabling use of joysticks with
  SoXt through the Linux Joystick driver.

  This class is not included with Open Inventor, but is an SoXt-specific
  extension.
*/

// *************************************************************************

/*!
*/

SoXtLinuxJoystick::SoXtLinuxJoystick(
  int events )
{
  this->events = events;
  this->joydev = 0;
  SoDebugError::postInfo( "SoXtLinuxJoystick::SoXtLinuxJoystick",
    "attached, but not implemented" );
} // SoXtLinuxJoystick()

/*!
*/

SoXtLinuxJoystick::~SoXtLinuxJoystick( // virtual
  void )
{
} // ~SoXtLinuxJoystick()

// *************************************************************************

/*!
*/

void
SoXtLinuxJoystick::enable( // virtual
  Widget widget,
  XtEventHandler handler,
  XtPointer closure,
  Window window )
{
  SOXT_STUB();
} // enable()

/*!
*/

void
SoXtLinuxJoystick::disable( // virtual
  Widget widget,
  XtEventHandler handler,
  XtPointer closure )
{
  SOXT_STUB();
} // disable()

// *************************************************************************

/*!
  This method translates X events into Open Inventor events suitable for
  propagating into the scene graph.  NULL is returned if the event can't
  be translated (wrong event type for this device).
*/

const SoEvent *
SoXtLinuxJoystick::translateEvent(
  XAnyEvent * xevent )
{
  SOXT_STUB();
  return (SoEvent *) NULL;
} // translateEvent()

// *************************************************************************

/*!
  This method returns wether there is a Joystick device on the given
  display or not.  A NULL \a display argument means the default display, of
  course.
*/

SbBool
SoXtLinuxJoystick::exists( // static
  void )
{
  // FIXME: use some $COIN_JOYSTICK_DEVICE variable or something...

  int joydev = open( "/dev/js0", O_RDONLY );
  if ( joydev <= 0 )
    return FALSE;

  // do some ioctl calls or read the init messages...

  close( joydev );

  return TRUE;
} // exists()

// *************************************************************************

/*!
*/

void
SoXtLinuxJoystick::setFocusToWindow(
  SbBool enable )
{
  SOXT_STUB();
} // setFocusToWindow()

/*!
*/

SbBool
SoXtLinuxJoystick::isFocusToWindow(
  void ) const
{
  SOXT_STUB();
  return FALSE;
} // isFocusToWindow()

// *************************************************************************

/*!
  This method is invoked when the joystick is enabled and there are joystick
  events coming in.

  See linux/Documentation/joystick-api.txt.
*/

void
SoXtLinuxJoystick::input( // private
  int * source,
  XtInputId * id )
{
} // input()

/*!
  This callback just jumps to the input handler in the device object.
*/

void
SoXtLinuxJoystick::input_cb( // static, private
  XtPointer closure,
  int * source,
  XtInputId * id )
{
  assert( closure != NULL );
  SoXtLinuxJoystick * device = (SoXtLinuxJoystick *) closure;
  device->input( source, id );
} // input_cb()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtLinuxJoystickRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

