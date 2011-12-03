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

// *************************************************************************

#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>

#include <Inventor/Xt/SoXtSliderSetBase.h>

// *************************************************************************

/*!
  \class SoXtSliderSetBase Inventor/Xt/SoXtSliderSetBase.h
  \brief The SoXtSliderSetBase class is yet to be documented.
*/

// *************************************************************************

SOXT_OBJECT_ABSTRACT_SOURCE(SoXtSliderSetBase);

// *************************************************************************

/*!
  Constructor.
*/

SoXtSliderSetBase::SoXtSliderSetBase(// protected
  const Widget parent,
  const char * const name,
  const SbBool embed,
  SoNode * const node)
: inherited(parent, name, embed)
{
  this->node = node;
} // SoXtSliderSetBase()

/*!
  Destructor.
*/

SoXtSliderSetBase::~SoXtSliderSetBase(// protected
  void)
{
} // ~SoXtSliderSetBase()

// *************************************************************************

/*!
  This method sets the node the slider set is attached to.
*/

void
SoXtSliderSetBase::setNode(// virtual
  SoNode * node)
{
  this->node = node;
} // setNode()

/*!
  This method returns the node the sliderset is attached to, or NULL if there
  is no node attached.
*/

SoNode *
SoXtSliderSetBase::getNode(
  void) const
{
  return this->node;
} // getNode()

/*!
  This method returns the layout size.

  Not implemented.
*/

void
SoXtSliderSetBase::getLayoutSize(// virtual
  int & width,
  int & height)
{
  SOXT_STUB();
}

/*!
  This method shows the component.
*/

void
SoXtSliderSetBase::show(// virtual
  void)
{
  inherited::show();
} // show()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtSliderSetBaseRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

