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

// *************************************************************************

/*!
  \class SoXtFlyViewer Inventor/Xt/viewers/SoXtFlyViewer.h
  \brief The SoXtFlyViewer class is for making the browsing experience more
  like in a flight simulator.
  \ingroup components viewers
*/

// *************************************************************************

#include <assert.h>

#include <Xm/Xm.h>

#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/viewers/SoAnyFlyViewer.h>
#include <Inventor/Xt/viewers/SoXtFlyViewer.h>

// *************************************************************************

/*!
  Public constructor.
*/

SoXtFlyViewer::SoXtFlyViewer( // public
  Widget parent,
  const char * const name,
  SbBool embed,
  SoXtFullViewer::BuildFlag flags,
  SoXtViewer::Type type )
: inherited( parent, name, embed, flags, type, FALSE )
, common( new SoAnyFlyViewer( this ) )
{
  this->constructor( TRUE );
} // SoXtFlyViewer()

/*!
  Protected constructor, used by SoXtFlyViewer-derived components.
*/

SoXtFlyViewer::SoXtFlyViewer( // protected
  Widget parent,
  const char * const name,
  SbBool embed,
  SoXtFullViewer::BuildFlag flags,
  SoXtViewer::Type type,
  SbBool build )
: inherited( parent, name, embed, flags, type, FALSE )
, common( new SoAnyFlyViewer( this ) )
{
  this->constructor( build );
} // SoXtFlyViewer()

/*!
  Common constructor code.
*/

void
SoXtFlyViewer::constructor( // private
  SbBool build )
{
  this->prefshell = NULL;
  this->prefsheet = NULL;
  this->prefparts = NULL;
  this->numprefparts = 0;

  this->setClassName( this->getDefaultWidgetName() );

  if ( build ) {
    Widget viewer = inherited::buildWidget( this->getParentWidget() );
    this->setBaseWidget( viewer );
    this->fitSize( SbVec2s( 500, 300 ) );
  }
} // constructor()

/*!
  Destructor.
*/

SoXtFlyViewer::~SoXtFlyViewer(
  void )
{
  delete [] this->prefparts;
  delete this->common;
} // ~SoXtFlyViewer()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtFlyViewer::setViewing( // virtual
  SbBool enable )
{
  inherited::setViewing( enable );
} // setViewing()

/*!
  FIXME: write doc
*/

void
SoXtFlyViewer::resetToHomePosition( // virtual
  void )
{
  inherited::resetToHomePosition();
} // resetToHomePosition()

/*!
  FIXME: write doc
*/

void
SoXtFlyViewer::setCamera( // virtual
  SoCamera * camera )
{
  inherited::setCamera( camera );
} // setCamera()

/*!
  FIXME: write doc
*/

void
SoXtFlyViewer::setCursorEnabled( // virtual
  SbBool enable )
{
  inherited::setCursorEnabled( enable );
} // setCursorEnabled()

/*!
  FIXME: write doc
*/

void
SoXtFlyViewer::setCameraType( // virtual
  SoType type )
{
  inherited::setCameraType( type );
  SOXT_STUB();
} // setCameraType()

// *************************************************************************

/*!
  FIXME: write doc
*/

const char *
SoXtFlyViewer::getDefaultWidgetName( // virtual, protected
  void ) const
{
  static const char defaultWidgetName[] = "SoXtFlyViewer";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
  FIXME: write doc
*/

const char *
SoXtFlyViewer::getDefaultTitle( // virtual, protected
  void ) const
{
  static const char defaultTitle[] = "Fly Viewer";
  return defaultTitle;
} // getDefaultTitle()

/*!
  FIXME: write doc
*/

const char *
SoXtFlyViewer::getDefaultIconTitle( // virtual, protected
  void ) const
{
  static const char defaultIconTitle[] = "Fly Viewer";
  return defaultIconTitle;
} // getDefaultIconTitle()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtFlyViewer::processEvent( // virtual, protected
  XAnyEvent * event )
{
  if ( this->processCommonEvents( event ) )
    return;

  inherited::processEvent( event );
} // processEvent()

/*!
  FIXME: write doc
*/

void
SoXtFlyViewer::setSeekMode( // virtual, protected
  SbBool enable )
{
  inherited::setSeekMode( enable );
} // setSeekMode()

/*!
  FIXME: write doc
*/

void
SoXtFlyViewer::actualRedraw( // virtual, protected
  void )
{
  inherited::actualRedraw();
  // add fly viewer graphics here
} // actualRedraw()

/*!
  FIXME: write doc
*/

void
SoXtFlyViewer::rightWheelMotion( // virtual, protected
  float value )
{
  common->dolly( value - this->getRightWheelValue() );
  inherited::rightWheelMotion( value );
} // rightWheelMotion()

/*!
  FIXME: write doc
*/

void
SoXtFlyViewer::createPrefSheet( // virtual, protected
  void )
{
  if ( ! this->prefshell ) {
    this->prefparts = new Widget [ 16 ];
    this->createPrefSheetShellAndForm( this->prefshell, this->prefsheet );
    this->createDefaultPrefSheetParts( this->prefparts, this->numprefparts,
      this->prefsheet );
    this->prefparts[this->numprefparts] =
      this->createFramedSpeedPrefSheetGuts( this->prefsheet );
    if ( this->prefparts[this->numprefparts] != NULL ) this->numprefparts++;
  }
  this->layoutPartsAndMapPrefSheet( this->prefparts, this->numprefparts,
    this->prefsheet, this->prefshell );
} // createPrefSheet()

/*!
  FIXME: write doc
*/

void
SoXtFlyViewer::openViewerHelpCard( // virtual, protected
  void )
{
  this->openHelpCard( "SoXtFlyViewer.help" );
} // openViewerHelpCard()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtFlyViewerRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

