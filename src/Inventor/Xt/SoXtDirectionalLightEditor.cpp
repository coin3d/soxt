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
#include <Inventor/nodes/SoDirectionalLight.h>

#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXtDirectionalLightEditor.h>

/*!
  \class SoXtDirectionalLightEditor Inventor/Xt/SoXtDirectionalLightEditor.h
  \brief The SoXtDirectionalLightEditor class is yet to be documented.
*/

// *************************************************************************

/*!
*/

SoXtDirectionalLightEditor::SoXtDirectionalLightEditor(
  Widget parent,
  const char * const name,
  SbBool inParent )
{
  SOXT_STUB();
  this->constructor( TRUE );
} // SoXtDirectionalLightEditor()

/*!
*/

SoXtDirectionalLightEditor::SoXtDirectionalLightEditor( // protected
  Widget parent,
  const char * const name,
  SbBool inParent,
  SbBool build )
{
  SOXT_STUB();
  this->constructor( build );
} // SoXtDirectionalLightEditor()

/*!
*/

SoXtDirectionalLightEditor::~SoXtDirectionalLightEditor(
  void )
{
  SOXT_STUB();
} // ~SoXtDirectionalLightEditor()

void
SoXtDirectionalLightEditor::attach(
  SoPath * pathToLight )
{
  SOXT_STUB();
} // attach()

void
SoXtDirectionalLightEditor::detach(
  void )
{
  SOXT_STUB();
} // detach()

SbBool
SoXtDirectionalLightEditor::isAttached(
  void )
{
  SOXT_STUB();
  return FALSE;
} // isAttached()

void
SoXtDirectionalLightEditor::setLight(
  const SoDirectionalLight & light )
{
  SOXT_STUB();
} // setLight()

const SoDirectionalLight &
SoXtDirectionalLightEditor::getLight(
  void ) const
{
  SOXT_STUB();
  return *((SoDirectionalLight *) SoDirectionalLight::getClassTypeId().createInstance());
} // getLight()

void
SoXtDirectionalLightEditor::addLightChangedCallback(
  SoXtDirectionalLightEditorCB * callback,
  void * user )
{
  SOXT_STUB();
} // addLightChangedCallback()

void
SoXtDirectionalLightEditor::removeLightChangedCallback(
  SoXtDirectionalLightEditorCB * callback,
  void * user )
{
  SOXT_STUB();
} // removeLightChangedCallback()

void
SoXtDirectionalLightEditor::show( // virtual
  void )
{
  SOXT_STUB();
} // show()

void
SoXtDirectionalLightEditor::hide( // virtual
  void )
{
  SOXT_STUB();
} // hide()

// *************************************************************************

/*!
*/

const char *
SoXtDirectionalLightEditor::getDefaultWidgetName( // virtual, protected
  void ) const
{
  static const char defaultWidgetName[] = "SoXtDirectionalLightEditor";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
*/

const char *
SoXtDirectionalLightEditor::getDefaultTitle( // virtual, protected
  void ) const
{
  static const char defaultTitle[] = "Directional Light";
  return defaultTitle;
} // getDefaultTitle()

/*!
*/

const char *
SoXtDirectionalLightEditor::getDefaultIconTitle( // virtual, protected
  void ) const
{
  static const char defaultIconTitle[] = "Directional Light";
  return defaultIconTitle;
} // getDefaultIconTitle()

/*!
*/

void
SoXtDirectionalLightEditor::copyLight( // protected
  SoDirectionalLight * destination,
  const SoDirectionalLight * source )
{
  SOXT_STUB();
} // copyLight()

/*!
*/

void
SoXtDirectionalLightEditor::updateLocalComponents( // protected
  void )
{
  SOXT_STUB();
} // updateLocalComponents()

// *************************************************************************

/*!
*/

// this method will be moved to protected - no point otherwise...

Widget
SoXtDirectionalLightEditor::buildWidget( // private
  Widget parent )
{
  SOXT_STUB();
  return (Widget) NULL;
} // buildWidget()

/*!
*/

void
SoXtDirectionalLightEditor::constructor( // private
  SbBool build )
{
  SOXT_STUB();
} // constructor()

// *************************************************************************
