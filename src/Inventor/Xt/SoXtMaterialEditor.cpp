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

// *************************************************************************

/*!
  \class SoXtMaterialEditor Inventor/Xt/SoXtMaterialEditor.h
  \brief The SoXtMaterialEditor class is yet to be documented.
  \ingroup components editors

  This class is not implemented yet.
*/

// *************************************************************************

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoMaterial.h>

#include <soxtdefs.h>

#include <Inventor/Xt/SoXtMaterialEditor.h>

// *************************************************************************

/*!
  \enum SoXtMaterialEditor::UpdateFrequency
  FIXME: write doc
*/

// *************************************************************************

/*!
  Constructor.
*/

SoXtMaterialEditor::SoXtMaterialEditor(
  Widget parent,
  const char * name,
  SbBool embed )
: inherited( parent, name, embed )
{
} // SoXtMaterialEditor()

/*!
  Constructor.
*/

SoXtMaterialEditor::SoXtMaterialEditor( // protected
  Widget parent,
  const char * const name,
  SbBool embed,
  SbBool build )
: inherited( parent, name, embed )
{
} // SoXtMaterialEditor()

/*!
  Destructor.
*/

SoXtMaterialEditor::~SoXtMaterialEditor(
  void )
{
} // ~SoXtMaterialEditor()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtMaterialEditor::attach(
  SoMaterial * material,
  int idx )
{
} // attach()

/*!
  FIXME: write doc
*/

void
SoXtMaterialEditor::attach(
  SoVRMLMaterial * material )
{
} // attach()

/*!
  FIXME: write doc
*/

void
SoXtMaterialEditor::detach(
  void )
{
} // detach()

/*!
  FIXME: write doc
*/

SbBool
SoXtMaterialEditor::isAttached(
  void )
{
  SOXT_STUB();
  return FALSE;
} // isAttached()

/*!
  FIXME: write doc
*/

void
SoXtMaterialEditor::addMaterialChangedCallback(
  SoXtMaterialEditorCB * callback,
  void * user )
{
} // addMaterialChangedCallback()

/*!
  FIXME: write doc
*/

void
SoXtMaterialEditor::removeMaterialChangedCallback(
  SoXtMaterialEditorCB * callback,
  void * user )
{
} // removeMaterialChangedCallback()

/*!
  FIXME: write doc
*/

void
SoXtMaterialEditor::setUpdateFrequency(
  SoXtMaterialEditor::UpdateFrequency freq )
{
} // setUpdateFrequency()

/*!
  FIXME: write doc
*/

SoXtMaterialEditor::UpdateFrequency
SoXtMaterialEditor::getUpdateFrequency(
  void ) const
{
  SOXT_STUB();
  return CONTINUOUS;
} // getUpdateFrequency()

/*!
  FIXME: write doc
*/

void
SoXtMaterialEditor::show( // virtual
  void )
{
} // show()

/*!
  FIXME: write doc
*/

void
SoXtMaterialEditor::hide( // virtual
  void )
{
} // hide()

/*!
  FIXME: write doc
*/

void
SoXtMaterialEditor::setMaterial(
  const SoMaterial & material )
{
} // setMaterial()

/*!
  FIXME: write doc
*/

void
SoXtMaterialEditor::setMaterial(
  const SoVRMLMaterial & material )
{
} // setMaterial()

/*!
  FIXME: write doc
*/

const SoMaterial &
SoXtMaterialEditor::getMaterial(
  void ) const
{
  SOXT_STUB();
  return *((SoMaterial *) SoMaterial::getClassTypeId().createInstance());
} // getMaterial()

/*!
  FIXME: write doc
*/

SbBool
SoXtMaterialEditor::isAttachedVRML(
  void )
{
  return FALSE;
} // isAttachedVRML()

// *************************************************************************

/*!
  FIXME: write doc
*/

Widget
SoXtMaterialEditor::buildWidget( // protected
  Widget parent )
{
  return (Widget) NULL;
} // buildWidget()

/*!
  FIXME: write doc
*/

const char *
SoXtMaterialEditor::getDefaultWidgetName( // virtual, protected
  void ) const
{
  static const char defaultWidgetName[] = "SoXtMaterialEditor";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
  FIXME: write doc
*/

const char *
SoXtMaterialEditor::getDefaultTitle( // virtual, protected
  void ) const
{
  static const char defaultTitle[] = "Material Editor";
  return defaultTitle;
} // getDefaultTitle()

/*!
  FIXME: write doc
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
  Common constructor code.
*/

void
SoXtMaterialEditor::constructor(
  SbBool build )
{
} // constructor()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtMaterialEditorRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

