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

#ifndef SOXTINTERNALUTILS_H
#define SOXTINTERNALUTILS_H

#include <X11/Intrinsic.h>
#include <Inventor/SbBasic.h> // SbBool

// ************************************************************************

// This class contains common data and methods that we want to share
// among classes within SoXt, but which should not be publicly visible
// in the library API.

class SoXtInternal {
public:
  static void selectBestVisual(Display * dpy, Visual * & visual,
                               Colormap & cmap, int & depth);

  static Pixmap createPixmapFromXpm(Widget button, const char ** xpm,
                                    SbBool ghost = FALSE);

  static void setAppName(const char * appname);
  static void setAppClass(const char * appclass);
  static const char * getAppName(void);
  static const char * getAppClass(void);

private:
  static const char * xpmErrorString(int error);

  static char * appname;
  static char * appclass;
};

// ************************************************************************

#endif // ! SOXTINTERNALUTILS_H
