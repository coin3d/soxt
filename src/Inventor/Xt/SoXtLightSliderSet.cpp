/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#if SOXT_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOXT_DEBUG

#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>

#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtResource.h>

#include <Inventor/Xt/SoXtLightSliderSet.h>

// *************************************************************************

/*!
  \class SoXtLightSliderSet Inventor/Xt/SoXtLightSliderSet.h
  \brief The SoXtLightSliderSet class is a component for modifying light nodes.
  \ingroup components slidersets

  This component is designed for editing SoLight nodes.
  One slider is created for adjusting the intensity of the light,
  while another slider set is created for adjusting the light color.
*/

// *************************************************************************

SOXT_OBJECT_SOURCE(SoXtLightSliderSet);

// *************************************************************************

/*!
  Constructor.
*/

SoXtLightSliderSet::SoXtLightSliderSet(
  const Widget parent,
  const char * const name,
  const SbBool embed,
  SoNode * const node)
: inherited(parent, name, embed, node, FALSE)
{
  this->constructor(TRUE);
} // SoXtLightSliderSet()

/*!
  Constructor.
*/

SoXtLightSliderSet::SoXtLightSliderSet(// protected
  const Widget parent,
  const char * const name,
  const SbBool embed,
  SoNode * const node,
  const SbBool build)
: inherited(parent, name, embed, node, FALSE)
{
  this->constructor(build);
} // SoXtLightSliderSet()

/*!
  Common code for all constructors.
*/

void
SoXtLightSliderSet::constructor(// private
  const SbBool build)
{
  if (build) {
    this->setClassName("SoXtLightSliderSet");
    Widget sliderset = this->buildWidget(this->getParentWidget());
    this->setBaseWidget(sliderset);
    this->fitSize(SbVec2s(300, 0));
  }
} // constructor()

/*!
  Destructor.
*/

SoXtLightSliderSet::~SoXtLightSliderSet(
  void)
{
} // ~SoXtLightSliderSet()

// *************************************************************************

/*!
  FIXME: build widget
*/

Widget
SoXtLightSliderSet::buildWidget(// protected
  Widget parent)
{
  return inherited::buildWidget(parent);
} // buildWidget()

// *************************************************************************

/*!
  FIXME: build widget
*/

const char *
SoXtLightSliderSet::getDefaultWidgetName(// virtual, protected
  void) const
{
  static const char defaultWidgetName[] = "SoXtLightSliderSet";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
  FIXME: build widget
*/

const char *
SoXtLightSliderSet::getDefaultTitle(// virtual, protected
  void) const
{
  static const char defaultTitle[] = "Light Editor";
  return defaultTitle;
} // getDefaultTitle()

/*!
  FIXME: build widget
*/

const char *
SoXtLightSliderSet::getDefaultIconTitle(// virtual, protected
  void) const
{
  static const char defaultIconTitle[] = "Light Editor";
  return defaultIconTitle;
} // getDefaultIconTitle()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtLightSliderSetRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

