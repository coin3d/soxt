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
  SoXtResource( const Widget widget );
  ~SoXtResource(void);

  SbBool getResource( const char * const rname, const char * const rclass,
    SbColor & retval ) const;
  SbBool getResource( const char * const rname, const char * const rclass,
    short & retval ) const;
  SbBool getResource( const char * const rname, const char * const rclass,
    unsigned short & retval ) const;
  SbBool getResource( const char * const rname, const char * const rclass,
    char * & retval ) const;
  SbBool getResource( const char * const rname, const char * const rclass,
    SbBool & retval ) const;
  SbBool getResource( const char * const rname, const char * const rclass,
    float & retval ) const;

  void DumpInternals(void) const;

private:
  Display * display;
  XrmQuark * name_hierarchy;
  XrmQuark * class_hierarchy;
  int hierarchy_depth;

}; // class SoXtResource

// *************************************************************************

#endif // ! SOXT_RESOURCE_H
