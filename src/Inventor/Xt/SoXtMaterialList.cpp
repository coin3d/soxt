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

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtMaterialList.h>

/*!
  \class SoXtMaterialList Inventor/Xt/SoXtMaterialList.h
  \brief The SoXtMaterialList class is yet to be documented.
*/

// *************************************************************************

/*!
*/

SoXtMaterialList::SoXtMaterialList(
  Widget parent,
  const char * const name,
  SbBool inParent,
  const char * const dir )
: inherited( parent, name, inParent )
{
  this->constructor( TRUE );
} // SoXtMaterialList()

/*!
*/

SoXtMaterialList::SoXtMaterialList( // protected
  Widget parent,
  const char * const name,
  SbBool inParent,
  const char * const dir,
  SbBool build )
: inherited( parent, name, inParent )
{
  this->constructor( build );
} // SoXtMaterialList()

/*!
*/

void
SoXtMaterialList::constructor( // private
  SbBool build )
{
  SOXT_STUB();
} // constructor()

/*!
*/

SoXtMaterialList::~SoXtMaterialList(
  void )
{
  SOXT_STUB();
} // ~SoXtMaterialList()

// *************************************************************************

void
SoXtMaterialList::addCallback(
  SoXtMaterialListCB * const callback,
  void * const user )
{
  SOXT_STUB();
} // addCallback()

void
SoXtMaterialList::removeCallback(
  SoXtMaterialListCB * const callback,
  void * const user )
{
  SOXT_STUB();
} // removeCallback()

// *************************************************************************

/*!
*/

const char *
SoXtMaterialList::getDefaultWidgetName( // virtual, protected
  void ) const
{
  static const char defaultWidgetName[] = "SoXtMaterialList";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
*/

const char *
SoXtMaterialList::getDefaultTitle( // virtual, protected
  void ) const
{
  static const char defaultTitle[] = "Material Sliders";
  return defaultTitle;
} // getDefaultTitle()

/*!
*/

const char *
SoXtMaterialList::getDefaultIconTitle( // virtual, protected
  void ) const
{
  static const char defaultIconTitle[] = "Material Sliders";
  return defaultIconTitle;
} // getDefaultIconTitle()

// *************************************************************************

/*!
*/

Widget
SoXtMaterialList::buildWidget( // protected
  Widget parent )
{
  SOXT_STUB();
  return (Widget) NULL;
} // buildWidget()

/*!
*/

Widget
SoXtMaterialList::buildPulldownMenu( // protected
  Widget parent )
{
  SOXT_STUB();
  return (Widget) NULL;
} // buildPulldownMenu()

// *************************************************************************
