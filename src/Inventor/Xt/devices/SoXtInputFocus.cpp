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

#include <X11/X.h>

#include <Inventor/misc/SoBasic.h>

#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/devices/SoXtInputFocus.h>

// *************************************************************************

/*!
*/

SoXtInputFocus::SoXtInputFocus(
  int events )
{
  this->events = events;
} // SoXtInputfocus()

/*!
*/

SoXtInputFocus::~SoXtInputFocus( // virtual
  void )
{
} // ~SoXtInputfocus()

/*!
*/

void
SoXtInputFocus::enable( // virtual
  Widget, // widget,
  XtEventHandler, // func,
  XtPointer, // data,
  Window ) // window )
{
  SOXT_STUB();
} // enable()

/*!
*/

void
SoXtInputFocus::disable( // virtual
  Widget, // widget,
  XtEventHandler, // func,
  XtPointer ) // data )
{
  SOXT_STUB();
} // disable()

/*!
*/

const SoEvent *
SoXtInputFocus::translateEvent( // virtual
  XAnyEvent * event )
{
  switch ( event->type ) {

  // events we should handle:
  case FocusIn:
  case FocusOut:
    do {
      SOXT_STUB();
    } while ( FALSE );
    // return SoEvent here
    break;

  // events we should ignore:
  default:
    break;
  } // switch ( event->type )

  return (SoEvent *) NULL;
} // translateEvent()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtInputFocusRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

