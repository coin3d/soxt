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

#include <assert.h>

#include <Xm/Xm.h>
#include <Xm/Label.h>

#include <Inventor/errors/SoDebugError.h>

#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/widgets/SoXtThumbWheel.h>
#include <Inventor/Xt/widgets/SoAnyPopupMenu.h>
#include <Inventor/Xt/viewers/SoAnyWalkViewer.h>
#include <Inventor/Xt/viewers/SoXtWalkViewer.h>

// *************************************************************************

/*!
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
*/

void
SoXtWalkViewer::constructor(
  SbBool build )
{
  this->prefparts = NULL;
  this->prefshell = NULL;
  this->prefsheet = NULL;
  this->numprefparts = 0;
  this->tiltwheel = NULL;
  this->heightwheel = NULL;
  this->heightvalue = 0.0f;

  this->setClassName( this->getDefaultWidgetName() );
  if ( build ) {
    Widget viewer = this->buildWidget( this->getParentWidget() );
    this->setBaseWidget( viewer );
    XtVaSetValues( viewer,
      XmNleftAttachment, XmATTACH_FORM,
      XmNtopAttachment, XmATTACH_FORM,
      XmNrightAttachment, XmATTACH_FORM,
      XmNbottomAttachment, XmATTACH_FORM,
      NULL );
    // resources
  }
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
  SOXT_STUB();
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
*/

void
SoXtWalkViewer::setSeekMode( // virtual, protected
  SbBool enable )
{
  inherited::setSeekMode( enable );
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
*/

Widget
SoXtWalkViewer::buildLeftTrim( // virtual, protected
  Widget parent )
{
  Widget form = inherited::buildLeftTrim( parent );

  this->tiltwheel = this->getThumbWheel( LEFTDECORATION );

  Widget label = XtVaCreateManagedWidget( "label",
    xmLabelWidgetClass, form,
    XmNleftAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_WIDGET,
    XmNbottomWidget, tiltwheel,
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
*/

void
SoXtWalkViewer::openViewerHelpCard( // virtual, protected
  void )
{
  this->openHelpCard( "SoXtWalkViewer.help" );
} // openViewerHelpCard()

// *************************************************************************

/*!
*/

void
SoXtWalkViewer::leftWheel2Start( // virtual, protected
  void )
{
} // leftWheel2Start()

/*!
*/

void
SoXtWalkViewer::leftWheel2Motion( // virtual, protected
  float value )
{
  common->elevateCamera( value - this->getLeftWheel2Value() );
  this->heightvalue = value;
} // rightWheelMotion()

/*!
*/

void
SoXtWalkViewer::leftWheel2Finish( // virtual, protected
  void )
{
} // leftWheel2Finish()

/*!
*/

float
SoXtWalkViewer::getLeftWheel2Value( // virtual, protected
  void ) const
{
  return this->heightvalue;
} // getLeftWheel2Value()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtWalkViewer,
  wheelarmed )
{
  this->leftWheel2Start();
} // wheelarmed()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtWalkViewer,
  wheelchanged )
{
  SoXtThumbWheelCallbackData * data = (SoXtThumbWheelCallbackData *) call_data;
  this->leftWheel2Motion( data->current );
} // wheelchanged()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtWalkViewer,
  wheeldisarmed )
{
  this->leftWheel2Finish();
} // wheeldisarmed()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtWalkViewerRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

