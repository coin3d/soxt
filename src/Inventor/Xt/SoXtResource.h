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

#ifndef SOXT_RESOURCE_H
#define SOXT_RESOURCE_H

#include <X11/Intrinsic.h>
#include <X11/Xresource.h>

#include <Inventor/SbBasic.h>
#include <Inventor/SbColor.h>

// *************************************************************************

class SoXtResource {
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
