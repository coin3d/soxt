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

// $Id$

#ifndef SOXT_SPACEBALL_H
#define SOXT_SPACEBALL_H

#include <Inventor/SbBasic.h>
#include <Inventor/events/SoMotion3Event.h>                                     
#include <Inventor/events/SoSpaceballButtonEvent.h>                             

#include <Inventor/Xt/devices/SoXtDevice.h>

// *************************************************************************

class SOXT_DLL_EXPORT SoXtSpaceball : public SoXtDevice {
  SOXT_TYPED_OBJECT_HEADER(SoXtSpaceball);
  typedef SoXtDevice inherited;

public:
  enum Mask {
    MOTION_EVENT  = 0x01,  MOTION = MOTION_EVENT,
    PRESS_EVENT   = 0x02,  PRESS = PRESS_EVENT,
    RELEASE_EVENT = 0x04,  RELEASE = RELEASE_EVENT,
    ALL_EVENTS = MOTION_EVENT | PRESS_EVENT | RELEASE_EVENT,
    ALL = ALL_EVENTS
  };

  enum DeviceType {
    SPACE_BALL             = 0x01,
    MAGELLAN_SPACE_MOUSE   = 0x02
  };

  SoXtSpaceball( int events = SoXtSpaceball::ALL_EVENTS );
  SoXtSpaceball( Display * display, int events = SoXtSpaceball::ALL_EVENTS );
  virtual ~SoXtSpaceball(void);

  virtual void enable( Widget widget, XtEventHandler handler,
      XtPointer closure, Window window = (Window) NULL );
  virtual void disable( Widget widget, XtEventHandler handler,
      XtPointer closure );

  virtual const SoEvent * translateEvent( XAnyEvent * xevent );

  void setRotationScaleFactor( float factor );
  float getRotationScaleFactor(void) const;
  void setTranslationScaleFactor( float factor );
  float getTranslationScaleFactor(void) const;

  static SbBool exists( Display * display = NULL );

  void setFocusToWindow( SbBool enable );
  SbBool isFocusToWindow(void) const;

private:
  int events;

  float rotationFactor;
  float translationFactor;

}; // class SoXtSpaceball

#define SO_XT_ALL_SPACEBALL_EVENTS SoXtSpaceball::ALL_EVENTS

// *************************************************************************

#endif // ! SOXT_SPACEBALL_H
