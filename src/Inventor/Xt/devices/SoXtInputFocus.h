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

#ifndef SOXT_INPUTFOCUS_H
#define SOXT_INPUTFOCUS_H

#include <Inventor/Xt/devices/SoXtDevice.h>

// *************************************************************************

class SOXT_DLL_EXPORT SoXtInputFocus : public SoXtDevice {
  SOXT_TYPED_OBJECT_HEADER(SoXtInputFocus);
  typedef SoXtDevice inherited;

public:
  enum InputFocusEvents {
    ENTER_WINDOW = EnterWindowMask, // 0x01,
    LEAVE_WINDOW = LeaveWindowMask, // 0x02,
    ALL_EVENTS = ENTER_WINDOW | LEAVE_WINDOW
  };

  SoXtInputFocus( int events = SoXtInputFocus::ALL_EVENTS );
  virtual ~SoXtInputFocus(void);

  virtual void enable( Widget widget, XtEventHandler handler,
      XtPointer closure, Window window = (Window) NULL );
  virtual void disable( Widget widget, XtEventHandler handler,
      XtPointer closure );

  virtual const SoEvent * translateEvent( XAnyEvent * xevent );

private:
  int events;

}; // class SoXtInputFocus

#define SO_XT_ALL_FOCUS_EVENTS SoXtInputFocus::ALL_EVENTS

// *************************************************************************

#endif // ! SOXT_INPUTFOCUS_H
