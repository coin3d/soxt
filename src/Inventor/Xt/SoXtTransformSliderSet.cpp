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

