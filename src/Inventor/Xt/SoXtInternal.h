/**************************************************************************\
 *
 *  This file is part of the Coin family of 3D visualization libraries.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and / or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use this library in software that is incompatible
 *  with the GNU GPL, and / or you would like to take advantage of the
 *  additional benefits with regard to our support services, please
 *  contact Systems in Motion about acquiring a Coin Professional
 *  Edition License.  See <URL:http://www.coin3d.org> for more
 *  information.
 *
 *  Systems in Motion, Abels gate 5, Teknobyen, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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
