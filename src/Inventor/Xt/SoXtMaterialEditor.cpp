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
#include <Inventor/nodes/SoMaterial.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtMaterialEditor.h>

/*!
  \class SoXtMaterialEditor Inventor/Xt/SoXtMaterialEditor.h
  \brief The SoXtMaterialEditor class is yet to be documented.
*/

// *************************************************************************

/*!
*/

SoXtMaterialEditor::SoXtMaterialEditor(
  Widget parent,
  const char * name,
  SbBool inParent )
{
} // SoXtMaterialEditor()

/*!
*/

SoXtMaterialEditor::SoXtMaterialEditor( // protected
  Widget parent,
  const char * const name,
  SbBool inParent,
  SbBool build )
{
} // SoXtMaterialEditor()

/*!
*/

SoXtMaterialEditor::~SoXtMaterialEditor(
  void )
{
} // ~SoXtMaterialEditor()

// *************************************************************************

/*!
*/

void
SoXtMaterialEditor::attach(
  SoMaterial * material,
  int idx )
{
} // attach()

/*!
*/

void
SoXtMaterialEditor::attach(
  SoVRMLMaterial * material )
{
} // attach()

/*!
*/

void
SoXtMaterialEditor::detach(
  void )
{
} // detach()

/*!
*/

SbBool
SoXtMaterialEditor::isAttached(
  void )
{
  SOXT_STUB();
  return FALSE;
} // isAttached()

/*!
*/

void
SoXtMaterialEditor::addMaterialChangedCallback(
  SoXtMaterialEditorCB * callback,
  void * user )
{
} // addMaterialChangedCallback()

/*!
*/

void
SoXtMaterialEditor::removeMaterialChangedCallback(
  SoXtMaterialEditorCB * callback,
  void * user )
{
} // removeMaterialChangedCallback()

/*!
*/

void
SoXtMaterialEditor::setUpdateFrequency(
  SoXtMaterialEditor::UpdateFrequency freq )
{
} // setUpdateFrequency()

/*!
*/

SoXtMaterialEditor::UpdateFrequency
SoXtMaterialEditor::getUpdateFrequency(
  void ) const
{
  SOXT_STUB();
  return CONTINUOUS;
} // getUpdateFrequency()

/*!
*/

void
SoXtMaterialEditor::show( // virtual
  void )
{
} // show()

/*!
*/

void
SoXtMaterialEditor::hide( // virtual
  void )
{
} // hide()

/*!
*/

void
SoXtMaterialEditor::setMaterial(
  const SoMaterial & material )
{
} // setMaterial()

/*!
*/

void
SoXtMaterialEditor::setMaterial(
  const SoVRMLMaterial & material )
{
} // setMaterial()

/*!
*/

const SoMaterial &
SoXtMaterialEditor::getMaterial(
  void ) const
{
  SOXT_STUB();
  return *((SoMaterial *) SoMaterial::getClassTypeId().createInstance());
} // getMaterial()

/*!
*/

SbBool
SoXtMaterialEditor::isAttachedVRML(
  void )
{
  return FALSE;
} // isAttachedVRML()

// *************************************************************************

/*!
*/

Widget
SoXtMaterialEditor::buildWidget( // protected
  Widget parent )
{
  return (Widget) NULL;
} // buildWidget()

/*!
*/

const char *
SoXtMaterialEditor::getDefaultWidgetName( // virtual, protected
  void ) const
{
  static const char defaultWidgetName[] = "SoXtMaterialEditor";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
*/

const char *
SoXtMaterialEditor::getDefaultTitle( // virtual, protected
  void ) const
{
  static const char defaultTitle[] = "Material Editor";
  return defaultTitle;
} // getDefaultTitle()

/*!
*/

const char *
SoXtMaterialEditor::getDefaultIconTitle( // virtual, protected
  void ) const
{
  static const char defaultIconTitle[] = "Material Editor";
  return defaultIconTitle;
} // getDefaultIconTitle()

// *************************************************************************

/*!
*/

void
SoXtMaterialEditor::constructor(
  SbBool build )
{
} // constructor()

// *************************************************************************
