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
#include <stdio.h>

#include <X11/IntrinsicP.h>
#include <X11/CoreP.h>

#include <Inventor/SoLists.h>
#include <Inventor/errors/SoDebugError.h>

#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXtComponent.h>

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
  const Widget widget )
: display( XtDisplay( widget ) )
{
  assert( widget != NULL && display != NULL );

  static SbBool initialized = FALSE;
  if ( ! initialized ) {
    XrmInitialize();
    initialized = TRUE;
  }

  this->hierarchy_depth = 0;
  SbIntList quarks;
  Widget stop = NULL;

  SoXtComponent * component = SoXtComponent::getComponent( widget );
  if ( component )
    stop = XtParent( component->getWidget() );

  Widget w = widget;
  while ( w && w != stop ) {
    quarks.append( ((CorePart *) w)->xrm_name );
    quarks.append( ((CoreClassPart *) XtClass(w))->xrm_class );
    this->hierarchy_depth++;
    if ( XtIsShell( w ) )
      break;
    if ( (component == NULL) &&
         ((component = SoXtComponent::getComponent( widget )) != NULL) ) {
      stop = XtParent( component->getWidget() );
    }
    w = XtParent( w );
  }

#if SOXT_DEBUG
  if ( ! component )
    SoDebugError::postInfo( "SoXtResource",
      "using SoXtResource for non-component widget (which is OK)" );
#endif // SOXT_DEBUG

  this->name_hierarchy = new XrmQuark [ this->hierarchy_depth + 2];
  this->class_hierarchy = new XrmQuark [ this->hierarchy_depth + 2];
  int i;
  for ( i = 0; i < this->hierarchy_depth; i++ ) {
    this->name_hierarchy[this->hierarchy_depth - i - 1] = quarks[i*2];
    this->class_hierarchy[this->hierarchy_depth - i - 1] = quarks[i*2+1];
  }
  this->name_hierarchy[ this->hierarchy_depth ] = 0;
  this->name_hierarchy[ this->hierarchy_depth + 1 ] = 0;
  this->class_hierarchy[ this->hierarchy_depth ] = 0;
  this->class_hierarchy[ this->hierarchy_depth + 1 ] = 0;

#if SOXT_DEBUG
  this->DumpInternals();
#endif // SOXT_DEBUG
} // SoXtResource()

/*!
  Destructor.
*/

SoXtResource::~SoXtResource(
  void )
{
  delete [] this->name_hierarchy;
  delete [] this->class_hierarchy;
} // ~SoXtResource()

/*!
*/

void
SoXtResource::DumpInternals(
  void ) const
{
  SoDebugError::postInfo( "SoXtResource::DumpInternals", "dumping" );
  fprintf( stdout, "Class hierarchy:\n  " );
  int i;
  for ( i = 0; i < this->hierarchy_depth; i++ ) {
    fprintf( stdout, "%s", XrmQuarkToString( this->class_hierarchy[i] ) );
    if ( i < (this->hierarchy_depth - 1) )
      fprintf( stdout, "." );
  }
  fprintf( stdout, "\nName hierarchy:\n  " );
  for ( i = 0; i < this->hierarchy_depth; i++ ) {
    fprintf( stdout, "%s", XrmQuarkToString( this->name_hierarchy[i] ) );
    if ( i < (this->hierarchy_depth - 1) )
      fprintf( stdout, "." );
  }
  fprintf( stdout, "\n" );
} // DumpInternals()

// *************************************************************************

#define GET_RESOURCE()                                                         \
  XrmValue value;                                                              \
  XrmRepresentation format;                                                    \
  char * formatstr = NULL;                                                     \
  do {                                                                         \
    XrmDatabase database = XrmGetDatabase( this->display );                    \
    this->name_hierarchy[this->hierarchy_depth] = XrmStringToQuark( rname );   \
    this->class_hierarchy[this->hierarchy_depth] = XrmStringToQuark( rclass ); \
    if (   ! XrmQGetResource( database, this->name_hierarchy,                  \
             this->class_hierarchy, &format, &value ) &&                       \
         ! XrmGetResource( database, rname, rclass, &formatstr, &value ) ) {   \
      this->name_hierarchy[this->hierarchy_depth] = 0;                         \
      this->class_hierarchy[this->hierarchy_depth] = 0;                        \
      SoDebugError::postInfo( "getResource", "resource \"%s\" (%s) not found", \
        rname, rclass );                                                       \
      return FALSE;                                                            \
    }                                                                          \
    this->name_hierarchy[this->hierarchy_depth] = 0;                           \
    this->class_hierarchy[this->hierarchy_depth] = 0;                          \
  } while ( FALSE )

/*!
  This method retrieves the given X resource and puts it into the
  SbColor object \a retval.

  TRUE is returned if the resource is found, and FALSE otherwise.
*/

SbBool
SoXtResource::getResource(
  const char * const rname,
  const char * const rclass,
  SbColor & retval ) const
{
  GET_RESOURCE();

  if ( ! formatstr ) {
    SoDebugError::postInfo( "getResource", "resource format = %s\n",
      XrmQuarkToString(format) );
  } else {
    SoDebugError::postInfo( "getResource", "resource format = %s\n",
      formatstr );
  }

  SOXT_STUB_ONCE();
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
  short & retval ) const
{
  GET_RESOURCE();

  if ( ! formatstr ) {
    SoDebugError::postInfo( "getResource", "resource format = %s\n",
      XrmQuarkToString(format) );
  } else {
    SoDebugError::postInfo( "getResource", "resource format = %s\n",
      formatstr );
  }

  SOXT_STUB_ONCE();
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
  unsigned short & retval ) const
{
  GET_RESOURCE();

  if ( ! formatstr ) {
    SoDebugError::postInfo( "getResource", "resource format = %s\n",
      XrmQuarkToString(format) );
  } else {
    SoDebugError::postInfo( "getResource", "resource format = %s\n",
      formatstr );
  }

  SOXT_STUB_ONCE();
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
  char * & retval ) const
{
  GET_RESOURCE();

  if ( ! formatstr ) {
    SoDebugError::postInfo( "getResource", "resource format = %s\n",
      XrmQuarkToString(format) );
  } else {
    SoDebugError::postInfo( "getResource", "resource format = %s\n",
      formatstr );
  }

  SOXT_STUB_ONCE();
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
  SbBool & retval ) const
{
  GET_RESOURCE();

  if ( ! formatstr ) {
    SoDebugError::postInfo( "getResource", "resource format = %s\n",
      XrmQuarkToString(format) );
  } else {
    SoDebugError::postInfo( "getResource", "resource format = %s\n",
      formatstr );
  }

  SOXT_STUB_ONCE();
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
  float & retval ) const
{
  GET_RESOURCE();

  if ( ! formatstr ) {
    SoDebugError::postInfo( "getResource", "resource format = %s\n",
      XrmQuarkToString(format) );
  } else {
    SoDebugError::postInfo( "getResource", "resource format = %s\n",
      formatstr );
  }

  SOXT_STUB_ONCE();
  return FALSE;
} // getResource()

// *************************************************************************