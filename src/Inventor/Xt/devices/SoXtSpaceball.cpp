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

static const char rcsid[] =
  "$Id$";

#include <X11/X.h>

#include <Inventor/misc/SoBasic.h>

#include <common/soguidefs.h> // SOGUI_STUB()
#include <Inventor/Xt/devices/SoXtSpaceball.h>

// *************************************************************************

/*!
*/

SoXtSpaceball::SoXtSpaceball(
  int events )
{
  this->events = events;
} // SoXtSpaceball()

/*!
*/

SoXtSpaceball::SoXtSpaceball(
  Display * display,
  int events )
{
  this->events = events;
} // SoXtSpaceball()

/*!
*/

SoXtSpaceball::~SoXtSpaceball(
  void )
{
} // ~SoXtSpaceball()

/*!
*/

void
SoXtSpaceball::enable( // virtual
  Widget widget,
  XtEventHandler func,
  XtPointer data,
  Window window )
{
  SOGUI_STUB();
} // enable()

/*!
*/

void
SoXtSpaceball::disable( // virtual
  Widget widget,
  XtEventHandler func,
  XtPointer data )
{
  SOGUI_STUB();
} // disable()

/*!
*/

const SoEvent *
SoXtSpaceball::translateEvent( // virtual
 XAnyEvent * event )
{
  switch ( event->type ) {

  // events we should handle:
  case ClientMessage: // ???
    do {
      SOGUI_STUB();
    } while ( FALSE );
    // return SoEvent * here
    break;

  // events we should ignore:
  default:
    break;

  } // switch ( event->type )

  return (SoEvent *) NULL;
} // translateEvent()

/*!
*/

void
SoXtSpaceball::setRotationScaleFactor(
  float factor )
{
  this->rotationFactor = factor;
} // setRotationScaleFactor()

/*!
*/

float
SoXtSpaceball::getRotationScaleFactor(
  void ) const
{
  return this->rotationFactor;
} // getRotationScaleFactor()

/*!
*/

void
SoXtSpaceball::setTranslationScaleFactor(
  float factor )
{
  this->translationFactor = factor;
} // setTranslationScaleFactor()

/*!
*/

float
SoXtSpaceball::getTranslationScaleFactor(
  void ) const
{
  return this->translationFactor;
} // getTranslationScaleFactor()

/*!
*/

SbBool
SoXtSpaceball::exists( // static
  Display * display )
{
  SOGUI_STUB();
  return FALSE;
} // exists()

/*!
*/

void
SoXtSpaceball::setFocusToWindow(
  SbBool flag )
{
  SOGUI_STUB();
} // setFocusToWindow()

/*!
*/

SbBool
SoXtSpaceball::isFocusToWindow(
  void ) const
{
  SOGUI_STUB();
  return FALSE;
} // isFocusToWindow()

// *************************************************************************
