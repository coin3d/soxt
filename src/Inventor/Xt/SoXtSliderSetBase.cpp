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

