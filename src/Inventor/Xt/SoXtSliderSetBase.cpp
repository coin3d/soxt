/**************************************************************************
 *
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
 **************************************************************************/

#if SOXT_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOXT_DEBUG

#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>

#include <Inventor/Xt/SoXtSliderSetBase.h>

/*!
  \class SoXtSliderSetBase Inventor/Xt/SoXtSliderSetBase.h
  \brief The SoXtSliderSetBase class is yet to be documented.
*/

// *************************************************************************

/*!
*/

SoXtSliderSetBase::SoXtSliderSetBase( // protected
  Widget parent,
  const char * const name,
  SbBool embed,
  SoNode * const node )
: inherited( parent, name, embed )
{
  this->node = node;
} // SoXtSliderSetBase()

/*!
*/

SoXtSliderSetBase::~SoXtSliderSetBase( // protected
  void )
{
} // ~SoXtSliderSetBase()

// *************************************************************************

/*!
  This method sets the node the slider set is attached to.
*/

void
SoXtSliderSetBase::setNode( // virtual
  SoNode * node )
{
  this->node = node;
} // setNode()

/*!
  This method returns the node the sliderset is attached to, or NULL if there
  is no node attached.
*/

SoNode *
SoXtSliderSetBase::getNode(
  void ) const
{
  return this->node;
} // getNode()

/*!
*/

void
SoXtSliderSetBase::show( // virtual
  void )
{
  inherited::show();
} // show()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtSliderSetBaseRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

