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

#include <assert.h>

#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>

#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtResource.h>

#include <Inventor/Xt/SoXtLightSliderSet.h>

/*!
  \class SoXtLightSliderSet Inventor/Xt/SoXtLightSliderSet.h
  \brief The SoXtLightSliderSet class is a component for modifying light
  nodes.

  This component is designed for editing SoLight nodes.
  One slider is created for adjusting the intensity of the light,
  while another slider set is created for adjusting the light color.
*/

// *************************************************************************

/*!
*/

SoXtLightSliderSet::SoXtLightSliderSet(
  const Widget parent,
  const char * const name,
  const SbBool embed,
  SoNode * const node )
: inherited( parent, name, embed, node, FALSE )
{
  this->constructor( TRUE );
} // SoXtLightSliderSet()

/*!
*/

SoXtLightSliderSet::SoXtLightSliderSet( // protected
  const Widget parent,
  const char * const name,
  const SbBool embed,
  SoNode * const node,
  const SbBool build )
: inherited( parent, name, embed, node, FALSE )
{
  this->constructor( build );
} // SoXtLightSliderSet()

/*!
  Common code for all constructors.
*/

void
SoXtLightSliderSet::constructor( // private
  const SbBool build )
{
  if ( build ) {
    this->setClassName( "SoXtLightSliderSet" );
    Widget sliderset = this->buildWidget( this->getParentWidget() );
    this->setBaseWidget( sliderset );
    this->fitSize( SbVec2s( 300, 0 ) );
  }
} // constructor()

/*!
*/

SoXtLightSliderSet::~SoXtLightSliderSet(
  void )
{
} // ~SoXtLightSliderSet()

// *************************************************************************

/*!
*/

Widget
SoXtLightSliderSet::buildWidget( // protected
  Widget parent )
{
  return inherited::buildWidget( parent );
} // buildWidget()

// *************************************************************************

/*!
*/

const char *
SoXtLightSliderSet::getDefaultWidgetName( // virtual, protected
  void ) const
{
  static const char defaultWidgetName[] = "SoXtLightSliderSet";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
*/

const char *
SoXtLightSliderSet::getDefaultTitle( // virtual, protected
  void ) const
{
  static const char defaultTitle[] = "Light Editor";
  return defaultTitle;
} // getDefaultTitle()

/*!
*/

const char *
SoXtLightSliderSet::getDefaultIconTitle( // virtual, protected
  void ) const
{
  static const char defaultIconTitle[] = "Light Editor";
  return defaultIconTitle;
} // getDefaultIconTitle()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtLightSliderSetRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

