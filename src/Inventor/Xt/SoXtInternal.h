/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

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
