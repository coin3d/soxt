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
  SbBool inParent,
  SoNode * const node )
: inherited( parent, name, inParent, node )
{
  SOXT_STUB();
} // SoXtMaterialSliderSet()

/*!
*/

SoXtMaterialSliderSet::SoXtMaterialSliderSet( // protected
  Widget parent,
  const char * const name,
  SbBool inParent,
  SoNode * const node,
  SbBool build )
: inherited( parent, name, inParent, node )
{
  SOXT_STUB();
} // SoXtMaterialSliderSet()

/*!
*/

SoXtMaterialSliderSet::~SoXtMaterialSliderSet(
  void )
{
  SOXT_STUB();
} // ~SoXtMaterialSliderSet()

// *************************************************************************

/*!
*/

void
SoXtMaterialSliderSet::constructor(
  SbBool build )
{
  SOXT_STUB();
} // constructor()

/*!
*/

Widget
SoXtMaterialSliderSet::buildWidget(
  Widget parent )
{
  SOXT_STUB();
  return (Widget) NULL;
} // buildWidget()

// *************************************************************************
