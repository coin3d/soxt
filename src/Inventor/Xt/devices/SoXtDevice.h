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

#ifndef SOXT_DEVICE_H
#define SOXT_DEVICE_H

#include <X11/Intrinsic.h>

#include <Inventor/SbVec2s.h>

class SoEvent;

// *************************************************************************

class SoXtDevice {
public:
  virtual void enable( Widget widget,
      XtEventHandler func, XtPointer data, Window window = (Window) NULL ) = 0;
  virtual void disable( Widget widget,
      XtEventHandler func, XtPointer data ) = 0;

  virtual const SoEvent * translateEvent( XAnyEvent * xevent ) = 0;

  void setWindowSize( const SbVec2s size );
  const SbVec2s getWindowSize(void) const;

protected:
  void setEventPosition( SoEvent * event, int x, int y ) const;

private:
  SbVec2s size;

}; // class SoXtDevice

// *************************************************************************

#endif // ! SOXT_DEVICE_H
