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

static const char rcsid[] =
  "$Id$";

#include <Inventor/Xt/SoXtBasic.h>

#include <Inventor/Xt/SoXtSliderSetBase.h>

// *************************************************************************

/*!
*/

SoXtSliderSetBase::SoXtSliderSetBase( // protected
  Widget parent,
  const char * name,
  SbBool inParent,
  SoNode * newEditNode )
: inherited( parent, name )
{
} // SoXtSliderSetBase()

/*!
*/

SoXtSliderSetBase::~SoXtSliderSetBase( // protected
  void )
{
} // ~SoXtSliderSetBase()

// *************************************************************************

/*!
*/

void
SoXtSliderSetBase::setNode( // virtual
  SoNode * node )
{
  SOXT_STUB();
} // setNode()

/*!
*/

SoNode *
SoXtSliderSetBase::getNode(
  void ) const
{
  SOXT_STUB();
  return (SoNode *) NULL;
} // getNode()

/*!
*/

void
SoXtSliderSetBase::getLayoutSize( // virtual
  int & width,
  int & height )
{
  SOXT_STUB();
} // getLayoutSize()

/*!
*/

void
SoXtSliderSetBase::show( // virtual
  void )
{
  SOXT_STUB();
} // show()

// *************************************************************************
