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

#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>

#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtResource.h>

#include <Inventor/Xt/SoXtTransformSliderSet.h>

// *************************************************************************

/*!
  \class SoXtTransformSliderSet Inventor/Xt/SoXtTransformSliderSet.h
  \brief The SoXtTransformSliderSet class is yet to be documented.
*/

// *************************************************************************

SOXT_OBJECT_SOURCE(SoXtTransformSliderSet);

// *************************************************************************

/*!
  Constructor.
*/

SoXtTransformSliderSet::SoXtTransformSliderSet(
  Widget parent,
  const char * const name,
  const SbBool embed,
  SoNode * const node)
: inherited(parent, name, embed, node, FALSE)
{
  this->constructor(TRUE);
} // SoXtTransformSliderSet()

/*!
  Constructor.
*/

SoXtTransformSliderSet::SoXtTransformSliderSet(// protected
  Widget parent,
  const char * const name,
  const SbBool embed,
  SoNode * const node,
  const SbBool build)
: inherited(parent, name, embed, node, FALSE)
{
  this->constructor(build);
} // SoXtTransformSliderSet()

/*!
  Common constructor code.
*/

void
SoXtTransformSliderSet::constructor(// private
  SbBool build)
{
  if (build) {
    SoDebugError::postInfo("", "getWidgetName = %s", this->getWidgetName());
    this->setClassName(this->getWidgetName());
    Widget sliderset = this->buildWidget(this->getParentWidget());
    this->setBaseWidget(sliderset);
    this->fitSize(SbVec2s(300, 0));
  }
} // constructor()

/*!
  Destructor.
*/

SoXtTransformSliderSet::~SoXtTransformSliderSet(
  void)
{
} // ~SoXtTransformSliderSet()

// *************************************************************************

/*!
  FIXME: write code
*/

Widget
SoXtTransformSliderSet::buildWidget(// protected
  Widget parent)
{
  return inherited::buildWidget(parent);
} // buildWidget()

// *************************************************************************

/*!
  FIXME: write code
*/

const char *
SoXtTransformSliderSet::getDefaultWidgetName(// virtual, protected
  void) const
{
  static const char defaultWidgetName[] = "SoXtTransformSliderSet";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
  FIXME: write code
*/

const char *
SoXtTransformSliderSet::getDefaultTitle(// virtual, protected
  void) const
{
  static const char defaultTitle[] = "Transform Sliders";
  return defaultTitle;
} // getDefaultTitle()

/*!
  FIXME: write code
*/

const char *
SoXtTransformSliderSet::getDefaultIconTitle(// virtual, protected
  void) const
{
  static const char defaultIconTitle[] = "Transform Sliders";
  return defaultIconTitle;
} // getDefaultIconTitle()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtTransformSliderSetRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

