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

#include <Inventor/errors/SoDebugError.h>

#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXtLightSliderSet.h>

/*!
  \class SoXtLightSliderSet Inventor/Xt/SoXtLightSliderSet.h
  \brief The SoXtLightSliderSet class is yet to be documented.
*/

// *************************************************************************

/*!
*/

SoXtLightSliderSet::SoXtLightSliderSet(
  Widget parent,
  const char * const name,
  SbBool inParent,
  SoNode * node )
: inherited( parent, name, inParent, node )
{
  this->constructor( TRUE );
} // SoXtLightSliderSet()

/*!
*/

SoXtLightSliderSet::SoXtLightSliderSet( // protected
  Widget parent,
  const char * const name,
  SbBool inParent,
  SoNode * node,
  SbBool build )
: inherited( parent, name, inParent, node )
{
  this->constructor( build );
} // SoXtLightSliderSet()

/*!
*/

void
SoXtLightSliderSet::constructor( // protected
  SbBool build )
{
  SOXT_STUB();
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
  SOXT_STUB();
  return (Widget) NULL;
} // buildWidget()

// *************************************************************************
