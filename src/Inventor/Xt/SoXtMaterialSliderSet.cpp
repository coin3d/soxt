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

#if SOXT_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOXT_DEBUG

#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>

#include <Inventor/Xt/SoXtMaterialSliderSet.h>

// *************************************************************************

/*!
  \class SoXtMaterialSliderSet Inventor/Xt/SoXtMaterialSliderSet.h
  \brief The SoXtMaterialSliderSet class is yet to be documented.
  \ingroup components slidersets
*/

// *************************************************************************

SOXT_OBJECT_SOURCE(SoXtMaterialSliderSet);

// *************************************************************************

/*!
  Constructor.
*/

SoXtMaterialSliderSet::SoXtMaterialSliderSet(
  Widget parent,
  const char * const name,
  SbBool embed,
  SoNode * const node )
: inherited( parent, name, embed, node, FALSE )
{
  this->constructor( TRUE );
} // SoXtMaterialSliderSet()

/*!
  Constructor.
*/

SoXtMaterialSliderSet::SoXtMaterialSliderSet( // protected
  Widget parent,
  const char * const name,
  SbBool embed,
  SoNode * const node,
  SbBool build )
: inherited( parent, name, embed, node, FALSE )
{
  this->constructor( build );
} // SoXtMaterialSliderSet()

/*!
  Common code for all constructors.
*/

void
SoXtMaterialSliderSet::constructor( // private
  SbBool build )
{
  if ( build ) {
    this->setClassName( "SoXtMaterialSliderSet" );
    Widget sliderset = this->buildWidget( this->getParentWidget() );
    this->setBaseWidget( sliderset );
    this->fitSize( SbVec2s( 300, 0 ) );
  }
} // constructor()

/*!
  Destructor.
*/

SoXtMaterialSliderSet::~SoXtMaterialSliderSet(
  void )
{
} // ~SoXtMaterialSliderSet()

// *************************************************************************

/*!
  FIXME: write doc
*/

Widget
SoXtMaterialSliderSet::buildWidget(
  Widget parent )
{
  return inherited::buildWidget( parent );
} // buildWidget()

// *************************************************************************

/*!
  FIXME: write doc
*/

const char *
SoXtMaterialSliderSet::getDefaultWidgetName( // virtual, protected
  void ) const
{
  static const char defaultWidgetName[] = "SoXtMaterialSliderSet";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
  FIXME: write doc
*/

const char *
SoXtMaterialSliderSet::getDefaultTitle( // virtual, protected
  void ) const
{
  static const char defaultTitle[] = "Material Sliders";
  return defaultTitle;
} // getDefaultTitle()

/*!
  FIXME: write doc
*/

const char *
SoXtMaterialSliderSet::getDefaultIconTitle( // virtual, protected
  void ) const
{
  static const char defaultIconTitle[] = "Material Sliders";
  return defaultIconTitle;
} // getDefaultIconTitle()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtMaterialSliderSetRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

