/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

// $Id$

#ifndef SOXT_RESOURCE_H
#define SOXT_RESOURCE_H

#include <X11/Intrinsic.h>
#include <X11/Xresource.h>

#include <Inventor/SbBasic.h>
#include <Inventor/SbColor.h>

#include <Inventor/Xt/SoXtBasic.h>

// *************************************************************************

class SOXT_DLL_API SoXtResource {
public:
  SoXtResource(const Widget widget);
  ~SoXtResource(void);

  SbBool getResource(const char * const rname, const char * const rclass,
    SbColor & retval) const;
  SbBool getResource(const char * const rname, const char * const rclass,
    short & retval) const;
  SbBool getResource(const char * const rname, const char * const rclass,
    unsigned short & retval) const;
  SbBool getResource(const char * const rname, const char * const rclass,
    char * & retval) const;
  SbBool getResource(const char * const rname, const char * const rclass,
    SbBool & retval) const;
  SbBool getResource(const char * const rname, const char * const rclass,
    float & retval) const;

  void DumpInternals(void) const;

private:
  Display * display;
  XrmQuark * name_hierarchy;
  XrmQuark * class_hierarchy;
  int hierarchy_depth;

}; // class SoXtResource

// *************************************************************************

#endif // ! SOXT_RESOURCE_H
