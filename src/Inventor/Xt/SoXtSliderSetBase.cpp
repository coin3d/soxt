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

