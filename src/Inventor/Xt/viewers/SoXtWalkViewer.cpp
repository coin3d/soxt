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

#include <Xm/Xm.h>
#include <Xm/Label.h>

#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/widgets/SoXtThumbWheel.h>
#include <Inventor/Xt/widgets/SoAnyPopupMenu.h>
#include <Inventor/Xt/viewers/SoAnyWalkViewer.h>
#include <Inventor/Xt/viewers/SoXtWalkViewer.h>

// *************************************************************************

/*!
  \class SoXtWalkViewer Inventor/Xt/viewers/SoXtWalkViewer.h
  \brief The SoXtWalkViewer class is a viewer for giving a "walk"-like
  experience in the scene.
  \ingroup components viewers
*/

// *************************************************************************

SOXT_OBJECT_SOURCE(SoXtWalkViewer);

// *************************************************************************

/*!
  Public constructor.
*/

SoXtWalkViewer::SoXtWalkViewer(
  Widget parent,
  const char * const name,
  SbBool embed,
  SoXtFullViewer::BuildFlag flag,
  SoXtViewer::Type type )
: inherited( parent, name, embed, flag, type, FALSE )
, common( new SoAnyWalkViewer( this ) )
{
  this->constructor( TRUE );
} // SoXtWalkViewer()

/*!
  Protected constructor for derived classes.
*/

SoXtWalkViewer::SoXtWalkViewer( // protected
  Widget parent,
  const char * const name,
  SbBool embed,
  SoXtFullViewer::BuildFlag flag,
  SoXtViewer::Type type,
  SbBool build )
: inherited( parent, name, embed, flag, type, FALSE )
, common( new SoAnyWalkViewer( this ) )
{
  this->constructor( build );
} // SoXtWalkViewer()

/*!
  Common constructor code.
*/

void
SoXtWalkViewer::constructor(
  SbBool build )
{
  this->prefparts = NULL;
  this->prefshell = NULL;
  this->prefsheet = NULL;
  this->numprefparts = 0;
  this->heightwheel = NULL;
  this->heightvalue = 0.0f;

  this->setClassName( this->getDefaultWidgetName() );
  if ( build ) {
    Widget viewer = this->buildWidget( this->getParentWidget() );
    this->setBaseWidget( viewer );
    this->fitSize( SbVec2s( 500, 300 ) );
    // resources
  }
} // constructor()

/*!
  Destructor.
*/

SoXtWalkViewer::~SoXtWalkViewer(
  void )
{
  delete this->common;
} // ~SoXtWalkViewer()

// *************************************************************************

/*!
  FIXME: write doc.
*/

void
SoXtWalkViewer::setViewing( // virtual
  SbBool enable )
{
  inherited::setViewing( enable );
} // setViewing()

/*!
  FIXME: write doc.
*/

void
SoXtWalkViewer::setCamera( // virtual
  SoCamera * camera )
{
  inherited::setCamera( camera );
} // setCamera()

/*!
  FIXME: write doc.
*/

void
SoXtWalkViewer::setCursorEnabled( // virtual
  SbBool enable )
{
  inherited::setCursorEnabled( enable );
} // setCursorEnabled()

/*!
  FIXME: write doc.
*/

void
SoXtWalkViewer::setCameraType( // virtual
  SoType type )
{
  SOXT_STUB();
} // setCameraType()

// *************************************************************************

/*!
  FIXME: write doc.
*/

const char *
SoXtWalkViewer::getDefaultWidgetName( // virtual, protected
  void ) const
{
  static const char defaultWidgetName[] = "SoXtWalkViewer";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
  FIXME: write doc.
*/

const char *
SoXtWalkViewer::getDefaultTitle( // virtual, protected
  void ) const
{
  static const char defaultTitle[] = "Walk Viewer";
  return defaultTitle;
} // getDefaultTitle()

/*!
  FIXME: write doc.
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
  FIXME: write doc.
*/

void
SoXtWalkViewer::processEvent( // virtual, protected
  XAnyEvent * event )
{
  if ( this->processCommonEvents( event ) )
    return;

  if ( this->isViewing() && event->type == ButtonPress ) {
    XButtonEvent * bevent = (XButtonEvent *) event;
    if ( bevent->button == 3 && this->isPopupMenuEnabled() ) {
      if ( ! this->prefmenu )
        this->buildPopupMenu();
      this->prefmenu->PopUp( this->getParentWidget(),
        bevent->x_root, bevent->y_root );
      return;
    }
  }

  inherited::processEvent( event );
} // processEvent()

/*!
  FIXME: write doc.
*/

void
SoXtWalkViewer::setSeekMode( // virtual, protected
  SbBool enable )
{
  inherited::setSeekMode( enable );
} // setSeekMode()

/*!
  FIXME: write doc.
*/

void
SoXtWalkViewer::actualRedraw( // virtual, protected
  void )
{
  inherited::actualRedraw();
  // add walk viewer gfx here
} // actualRedraw()

/*!
  This method hooks up the right wheel to dolly the camera back and forth.
*/

void
SoXtWalkViewer::rightWheelMotion( // virtual, protected
  float value )
{
  common->dollyCamera( value - this->getRightWheelValue() );
  inherited::rightWheelMotion( value );
} // rightWheelMotion()

// *************************************************************************

/*!
  FIXME: write doc.
*/

Widget
SoXtWalkViewer::buildLeftTrim( // virtual, protected
  Widget parent )
{
  Widget form = inherited::buildLeftTrim( parent );

//  this->tiltwheel = this->getLeftThumbWheel();

  Widget label = XtVaCreateManagedWidget( "label",
    xmLabelWidgetClass, form,
    XmNleftAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_WIDGET,
    XmNbottomWidget, this->leftWheel,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "H", 2,
    NULL );

  this->heightwheel = XtVaCreateManagedWidget( "heightwheel",
    soxtThumbWheelWidgetClass, form,
    XmNleftAttachment, XmATTACH_FORM,
    XmNleftOffset, 2,
    XmNrightAttachment, XmATTACH_FORM,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_WIDGET,
    XmNbottomWidget, label,
    XmNheight, 90,
    XmNorientation, XmVERTICAL,
    NULL );

  XtAddCallback( this->heightwheel, XmNarmCallback,
    SoXtWalkViewer::wheelarmedCB, (XtPointer) this );
  XtAddCallback( this->heightwheel, XmNdisarmCallback,
    SoXtWalkViewer::wheeldisarmedCB, (XtPointer) this );
  XtAddCallback( this->heightwheel, XmNvalueChangedCallback,
    SoXtWalkViewer::wheelchangedCB, (XtPointer) this );

  return form;
} // buildLeftTrim()

/*!
  FIXME: write doc.
*/

void
SoXtWalkViewer::createPrefSheet( // virtual, protected
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
  FIXME: write doc.
*/

void
SoXtWalkViewer::openViewerHelpCard( // virtual, protected
  void )
{
  this->openHelpCard( "SoXtWalkViewer.help" );
} // openViewerHelpCard()

// *************************************************************************

/*!
  FIXME: write doc.
*/

void
SoXtWalkViewer::leftWheel2Start( // virtual, protected
  void )
{
} // leftWheel2Start()

/*!
  FIXME: write doc.
*/

void
SoXtWalkViewer::leftWheel2Motion( // virtual, protected
  float value )
{
  common->elevateCamera( value - this->getLeftWheel2Value() );
  this->heightvalue = value;
} // rightWheelMotion()

/*!
  FIXME: write doc.
*/

void
SoXtWalkViewer::leftWheel2Finish( // virtual, protected
  void )
{
} // leftWheel2Finish()

/*!
  FIXME: write doc.
*/

float
SoXtWalkViewer::getLeftWheel2Value( // virtual, protected
  void ) const
{
  return this->heightvalue;
} // getLeftWheel2Value()

/*!
  FIXME: write doc
*/

void
SoXtWalkViewer::setLeftWheel2String(
  char * string )
{
  SOXT_STUB();
} // setLeftWheel2String()

/*!
  FIXME: write doc.
*/

void
SoXtWalkViewer::wheelarmed(
  void )
{
  this->leftWheel2Start();
} // wheelarmed()

/*!
  static callback
*/

void
SoXtWalkViewer::wheelarmedCB( // static
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtWalkViewer * viewer = (SoXtWalkViewer *) closure;
  viewer->wheelarmed();
} // wheelarmedCB()

/*!
  FIXME: write doc.
*/

void
SoXtWalkViewer::wheelchanged(
  float value )
{
  this->leftWheel2Motion( value );
} // wheelchanged()

/*!
  static callback
*/

void
SoXtWalkViewer::wheelchangedCB( // static
  Widget,
  XtPointer closure,
  XtPointer call_data )
{
  SoXtWalkViewer * viewer = (SoXtWalkViewer *) closure;
  SoXtThumbWheelCallbackData * data = (SoXtThumbWheelCallbackData *) call_data;
  viewer->wheelchanged( data->current );
} // wheelchangedCB()

/*!
  FIXME: write doc.
*/

void
SoXtWalkViewer::wheeldisarmed(
  void )
{
  this->leftWheel2Finish();
} // wheeldisarmed()

/*!
  static callback
*/

void
SoXtWalkViewer::wheeldisarmedCB(
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtWalkViewer * viewer = (SoXtWalkViewer *) closure;
  viewer->wheeldisarmed();
} // wheeldisarmedCB()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtWalkViewerRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

