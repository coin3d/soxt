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

#include <assert.h>

#include <Inventor/errors/SoDebugError.h>

#include <Inventor/Xt/viewers/SoAnyWalkViewer.h>
#include <Inventor/Xt/viewers/SoXtWalkViewer.h>

// *************************************************************************

/*!
*/

SoXtWalkViewer::SoXtWalkViewer(
  Widget parent,
  const char * const name,
  SbBool inParent,
  SoXtFullViewer::BuildFlag flag,
  SoXtViewer::Type type )
: inherited( parent, name, inParent, flag, type, FALSE )
, common( new SoAnyWalkViewer( this ) )
{
} // SoXtWalkViewer()

/*!
*/

SoXtWalkViewer::SoXtWalkViewer( // protected
  Widget parent,
  const char * const name,
  SbBool inParent,
  SoXtFullViewer::BuildFlag flag,
  SoXtViewer::Type type,
  SbBool build )
: inherited( parent, name, inParent, flag, type, FALSE )
, common( new SoAnyWalkViewer( this ) )
{
} // SoXtWalkViewer()

/*!
*/

void
SoXtWalkViewer::constructor(
  SbBool build )
{
} // constructor()

/*!
*/

SoXtWalkViewer::~SoXtWalkViewer(
  void )
{
  delete this->common;
} // ~SoXtWalkViewer()

// *************************************************************************

/*!
*/

void
SoXtWalkViewer::setViewing( // virtual
  SbBool enable )
{
  inherited::setViewing( enable );
} // setViewing()

/*!
*/

void
SoXtWalkViewer::setCamera( // virtual
  SoCamera * camera )
{
  inherited::setCamera( camera );
} // setCamera()

/*!
*/

void
SoXtWalkViewer::setCursorEnabled( // virtual
  SbBool enable )
{
  inherited::setCursorEnabled( enable );
} // setCursorEnabled()

/*!
*/

void
SoXtWalkViewer::setCameraType( // virtual
  SoType type )
{
} // setCameraType()

// *************************************************************************

/*!
*/

const char *
SoXtWalkViewer::getDefaultWidgetName( // virtual, protected
  void ) const
{
  static const char defaultWidgetName[] = "SoXtWalkViewer";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
*/

const char *
SoXtWalkViewer::getDefaultTitle( // virtual, protected
  void ) const
{
  static const char defaultTitle[] = "Walk Viewer";
  return defaultTitle;
} // getDefaultTitle()

/*!
*/

const char *
SoXtWalkViewer::getDefaultIconTitle( // virtual, protected
  void ) const
{
  static const char defaultIconTitle[] = "Walk Viewer";
  return defaultIconTitle;
} // getDefaultIconTitle()

// *************************************************************************

/*!
*/

void
SoXtWalkViewer::processEvent( // virtual, protected
  XAnyEvent * event )
{
  if ( this->processCommonEvents( event ) )
    return;

  inherited::processEvent( event );
} // processEvent()

/*!
*/

void
SoXtWalkViewer::setSeekMode( // virtual, protected
  SbBool enable )
{
} // setSeekMode()

/*!
*/

void
SoXtWalkViewer::actualRedraw( // virtual, protected
  void )
{
  inherited::actualRedraw();
  // add walk viewer gfx here
} // actualRedraw()

/*!
*/

void
SoXtWalkViewer::rightWheelMotion( // virtual, protected
  float value )
{
} // rightWheelMotion()

/*!
*/

Widget
SoXtWalkViewer::buildLeftTrim( // virtual, protected
  Widget parent )
{
} // buildLeftTrim()

/*!
*/

void
SoXtWalkViewer::createPrefSheet( // virtual, protected
  void )
{
} // createPrefSheet()

/*!
*/

void
SoXtWalkViewer::openViewerHelpCard( // virtual, protected
  void )
{
  this->openHelpCard( "SoXtWalkViewer.help" );
} // openViewerHelpCard()

// *************************************************************************
