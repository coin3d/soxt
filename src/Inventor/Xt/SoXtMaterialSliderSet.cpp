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

