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

//  $Id$

#ifndef SOXT_SPACEBALL_H
#define SOXT_SPACEBALL_H

#include <Inventor/SbBasic.h>

#include <Inventor/Xt/devices/SoXtDevice.h>

// *************************************************************************

class SOXT_DLL_EXPORT SoXtSpaceball : public SoXtDevice {
  typedef SoXtDevice inherited;

public:
  enum SpaceballEvents {
    MOTION_EVENT  = 0x01,  MOTION = MOTION_EVENT,
    PRESS_EVENT   = 0x02,  PRESS = PRESS_EVENT,
    RELEASE_EVENT = 0x04,  RELEASE = RELEASE_EVENT,
    ALL_EVENTS = MOTION_EVENT | PRESS_EVENT | RELEASE_EVENT, ALL = ALL_EVENTS
  };

  SoXtSpaceball( int events = SoXtSpaceball::ALL_EVENTS );
  SoXtSpaceball( Display * display, int events = SoXtSpaceball::ALL_EVENTS );
  virtual ~SoXtSpaceball(void);

  virtual void enable( Widget widget,
      XtEventHandler func, XtPointer data, Window window = (Window) NULL );
  virtual void disable( Widget widget,
      XtEventHandler func, XtPointer data );

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
