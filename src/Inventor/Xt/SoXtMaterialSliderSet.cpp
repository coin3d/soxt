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

#if SOXT_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOXT_DEBUG

#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>

#include <Inventor/Xt/SoXtMaterialSliderSet.h>

/*!
  \class SoXtMaterialSliderSet Inventor/Xt/SoXtMaterialSliderSet.h
  \brief The SoXtMaterialSliderSet class is yet to be documented.
*/

// *************************************************************************

/*!
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
*/

SoXtMaterialSliderSet::~SoXtMaterialSliderSet(
  void )
{
} // ~SoXtMaterialSliderSet()

// *************************************************************************

/*!
*/

void
SoXtMaterialSliderSet::constructor(
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
*/

Widget
SoXtMaterialSliderSet::buildWidget(
  Widget parent )
{
  return inherited::buildWidget( parent );
} // buildWidget()

// *************************************************************************

/*!
*/

const char *
SoXtMaterialSliderSet::getDefaultWidgetName( // virtual, protected
  void ) const
{
  static const char defaultWidgetName[] = "SoXtMaterialSliderSet";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
*/

const char *
SoXtMaterialSliderSet::getDefaultTitle( // virtual, protected
  void ) const
{
  static const char defaultTitle[] = "Material Sliders";
  return defaultTitle;
} // getDefaultTitle()

/*!
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

