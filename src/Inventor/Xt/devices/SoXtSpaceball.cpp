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

#include <X11/extensions/XInput.h>

#include <Inventor/misc/SoBasic.h>

// FIXME: really ugly, but necessary to avoid XInput.h/Print.h conflict
#define _XpPrint_H_
#define _XP_PRINT_SERVER_
typedef void * XPContext;

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXt.h>

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
  Display *, // display,
  int events )
{
  this->events = events;
} // SoXtSpaceball()

/*!
*/

SoXtSpaceball::~SoXtSpaceball( // virtual
  void )
{
} // ~SoXtSpaceball()

// *************************************************************************

/*!
*/

void
SoXtSpaceball::enable( // virtual
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
SoXtSpaceball::disable( // virtual
  Widget, // widget,
  XtEventHandler, // func,
  XtPointer ) // data )
{
  SOXT_STUB();
} // disable()

// *************************************************************************

/*!
*/

const SoEvent *
SoXtSpaceball::translateEvent( // virtual
 XAnyEvent * event )
{
  SOXT_STUB_ONCE();
  return (SoEvent *) NULL;
} // translateEvent()

// *************************************************************************

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

// *************************************************************************

/*!
*/

SbBool
SoXtSpaceball::exists( // static
  Display * display )
{
  if ( display == NULL )
    display = SoXt::getDisplay();
  assert( display != NULL );
  Atom SpaceballAtom = XInternAtom( display, XI_SPACEBALL, True );
  if ( SpaceballAtom == None )
    return FALSE;
  int numDevices = 0;
  XDeviceInfo * devices = XListInputDevices( display, &numDevices );
  for ( int i = 0; i < numDevices; i++ ) {
    if ( devices[i].type == SpaceballAtom ) {
      XFreeDeviceList( devices );
      return TRUE;
    }
  }
  XFreeDeviceList( devices );
  return FALSE;
} // exists()

// *************************************************************************

/*!
*/

void
SoXtSpaceball::setFocusToWindow(
  SbBool ) // enable )
{
  SOXT_STUB();
} // setFocusToWindow()

/*!
*/

SbBool
SoXtSpaceball::isFocusToWindow(
  void ) const
{
  SOXT_STUB();
  return FALSE;
} // isFocusToWindow()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtSpaceballRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

