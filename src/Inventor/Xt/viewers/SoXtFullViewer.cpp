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

#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoBasic.h>

/*
  SOXT_THUMBWHEELTEST
  - for enabling the testcode for the thumbwheel widget in development.
  Be aware; it is disabled for a reason...

  [bash$ CPPFLAGS="-DSOXT_THUMBWHEELTEST" ../../src/SoXt/configure]
*/

#ifdef SOXT_THUMBWHEELTEST
#include <Inventor/Xt/widgets/SoXtThumbWheel.h>
#endif // SOXT_THUMBWHEELTEST

#include <Inventor/Xt/common/soguidefs.h> // SOGUI_STUB()
#include <Inventor/Xt/viewers/SoXtFullViewer.h>

enum DefaultViewerButtons {
  INTERACT_BUTTON = 0,
  EXAMINE_BUTTON,
  HELP_BUTTON,
  HOME_BUTTON,
  SET_HOME_BUTTON,
  VIEW_ALL_BUTTON,
  SEEK_BUTTON
};

enum MenuIdValues {
  MENUTITLE_ITEM,

  FUNCTIONS_ITEM,
  HELP_ITEM,
  HOME_ITEM,
  SET_HOME_ITEM,
  VIEW_ALL_ITEM,
  SEEK_ITEM,
  COPY_VIEW_ITEM,
  PASTE_VIEW_ITEM,

  DRAWSTYLES_ITEM,
  AS_IS_ITEM,
  HIDDEN_LINE_ITEM,
  NO_TEXTURE_ITEM,
  LOW_RESOLUTION_ITEM,
  WIREFRAME_ITEM,
  POINTS_ITEM,
  BOUNDING_BOX_ITEM,
  MOVE_SAME_AS_STILL_ITEM,
  MOVE_NO_TEXTURE_ITEM,
  MOVE_LOW_RES_ITEM,
  MOVE_WIREFRAME_ITEM,
  MOVE_LOW_RES_WIREFRAME_ITEM,
  MOVE_POINTS_ITEM,
  MOVE_LOW_RES_POINTS_ITEM,
  MOVE_BOUNDING_BOX_ITEM,
  SINGLE_BUFFER_ITEM,
  DOUBLE_BUFFER_ITEM,
  INTERACTIVE_BUFFER_ITEM,

  EXAMINING_ITEM,
  DECORATION_ITEM,
  HEADLIGHT_ITEM,
  PREFERENCES_ITEM
};

// *************************************************************************

/*!
  Constructor.
*/

SoXtFullViewer::SoXtFullViewer( // protected
  Widget parent,
  const char * name,
  SbBool inParent,
  BuildFlag flags,
  Type type,
  SbBool build )
: inherited( parent, name, inParent, type, FALSE )
{
  this->viewerbase = NULL;
  this->canvas = NULL;

  char axisindicator[] = { 'Y', 'X', 'Z' };
  for ( int i = FIRSTDECORATION; i <= LASTDECORATION; i++) {
    this->wheelstrings[i] = "Motion ";
    this->wheelstrings[i] += axisindicator[i - FIRSTDECORATION];
    this->decorform[i] = NULL;
    this->wheellabels[i] = NULL;
  }

  this->zoomrange = SbVec2f( 1.0f, 140.0f );

  this->menuenabled = (flags & SoXtFullViewer::BUILD_POPUP) ? TRUE : FALSE;
  this->decorations = (flags & SoXtFullViewer::BUILD_DECORATION) ? TRUE : FALSE;

  this->appButtonsList = new SbPList;
  this->viewerButtonsList = new SbPList;

  this->setSize( SbVec2s( 500, 390 ) );
  this->setClassName( "SoXtFullViewer" );

  if ( build != FALSE )
    this->buildWidget( parent );
} // SoXtFullViewer()

/*!
  The destructor.
*/

SoXtFullViewer::~SoXtFullViewer( // protected
  void )
{
  delete this->appButtonsList;
  delete this->viewerButtonsList;
} // ~SoXtFullViewer()

// *************************************************************************

/*!
*/

void
SoXtFullViewer::setDecoration(
  const SbBool enable )
{

  if ( (this->decorations != enable) && (this->viewerbase != (Widget) NULL) )
    this->showDecorationWidgets( enable );
  this->decorations = enable;
} // setDecoration()

/*!
*/

SbBool
SoXtFullViewer::isDecoration(
  void ) const
{
  return this->decorations;
} // isDecoration()

// *************************************************************************

/*!
*/

void
SoXtFullViewer::setPopupMenuEnabled(
  const SbBool enable )
{
  this->menuenabled = enable;
  // FIXME: lazily create menu here? - or better; on first popup?
} // setPopupMenuEnabled()

/*!
*/

SbBool
SoXtFullViewer::isPopupMenuEnabled(
  void ) const
{
  return this->menuenabled;
} // isPopupMenuEnabled()

// *************************************************************************

/*!
  Return value is either NULL or a widget of type xmFormWidgetClass (Xm/Form.h)
*/

Widget
SoXtFullViewer::getAppPushButtonParent(
  void ) const
{
  return this->appButtonsForm;
} // getAppPushButtonParent()

/*!
*/

void
SoXtFullViewer::addAppPushButton(
  Widget button )
{
  this->appButtonsList->append( button );
  this->layoutAppPushButtons( this->getAppPushButtonParent() );
} // addAppPushButton()

/*!
*/

void
SoXtFullViewer::insertAppPushButton(
  Widget button,
  int idx )
{
  this->appButtonsList->insert( button, idx );
  this->layoutAppPushButtons( this->getAppPushButtonParent() );
} // insertAppPushButton()

/*!
*/

void
SoXtFullViewer::removeAppPushButton(
  Widget button )
{
  int idx = this->appButtonsList->find( button );
  if ( idx == -1 ) {
#if SOXT_DEBUG
    SoDebugError::postWarning( "SoXtFullViewer::removeAppPushButton",
                               "tried to remove non-existant button" );
#endif // SOXT_DEBUG
    return;
  }
  this->appButtonsList->remove( idx );
  this->layoutAppPushButtons( this->getAppPushButtonParent() );
} // removeAppPushButton()

/*!
*/

int
SoXtFullViewer::findAppPushButton(
  Widget button ) const
{
  return this->appButtonsList->find( button );
} // findAppPushButton()

/*!
*/

int
SoXtFullViewer::lengthAppPushButton(
  void ) const
{
  return this->appButtonsList->getLength();
} // lengthAppPushButton()

/*!
*/

void
SoXtFullViewer::layoutAppPushButtons(
  Widget parent )
{
  SOGUI_STUB();
} // layoutAppPushButtons()

// *************************************************************************

/*!
*/

Widget
SoXtFullViewer::getRenderAreaWidget(
  void ) const
{
  return this->canvas;
} // getRenderAreaWidget()

// *************************************************************************

/*!
*/

Widget
SoXtFullViewer::buildWidget( // protected
  Widget parent )
{
  this->viewerbase = XtVaCreateManagedWidget( "ViewerRoot",
      xmFormWidgetClass, parent, NULL );

  this->buildDecoration( this->viewerbase );
  
  this->canvas = inherited::buildWidget( this->viewerbase );

  XtVaSetValues( this->canvas,
      XmNtopAttachment, XmATTACH_FORM,
      XmNleftAttachment, XmATTACH_FORM,
      XmNleftOffset, 30,
      XmNbottomAttachment, XmATTACH_FORM,
      XmNbottomOffset, 30,
      XmNrightAttachment, XmATTACH_FORM,
      XmNrightOffset, 30,
      NULL );
  this->setBorder( FALSE );

  XtManageChild( this->canvas );
  this->setBaseWidget( this->viewerbase );
  return this->viewerbase;
} // buildWidget()

// *************************************************************************

void
SoXtFullViewer::buildDecoration( // virtual
  Widget parent )
{
  this->decorform[BOTTOMDECORATION] = this->buildBottomTrim( parent );
  XtManageChild( this->decorform[BOTTOMDECORATION] );
  this->decorform[LEFTDECORATION]   = this->buildLeftTrim( parent );
  XtManageChild( this->decorform[LEFTDECORATION] );
  this->decorform[RIGHTDECORATION]  = this->buildRightTrim( parent );
  XtManageChild( this->decorform[RIGHTDECORATION] );
} // buildDecorations()

/*!
*/

Widget
SoXtFullViewer::buildBottomTrim( // virtual
  Widget parent )
{
  Widget trim = XtVaCreateManagedWidget( "BottomTrim",
      xmFormWidgetClass, parent,
      XmNleftAttachment, XmATTACH_FORM,
      XmNrightAttachment, XmATTACH_FORM,
      XmNbottomAttachment, XmATTACH_FORM,
      XmNheight, 30,
      NULL );

  Widget label1 = XtVaCreateManagedWidget( "Roty", xmLabelWidgetClass,
    trim, XmNleftAttachment, XmATTACH_FORM, XmNtopAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNleftOffset, 5,
    NULL );

  Widget label2 = XtVaCreateManagedWidget( "Rotx", xmLabelWidgetClass,
    trim, XmNleftAttachment, XmATTACH_WIDGET, XmNleftWidget, label1,
    XmNtopAttachment, XmATTACH_FORM, XmNbottomAttachment, XmATTACH_FORM,
    XmNleftOffset, 5,
    NULL );

#ifdef SOXT_THUMBWHEELTEST
  // add right thumb wheel
  Widget wheel = XtVaCreateManagedWidget( "BottomWheel",
    soXtThumbWheelWidgetClass, trim,
    XmNorientation, XmHORIZONTAL,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, label2,
    XmNtopAttachment, XmATTACH_FORM,
    XmNtopOffset, 2,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNbottomOffset, 2,
    XmNheight, 90,
    NULL );
#endif // SOXT_THUMBWHEELTEST

  Widget label3 = XtVaCreateManagedWidget( "Dolly", xmLabelWidgetClass,
    trim, XmNtopAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM, XmNrightAttachment, XmATTACH_FORM,
    XmNrightOffset, 5, NULL );

  return trim;
} // buildBottomTrim()

/*!
*/

Widget
SoXtFullViewer::buildLeftTrim( // virtual
  Widget parent )
{
  Widget trim = XtVaCreateManagedWidget( "LeftTrim",
      xmFormWidgetClass, parent,
      XmNwidth, 30,
      XmNleftAttachment, XmATTACH_FORM,
      XmNtopAttachment, XmATTACH_FORM,
      XmNbottomAttachment, XmATTACH_WIDGET,
      XmNbottomWidget, this->decorform[BOTTOMDECORATION],
      NULL );

  // build application buttons
  this->appButtonsForm = this->buildAppButtonsForm( trim );

  XtVaSetValues( this->appButtonsForm,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    NULL );
  XtManageChild( this->appButtonsForm );

#ifdef SOXT_THUMBWHEELTEST
  // add right thumb wheel
  Widget wheel = XtVaCreateManagedWidget( "LeftWheel",
    soXtThumbWheelWidgetClass, trim,
    XmNorientation, XmVERTICAL,
    XmNleftAttachment, XmATTACH_FORM,
    XmNleftOffset, 2,
    XmNrightAttachment, XmATTACH_FORM,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNbottomOffset, 2,
    XmNheight, 90,
    NULL );
#endif // SOXT_THUMBWHEELTEST

  return trim;
} // buildLeftTrim()

/*!
*/

Widget
SoXtFullViewer::buildRightTrim( // virtual
  Widget parent )
{
  Widget trim = XtVaCreateManagedWidget( "RightTrim",
      xmFormWidgetClass, parent,
      XmNwidth, 30,
      XmNtopAttachment, XmATTACH_FORM,
      XmNrightAttachment, XmATTACH_FORM,
      XmNbottomAttachment, XmATTACH_WIDGET,
      XmNbottomWidget, this->decorform[BOTTOMDECORATION],
      NULL );

  Widget buttonForm = this->buildViewerButtons( trim );
  XtManageChild( buttonForm );

#ifdef SOXT_THUMBWHEELTEST
  // add right thumb wheel
  Widget wheel = XtVaCreateManagedWidget( "RightWheel",
    soXtThumbWheelWidgetClass, trim,
    XmNorientation, XmVERTICAL,
    XmNleftAttachment, XmATTACH_FORM,
    XmNleftOffset, 2,
    XmNrightAttachment, XmATTACH_FORM,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNbottomOffset, 2,
    XmNheight, 90,
    NULL );
#endif // SOXT_THUMBWHEELTEST

  return trim;
} // buildRightTrim()

// *************************************************************************

/*!
*/

void
SoXtFullViewer::setViewing( // virtual
  SbBool enable )
{
#if SOXT_DEBUG
  if ( this->isViewing() == enable )
    SoDebugError::postWarning( "SoXtFullViewer::setViewing",
      "current state already that of argument" );
#endif // SOXT_DEBUG

  inherited::setViewing( enable );
//  if ( this->prefmenu )
//    this->prefmenu ...
} // setViewing()

/*!
*/

void
SoXtFullViewer::setHeadlight( // virtual
  SbBool enable )
{
  inherited::setHeadlight( enable );
//  if ( this->prefmenu )
//    this->prefmenu->setItemChecked( HEADLIGHT_ITEM, enable );
} // setHeadlight()

/*!
*/

void
SoXtFullViewer::setDrawStyle( // virtual
  SoXtViewer::DrawType type,
  SoXtViewer::DrawStyle style )
{
  inherited::setDrawStyle( type, style );
//  if ( this->prefmenu )
//    this->setDrawStyleMenuActivation( type, style );
} // setDrawStyle()

/*!
*/

void
SoXtFullViewer::setBufferingType( // virtual
  SoXtViewer::BufferType type )
{
  inherited::setBufferingType( type );

//  if ( this->prefmenu ) {
//
} // setBufferingType()

/*!
*/

void
SoXtFullViewer::setCamera( // virtual
  SoCamera * camera )
{
  inherited::setCamera( camera );

#if 0
  if ( this->prefwindow ) {
    this->setZoomSliderPosition( this->getCameraZoom() );
    this->setZoomFieldString( this->getCameraZoom() );

    SbBool enable = camera ? TRUE : FALSE;
    this->zoomslider->setEnabled( enable );
    this->zoomfield->setEnabled( enable );
    this->zoomrangefrom->setEnabled( enable );
    this->zoomrangeto->setEnabled( enable );
  }
#endif // 0
} // setCamera()

/*!
*/

void
SoXtFullViewer::hide( // virtual
  void )
{
  inherited::hide();
//  if ( this->prefwindow )
//    SoXt::hide( this->prefwindow );
} // hide()

// *************************************************************************

/*!
*/

SbBool
SoXtFullViewer::eventFilter( // virtual
  Widget widget,
  XAnyEvent * event )
{
  SoDebugError::postInfo( "SoXtFullViewer::eventFilter",
    "widget = 0x%08x, event->type = %d", widget, event->type );
//  inherited::eventFilter( widget, event );
  SOGUI_STUB();
  // catch close events
  // activate popupmenu
  return FALSE;
} // eventFilter()

// *************************************************************************

/*!
*/

Widget
SoXtFullViewer::buildViewerButtons(
  Widget parent )
{
  Widget form = XtVaCreateManagedWidget( "ViewerButtons",
      xmFormWidgetClass, parent,
      XmNleftAttachment, XmATTACH_FORM,
      XmNtopAttachment, XmATTACH_FORM,
      XmNrightAttachment, XmATTACH_FORM,
      NULL );

  this->createViewerButtons( form, this->viewerButtonsList );

  for ( int i = 0; i < this->viewerButtonsList->getLength(); i++ ) {
    Widget button = (Widget) (*this->viewerButtonsList)[i];
    if ( i == 0 )
      XtVaSetValues( button,
         XmNleftOffset, 0,
         XmNtopOffset, 0,
         XmNrightOffset, 0,
         XmNbottomOffset, 0,
         XmNleftAttachment, XmATTACH_FORM,
         XmNtopAttachment, XmATTACH_FORM,
         XmNrightAttachment, XmATTACH_FORM,
         XmNwidth, 30, XmNheight, 30,
         NULL );
    else
      XtVaSetValues( button,
         XmNleftOffset, 0,
         XmNtopOffset, 0,
         XmNrightOffset, 0,
         XmNbottomOffset, 0,
         XmNleftAttachment, XmATTACH_FORM,
         XmNtopAttachment, XmATTACH_WIDGET,
         XmNtopWidget, (Widget) (*this->viewerButtonsList)[i-1],
         XmNrightAttachment, XmATTACH_FORM,
         XmNwidth, 30, XmNheight, 30,
         NULL );
  }

  return form;
} // buildViewerButtons()

/*!
*/

void
SoXtFullViewer::createViewerButtons(
  Widget parent,
  SbPList * buttonlist )
{
  assert( buttonlist != NULL );
  for ( int i = 0; i <= SEEK_BUTTON; i++ ) {
    Widget button = XtVaCreateManagedWidget( "b",
        xmPushButtonWidgetClass, parent, NULL );
    buttonlist->append( button );
  }
} // createViewerButtons()

/*!
*/

void
SoXtFullViewer::buildPopupMenu(
  void )
{
  SOGUI_STUB();
} // buildPopupMenu()

/*!
*/

Widget
SoXtFullViewer::makeSubPreferences(
  Widget parent )
{
  SOGUI_STUB();
  return (Widget) NULL;
} // makeSubPreferences()

// *************************************************************************

/*!
*/

void
SoXtFullViewer::leftWheelStart( // virtual
  void )
{
  this->interactiveCountInc();
} // leftWheelStart()

/*!
*/

void
SoXtFullViewer::leftWheelMotion( // virtual
  float value )
{
  SOGUI_STUB();
} // leftWheelMotion()

/*!
*/

void
SoXtFullViewer::leftWheelFinish( // virtual
  void )
{
  this->interactiveCountDec();
} // leftWheelFinished()

/*!
*/

float
SoXtFullViewer::getLeftWheelValue(
  void ) const
{
  return this->wheelvalues[LEFTDECORATION];
} // getLeftWheelValue()

// *************************************************************************

/*!
*/

void
SoXtFullViewer::bottomWheelStart( // virtual
  void )
{
  this->interactiveCountInc();
} // bottomWheelStart()

/*!
*/

void
SoXtFullViewer::bottomWheelMotion( // virtual
  float value )
{
  SOGUI_STUB();
} // bottomWheelMode()

/*!
*/

void
SoXtFullViewer::bottomWheelFinish( // virtual
  void )
{
  this->interactiveCountDec();
} // bottomWheelFinish()

/*!
*/

float
SoXtFullViewer::getBottomWheelValue(
  void ) const
{
  return this->wheelvalues[BOTTOMDECORATION];
} // getBottomWheelValue()

// *************************************************************************

/*!
*/

void
SoXtFullViewer::rightWheelStart( // virtual
  void )
{
  this->interactiveCountInc();
} // rightWheelStart()

/*!
*/

void
SoXtFullViewer::rightWheelMotion( // virtual
  float value )
{
  SOGUI_STUB();
} // rightWheelMotion()

/*!
*/

void
SoXtFullViewer::rightWheelFinish( // virtual
  void )
{
  this->interactiveCountDec();
} // rightWheelFinish()

/*!
*/

float
SoXtFullViewer::getRightWheelValue(
  void ) const
{
  SOGUI_STUB();
  return 0.0f;
} // getRightWheelValue()

// *************************************************************************

/*!
  Default implementation does nothing.
*/

void
SoXtFullViewer::openViewerHelpCard( // virtual
  void )
{
} // openViewerHelpCard()

// *************************************************************************

/*!
*/

Widget
SoXtFullViewer::buildAppButtonsForm(
  Widget parent )
{
  Widget form = XtVaCreateManagedWidget( "appbuttons", xmFormWidgetClass,
    parent, NULL );

  return form;
} // buildAppButtonsForm()

// *************************************************************************

void
SoXtFullViewer::showDecorationWidgets(
  SbBool enable )
{
  assert( this->canvas != (Widget) NULL );
  assert( this->decorform[0] != (Widget) NULL );
  assert( this->decorform[1] != (Widget) NULL );
  assert( this->decorform[2] != (Widget) NULL );

  if ( ! enable ) {
    XtRealizeWidget( this->decorform[0] );
    XtManageChild( this->decorform[0] );

    XtRealizeWidget( this->decorform[1] );
    XtManageChild( this->decorform[1] );

    XtRealizeWidget( this->decorform[2] );
    XtManageChild( this->decorform[2] );

    XtVaSetValues( this->canvas,
      XmNleftOffset, 30, XmNrightOffset, 30, XmNbottomOffset, 30, NULL );
  } else {
    XtUnrealizeWidget( this->decorform[0] );
    XtUnrealizeWidget( this->decorform[1] );
    XtUnrealizeWidget( this->decorform[2] );
    XtVaSetValues( this->canvas,
      XmNleftOffset, 0, XmNrightOffset, 0, XmNbottomOffset, 0, NULL );
  }
} // showDecorationWidgets()

// *************************************************************************
