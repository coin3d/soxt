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

#ifndef SOXT_DEVICE_H
#define SOXT_DEVICE_H

#include <X11/Intrinsic.h>

#include <Inventor/SbLinear.h>

#include <Inventor/Xt/SoXtObject.h>

class SoEvent;
class SbPList;

typedef void SoXtEventHandler(Widget, XtPointer, XEvent *, Boolean *);

// *************************************************************************

class SOXT_DLL_API SoXtDevice : public SoXtObject {
  SOXT_OBJECT_ABSTRACT_HEADER(SoXtDevice, SoXtObject);

public:
  virtual ~SoXtDevice(void);

  virtual void enable(Widget widget, SoXtEventHandler * handler,
                      XtPointer closure, Window window = (Window) NULL) = 0;
  virtual void disable(Widget widget, SoXtEventHandler * handler,
                       XtPointer closure) = 0;

  virtual const SoEvent * translateEvent(XAnyEvent * xevent) = 0;

  void setWindowSize(const SbVec2s size);
  const SbVec2s getWindowSize(void) const;

  static void initClasses(void);

protected:
  SoXtDevice(void);

  void setEventPosition(SoEvent * event, int x, int y) const;

  void addEventHandler(Widget, SoXtEventHandler *, XtPointer, Window);
  void removeEventHandler(Widget, SoXtEventHandler *, XtPointer);

  void invokeHandlers(XEvent * const event);

private:
  SbVec2s size;

  SbPList * handlers;

};

// *************************************************************************

#endif // ! SOXT_DEVICE_H
