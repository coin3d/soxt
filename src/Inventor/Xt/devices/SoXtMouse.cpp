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

#include <Inventor/misc/SoBasic.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMouseButtonEvent.h>

#include <Inventor/Xt/devices/SoXtMouse.h>

// *************************************************************************

SoXtMouse::SoXtMouse(
  int events )
{
  this->events = events;
  this->locationEvent = NULL;
  this->buttonEvent = NULL;
} // SoXtMouse()

SoXtMouse::~SoXtMouse(
  void )
{
  if ( this->locationEvent != NULL )
    delete this->locationEvent;
  if ( this->buttonEvent != NULL )
    delete this->buttonEvent;
} // ~SoXtMouse()

void
SoXtMouse::enable( // virtual
  Widget widget,
  XtEventHandler func,
  XtPointer data,
  Window window )
{
  COIN_STUB();
} // enable()

void
SoXtMouse::disable( // virtual
  Widget widget,
  XtEventHandler func,
  XtPointer data )
{
  COIN_STUB();
} // disable()

const SoEvent *
SoXtMouse::translateEvent( // virtual
  XAnyEvent * xevent )
{
  COIN_STUB();
  return (SoEvent *) NULL;
} // translateEvent()

SoLocation2Event *
SoXtMouse::makeLocationEvent( // private
  XMotionEvent * event )
{
  COIN_STUB();
  return (SoLocation2Event *) NULL;
} // makeLocationEvent()

SoMouseButtonEvent *
SoXtMouse::makeButtonEvent( // private
  XButtonEvent * event,
  SoButtonEvent::State state )
{
  COIN_STUB();
  return (SoMouseButtonEvent *) NULL;
} // makeButtonEvent()

// *************************************************************************
