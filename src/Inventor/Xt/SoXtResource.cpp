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
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtComponent.h>

#include <Inventor/Xt/SoXtResource.h>

/*!
  \class SoXtResource Inventor/Xt/SoXtResource.h
  \brief The SoXtResource class is a utility class for fetching X resource
  values for widgets.  Special care is taken for SoXt components.

  The SoXtResource objects are
*/

// *************************************************************************

/*!
  Constructor.   sets up the SoXtResource object to fetch resources for
  the \a widget Widget.
*/

SoXtResource::SoXtResource(
  const Widget widget )
{
  static SbBool initialized = FALSE;
  if ( ! initialized ) {
    XrmInitialize();
    initialized = TRUE;
  }

  this->hierarchy_depth = 0;

  if ( ! widget ) {
    this->name_hierarchy = NULL;
    this->class_hierarchy = NULL;
    this->display = SoXt::getDisplay();
    return;
  }

  this->display = XtDisplay( widget );

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
         ((component = SoXtComponent::getComponent( w )) != NULL) ) {
      stop = XtParent( component->getWidget() );
    }
    w = XtParent( w );
  }

#if SOXT_DEBUG
  if ( component == NULL )
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

#if SOXT_DEBUG && 0
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
  fprintf( stdout, "Classes: " );
  int i;
  for ( i = 0; i < this->hierarchy_depth; i++ ) {
    fprintf( stdout, "%s", XrmQuarkToString( this->class_hierarchy[i] ) );
    if ( i < (this->hierarchy_depth - 1) )
      fprintf( stdout, "." );
  }
  fprintf( stdout, "\n" );

  fprintf( stdout, "Names:  " );
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
    SbBool found = FALSE;                                                      \
    XrmDatabase database = XrmGetDatabase( this->display );                    \
    if ( this->name_hierarchy != NULL ) {                                      \
      this->name_hierarchy[this->hierarchy_depth] = XrmStringToQuark( rname ); \
      this->class_hierarchy[this->hierarchy_depth] =                           \
        XrmStringToQuark( rclass );                                            \
      found = XrmQGetResource( database, this->name_hierarchy,                 \
                this->class_hierarchy, &format, &value ) ? TRUE : FALSE;       \
      this->name_hierarchy[this->hierarchy_depth] = 0;                         \
      this->class_hierarchy[this->hierarchy_depth] = 0;                        \
    }                                                                          \
    if ( ! found )                                                             \
      found = XrmGetResource( database, rname,                                 \
                rclass, &formatstr, &value ) ? TRUE : FALSE;                   \
    if ( ! found ) {                                                           \
      this->DumpInternals();                                                   \
      SoDebugError::postInfo( "getResource",                                   \
        "resource \"%s\" (%s) not found", rname, rclass );                     \
      return FALSE;                                                            \
    }                                                                          \
  } while ( FALSE )

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
  SbColor & retval ) const
{
  GET_RESOURCE();

  if ( ! formatstr ) {
    SoDebugError::postInfo( "getResource", "resource format (quark) = %s\n",
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
    SoDebugError::postInfo( "getResource", "resource format (quark) = %s\n",
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
    SoDebugError::postInfo( "getResource", "resource format (quark) = %s\n",
      XrmQuarkToString(format) );
  } else {
    SoDebugError::postInfo( "getResource", "resource format = %s\n",
      formatstr );
  }

  SOXT_STUB_ONCE();
  return FALSE;
} // getResource()

/*!
  This method retrieves the given X resource and points the \a retval
  pointer to it's data.

  TRUE is returned if the resource is found, and FALSE otherwise.
*/

SbBool
SoXtResource::getResource(
  const char * const rname,
  const char * const rclass,
  char * & retval ) const
{
  GET_RESOURCE();

  XrmQuark strq = XrmStringToQuark( XmRString );
  if ( formatstr != NULL )
    format = XrmStringToQuark( formatstr );

  if ( strq != format ) {
    SoDebugError::postInfo( "getResource",
      "resource format \"%s\" not supported\n", XrmQuarkToString( format ) );
    return FALSE;
  }
  retval = (char *) value.addr;
  return TRUE;
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
    SoDebugError::postInfo( "getResource", "resource format (quark) = %s\n",
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
    SoDebugError::postInfo( "getResource", "resource format (quark) = %s\n",
      XrmQuarkToString(format) );
  } else {
    SoDebugError::postInfo( "getResource", "resource format = %s\n",
      formatstr );
  }

  SOXT_STUB_ONCE();
  return FALSE;
} // getResource()

// *************************************************************************
