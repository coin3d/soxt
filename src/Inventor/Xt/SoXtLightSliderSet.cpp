/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

