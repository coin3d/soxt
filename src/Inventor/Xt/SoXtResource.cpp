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

static const char rcsid[] =
  "$Id$";

#include <assert.h>

#include <Inventor/Xt/SoXtBasic.h>

#include <Inventor/Xt/SoXtResource.h>

/*!
  \class SoXtResource Inventor/Xt/SoXtResource.h
  \brief The SoXtResource class is a utility class for fetching X resource
  values for widgets.
*/

// *************************************************************************

/*!
*/

SoXtResource::SoXtResource(
  Widget widget )
{
  assert( widget != NULL );
  this->display = XtDisplay( widget );
  // set up Xrm quarks for widget
} // SoXtResource()

/*!
  Destructor.
*/

SoXtResource::~SoXtResource(
  void )
{
} // ~SoXtResource()

// *************************************************************************

/*!
  This method retrieves the given X resource and puts it into the
  SbColor object \a retval.

  TRUE is returned if the resource is found, and FALSE otherwise.
*/

SbBool
SoXtResource::getResource(
  const char * const rname,
  const char * const rclass,
  SbColor & retval )
{
  SOXT_STUB();
  return FALSE;
} // getResource()

/*!
  This method retrieves the given X resource and puts it into the
  short \a retval.

  TRUE is returned if the resource is found, and FALSE otherwise.
*/

SbBool
SoXtResource::getResource(
  const char * const rname,
  const char * const rclass,
  short & retval )
{
  SOXT_STUB();
  return FALSE;
} // getResource()

/*!
  This method retrieves the given X resource and puts it into the
  unsigned short \a retval.

  TRUE is returned if the resource is found, and FALSE otherwise.
*/

SbBool
SoXtResource::getResource(
  const char * const rname,
  const char * const rclass,
  unsigned short & retval )
{
  SOXT_STUB();
  return FALSE;
} // getResource()

/*!
  This method retrieves the given X resource and puts it into the
  char * \a retval.

  TRUE is returned if the resource is found, and FALSE otherwise.
*/

SbBool
SoXtResource::getResource(
  const char * const rname,
  const char * const rclass,
  char * & retval )
{
  SOXT_STUB();
  return FALSE;
} // getResource()

/*!
  This method retrieves the given X resource and puts it into the
  SbBool \a retval.

  TRUE is returned if the resource is found, and FALSE otherwise.
*/

SbBool
SoXtResource::getResource(
  const char * const rname,
  const char * const rclass,
  SbBool & retval )
{
  SOXT_STUB();
  return FALSE;
} // getResource()

/*!
  This method retrieves the given X resource and puts it into the
  float \a retval.

  TRUE is returned if the resource is found, and FALSE otherwise.
*/

SbBool
SoXtResource::getResource(
  const char * const rname,
  const char * const rclass,
  float & retval )
{
  SOXT_STUB();
  return FALSE;
} // getResource()

// *************************************************************************
