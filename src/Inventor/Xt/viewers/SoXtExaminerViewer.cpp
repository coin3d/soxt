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

#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>

// *************************************************************************

SoXtExaminerViewer::SoXtExaminerViewer(
  Widget parent,
  const char * name,
  SbBool inParent,
  SoXtFullViewer::BuildFlag flag,
  SoXtViewer::Type type )
: inherited( parent, name, inParent, flag, type, TRUE )
{
  this->constructor( TRUE );
  this->setViewing( FALSE );
} // SoXtExaminerViewer()

SoXtExaminerViewer::SoXtExaminerViewer( // protected
  Widget parent,
  const char * name,
  SbBool inParent,
  SoXtFullViewer::BuildFlag flag,
  SoXtViewer::Type type,
  SbBool build )
: inherited( parent, name, inParent, flag, type, FALSE )
{
  this->constructor( build );
} // SoXtExaminerViewer()

void
SoXtExaminerViewer::constructor( // private
  SbBool build )
{
// don't uncomment this until constructor sends FALSE to FullViewer class
//  if ( build )
//    this->setBaseWidget( this->buildWidget( this->getParentWidget() ) );
} // constructor()

/*!
  The destructor.
*/

SoXtExaminerViewer::~SoXtExaminerViewer(
  void )
{
} // ~SoXtExaminerViewer()

// *************************************************************************
