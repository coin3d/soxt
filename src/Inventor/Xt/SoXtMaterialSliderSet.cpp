/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

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
  SoNode * const node)
: inherited(parent, name, embed, node, FALSE)
{
  this->constructor(TRUE);
} // SoXtMaterialSliderSet()

/*!
  Constructor.
*/

SoXtMaterialSliderSet::SoXtMaterialSliderSet(// protected
  Widget parent,
  const char * const name,
  SbBool embed,
  SoNode * const node,
  SbBool build)
: inherited(parent, name, embed, node, FALSE)
{
  this->constructor(build);
} // SoXtMaterialSliderSet()

/*!
  Common code for all constructors.
*/

void
SoXtMaterialSliderSet::constructor(// private
  SbBool build)
{
  if (build) {
    this->setClassName("SoXtMaterialSliderSet");
    Widget sliderset = this->buildWidget(this->getParentWidget());
    this->setBaseWidget(sliderset);
    this->fitSize(SbVec2s(300, 0));
  }
} // constructor()

/*!
  Destructor.
*/

SoXtMaterialSliderSet::~SoXtMaterialSliderSet(
  void)
{
} // ~SoXtMaterialSliderSet()

// *************************************************************************

/*!
  FIXME: write doc
*/

Widget
SoXtMaterialSliderSet::buildWidget(
  Widget parent)
{
  return inherited::buildWidget(parent);
} // buildWidget()

// *************************************************************************

/*!
  FIXME: write doc
*/

const char *
SoXtMaterialSliderSet::getDefaultWidgetName(// virtual, protected
  void) const
{
  static const char defaultWidgetName[] = "SoXtMaterialSliderSet";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
  FIXME: write doc
*/

const char *
SoXtMaterialSliderSet::getDefaultTitle(// virtual, protected
  void) const
{
  static const char defaultTitle[] = "Material Sliders";
  return defaultTitle;
} // getDefaultTitle()

/*!
  FIXME: write doc
*/

const char *
SoXtMaterialSliderSet::getDefaultIconTitle(// virtual, protected
  void) const
{
  static const char defaultIconTitle[] = "Material Sliders";
  return defaultIconTitle;
} // getDefaultIconTitle()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtMaterialSliderSetRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

