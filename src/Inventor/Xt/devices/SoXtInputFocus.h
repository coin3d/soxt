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

#ifndef SOXT_INPUTFOCUS_H
#define SOXT_INPUTFOCUS_H

#include <Inventor/Xt/devices/SoXtDevice.h>

// *************************************************************************

class SoXtInputFocus : public SoXtDevice {
  typedef SoXtDevice inherited;

public:
  enum InputFocusEvents {
    ENTER_WINDOW = EnterWindowMask, // 0x01,
    LEAVE_WINDOW = LeaveWindowMask, // 0x02,
    ALL_EVENTS = ENTER_WINDOW | LEAVE_WINDOW
  };

  SoXtInputFocus( int events = SoXtInputFocus::ALL_EVENTS );
  virtual ~SoXtInputFocus(void);

  virtual void enable( Widget widget,
      XtEventHandler func, XtPointer data, Window window = (Window) NULL );
  virtual void disable( Widget widget,
      XtEventHandler func, XtPointer data );

  virtual const SoEvent * translateEvent( XAnyEvent * xevent );

private:
  int events;

}; // class SoXtInputFocus

#define SO_XT_ALL_FOCUS_EVENTS SoXtInputFocus::ALL_EVENTS

// *************************************************************************

#endif // ! SOXT_INPUTFOCUS_H
