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

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <X11/IntrinsicP.h>

#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/DialogS.h>
#include <Xm/RowColumn.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/Scale.h>

#include <Inventor/misc/SoBasic.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtResource.h>
#include <Inventor/Xt/widgets/SoXtThumbWheel.h>
#include <Inventor/Xt/widgets/SoAnyPopupMenu.h>

#include <Inventor/Xt/viewers/SoAnyFullViewer.h>
#include <Inventor/Xt/viewers/SoXtFullViewer.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#if HAVE_LIBXPM
#include <X11/xpm.h>
#include <Inventor/Xt/common/pixmaps/pick.xpm>
#include <Inventor/Xt/common/pixmaps/view.xpm>
#include <Inventor/Xt/common/pixmaps/help.xpm>
#include <Inventor/Xt/common/pixmaps/home.xpm>
#include <Inventor/Xt/common/pixmaps/set_home.xpm>
#include <Inventor/Xt/common/pixmaps/view_all.xpm>
#include <Inventor/Xt/common/pixmaps/seek.xpm>
#endif // HAVE_LIBXPM

#ifndef M_PI
#define M_PI 3.1415926535f
#endif // ! M_PI

// *************************************************************************

/*!
  \class SoXtFullViewer Inventor/Xt/SoXtFullViewer.h
  \brief The SoXtFullViewer class adds GUI decorations to the viewer component.
  \ingroup components viewers
*/

// *************************************************************************

SOXT_OBJECT_ABSTRACT_SOURCE(SoXtFullViewer);

// *************************************************************************

enum DefaultViewerButtons {
  INTERACT_BUTTON = 0,
  EXAMINE_BUTTON,
  HELP_BUTTON,
  HOME_BUTTON,
  SET_HOME_BUTTON,
  VIEW_ALL_BUTTON,
  SEEK_BUTTON,

  FIRST_BUTTON = INTERACT_BUTTON,
  LAST_BUTTON = SEEK_BUTTON
};

#define VIEWERBUTTON(button) ((Widget) ((*this->viewerButtonWidgets)[button]))

// *************************************************************************

/*!
  \var float SoXtFullViewer::leftWheelVal

  The value of the left thumb wheel.  Do not set directly, use access method.
*/

/*!
  \var Widget SoXtFullViewer::leftWheel

  The widget for the left thumb wheel.
*/

/*!
  \var Widget SoXtFullViewer::leftWheelLabel

  The widget for the label below the left thumb wheel.
*/

/*!
  \var char * SoXtFullViewer::leftWheelStr

  The string used in the label under the left thumb wheel.
*/

/*!
  \var float SoXtFullViewer::bottomWheelVal

  The value of the bottom thumb wheel.  Do not set directly, use access method.
*/

/*!
  \var Widget SoXtFullViewer::bottomWheel

  The widget for the bottom thumb wheel.
*/

/*!
  \var Widget SoXtFullViewer::bottomWheelLabel

  The widget for the label besides the bottom thumb wheel.
*/

/*!
  \var char * SoXtFullViewer::bottomWheelStr

  The string used in the label besides the bottom thumb wheel.
*/

/*!
  \var float SoXtFullViewer::rightWheelVal

  The value of the right thumb wheel.  Do not set directly, use access method.
*/

/*!
  \var Widget SoXtFullViewer::rightWheel

  The widget for the right thumb wheel.
*/

/*!
  \var Widget SoXtFullViewer::rightWheelLabel

  The widget for the label below the right thumb wheel.
*/

/*!
  \var char * SoXtFullViewer::rightWheelStr

  The string used in the label below the right thumb wheel.
*/

/*!
  \enum SoXtFullViewer::BuildFlag

  Flags for building various parts of the viewer.
*/

/*!
  \var SbPList * SoXtFullViewer::viewerButtonWidgets

  The list of button widgets for the right viewer decoration.
*/

/*!
  \var Widget SoXtFullViewer::prefsheet

  The preferences sheet.
*/

/*!
  \var Widget SoXtFullViewer::prefshell

  The shell for the preferences window.
*/

/*!
  \var char * SoXtFullViewer::prefstring

  The title string for the preferences window.
*/

/*!
  \var SbBool SoXtFullViewer::popupEnabled

  Whether the popup menu is enabled or not.
*/

/*!
  \var SoAnyPopupMenu * SoXtFullViewer::prefmenu

  The popup menu manager object.
*/

/*!
  \var char * SoXtFullViewer::popupTitle

  The title for the popup menu,
*/


// *************************************************************************

/*!
  Constructor.
*/

SoXtFullViewer::SoXtFullViewer( // protected
  Widget parent,
  const char * name,
  SbBool embed,
  BuildFlag flags,
  Type type,
  SbBool build )
: inherited( parent, name, embed, type, FALSE )
, popupTitle( NULL )
, common( new SoAnyFullViewer( this ) )
{
  // initialization of protected members
  this->leftWheel = NULL;
  this->leftWheelStr = strcpy( new char [sizeof("Motion X")+1], "Motion X" );
  this->leftWheelVal = 0.0f;
  this->leftWheelLabel = NULL;

  this->bottomWheel = NULL;
  this->bottomWheelStr = strcpy( new char [sizeof("Motion Y")+1], "Motion Y" );
  this->bottomWheelVal = 0.0f;
  this->bottomWheelLabel = NULL;

  this->rightWheel = NULL;
  this->rightWheelStr = strcpy( new char [sizeof("Motion Z")+1], "Motion Z" );
  this->rightWheelVal = 0.0f;
  this->rightWheelLabel = NULL;

  // initialization of private members
  this->leftDecoration = NULL;
  this->rightDecoration = NULL;
  this->bottomDecoration = NULL;

  this->viewerbase = NULL;
  this->canvas = NULL;

  this->seekdistance = 50.0f;
  this->seekdistaspercentage = TRUE;

  this->zoomrange = SbVec2f( 1.0f, 140.0f );

  this->popupEnabled = (flags & SoXtFullViewer::BUILD_POPUP) ? TRUE : FALSE;
  this->decorations = (flags & SoXtFullViewer::BUILD_DECORATION) ? TRUE : FALSE;

  this->appButtonsList = new SbPList;
  this->viewerButtonWidgets = new SbPList;

  this->prefshell = NULL;
  this->prefmenu = NULL;

  pixmaps.pick = 0;
  pixmaps.pick_ins = 0;
  pixmaps.view = 0;
  pixmaps.view_ins = 0;
  pixmaps.help = 0;
  pixmaps.help_ins = 0;
  pixmaps.home = 0;
  pixmaps.home_ins = 0;
  pixmaps.set_home = 0;
  pixmaps.set_home_ins = 0;
  pixmaps.view_all = 0;
  pixmaps.view_all_ins = 0;
  pixmaps.seek = 0;
  pixmaps.seek_ins = 0;

  if ( build != FALSE ) {
    this->setClassName( "SoXtFullViewer" );
    Widget viewer = this->buildWidget( this->getParentWidget() );
    this->setBaseWidget( viewer );
    this->fitSize( SbVec2s( 500, 300 ) );
  }
} // SoXtFullViewer()

/*!
  The destructor.
*/

SoXtFullViewer::~SoXtFullViewer( // protected
  void )
{
  delete this->common;
  delete this->appButtonsList;
  delete this->viewerButtonWidgets;
} // ~SoXtFullViewer()

// *************************************************************************

static const int SOXT_VIEWER_MIN_HEIGHT_BASE = 30 + /* 90 */ 122 + 8;
static const int SOXT_VIEWER_MIN_WIDTH = 300;

/*!
  This method sets whether the decorations should be shown or not.
*/

void
SoXtFullViewer::setDecoration(
  const SbBool enable )
{
  if ( (this->decorations != enable) && (this->viewerbase != (Widget) NULL) )
    this->showDecorationWidgets( enable );
  this->decorations = enable;

  if ( this->isTopLevelShell() || XtIsShell(XtParent(this->getBaseWidget())) ) {
    Widget shell = this->getShellWidget();
    Dimension minwidth = SOXT_VIEWER_MIN_WIDTH;
    Dimension minheight = SOXT_VIEWER_MIN_HEIGHT_BASE +
       30 * this->viewerButtonWidgets->getLength();
    if ( enable ) {
      Dimension width, height;
      XtVaGetValues( shell, 
        XmNwidth, &width,
        XmNheight, &height,
        NULL );
      width = SoXtMax( width, minwidth );
      height = SoXtMax( height, minheight );
      XtVaSetValues( shell,
        XmNminWidth, minwidth,
        XmNminHeight, minheight,
        XmNwidth, width,
        XmNheight, height,
        NULL );
    } else {
      XtVaSetValues( shell,
        XmNminWidth, 0,
        XmNminHeight, 0,
        NULL );
    }
  }
} // setDecoration()

/*!
  This method returns whether the decorations are shown or not.
*/

SbBool
SoXtFullViewer::isDecoration(
  void ) const
{
  return this->decorations;
} // isDecoration()

// *************************************************************************

/*!
  This method sets whether the popup menu should be enabled or not.
*/

void
SoXtFullViewer::setPopupMenuEnabled(
  const SbBool enable )
{
  this->popupEnabled = enable;
} // setPopupMenuEnabled()

/*!
  This method returns whether the popup menu is enabled or not.
*/

SbBool
SoXtFullViewer::isPopupMenuEnabled(
  void ) const
{
  return this->popupEnabled;
} // isPopupMenuEnabled()


/*!
  Set title of popup menu.
*/
void
SoXtFullViewer::setPopupMenuString(
  const char * const title )
{
  this->common->setPopupMenuString( str );
} // setPopupMenuString()


// *************************************************************************

/*!
  Return value is either NULL or a widget of type xmFormWidgetClass (Xm/Form.h)

  AppPushButtons are on the left decoration.
*/

Widget
SoXtFullViewer::getAppPushButtonParent(
  void ) const
{
  return this->appButtonsForm;
} // getAppPushButtonParent()

/*!
  Not implemented.

  Each button will be set to 30x30 pixels, and they will appear on the left
  viewer decoration.
*/

void
SoXtFullViewer::addAppPushButton(
  Widget button )
{
  this->appButtonsList->append( button );
  this->layoutAppPushButtons( this->getAppPushButtonParent() );
} // addAppPushButton()

/*!
  FIXME: write doc
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
  FIXME: write doc
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
  FIXME: write doc
*/

int
SoXtFullViewer::findAppPushButton(
  Widget button ) const
{
  return this->appButtonsList->find( button );
} // findAppPushButton()

/*!
  FIXME: write doc
*/

int
SoXtFullViewer::lengthAppPushButton(
  void ) const
{
  return this->appButtonsList->getLength();
} // lengthAppPushButton()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::layoutAppPushButtons(
  Widget parent )
{
  SOXT_STUB();
} // layoutAppPushButtons()

// *************************************************************************

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::getRenderAreaWidget(
  void ) const
{
  return this->canvas;
} // getRenderAreaWidget()

// *************************************************************************

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::buildWidget( // protected
  Widget parent )
{
  int depth = 0;
  XtVaGetValues( parent, XmNdepth, &depth, NULL );

  this->viewerbase = XtVaCreateManagedWidget( this->getWidgetName(),
    xmFormWidgetClass, parent,
    NULL );
  this->registerWidget( this->viewerbase );

  char * titleString = NULL;
  SoXtResource rsc( this->viewerbase );
  rsc.getResource( "title", XmRString, titleString );
  if ( titleString != NULL )
    this->setTitle( titleString );

  this->canvas = inherited::buildWidget( this->viewerbase );
  XtVaSetValues( this->canvas,
      XmNleftAttachment, XmATTACH_FORM,
      XmNleftOffset, 30,
      XmNtopAttachment, XmATTACH_FORM,
      XmNrightAttachment, XmATTACH_FORM,
      XmNrightOffset, 30,
      XmNbottomAttachment, XmATTACH_FORM,
      XmNbottomOffset, 30,
      NULL );

  this->buildDecoration( this->viewerbase );
  
  if ( this->isTopLevelShell() &&
       this->decorations != FALSE ) {
    Widget shell = this->getShellWidget();
    if ( shell != NULL ) {
      Dimension minheight =
        30 + 122 + 30 * this->viewerButtonWidgets->getLength() + 8;
      Dimension width, height;
      XtVaGetValues( shell,
        XmNwidth, &width,
        XmNheight, &height,
        NULL );
      width = SoXtMax( width, (Dimension) 300 );
      height = SoXtMax( height, minheight );
      XtVaSetValues( shell,
        XmNminWidth, 300,
        XmNminHeight, minheight,
        XmNwidth, width,
        XmNheight, height,
        NULL );
    }
  }
  return this->viewerbase;
} // buildWidget()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::buildDecoration( // virtual
  Widget parent )
{
  this->leftDecoration = this->buildLeftTrim( parent );
  XtVaSetValues( this->leftDecoration,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_OPPOSITE_FORM,
    XmNrightOffset, -30,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNbottomOffset, 30,
    NULL );

  this->rightDecoration = this->buildRightTrim( parent );
  XtVaSetValues( this->rightDecoration,
    XmNleftAttachment, XmATTACH_OPPOSITE_FORM,
    XmNleftOffset, -30,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNbottomOffset, 30,
    NULL );

  this->bottomDecoration = this->buildBottomTrim( parent );
  XtVaSetValues( this->bottomDecoration,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_OPPOSITE_FORM,
    XmNtopOffset, -30,
    XmNrightAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    NULL );
} // buildDecorations()

// *************************************************************************

/*!
  This method creates the left decoration form.

  Return value is a Motif widget of the xmFormWidgetClass type.
  The left decoration is 30 pixels wide.
*/

Widget
SoXtFullViewer::buildLeftTrim( // virtual
  Widget parent )
{
  Widget trim = XtVaCreateManagedWidget( "LeftTrim",
      xmFormWidgetClass, parent,
      NULL );

#if 0
  // build application buttons
  this->appButtonsForm = this->buildAppButtonsForm( trim );
  XtVaSetValues( this->appButtonsForm,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    NULL );
#endif

  // add left thumb wheel
  this->leftWheel = XtVaCreateManagedWidget( "LeftWheel",
    soxtThumbWheelWidgetClass, trim,
    XmNleftAttachment, XmATTACH_FORM,
    XmNleftOffset, 2,
    XmNrightAttachment, XmATTACH_FORM,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNbottomOffset, 2,
    XmNheight, 122,
    XmNorientation, XmVERTICAL,
    XmNshadowType, XmSHADOW_OUT,
    XmNhighlightThickness, 0,
    XmNshadowThickness, 2,
    XmNtraversalOn, False,
    NULL );

  XtAddCallback( this->leftWheel,
    XmNarmCallback, SoXtFullViewer::leftWheelStartCB, this );
  XtAddCallback( this->leftWheel,
    XmNdisarmCallback, SoXtFullViewer::leftWheelFinishCB, this );
  XtAddCallback( this->leftWheel,
    XmNvalueChangedCallback, SoXtFullViewer::leftWheelMotionCB, this );

  return trim;
} // buildLeftTrim()

// *************************************************************************

/*!
  This method creates the right decoration form.

  Returned value is a Motif widget of the xmFormWidgetClass type.
  The form is 30 pixels wide.
*/

Widget
SoXtFullViewer::buildRightTrim( // virtual
  Widget parent )
{
  Widget trim = XtVaCreateManagedWidget( "RightTrim",
    xmFormWidgetClass, parent,
    NULL );

  Widget buttons = this->buildViewerButtons( trim );

  XtVaSetValues( buttons,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_OPPOSITE_FORM,
    XmNbottomOffset, 0 - this->viewerButtonWidgets->getLength() * 30,
    NULL );

  // add right thumb wheel
  this->rightWheel = XtVaCreateManagedWidget( "RightWheel",
    soxtThumbWheelWidgetClass, trim,
    XmNleftAttachment, XmATTACH_FORM,
    XmNleftOffset, 2,
    XmNrightAttachment, XmATTACH_FORM,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNbottomOffset, 2,
    XmNheight, 122,
    XmNorientation, XmVERTICAL,
    XmNshadowType, XmSHADOW_OUT,
    XmNhighlightThickness, 0,
    XmNshadowThickness, 2,
    XmNtraversalOn, False,
    NULL );

  XtAddCallback( this->rightWheel,
    XmNarmCallback, SoXtFullViewer::rightWheelStartCB, this );
  XtAddCallback( this->rightWheel,
    XmNdisarmCallback, SoXtFullViewer::rightWheelFinishCB, this );
  XtAddCallback( this->rightWheel,
    XmNvalueChangedCallback, SoXtFullViewer::rightWheelMotionCB, this );

  return trim;
} // buildRightTrim()

// *************************************************************************

/*!
  This method created the bottom decoration form.

  Return value is a Motif widget of the xmFormWidgetClass type.
  It is 30 pixels high.
*/

Widget
SoXtFullViewer::buildBottomTrim( // virtual
  Widget parent )
{
  Widget trim = XtVaCreateManagedWidget( "BottomTrim",
    xmFormWidgetClass, parent,
    NULL );

  this->leftWheelLabel = XtVaCreateManagedWidget( "LeftWheelLabel",
    xmLabelWidgetClass, trim,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNleftOffset, 5,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      this->leftWheelStr, strlen(this->leftWheelStr)+1,
    NULL );

  this->bottomWheelLabel = XtVaCreateManagedWidget( "BottomWheelLabel",
    xmLabelWidgetClass, trim,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, this->leftWheelLabel,
    XmNtopAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNleftOffset, 5,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      this->bottomWheelStr, strlen(this->bottomWheelStr)+1,
    NULL );

  // add bottom thumb wheel
  this->bottomWheel = XtVaCreateManagedWidget( "BottomWheel",
    soxtThumbWheelWidgetClass, trim,
    XmNorientation, XmHORIZONTAL,
    XmNshadowType, XmSHADOW_OUT,
    XmNhighlightThickness, 0,
    XmNshadowThickness, 2,
    XmNtraversalOn, False,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, this->bottomWheelLabel,
    XmNleftOffset, 2,
    XmNtopAttachment, XmATTACH_FORM,
    XmNtopOffset, 2,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNbottomOffset, 2,
    XmNwidth, 122,
    NULL );

  XtAddCallback( this->bottomWheel,
    XmNarmCallback, SoXtFullViewer::bottomWheelStartCB, this );
  XtAddCallback( this->bottomWheel,
    XmNdisarmCallback, SoXtFullViewer::bottomWheelFinishCB, this );
  XtAddCallback( this->bottomWheel,
    XmNvalueChangedCallback, SoXtFullViewer::bottomWheelMotionCB, this );

  this->rightWheelLabel = XtVaCreateManagedWidget( "RightWheelLabel",
    xmLabelWidgetClass, trim,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, this->bottomWheel,
    XmNalignment, XmALIGNMENT_END,
    XmNtopAttachment, XmATTACH_OPPOSITE_FORM,
    XmNtopOffset, -30,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XmNrightOffset, 5,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      this->rightWheelStr, strlen(this->rightWheelStr)+1,
    NULL );

  return trim;
} // buildBottomTrim()

// *************************************************************************

/*!
  This method sets whether the viewer should be in view mode or interaction
  mode.
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

  if ( this->viewerbuttons.pick != 0 ) {
    XtSetSensitive( this->viewerbuttons.pick, enable ? True : False );
    XtVaSetValues( this->viewerbuttons.pick,
      XmNset, enable ? False : True,
      NULL );
  }

  if ( this->viewerbuttons.pick != 0 ) {
    XtSetSensitive( this->viewerbuttons.view, enable ? False : True );
    XtVaSetValues( this->viewerbuttons.view,
      XmNset, enable ? True : False,
      NULL );
  }

  if ( this->viewerbuttons.seek != 0 ) {
    XtSetSensitive( this->viewerbuttons.seek, enable ? True : False );
  }
} // setViewing()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::setCamera( // virtual
  SoCamera * camera )
{
  inherited::setCamera( camera );

  if ( this->prefshell != NULL ) {
//    this->setZoomSliderPosition( this->getCameraZoom() );
//    this->setZoomFieldString( this->getCameraZoom() );

    Boolean enable = False;
    if ( camera ) {
      SoType camtype = camera->getTypeId();
      if ( camtype.isDerivedFrom( SoPerspectiveCamera::getClassTypeId() ) )
        enable = True;
    }
    XtVaSetValues( this->zoomfrom,
      XmNsensitive, enable,
      XmNeditable, enable,
      XmNcursorPositionVisible, enable,
      NULL );
    XtVaSetValues( this->zoomslider,
      XmNsensitive, enable,
      XmNeditable, enable,
      NULL );
    XtVaSetValues( this->zoomto,
      XmNsensitive, enable,
      XmNeditable, enable,
      XmNcursorPositionVisible, enable,
      NULL );
    XtVaSetValues( this->zoomvalue,
      XmNsensitive, enable,
      XmNeditable, enable,
      XmNcursorPositionVisible, enable,
      NULL );
  }
} // setCamera()

/*!
  FIXME: write doc
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
  FIXME: write doc
*/

Widget
SoXtFullViewer::buildViewerButtons(
  Widget parent )
{
  Widget buttons = XtVaCreateWidget( "ViewerButtons",
    xmRowColumnWidgetClass, parent,
    XmNrowColumnType, XmWORK_AREA,
    XmNpacking, XmPACK_COLUMN,
    XmNnumColumns, 1,
    XmNspacing, 0,
    XmNmarginWidth, 0,
    XmNmarginHeight, 0,
    NULL );

  this->createViewerButtons( buttons, this->viewerButtonWidgets );

  XtSetSensitive( this->viewerbuttons.pick, this->isViewing() ? True : False );
  XtVaSetValues( this->viewerbuttons.pick,
    XmNset, this->isViewing() ? False : True,
    NULL );

  XtSetSensitive( this->viewerbuttons.view, this->isViewing() ? False : True );
  XtVaSetValues( this->viewerbuttons.view,
                 XmNset, this->isViewing() ? True : False,
                 NULL );

  const int numbuttons = this->viewerButtonWidgets->getLength();
  for ( int i = 0; i < numbuttons; i++ ) {
    Widget button = (Widget) (*this->viewerButtonWidgets)[i];
    XtVaSetValues( button,
      XmNshadowType, XmSHADOW_OUT,
      XmNhighlightThickness, 0,
      XmNshadowThickness, 2,
      XmNtraversalOn, False,
      XmNmarginWidth, 0,
      XmNmarginHeight, 0,
      XmNmarginLeft, 0,
      XmNmarginTop, 0,
      XmNmarginRight, 0,
      XmNmarginBottom, 0,
      XmNrecomputeSize, False,
      XmNwidth, 28,
      XmNheight, 28,
      NULL );
  }

/*
  XtVaSetValues( buttons,
    XmNwidth, 30,
    XmNheight, numbuttons * 31,
    NULL );
*/

  XtManageChild( buttons );
  return buttons;
} // buildViewerButtons()

// *************************************************************************

/*!
  \internal
*/

#ifdef HAVE_LIBXPM
static const char *
_xpmErrorString(
  int error )
{
  switch ( error ) {
  case XpmSuccess:      return "success";
  case XpmColorError:   return "color error";
  case XpmOpenFailed:   return "open failed";
  case XpmFileInvalid:  return "file invalid";
  case XpmNoMemory:     return "no memory";
  case XpmColorFailed:  return "color failed";
  default:              return "<unknown>";
  } // switch ( error )
} // _xpmStringError()
#endif

/*
  \internal
  Does nothing if libXpm use hasn't been enabled.
*/

Pixmap
SoXtFullViewer::createPixmapFromXpmData(
  Widget widget,
  char ** xpm )
{
  Pixmap pixels = 0;
#if HAVE_LIBXPM

  Widget shell = widget;
  while ( ! XtIsShell( shell ) && shell != (Widget) NULL )
    shell = XtParent(shell);
  assert( shell != (Widget) NULL );
  Display * dpy = XtDisplay( shell );

  XpmAttributes attrs;
  attrs.visual = NULL;
  attrs.colormap = 0;
  attrs.depth = 0;

  XtVaGetValues( shell,
    XmNcolormap, &attrs.colormap,
    XmNdepth,    &attrs.depth,
    XmNvisual,   &attrs.visual,
    NULL );

  attrs.valuemask = XpmVisual | XpmColormap | XpmDepth;

#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtFullViewer::createPixmapFromXpmData",
    "visualinfo: %p, %d, %d", attrs.visual, attrs.colormap, attrs.depth );
#endif // SOXT_DEBUG

  Drawable draw = RootWindow( dpy, DefaultScreen(dpy) );
  Pixmap stencil = 0;
  int error = XpmCreatePixmapFromData( dpy, draw, xpm,
                &pixels, &stencil, &attrs );

  if ( error != XpmSuccess ) {
#if SOXT_DEBUG
    SoDebugError::postInfo( "SoXtFullViewer::createPixmapFromXpmData",
      "XpmCreateImageFromData failed: %s", _xpmErrorString( error ) );
#endif // SOXT_DEBUG
    return 0;
  }

  if ( stencil ) {
    Pixel bg;
    XtVaGetValues( widget, XmNbackground, &bg, NULL );

    XImage * pixmap = XGetImage( dpy, pixels, 0, 0, attrs.width, attrs.height,
      0xffffffff, ZPixmap );
    XImage * mask = XGetImage( dpy, stencil, 0, 0, attrs.width, attrs.height,
      0xffffffff, ZPixmap );
    assert( pixmap != NULL && mask != NULL );

    for ( unsigned int x = 0; x < attrs.width; x++ ) {
      for ( unsigned int y = 0; y < attrs.height; y++ ) {
        Pixel pixel = XGetPixel( mask, x, y );
        if ( pixel == 0 ) // background must be set in image
          XPutPixel( pixmap, x, y, bg );
      }
    }

    GC temp = XCreateGC( dpy, pixels, 0, NULL );
    XPutImage( dpy, pixels, temp, pixmap,
      0, 0, 0, 0, attrs.width, attrs.height );
    XFreeGC( dpy, temp );

    XDestroyImage( pixmap );
    XDestroyImage( mask );
  }

#endif // HAVE_LIBXPM
  return pixels;
} // createPixmapFromXpmData()

/*!
  \internal
  Does nothing if libXpm use hasn't been enabled.
*/

Pixmap
SoXtFullViewer::createInsensitivePixmapFromXpmData(
  Widget widget,
  char ** xpm )
{
  Pixmap pixels = 0;

#if HAVE_LIBXPM
  Widget shell = widget;
  while ( ! XtIsShell( shell ) && widget != (Widget) NULL )
    shell = XtParent( shell );
  assert( shell != (Widget) NULL );

  Display * dpy = XtDisplay( shell );

  XpmAttributes attrs;
  attrs.visual = NULL;
  attrs.colormap = 0;
  attrs.depth = 0;

  XtVaGetValues( shell,
    XmNcolormap, &attrs.colormap,
    XmNdepth,    &attrs.depth,
    XmNvisual,   &attrs.visual,
    NULL );

  attrs.valuemask = XpmVisual | XpmColormap | XpmDepth;

  Drawable draw = RootWindow( dpy, DefaultScreen(dpy) );
  Pixmap stencil = 0;
  int error = XpmCreatePixmapFromData( dpy, draw, xpm,
     &pixels, &stencil, &attrs );

  if ( error != XpmSuccess ) {
#if SOXT_DEBUG
    SoDebugError::postInfo(
      "SoXtFullViewer::createInsensitivePixmapFromXpmData",
      "XpmCreatePixmapFromData() failed: %s", _xpmErrorString( error ) );
#endif // SOXT_DEBUG
    return (Pixmap) 0;
  }

  if ( stencil ) {
    Pixel bg;
    XtVaGetValues( widget, XmNbackground, &bg, NULL );

    XImage * pixmap = XGetImage( dpy, pixels, 0, 0, attrs.width, attrs.height,
      0xffffffff, ZPixmap );
    XImage * mask = XGetImage( dpy, stencil, 0, 0, attrs.width, attrs.height,
      0xffffffff, ZPixmap );
    assert( pixmap != NULL && mask != NULL );

    for ( unsigned int x = 0; x < attrs.width; x++ ) {
      for ( unsigned int y = 0; y < attrs.height; y++ ) {
        Pixel pixel = XGetPixel( mask, x, y );
        if ( (pixel == 0) || (((x+y) & 1) == 1) )
          XPutPixel( pixmap, x, y, bg );
      }
    }

    GC temp = XCreateGC( dpy, pixels, 0, NULL );
    XPutImage( dpy, pixels, temp, pixmap,
      0, 0, 0, 0, attrs.width, attrs.height );
    XFreeGC( dpy, temp );

    XDestroyImage( pixmap );
    XDestroyImage( mask );
  }

#endif // HAVE_LIBXPM
  return pixels;
} // createInsensitivePixmapFromXpmData()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::createViewerButtons(
  Widget parent,
  SbPList * buttonlist )
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtFullViewer::createViewerButtons", "[enter]" );
#endif // SOXT_DEBUG

  int viewerbutton;
  for ( viewerbutton = FIRST_BUTTON;
        viewerbutton <= LAST_BUTTON;
        viewerbutton++ ) {
    XtCallbackProc proc = NULL;
    char label[2];
    label[1] = '\0';
    switch ( viewerbutton ) {
    case INTERACT_BUTTON:
      proc = SoXtFullViewer::interactbuttonCB;
      label[0] = 'I';
      break;
    case EXAMINE_BUTTON:
      proc = SoXtFullViewer::examinebuttonCB;
      label[0] = 'E';
      break;
    case HELP_BUTTON:
      proc = SoXtFullViewer::helpbuttonCB;
      label[0] = '?';
      break;
    case HOME_BUTTON:
      proc = SoXtFullViewer::homebuttonCB;
      label[0] = 'H';
      break;
    case SET_HOME_BUTTON:
      proc = SoXtFullViewer::sethomebuttonCB;
      label[0] = 'N';
      break;
    case VIEW_ALL_BUTTON:
      proc = SoXtFullViewer::viewallbuttonCB;
      label[0] = 'V';
      break;
    case SEEK_BUTTON:
      proc = SoXtFullViewer::seekbuttonCB;
      label[0] = 'S';
      break;
    default:
      assert( 0 );
      break;
    } // switch ( viewerbutton )
    Widget button;
    if ( viewerbutton == EXAMINE_BUTTON || viewerbutton == INTERACT_BUTTON ) {
      button = XtVaCreateManagedWidget( label,
        xmToggleButtonWidgetClass, parent,
        XmNindicatorOn, False,
        NULL );
      if ( viewerbutton == EXAMINE_BUTTON ) {
        XtVaSetValues( button, XmNset, this->isViewing() ? True : False, NULL );
      }
      if ( viewerbutton == INTERACT_BUTTON ) {
        XtVaSetValues( button, XmNset, this->isViewing() ? False : True, NULL );
      }
    } else {
      button = XtVaCreateManagedWidget( label,
        xmPushButtonWidgetClass, parent,
        NULL );
    }

    switch ( viewerbutton ) {
    case INTERACT_BUTTON:
      this->viewerbuttons.pick = button;
      break;
    case EXAMINE_BUTTON:
      this->viewerbuttons.view = button;
      break;
    case HELP_BUTTON:
      this->viewerbuttons.help = button;
      break;
    case HOME_BUTTON:
      this->viewerbuttons.home = button;
      break;
    case SET_HOME_BUTTON:
      this->viewerbuttons.set_home = button;
      break;
    case VIEW_ALL_BUTTON:
      this->viewerbuttons.view_all = button;
      break;
    case SEEK_BUTTON:
      this->viewerbuttons.seek = button;
      break;
    default:
      assert( 0 && "impossible" );
      break;
    } // switch ( viewerbutton )
#if HAVE_LIBXPM
    Pixmap pixmap, pixmap_ins;
    switch ( viewerbutton ) {
    case INTERACT_BUTTON:
      pixmap = pixmaps.pick =
        createPixmapFromXpmData( button, pick_xpm );
      pixmap_ins = pixmaps.pick_ins =
        createInsensitivePixmapFromXpmData( button, pick_xpm );
      break;
    case EXAMINE_BUTTON:
      pixmap = pixmaps.view =
        createPixmapFromXpmData( button, view_xpm );
      pixmap_ins = pixmaps.view_ins =
        createInsensitivePixmapFromXpmData( button, view_xpm );
      break;
    case HELP_BUTTON:
      pixmap = pixmap_ins = pixmaps.help =
        createPixmapFromXpmData( button, help_xpm );
      break;
    case HOME_BUTTON:
      pixmap = pixmap_ins = pixmaps.home =
        createPixmapFromXpmData( button, home_xpm );
      break;
    case SET_HOME_BUTTON:
      pixmap = pixmap_ins = pixmaps.set_home =
        createPixmapFromXpmData( button, set_home_xpm );
      break;
    case VIEW_ALL_BUTTON:
      pixmap = pixmap_ins = pixmaps.view_all =
        createPixmapFromXpmData( button, view_all_xpm );
      break;
    case SEEK_BUTTON:
      pixmap = pixmaps.seek =
        createPixmapFromXpmData( button, seek_xpm );
      pixmap_ins = pixmaps.seek_ins =
        createInsensitivePixmapFromXpmData( button, seek_xpm );
      break;
    default:
      assert( 0 && "impossible" );
      break;
    } // switch ( viewerbutton )
    if ( pixmap && pixmap_ins ) {
      XtVaSetValues( button,
        XmNlabelType, XmPIXMAP,
        XmNlabelPixmap, pixmap,
        XmNlabelInsensitivePixmap, pixmap_ins,
        XmNselectPixmap, pixmap,
        XmNselectInsensitivePixmap, pixmap_ins,
        NULL );
    }
#endif // HAVE_LIBXPM

    if ( proc != NULL ) {
      if ( viewerbutton == INTERACT_BUTTON || viewerbutton == EXAMINE_BUTTON ) {
        XtAddCallback( button, XmNdisarmCallback, proc, this );
      } else {
        XtAddCallback( button, XmNactivateCallback, proc, this );
      }
    }
    if (buttonlist) buttonlist->append( button );
  }
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtFullViewer::createViewerButtons", "[exit]" );
#endif // SOXT_DEBUG
} // createViewerButtons()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::buildPopupMenu(
  void )
{
  if ( this->prefmenu == NULL )
    this->prefmenu = this->common->setupStandardPopupMenu();
} // buildPopupMenu()

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::buildFunctionsSubmenu(
  Widget popup )
{
  SOXT_STUB();
  return (Widget) NULL;
} // buildFunctionsSubmenu()

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::buildDrawStyleSubmenu(
  Widget popup )
{
  SOXT_STUB();
  return (Widget) NULL;
} // buildDrawStyleSubmenu()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::openPopupMenu(
  const SbVec2s position )
{
  if ( ! this->isPopupMenuEnabled() )
    return;
  if ( this->prefmenu == NULL )
    this->buildPopupMenu();
  assert( this->prefmenu != NULL );
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtFullViewer::openPopupMenu", "[invoked]" );
#endif // SOXT_DEBUG && 0
  Dimension x = 0, y = 0, xt = 0, yt = 0;
  Widget widget = this->getGLWidget();
  assert( widget != NULL );
  do {
    xt = yt = 0;
    XtVaGetValues( widget, XmNx, &xt, XmNy, &yt, NULL );
    x += xt; y += yt;
    widget = XtParent(widget);
  } while ( widget && ! XtIsShell(widget) );
  XtVaGetValues( this->getShellWidget(), XmNx, &xt, XmNy, &yt, NULL );
  x += xt + position[0] + 2;
  y += yt + this->getGLSize()[1] - position[1] + 2;

  this->common->prepareMenu( this->prefmenu );
  this->prefmenu->popUp( this->getGLWidget(), x, y );
} // openPopupMenu()

// *************************************************************************

/*!
  This method is invoked when the left thumb wheel is pressed.
*/

void
SoXtFullViewer::leftWheelStart( // virtual
  void )
{
  this->interactiveCountInc();
} // leftWheelStart()

/*!
  static callback
*/

void
SoXtFullViewer::leftWheelStartCB( // static
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->leftWheelStart();
} // leftWheelStartCB()

/*!
  This method is invoked when the left thumb wheel is moved.
*/

void
SoXtFullViewer::leftWheelMotion( // virtual
  float value )
{
  this->leftWheelVal = value;
} // leftWheelMotion()

/*!
  static callback
*/

void
SoXtFullViewer::leftWheelMotionCB( // static
  Widget,
  XtPointer closure,
  XtPointer call_data )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  SoXtThumbWheelCallbackData * data = (SoXtThumbWheelCallbackData *) call_data;
  viewer->leftWheelMotion( data->current );
} // leftWheelMotionCB()

/*!
  This method is invoked when the left thumb wheel is released.
*/

void
SoXtFullViewer::leftWheelFinish( // virtual
  void )
{
  this->interactiveCountDec();
} // leftWheelFinished()

/*!
  static callback
*/

void
SoXtFullViewer::leftWheelFinishCB( // static
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->leftWheelFinish();
} // leftWheelFinishCB()

/*!
  This method returns the value for the left thumb wheel.
*/

float
SoXtFullViewer::getLeftWheelValue(
  void ) const
{
  return this->leftWheelVal;
} // getLeftWheelValue()

/*!
  This method sets the value for the left thumb wheel.
*/

void
SoXtFullViewer::setLeftWheelValue(
  const float value )
{
  this->leftWheelVal = value;
  SoXtThumbWheelSetValue( this->leftWheel, value );
} // setLeftWheelValue()

// *************************************************************************

/*!
  This method is invoked when the bottom thumb wheel is pressed.
*/

void
SoXtFullViewer::bottomWheelStart( // virtual
  void )
{
  this->interactiveCountInc();
} // bottomWheelStart()

/*!
  static callback
*/

void
SoXtFullViewer::bottomWheelStartCB(
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->bottomWheelStart();
} // bottomWheelStart()

/*!
  This method is invoked when the bottom thumb wheel is moved.
*/

void
SoXtFullViewer::bottomWheelMotion( // virtual
  float value )
{
  this->bottomWheelVal = value;
} // bottomWheelMode()

/*!
  static callback
*/

void
SoXtFullViewer::bottomWheelMotionCB( // static
  Widget,
  XtPointer closure,
  XtPointer call_data )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  SoXtThumbWheelCallbackData * data = (SoXtThumbWheelCallbackData *) call_data;
  viewer->bottomWheelMotion( data->current );
} // bottomWheelStart()

/*!
  This method is invoked when the bottom thumb wheel is released
*/

void
SoXtFullViewer::bottomWheelFinish( // virtual
  void )
{
  this->interactiveCountDec();
} // bottomWheelFinish()

/*!
  static callback
*/

void
SoXtFullViewer::bottomWheelFinishCB( // static
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->bottomWheelFinish();
} // bottomWheelStart()

/*!
  This method returns the value for the bottom thumb wheel.
*/

float
SoXtFullViewer::getBottomWheelValue(
  void ) const
{
  return this->bottomWheelVal;
} // getBottomWheelValue()

/*!
  This method sets the value of the bottom thumb wheel.
*/

void
SoXtFullViewer::setBottomWheelValue(
  const float value )
{
  this->bottomWheelVal = value;
  SoXtThumbWheelSetValue( this->bottomWheel, value );
} // setBottomWheelValue()

// *************************************************************************

/*!
  This method is invoked when the right thumb wheel is pressed.
*/

void
SoXtFullViewer::rightWheelStart( // virtual
  void )
{
  this->interactiveCountInc();
} // rightWheelStart()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::rightWheelStartCB( // static
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->rightWheelStart();
} // rightWheelStartCB()

/*!
  This method is invoked when the right thumb wheel is moved.
*/

void
SoXtFullViewer::rightWheelMotion( // virtual
  float value )
{
  this->rightWheelVal = value;
} // rightWheelMotion()

/*!
  static callback
*/

void
SoXtFullViewer::rightWheelMotionCB(
  Widget,
  XtPointer closure,
  XtPointer call_data )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  SoXtThumbWheelCallbackData * data = (SoXtThumbWheelCallbackData *) call_data;
  viewer->rightWheelMotion( data->current );
} // rightWheelMotionCB()

/*!
  This method is invoked when the right thumb wheel is released.
*/

void
SoXtFullViewer::rightWheelFinish( // virtual
  void )
{
  this->interactiveCountDec();
} // rightWheelFinish()

/*!
  static callback
*/

void
SoXtFullViewer::rightWheelFinishCB( // static
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->rightWheelFinish();
} // rightWheelFinishCB()

/*!
  This method returns the value of the right thumb wheel.
*/

float
SoXtFullViewer::getRightWheelValue(
  void ) const
{
  return this->rightWheelVal;
} // getRightWheelValue()

/*!
  This method sets the value of the right thumb wheel.
*/

void
SoXtFullViewer::setRightWheelValue(
  const float value )
{
  this->rightWheelVal = value;
  SoXtThumbWheelSetValue( this->rightWheel, value );
} // setRightWheelValue()

// *************************************************************************

/*!
  This method sets the label text displayed below the thumb wheel on the
  left decoration.
*/

void
SoXtFullViewer::setLeftWheelString(
  const char * const string )
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtFullViewer::setLeftWheelString",
    "setting left wheel label" );
#endif // SOXT_DEBUG
  delete [] this->leftWheelStr;
  this->leftWheelStr = strcpy( new char [strlen(string)+1], string );
  if ( this->leftWheelLabel != NULL )
    XtVaSetValues( this->leftWheelLabel,
      XtVaTypedArg,
        XmNlabelString, XmRString,
        this->leftWheelStr, strlen(this->leftWheelStr) + 1,
      NULL );
} // setLeftWheelString()

/*!
  this method returns the Motif widget for the label below the left thumb wheel.
  The widget is of the xmLabelWidgetClass type.
*/

Widget
SoXtFullViewer::getLeftWheelLabelWidget( // protected
  void ) const
{
  return this->leftWheelLabel;
} // getLeftWheelLabelWidget()

/*!
  This method sets the label text that is displayed to the left og the
  bottom decoration thumb wheel.
*/

void
SoXtFullViewer::setBottomWheelString(
  const char * const string )
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtFullViewer::setBottomWheelString",
    "setting bottom wheel label" );
#endif // SOXT_DEBUG
  delete [] this->bottomWheelStr;
  this->bottomWheelStr = strcpy( new char [strlen(string)+1], string );
  if ( this->bottomWheelLabel != NULL )
    XtVaSetValues( this->bottomWheelLabel,
      XtVaTypedArg,
        XmNlabelString, XmRString,
        this->bottomWheelStr, strlen(this->bottomWheelStr) + 1,
      NULL );
} // setBottomWheelString()

/*!
  This method returns the Motif widget for the label beside the bottom
  thumb wheel.  The widget is of the xmLabelWidgetClass type.
*/

Widget
SoXtFullViewer::getBottomWheelLabelWidget( // protected
  void ) const
{
  return this->bottomWheelLabel;
} // getBottomWheelLabelWidget()

/*!
  This method sets the label text that is displayed under the right decoration
  thumb wheel.
*/

void
SoXtFullViewer::setRightWheelString(
  const char * const string )
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtFullViewer::setRightWheelString", "[enter]" );
#endif // SOXT_DEBUG

  delete [] this->rightWheelStr;
  this->rightWheelStr = strcpy( new char [strlen(string)+1], string );

  if ( this->rightWheelLabel != NULL )
    XtVaSetValues( this->rightWheelLabel,
      XtVaTypedArg,
        XmNlabelString, XmRString,
        this->rightWheelStr, strlen(this->rightWheelStr) + 1,
      NULL );

#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtFullViewer::setRightWheelString", "[exit]" );
#endif // SOXT_DEBUG
} // setRightWheelString()

/*!
  This method returns the widget for the label below the right thumb wheel.
  The widget is of the xmLabelWidgetClass type.
*/

Widget
SoXtFullViewer::getRightWheelLabelWidget( // protected
  void ) const
{
  return this->rightWheelLabel;
} // getRightWheelLabelWidget()

// *************************************************************************

/*!
  Default implementation does nothing.
*/

void
SoXtFullViewer::openViewerHelpCard( // virtual
  void )
{
#if SOXT_DEBUG
  SoDebugError::postInfo( "SoXtFullViewer::openViewerHelpCard",
    "should be overloaded" );
#endif // SOXT_DEBUG
} // openViewerHelpCard()

// *************************************************************************

/*!
  Not implemented.
*/

Widget
SoXtFullViewer::buildAppButtonsForm(
  Widget parent )
{
  Widget form = XtVaCreateManagedWidget( "appbuttons",
    xmFormWidgetClass, parent,
    NULL );

  SOXT_STUB();
  return form;
} // buildAppButtonsForm()

// *************************************************************************

/*!
  This method shows or hides the decoration widgets, depending on the
  \a enable argument.
*/

void
SoXtFullViewer::showDecorationWidgets(
  SbBool enable )
{
  if ( ! this->canvas ) return;
  assert( this->leftDecoration != (Widget) NULL );
  assert( this->rightDecoration != (Widget) NULL );
  assert( this->bottomDecoration != (Widget) NULL );

  if ( enable ) {
    XtVaSetValues( this->canvas,
      XmNleftOffset, 30,
      XmNrightOffset, 30,
      XmNbottomOffset, 30,
      NULL );
    if ( XtWindow( this->leftDecoration ) != 0 ) {
      XtMapWidget( this->leftDecoration );
      XtManageChild( this->leftDecoration );
      XtMapWidget( this->rightDecoration );
      XtManageChild( this->rightDecoration );
      XtMapWidget( this->bottomDecoration );
      XtManageChild( this->bottomDecoration );
    }
  } else {
    if ( XtWindow( this->leftDecoration ) != 0 ) {
      XtUnmapWidget( this->leftDecoration );
      XtUnmapWidget( this->rightDecoration );
      XtUnmapWidget( this->bottomDecoration );
    }
    XtVaSetValues( this->canvas,
      XmNleftOffset, 0,
      XmNrightOffset, 0,
      XmNbottomOffset, 0,
      NULL );
  }
} // showDecorationWidgets()

// *************************************************************************

/*!
  This method sets the title text for the viewer preferences window.
*/

void
SoXtFullViewer::setPrefSheetString( // protected
  const char * name )
{
  if ( this->prefstring )
    delete [] this->prefstring;
  this->prefstring = NULL;
  if ( name )
    this->prefstring = strcpy( new char [strlen(name) + 1], name );

  if ( this->prefshell )
    XtVaSetValues( this->prefshell, XtNtitle, this->prefstring, NULL );
} // setPrefSheetString()

/*!
  This is a hook for constructing the preferences window in the derived
  viewer classes.

  Basically, derived classes will do the following in this method:

  \code
    if ( ! this->prefshell ) {
      createPrefSheetShellAndForm( this->prefshell, this->prefsheet );
      createDefaultPrefSheetParts( this->prefparts, this->numprefparts,
        this->prefsheet );
      // add parts specific for derived viewer
    }
    layoutPartsAndMapPrefSheet( this->prefparts, this->numprefparts,
      this->prefsheet, this->prefshell );
  \endcode
*/

void
SoXtFullViewer::createPrefSheet( // protected, virtual
  void )
{
/*¡
  Method could be made abstract.?
*/
} // createPrefSheet()

/*!
  This method creates the preferences sheet shell.
*/

void
SoXtFullViewer::createPrefSheetShellAndForm(  // protected
  Widget & shell,
  Widget & form )
{
  assert( this->prefshell == NULL );

  if ( ! this->prefstring )
    this->setPrefSheetString( "Xt Viewer Preferences" );

  Widget pshell = this->getBaseWidget();
  while ( pshell && ! XtIsShell(pshell) )
    pshell = XtParent(pshell);
  assert( pshell != NULL );

  Visual * visual = NULL;
  int depth = 0;
  Colormap colormap = 0;

  XtVaGetValues( pshell,
    XmNvisual, &visual,
    XmNdepth, &depth,
    XmNcolormap, &colormap,
    NULL );

  shell = XtVaAppCreateShell( NULL, "SoXt",
    topLevelShellWidgetClass, SoXt::getDisplay(),
    XmNvisual, visual,
    XmNdepth, depth,
    XmNcolormap, colormap,
    XmNtitle, this->prefstring,
    NULL );

  XtAddCallback( shell, XmNdestroyCallback,
    prefSheetDestroyCB, (XtPointer) this );
  this->prefshell = shell;

  form = XtVaCreateWidget( "form", xmFormWidgetClass, shell, NULL );

  this->prefsheet = form;
} // createPrefSheetShellAndForm()

/*!
  This method creates all the preference sheel parts.
*/

void
SoXtFullViewer::createDefaultPrefSheetParts( // protected
  Widget * widgets,
  int & num,
  Widget form )
{
  assert( widgets != NULL );
  num = 0;

  if ( (widgets[num] = this->createFramedSeekPrefSheetGuts( form )) )
    num++;
  if ( (widgets[num] = this->createFramedSeekDistPrefSheetGuts( form )) )
    num++;
  if ( (widgets[num] = this->createFramedZoomPrefSheetGuts( form ) ))
    num++;
// FIXME: this barfs on 8-bit displays...
  if ( (widgets[num] = this->createFramedClippingPrefSheetGuts( form )) )
    num++;
  if ( (widgets[num] = this->createFramedStereoPrefSheetGuts( form )) )
    num++;
  // NOTE: SpeedPrefs is set up on SoXtFullViewer level, but is only used
  // by WalkViewer and FlyViewer, so it is not included as default...
} // createDefaultPrefSheetParts()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::layoutPartsAndMapPrefSheet( // protected
  Widget * widgets,
  int num,
  Widget form,
  Widget shell )
{
  assert( widgets != NULL );
  for ( int i = 0; i < num; i++ ) {
    int attachment_top, attachment_bottom;
    if ( i == 0 ) {
      attachment_top = XmATTACH_FORM;
      attachment_bottom = XmATTACH_NONE;
    } else if ( i == (num - 1) ) {
      attachment_top = XmATTACH_WIDGET;
      attachment_bottom = XmATTACH_NONE; // FORM;
    } else {
      attachment_top = XmATTACH_WIDGET;
      attachment_bottom = XmATTACH_NONE;
    }
    XtVaSetValues( widgets[i],
      XmNtopAttachment, attachment_top,
      XmNleftAttachment, XmATTACH_FORM,
      XmNrightAttachment, XmATTACH_FORM,
      XmNbottomAttachment, attachment_bottom,
      NULL );
    if ( attachment_top == XmATTACH_WIDGET )
      XtVaSetValues( widgets[i], XmNtopWidget, widgets[i-1], NULL );
  }
  XtManageChild( form );
  XtPopup( shell, XtGrabNone );
} // layoutPartsAndMapPrefSheet()
 
// *************************************************************************

/*!
  This method is a wrapper for the createSeekPrefSheetGuts() and
  createSeekDistPrefSheetGuts() methods, to place them both inside
  one frame.
*/

Widget
SoXtFullViewer::createFramedSeekPrefSheetGuts( // protected
  Widget parent )
{
  Widget frame = XtVaCreateManagedWidget( "seekframe",
    xmFrameWidgetClass, parent,
    NULL );

  XtVaCreateManagedWidget( "title",
    xmLabelGadgetClass, frame,
    XmNchildType, XmFRAME_TITLE_CHILD,
    XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Seek Settings", strlen( "Seek Settings" ) + 1,
    NULL );

  Widget form = XtVaCreateManagedWidget( "form",
    xmFormWidgetClass, frame,
    NULL );

  this->createSeekPrefSheetGuts( form );

  return frame;
} // createFramedSeekPrefSheetGuts()

/*!
  This method is a wrapper for the createSeekDistPrefSheetGuts() method,
  to wrap a frame around the widgets.
*/

Widget
SoXtFullViewer::createFramedSeekDistPrefSheetGuts( // protected
  Widget parent )
{
  Widget frame = XtVaCreateManagedWidget( "seekdistframe",
    xmFrameWidgetClass, parent,
    NULL );
  XtVaCreateManagedWidget( "seekdisttitle",
    xmLabelGadgetClass, frame,
    XmNchildType, XmFRAME_TITLE_CHILD,
    XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Seek Distance Settings", strlen( "Seek Distance Settings" ) + 1,
    NULL );

  Widget form = XtVaCreateManagedWidget( "form",
    xmFormWidgetClass, frame,
    NULL );

  this->createSeekDistPrefSheetGuts( form );

  return frame;
} // createFramedSeekDistPrefSheetGuts()

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::createFramedZoomPrefSheetGuts( // protected
  Widget parent )
{
  Widget frame = XtVaCreateManagedWidget( "zoomframe",
    xmFrameWidgetClass, parent,
    NULL );

  XtVaCreateManagedWidget( "title",
    xmLabelGadgetClass, frame,
    XmNchildType, XmFRAME_TITLE_CHILD,
    XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Zoom Settings", strlen( "Zoom Settings" ) + 1,
    NULL );

  this->createZoomPrefSheetGuts( frame );

  return frame;
} // createFramedZoomPrefSheetGuts()

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::createFramedClippingPrefSheetGuts(
  Widget parent )
{
  Widget frame = XtVaCreateManagedWidget( "clippingframe",
    xmFrameWidgetClass, parent,
    NULL );

  XtVaCreateManagedWidget( "title",
    xmLabelGadgetClass, frame,
    XmNchildType, XmFRAME_TITLE_CHILD,
    XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Clipping Plane Settings", strlen( "Clipping Plane Settings" ) + 1,
    NULL );

  this->createClippingPrefSheetGuts( frame );

  return frame;
} // createFramedClippingPrefSheetGuts()

// *************************************************************************

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::createFramedStereoPrefSheetGuts(
  Widget parent )
{
  Widget frame = XtVaCreateManagedWidget( "stereoframe",
    xmFrameWidgetClass, parent, NULL );

  Widget label = XtVaCreateManagedWidget( "stereoframelabel",
    xmLabelGadgetClass, frame,
    XmNchildType, XmFRAME_TITLE_CHILD,
    XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Stereo Viewing Settings", strlen( "Stereo Viewing Settings" ) + 1,
    NULL );

  this->createStereoPrefSheetGuts( frame );

  return frame;
} // createFramedStereoPrefSheetGuts()

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::createFramedSpeedPrefSheetGuts(
  Widget parent )
{
  Widget frame = XtVaCreateManagedWidget( "speedframe",
    xmFrameWidgetClass, parent, NULL );

  Widget label = XtVaCreateManagedWidget( "speedframelabel",
    xmLabelGadgetClass, frame,
    XmNchildType, XmFRAME_TITLE_CHILD,
    XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Speed Settings", strlen( "Speed Settings" ) + 1,
    NULL );

  this->createSpeedPrefSheetGuts( frame );

  return frame;
} // createFramedSpeedPrefSheetGuts()

// *************************************************************************

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::createSeekPrefSheetGuts( // protected
  Widget parent )
{
  //  Seek animation time  |NNN|  seconds
  //  Seek to:   <> point   <> object

  Widget form = XtVaCreateManagedWidget( "seekprefs",
    xmFormWidgetClass, parent, NULL );

  Widget line1 = XtVaCreateManagedWidget( "line 1",
    xmFormWidgetClass, form,
    XmNtopAttachment, XmATTACH_FORM,
    XmNleftAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    NULL );

  Widget label = XtVaCreateManagedWidget( "seektime",
    xmLabelWidgetClass, line1,
    XmNtopAttachment, XmATTACH_FORM,
    XmNleftAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Seek animation time: ", strlen( "Seek animation time: " ) + 1, 
    NULL );

  this->seektimefield = XtVaCreateManagedWidget( "seektimeinput",
    xmTextFieldWidgetClass, line1,
    XmNtopAttachment, XmATTACH_FORM,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, label,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNwidth, 60,
    NULL );

  XtAddCallback( this->seektimefield, XmNactivateCallback,
    SoXtFullViewer::seektimechangedCB, (XtPointer) this );
  XtAddCallback( this->seektimefield, XmNlosingFocusCallback,
    SoXtFullViewer::seektimechangedCB, (XtPointer) this );
  char buffer[16];
  sprintf( buffer, "%g", this->getSeekTime() );
  XmTextSetString( this->seektimefield, buffer );
  XmTextSetCursorPosition( this->seektimefield, (long) strlen(buffer) );

  Widget fieldlabed = XtVaCreateManagedWidget( "seconds",
    xmLabelWidgetClass, line1,
    XmNtopAttachment, XmATTACH_FORM,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, this->seektimefield,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNbottomWidget, this->seektimefield,
    NULL );

  Widget line2 = XtVaCreateManagedWidget( "line 2",
    xmFormWidgetClass, form,
    XmNleftAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, line1,
    XmNbottomAttachment, XmATTACH_FORM,
    NULL );

  Widget tolabel = XtVaCreateManagedWidget( "tolabel",
    xmLabelWidgetClass, line2,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Seek to: ", strlen( "Seek to: " ) + 1,
    NULL );

  this->pointtoggle = XtVaCreateManagedWidget( "point",
    xmToggleButtonWidgetClass, line2,
    XmNindicatorType, XmONE_OF_MANY,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, tolabel,
    XmNtopAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNset, this->isDetailSeek() ? True : False,
    NULL );

  XtAddCallback( this->pointtoggle, XmNvalueChangedCallback,
    SoXtFullViewer::pointtoggledCB, (XtPointer) this );

  this->objecttoggle = XtVaCreateManagedWidget( "object",
    xmToggleButtonWidgetClass, line2,
    XmNindicatorType, XmONE_OF_MANY,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, this->pointtoggle,
    XmNtopAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNset, this->isDetailSeek() ? False : True,
    NULL );

  XtAddCallback( this->objecttoggle, XmNvalueChangedCallback,
    SoXtFullViewer::objecttoggledCB, (XtPointer) this );

  return form;
} // createSeekPrefSheetGuts()

// *************************************************************************

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::createSeekDistPrefSheetGuts( // protected
  Widget parent )
{
  //   Seek distance:   |||||||||||||   |NN.NNN|
  //      <> percentage   <> absolute

  Widget form = XtVaCreateManagedWidget( "seekdistprefs",
    xmFormWidgetClass, parent, NULL );

  Widget line1 = XtVaCreateManagedWidget( "seekdistprefs",
    xmFormWidgetClass, form,
    XmNleftAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    NULL );

  Widget label = XtVaCreateManagedWidget( "seekdistlabel",
    xmLabelWidgetClass, line1,
    XmNtopAttachment, XmATTACH_FORM,
    XmNleftAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Seek distance: ", strlen( "Seek distance: " ) + 1,
    NULL );

  Widget wheel = XtVaCreateManagedWidget( "distance",
    soxtThumbWheelWidgetClass, line1,
    XmNtopAttachment, XmATTACH_FORM,
    XmNtopOffset, 2,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, label,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNbottomOffset, 2,
    XmNorientation, XmHORIZONTAL,
    XmNshadowType, XmSHADOW_OUT,
    XmNshadowThickness, 2,
    XmNtraversalOn, False,
    XmNwidth, 90,
    NULL );

  this->seekdistancefield = XtVaCreateManagedWidget( "seekdistancefield",
    xmTextFieldWidgetClass, line1,
    XmNtopAttachment, XmATTACH_FORM,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, wheel,
    XmNwidth, 80,
    NULL );
  XtAddCallback( this->seekdistancefield, XmNlosingFocusCallback,
    SoXtFullViewer::seekdistancechangedCB, (XtPointer) this );
  XtAddCallback( this->seekdistancefield, XmNactivateCallback,
    SoXtFullViewer::seekdistancechangedCB, (XtPointer) this );
  char buffer[16];
  sprintf( buffer, "%g", this->seekdistance );
  XmTextSetString( this->seekdistancefield, buffer );
  XmTextSetCursorPosition( this->seekdistancefield, (long) strlen(buffer) );

  Widget line2 = XtVaCreateManagedWidget( "line 2",
    xmFormWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, line1,
    XmNleftAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    NULL );

  this->percenttoggle = XtVaCreateManagedWidget( "percentagetoggle",
    xmToggleButtonWidgetClass, line2,
    XmNtopAttachment, XmATTACH_FORM,
    XmNleftAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "percentage", strlen( "percentage" ) + 1,
    XmNindicatorType, XmONE_OF_MANY,
    XmNset, this->seekdistaspercentage ? True : False,
    NULL );

  XtAddCallback( this->percenttoggle, XmNvalueChangedCallback,
    SoXtFullViewer::percenttoggledCB, (XtPointer) this );

  this->absolutetoggle = XtVaCreateManagedWidget( "absolutetoggle",
    xmToggleButtonWidgetClass, line2,
    XmNtopAttachment, XmATTACH_FORM,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, this->percenttoggle,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNindicatorType, XmONE_OF_MANY,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "absolute", strlen( "absolute" ) + 1,
    XmNset, this->seekdistaspercentage ? False : True,
    NULL );

  XtAddCallback( this->absolutetoggle, XmNvalueChangedCallback,
    SoXtFullViewer::absolutetoggledCB, (XtPointer) this );

  return form;
} // createSeekDistPrefSheetGuts()

// *************************************************************************

/*!
  This method creates and lays out the widgets in the Zoom preferences sheet
  frame.
*/

Widget
SoXtFullViewer::createZoomPrefSheetGuts( // protected
  Widget parent )
{
  //  Camera zoom  |          ||||        |     |NN.NN|
  //  Zoom slider ranges from:  |N.N|  to |N.N|

  Widget form = XtVaCreateManagedWidget( "zoomprefs",
    xmFormWidgetClass, parent,
    NULL );

  this->zoomfrom = XtVaCreateManagedWidget( "from",
    xmTextFieldWidgetClass, form,
    XmNtopAttachment, XmATTACH_FORM,
    XmNleftAttachment, XmATTACH_FORM,
    XmNwidth, 50,
    NULL );

  SoCamera * const camera = this->getCamera();
  Boolean enable = False;
  if ( camera ) {
    SoType camtype = camera->getTypeId();
    if ( camtype.isDerivedFrom( SoPerspectiveCamera::getClassTypeId() ) )
      enable = True;
  }

  char buf[16];
  sprintf( buf, "%.1f", this->zoomrange[0] );
  XmTextSetString( this->zoomfrom, buf );
  XmTextSetCursorPosition( this->zoomfrom, (long) strlen(buf) );

  XtVaSetValues( this->zoomfrom,
    XmNsensitive, enable,
    XmNeditable, enable,
    XmNcursorPositionVisible, enable,
    NULL );

  XtAddCallback( this->zoomfrom, XmNactivateCallback,
    SoXtFullViewer::zoomfromchangedCB, (XtPointer) this );
  XtAddCallback( this->zoomfrom, XmNlosingFocusCallback,
    SoXtFullViewer::zoomfromchangedCB, (XtPointer) this );

  this->zoomvalue = XtVaCreateManagedWidget( "zoomvalue",
    xmTextFieldWidgetClass, form,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XmNwidth, 60,
    NULL );

  sprintf( buf, "%.1f", this->getCameraZoom() );
  XmTextSetString( this->zoomvalue, buf );
  XmTextSetCursorPosition( this->zoomvalue, (long) strlen(buf) );

  XtVaSetValues( this->zoomvalue,
    XmNsensitive, enable,
    XmNeditable, enable,
    XmNcursorPositionVisible, enable,
    NULL );

  XtAddCallback( this->zoomvalue, XmNactivateCallback,
    SoXtFullViewer::zoomvaluechangedCB, (XtPointer) this );
  XtAddCallback( this->zoomvalue, XmNlosingFocusCallback,
    SoXtFullViewer::zoomvaluechangedCB, (XtPointer) this );

  Widget valuelabel = XtVaCreateManagedWidget( "valuelabel",
    xmLabelWidgetClass, form,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_WIDGET,
    XmNrightWidget, zoomvalue,
    XmNbottomAttachment, XmATTACH_FORM,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      " Value: ", strlen( " Value: " ) + 1,
    NULL );

  this->zoomto = XtVaCreateManagedWidget( "to",
    xmTextFieldWidgetClass, form,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_WIDGET,
    XmNrightWidget, valuelabel,
    XmNwidth, 50,
    NULL );

  sprintf( buf, "%.1f", this->zoomrange[1] );
  XmTextSetString( this->zoomto, buf );
  XmTextSetCursorPosition( this->zoomto, (long) strlen(buf) );

  XtVaSetValues( this->zoomto,
    XmNsensitive, enable,
    XmNeditable, enable,
    XmNcursorPositionVisible, enable,
    NULL );

  XtAddCallback( this->zoomto, XmNactivateCallback,
    SoXtFullViewer::zoomtochangedCB, (XtPointer) this );
  XtAddCallback( this->zoomto, XmNlosingFocusCallback,
    SoXtFullViewer::zoomtochangedCB, (XtPointer) this );

  this->zoomslider = XtVaCreateManagedWidget( "zoomslider",
    xmScaleWidgetClass, form,
    XmNtopAttachment, XmATTACH_FORM,
    XmNtopOffset, 6,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, this->zoomfrom,
    XmNrightAttachment, XmATTACH_WIDGET,
    XmNrightWidget, this->zoomto,
    XmNorientation, XmHORIZONTAL,
    XmNminimum, 0,
    XmNmaximum, 1000,
    XmNdecimalPoints, 1,
    XmNshowValue, False,
    NULL );

  float zoomvalue = this->getCameraZoom();
  if ( zoomvalue == 0.0f || (this->zoomrange[0] == this->zoomrange[1]) ) {
    XmScaleSetValue( this->zoomslider, 0 );
  } else {
    float normalized = (zoomvalue - this->zoomrange[0]) /
      (this->zoomrange[1] - this->zoomrange[0]);
    int scaledval = (int) (sqrt(normalized) * 1000.0f);
    XmScaleSetValue( this->zoomslider, scaledval );
  }

  XtVaSetValues( this->zoomslider,
    XmNsensitive, enable,
    XmNeditable, enable,
    NULL );

  XtAddCallback( this->zoomslider, XmNvalueChangedCallback,
    SoXtFullViewer::zoomsliderchangedCB, (XtPointer) this );
  XtAddCallback( this->zoomslider, XmNdragCallback,
    SoXtFullViewer::zoomsliderchangedCB, (XtPointer) this );

  return form;
} // createZoomPrefSheetGuts()

// *************************************************************************

/*!
  \internal
  This method sets up the clipping related part of the preferences window,
  excluding the surrounding frame.
*/

Widget
SoXtFullViewer::createClippingPrefSheetGuts( // protected
  Widget parent )
{
  Widget form = XtVaCreateManagedWidget( "clippingprefs",
    xmFormWidgetClass, parent, NULL );

#if 0
  this->autocliptoggle = XtVaCreateManagedWidget( "autocliptoggle",
    xmToggleButtonWidgetClass, form,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNset, this->isAutoClipping() ? True : False,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "auto", strlen( "auto" ) + 1,
    NULL );

  XtAddCallback( this->autocliptoggle, XmNvalueChangedCallback,
    SoXtFullViewer::autocliptoggledCB, (XtPointer) this );

  SoCamera * const camera = this->getCamera();

  this->farvalue = XtVaCreateManagedWidget( "farvalue",
    xmTextFieldWidgetClass, form,
    XmNrightAttachment, XmATTACH_FORM,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNbottomOffset, 2,
    XmNwidth, 100,
    XmNsensitive, this->isAutoClipping() ? False : True,
    XmNeditable, this->isAutoClipping() ? False : True,
    XmNcursorPositionVisible, this->isAutoClipping() ? False : True,
    NULL );

  float fardistance = 0.002f;
  if ( camera != NULL )
    fardistance = camera->farDistance.getValue();

  char buf[16];
  sprintf( buf, "%g", fardistance );
  XmTextSetString( this->farvalue, buf );
  XmTextSetCursorPosition( this->farvalue, (long) strlen(buf) );

  XtAddCallback( this->farvalue, XmNactivateCallback,
    SoXtFullViewer::farvaluechangedCB, (XtPointer) this );
  XtAddCallback( this->farvalue, XmNlosingFocusCallback,
    SoXtFullViewer::farvaluechangedCB, (XtPointer) this );

/*
  this->farwheel = XtVaCreateManagedWidget( "farwheel",
    soxtThumbWheelWidgetClass, form,
    XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNleftWidget, this->farvalue,
    XmNleftOffset, 2,
    XmNrightAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNrightWidget, this->farvalue,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_WIDGET,
    XmNbottomWidget, this->farvalue,
    XmNbottomOffset, 2,
    XmNheight, 28,
    XmNsensitive, this->isAutoClipping() ? False : True,
    XmNorientation, XmHORIZONTAL,
    NULL );

  XtAddCallback( this->farwheel, XmNvalueChangedCallback,
    SoXtFullViewer::farwheelvaluechangedCB, (XtPointer) this );
  XtAddCallback( this->farwheel, XmNarmCallback,
    SoXtFullViewer::increaseInteractiveCountCB, (XtPointer) this );
  XtAddCallback( this->farwheel, XmNdisarmCallback,
    SoXtFullViewer::decreaseInteractiveCountCB, (XtPointer) this );

  SoXtThumbWheelSetValue( this->farwheel, fardistance );

  Widget farlabel = XtVaCreateManagedWidget( "farlabel",
    xmLabelWidgetClass, form,
    XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNleftWidget, this->farvalue,
    XmNrightAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNrightWidget, this->farvalue,
    XmNbottomAttachment, XmATTACH_WIDGET,
    XmNbottomWidget, this->farwheel,
    XmNbottomOffset, 2,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Far plane:", strlen( "Far plane:" ) + 1,
    NULL );
*/

  float neardistance = 0.001f;
  if ( camera != NULL )
    neardistance = camera->nearDistance.getValue();

  this->nearvalue = XtVaCreateManagedWidget( "nearvalue",
    xmTextFieldWidgetClass, form,
    XmNrightAttachment, XmATTACH_WIDGET,
    XmNrightWidget, this->farvalue,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNbottomWidget, this->farvalue,
    XmNwidth, 100,
    XmNsensitive, this->isAutoClipping() ? False : True,
    XmNeditable, this->isAutoClipping() ? False : True,
    XmNcursorPositionVisible, this->isAutoClipping() ? False : True,
    NULL );

  sprintf( buf, "%g", neardistance );
  XmTextSetString( this->nearvalue, buf );
  XmTextSetCursorPosition( this->nearvalue, (long) strlen(buf) );

  XtAddCallback( this->nearvalue, XmNactivateCallback,
    SoXtFullViewer::nearvaluechangedCB, (XtPointer) this );
  XtAddCallback( this->nearvalue, XmNlosingFocusCallback,
    SoXtFullViewer::nearvaluechangedCB, (XtPointer) this );

/*
  this->nearwheel = XtVaCreateManagedWidget( "nearwheel",
    soxtThumbWheelWidgetClass, form,
    XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNleftWidget, this->nearvalue,
    XmNleftOffset, 2,
    XmNrightAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNrightWidget, this->nearvalue,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNbottomWidget, this->farwheel,
    XmNsensitive, this->isAutoClipping() ? False : True,
    XmNorientation, XmHORIZONTAL,
    XmNheight, 28,
    NULL );

  SoXtThumbWheelSetValue( this->nearwheel, neardistance );

  XtAddCallback( this->nearwheel, XmNvalueChangedCallback,
    SoXtFullViewer::nearwheelvaluechangedCB, (XtPointer) this );
  XtAddCallback( this->nearwheel, XmNarmCallback,
    SoXtFullViewer::increaseInteractiveCountCB, (XtPointer) this );
  XtAddCallback( this->nearwheel, XmNdisarmCallback,
    SoXtFullViewer::decreaseInteractiveCountCB, (XtPointer) this );

  Widget nearlabel = XtVaCreateManagedWidget( "nearlabel",
    xmLabelWidgetClass, form,
    XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNleftWidget, this->nearvalue,
    XmNrightAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNrightWidget, this->nearvalue,
    XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNbottomWidget, farlabel,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Near plane:", strlen( "Near plane:" ) + 1,
    NULL );
*/
#endif

  return form;
} // createClippingPrefSheetGuts()

// *************************************************************************

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::createStereoPrefSheetGuts( // protected
  Widget parent )
{
  Widget form = XtVaCreateManagedWidget( "stereoguts",
    xmFormWidgetClass, parent, NULL );

  this->stereotoggle = XtVaCreateManagedWidget( "stereotoggle",
    xmToggleButtonWidgetClass, form,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNset, False,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "stereo viewing", strlen( "stereo viewing" ) + 1,
    NULL );

  XtAddCallback( this->stereotoggle, XmNvalueChangedCallback,
    SoXtFullViewer::stereotoggledCB, (XtPointer) this );

  return form;
} // createStereoPrefSheetGuts()

// *************************************************************************

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::createSpeedPrefSheetGuts( // protected
  Widget parent )
{
  Widget form = XtVaCreateManagedWidget( "speedform",
    xmFormWidgetClass, parent, NULL );

  Widget label = XtVaCreateManagedWidget( "speedlabel",
    xmLabelWidgetClass, form,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Viewer speed: ", strlen( "Viewer speed: " ) + 1,
    NULL );

  this->incspeedbutton = XtVaCreateManagedWidget( "incbutton",
    xmPushButtonWidgetClass, form,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, label,
    XmNtopAttachment, XmATTACH_FORM,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "increase", strlen( "increase" ) + 1,
    NULL );

  XtAddCallback( this->incspeedbutton, XmNactivateCallback,
    SoXtFullViewer::speedIncCB, (XtPointer) this );

  this->decspeedbutton = XtVaCreateManagedWidget( "decbutton",
    xmPushButtonWidgetClass, form,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, this->incspeedbutton,
    XmNtopAttachment, XmATTACH_FORM,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "decrease", strlen( "decrease" ) + 1,
    NULL );

  XtAddCallback( this->decspeedbutton, XmNactivateCallback,
    SoXtFullViewer::speedDecCB, (XtPointer) this );

  return form;
} // createSpeedPrefSheetGuts()

// *************************************************************************
// preferences sheet hooks:

/*!
  \internal
  This callback is hooked up to the destruction of the preferences sheet
  window.
*/

void
SoXtFullViewer::prefSheetDestroy(
  void )
{
  XtUnrealizeWidget( this->prefshell );
  this->prefshell = NULL;
  this->prefsheet = NULL;
} // prefSheetDestroy()

/*!
  static callback
*/

void
SoXtFullViewer::prefSheetDestroyCB( // static
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->prefSheetDestroy();
} // prefSheetDestroyCB()

/*!
  \internal
  This callback is hooked up to the "seconds" preferences sheet text field.
*/

void
SoXtFullViewer::seekPrefSheetField(
  void )
{
  SOXT_STUB();
} // seekPrefSheetField()

/*!
  \internal
  static callback
*/

void
SoXtFullViewer::seekPrefSheetFieldCB(
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->seekPrefSheetField();
} // seekPrefSheetFieldCB()

/*!
  \internal

  This callback is hooked up to the seek-to-point-or-object toggling
  widgets.
*/

void
SoXtFullViewer::seekPrefSheetToggle1(
  void )
{
  SOXT_STUB();
} // seekPrefSheetToggle1CB()

/*!
  static callback
*/

void
SoXtFullViewer::seekPrefSheetToggle1CB( // static
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->seekPrefSheetToggle1();
} // seekPrefSheetToggle1CB()

/*!
  \internal
  This callback is hooked up to the seek-percentage-or-distance toggle
  widgets.
*/

void
SoXtFullViewer::seekPrefSheetToggle2(
  void )
{
  SOXT_STUB();
} // seekPrefSheetToggle2CB()

/*!
  static callback
*/

void
SoXtFullViewer::seekPrefSheetToggle2CB(
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->seekPrefSheetToggle2();
} // seekPrefSheetToggle2CB()

/*!
  \internal
*/

void
SoXtFullViewer::autocliptoggled(
  void )
{
  Boolean enable = False;
  XtVaGetValues( this->autocliptoggle, XmNset, &enable, NULL );
  this->setAutoClipping( enable ? TRUE : FALSE );

  XtVaSetValues( this->nearwheel,
    XmNsensitive, enable ? False : True,
    NULL );
  XtVaSetValues( this->farwheel,
    XmNsensitive, enable ? False : True,
    NULL );
  XtVaSetValues( this->nearvalue,
    XmNsensitive, enable ? False : True,
    XmNeditable, enable ? False : True,
    XmNcursorPositionVisible, enable ? False : True,
    NULL );
  XtVaSetValues( this->farvalue,
    XmNsensitive, enable ? False : True,
    XmNeditable, enable ? False : True,
    XmNcursorPositionVisible, enable ? False : True,
    NULL );

  SoCamera * const camera = this->getCamera();
  if ( ! camera ) {
#if SOXT_DEBUG
    SoDebugError::postInfo( "SoXtFullViewer::autocliptoggled", "no camera" );
#endif // SOXT_DEBUG
    return;
  }

  if ( ! enable ) {
    float nearval = SoXtThumbWheelGetValue( this->nearwheel );
    float farval = SoXtThumbWheelGetValue( this->farwheel );
    camera->nearDistance = nearval;
    camera->farDistance = farval;
  }
} // autocliptoggled()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::autocliptoggledCB( // static
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->autocliptoggled();
} // autocliptoggledCB()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::nearwheelvaluechanged(
  void )
{
  assert( this->nearwheel != NULL && this->farwheel != NULL );

  float val = SoXtThumbWheelGetValue( this->nearwheel );
  float farval = SoXtThumbWheelGetValue( this->farwheel );
  
  if ( val < 0.001f ) {
    val = 0.001f;
    SoXtThumbWheelSetValue( this->nearwheel, val );
  } else if ( val >= farval ) {
    val = farval - 0.001f;
    SoXtThumbWheelSetValue( this->nearwheel, val );
  }

  char valuestring[32];
  sprintf( valuestring, "%.3f", val );
  XmTextSetString( this->nearvalue, valuestring );
  XmTextSetCursorPosition( this->nearvalue, (long) strlen(valuestring) );

  SoCamera * const cam = this->getCamera();
  if ( ! cam ) {
#if SOXT_DEBUG
    SoDebugError::postInfo( "SoXtFullViewer::nearwheelvaluechanged",
      "no camera" );
#endif // SOXT_DEBUG
    return;
  }

  // FIXME: cut off at 0.0? And against far clipping value? 990223 mortene.
  cam->nearDistance = val;
} // nearwheelvaluechanged()

/*!
  static callback
*/

void
SoXtFullViewer::nearwheelvaluechangedCB(
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->nearwheelvaluechanged();
} // nearwheelvaluechangedCB()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::farwheelvaluechanged(
  void )
{
  assert( this->nearwheel != NULL && this->farwheel != NULL );

  float val = SoXtThumbWheelGetValue( this->farwheel );
  float nearval = SoXtThumbWheelGetValue( this->nearwheel );

  if ( val <= nearval ) {
    val = nearval + 0.001f;
    SoXtThumbWheelSetValue( this->farwheel, val );
  }

  char valuestring[32];
  sprintf( valuestring, "%.3f", val );
  XmTextSetString( this->farvalue, valuestring );
  XmTextSetCursorPosition( this->farvalue, (long) strlen(valuestring) );

  SoCamera * const cam = this->getCamera();
  if ( ! cam ) {
#if SOXT_DEBUG
    SoDebugError::postInfo( "SoXtFullViewer::farwheelvaluechanged",
      "no camera" );
#endif // SOXT_DEBUG
    return;
  }

  cam->farDistance = val;
} // farwheelvaluechanged()

/*!
  static callback
*/

void
SoXtFullViewer::farwheelvaluechangedCB(
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->farwheelvaluechanged();
} // farwheelvaluechangedCB()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::nearvaluechanged(
  void )
{
  float val = atof( XmTextGetString( this->nearvalue ) );
  float farval = SoXtThumbWheelGetValue( this->farwheel );
  if ( val >= farval )
    val = farval - 0.001f;
  float temp = SoXtThumbWheelGetValue( this->nearwheel );
  if ( val != temp )
    SoXtThumbWheelSetValue( this->nearwheel, val );
  char buf[16];
  sprintf( buf, "%g", val );
  XmTextSetString( this->nearvalue, buf );
  XmTextSetCursorPosition( this->nearvalue, (long) strlen(buf) );

  SoCamera * const camera = this->getCamera();
  if ( ! camera ) {
    SoDebugError::postInfo( "SoXtFullViewer::nearvaluechanged",
      "no camera!" );
    return;
  }
  camera->nearDistance = val;
} // nearvaluechanged()

/*!
  static callback
*/

void
SoXtFullViewer::nearvaluechangedCB(
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->nearvaluechanged();
} // nearvaluechangedCB()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::farvaluechanged(
  void )
{
  float nearval = SoXtThumbWheelGetValue( this->nearwheel );
  float val = atof( XmTextGetString( this->farvalue ) );
  if ( val <= nearval )
    val = nearval + 0.001f;
  float temp = SoXtThumbWheelGetValue( this->farwheel );
  if ( temp != val )
    SoXtThumbWheelSetValue( this->farwheel, val );
  char buf[16];
  sprintf( buf, "%g", val );
  XmTextSetString( this->farvalue, buf );
  XmTextSetCursorPosition( this->farvalue, (long) strlen(buf) );

  SoCamera * const camera = this->getCamera();
  if ( ! camera ) {
    SoDebugError::postInfo( "SoXtFullViewer::nearvaluechanged",
      "no camera!" );
    return;
  }
  camera->farDistance = val;
} // farvaluechanged()

/*!
  \internal
  static callback
*/

void
SoXtFullViewer::farvaluechangedCB(
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->farvaluechanged();
} // farvaluechangedCB()

// *************************************************************************
  
/*!
  FIXME: write doc
*/

void
SoXtFullViewer::selectedViewing(
  void )
{
  this->setViewing( this->isViewing() ? FALSE : TRUE );
} // selectedViewing()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::selectedDecoration(
  void )
{
  this->setDecoration( this->isDecoration() ? FALSE : TRUE );
} // selectedDecoration();

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::selectedHeadlight(
  void )
{
  SOXT_STUB();
} // selectedHeadlight();

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::selectedPrefs(
  void )
{
  if ( ! this->prefshell )
    this->createPrefSheet();
/*
  else
    FIXME: de-iconify prefshell and push to front
*/
} // selectedPrefs()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::interactbuttonClicked(
  Boolean set )
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtFullViewer::interactbuttonClicked",
    "%s", set ? "Set" : "Unset" );
#endif // SOXT_DEBUG
  if ( this->isViewing() && set )
    this->setViewing( FALSE );
  else if ( ! this->isViewing() && ! set )
    this->setViewing( TRUE );
} // interactbuttonClicked()

/*!
  staitc callback
*/

void
SoXtFullViewer::interactbuttonCB(
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->interactbuttonClicked( TRUE );
} // interactbuttonCB()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::examinebuttonClicked(
  Boolean set )
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtFullViewer::examinebuttonClicked",
    "%s", set ? "Set" : "Unset" );
#endif // SOXT_DEBUG
  if ( this->isViewing() && ! set )
    this->setViewing( FALSE );
  else if ( ! this->isViewing() && set )
    this->setViewing( TRUE );
} // examinebuttonClicked();

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::examinebuttonCB(
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->examinebuttonClicked( TRUE );
} // examinebuttonCB()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::helpbuttonClicked(
  void )
{
  this->openViewerHelpCard();
} // helpbuttonClicked()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::helpbuttonCB(
  Widget,
  XtPointer client_data,
  XtPointer )
{
  ((SoXtFullViewer *) client_data)->helpbuttonClicked();
} // helpbuttonCB()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::homebuttonClicked(
  void )
{
  this->resetToHomePosition();
} // homebuttonClicked()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::homebuttonCB(
  Widget,
  XtPointer client_data,
  XtPointer )
{
  ((SoXtFullViewer *) client_data)->homebuttonClicked();
} // homebuttonCB()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::sethomebuttonClicked(
  void )
{
  this->saveHomePosition();
} // sethomebuttonClicked()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::sethomebuttonCB(
  Widget,
  XtPointer client_data,
  XtPointer )
{
  ((SoXtFullViewer *) client_data)->sethomebuttonClicked();
} // sethomebuttonCB()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::viewallbuttonClicked(
  void )
{
  this->viewAll();
} // viewallbuttonClicked()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::viewallbuttonCB(
  Widget,
  XtPointer client_data,
  XtPointer )
{
  ((SoXtFullViewer *) client_data)->viewallbuttonClicked();
} // viewallbuttonCB()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::seekbuttonClicked(
  void )
{
  if ( ! this->isViewing() ) {
#if SOXT_DEBUG
    SoDebugError::postInfo( "SoXtFullViewer::seekbuttonClicked",
      "seek button should not be enabled in interaction mode!" );
#endif // SOXT_DEBUG
    return;
  }
  this->setSeekMode(this->isSeekMode() ? FALSE : TRUE);
} // seekbuttonClicked()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::seekbuttonCB(
  Widget,
  XtPointer client_data,
  XtPointer )
{
  ((SoXtFullViewer *) client_data)->seekbuttonClicked();
} // seekbuttonCB()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::copyviewSelected(
  void )
{
  this->copyView( SbTime::getTimeOfDay() );
} // copyviewSelected()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::pasteviewSelected(
  void )
{
  this->pasteView( SbTime::getTimeOfDay() );
} // pasteviewSelected()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::drawstyleActivated(
  int item )
{
  SOXT_STUB();
} // drawstyleActivated()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::increaseInteractiveCount(
  void )
{
  this->interactiveCountInc();
} // increaseInteractiveCount()

/*!
  static callback
*/

void
SoXtFullViewer::increaseInteractiveCountCB( // static
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->increaseInteractiveCount();
} // increaseInteractiveCountCB()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::decreaseInteractiveCount(
  void )
{
  this->interactiveCountDec();
} // decreaseInteractiveCount()

/*!
  static callback
*/

void
SoXtFullViewer::decreaseInteractiveCountCB(
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->decreaseInteractiveCount();
} // decreaseInteractiveCountCB()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::seektimechanged(
  void )
{
  float newtime = (float) atof( XmTextGetString( this->seektimefield ) );
  this->setSeekTime( newtime );
  char buf[16];
  sprintf( buf, "%g", newtime );
  XmTextSetString( this->seektimefield, buf );
  XmTextSetCursorPosition( this->seektimefield, (long) strlen(buf) );
} // seektimechanged()

/*!
  static callback
*/

void
SoXtFullViewer::seektimechangedCB( // static
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->seektimechanged();
} // seektimechangedCB()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::pointtoggled(
  void )
{
  XtVaSetValues( this->pointtoggle, XmNset, True, NULL );
  XtVaSetValues( this->objecttoggle, XmNset, False, NULL );
  if ( ! this->isDetailSeek() )
    this->setDetailSeek( TRUE );
} // pointtoggled()

/*!
  static callback
*/

void
SoXtFullViewer::pointtoggledCB( // static
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->pointtoggled();
} // pointtoggledCB()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::objecttoggled(
  void )
{
  XtVaSetValues( this->pointtoggle, XmNset, False, NULL );
  XtVaSetValues( this->objecttoggle, XmNset, True, NULL );
  if ( this->isDetailSeek() )
    this->setDetailSeek( FALSE );
} // objecttoggled()

/*!
  static callback
*/

void
SoXtFullViewer::objecttoggledCB( // static
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->objecttoggled();
} // objecttoggledCB()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::seekdistancechanged(
  void )
{
  float newdist = (float) atof( XmTextGetString( this->seekdistancefield ) );
  if ( newdist < 0.0f )
    newdist = 0.0f;
  this->seekdistance = newdist;
  char buf[16];
  sprintf( buf, "%g", newdist );
  XmTextSetString( this->seekdistancefield, buf );
  XmTextSetCursorPosition( this->seekdistancefield, (long) strlen(buf) );
} // seekdistancechanged()

/*!
  static callback
*/

void
SoXtFullViewer::seekdistancechangedCB( // static
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->seekdistancechanged();
} // seekdistancechangedCB()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::percenttoggled(
  void )
{
  XtVaSetValues( this->percenttoggle, XmNset, True, NULL );
  XtVaSetValues( this->absolutetoggle, XmNset, False, NULL );
  this->seekdistaspercentage = TRUE;
} // percenttoggled()

/*!
  static callback
*/

void
SoXtFullViewer::percenttoggledCB(
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->percenttoggled();
} // percenttoggledCB()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::absolutetoggled(
  void )
{
  XtVaSetValues( this->percenttoggle, XmNset, False, NULL );
  XtVaSetValues( this->absolutetoggle, XmNset, True, NULL );
  this->seekdistaspercentage = FALSE;
} // absolutetoggled()

/*!
  static callback
*/

void
SoXtFullViewer::absolutetoggledCB( // static
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->absolutetoggled();
} // absolutetoggledCB()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::stereotoggled(
  void )
{
  Boolean enable = False;
  XtVaGetValues( this->stereotoggle, XmNset, &enable, NULL );
  this->setStereoViewing( enable ? TRUE : FALSE );
  XtVaSetValues( this->stereotoggle,
    XmNset, this->isStereoViewing() ? True : False,
    NULL );
} // stereotoggled()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::stereotoggledCB( // static
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->stereotoggled();
} // stereotoggledCB()

// *************************************************************************

/*!
  This method returns the current camera zoom value (degrees used in the
  camera field of view) or 0.0f if "zoom" has no meaning (no camera or a
  non-perspective camera).
*/

float
SoXtFullViewer::getCameraZoom(
  void )
{
  SoCamera * const camera = this->getCamera();
  if ( ! camera ) {
#if SOXT_DEBUG
    SoDebugError::postInfo( "SoXtFullViewer::getCameraZoom",
      "no camera!" );
#endif // SOXT_DEBUG
    return 0.0f;
  }

  const SoType camtype( camera->getTypeId() );
  if ( ! camtype.isDerivedFrom( SoPerspectiveCamera::getClassTypeId() ) ) {
#if SOXT_DEBUG
    SoDebugError::postInfo( "SoXtFullViewer::getCameraZoom",
      "not using a perspective camera!" );
#endif // SOXT_DEBUG
    return 0.0f;
  }

  SoPerspectiveCamera * const pCamera = (SoPerspectiveCamera *) camera;
  return pCamera->heightAngle.getValue() * 180 / M_PI;
} // getCameraZoom()

/*!
  This method sets the camera zoom value (given the scene graph camera is a
  perspecive camera).  \a zoom must be within the range between 0 and 180
  (degrees to use as field of view).
*/

void
SoXtFullViewer::setCameraZoom(
  float zoom )
{
  SoCamera * const camera = this->getCamera();
  if ( camera == NULL ) {
#if SOXT_DEBUG
    SoDebugError::postInfo( "SoXtFullViewer::setCameraZoom",
      "no camera!" );
#endif // SOXT_DEBUG
    return;
  }

  const SoType camtype( camera->getTypeId() );
  if ( ! camtype.isDerivedFrom( SoPerspectiveCamera::getClassTypeId() ) ) {
#if SOXT_DEBUG
    SoDebugError::postInfo( "SoXtFullViewer::getCameraZoom",
      "not using a perspective camera!" );
#endif // SOXT_DEBUG
    return;
  }

  SoPerspectiveCamera * const pCamera = (SoPerspectiveCamera *) camera;
  pCamera->heightAngle = zoom * M_PI / 180.0f;
} // setCameraZoom()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::zoomfromchanged(
  void )
{
  float value = atof( XmTextGetString( this->zoomfrom ) );
  if ( value < 0.001f )
    value = 0.001f;
  else if ( value > this->zoomrange[1] )
    value = this->zoomrange[1];
  this->zoomrange[0] = value;

  char buf[16];
  sprintf( buf, "%.1f", value );
  XmTextSetString( this->zoomfrom, buf );
  XmTextSetCursorPosition( this->zoomfrom, (long) strlen(buf) );

  float zoom = this->getCameraZoom();
  if ( zoom < this->zoomrange[0] ) {
    zoom = this->zoomrange[0];
    this->setCameraZoom( zoom );
    sprintf( buf, "%.1f", zoom );
    XmTextSetString( this->zoomvalue, buf );
    XmTextSetCursorPosition( this->zoomvalue, (long) strlen(buf) );
  } else if ( zoom > this->zoomrange[1] ) {
    zoom = this->zoomrange[1];
    this->setCameraZoom( zoom );
    sprintf( buf, "%.1f", zoom );
    XmTextSetString( this->zoomvalue, buf );
    XmTextSetCursorPosition( this->zoomvalue, (long) strlen(buf) );
  }

  if ( this->zoomrange[0] == this->zoomrange[1] ) {
    XmScaleSetValue( this->zoomslider, 0 );
  } else {
    float normalized = (zoom - this->zoomrange[0]) /
      (this->zoomrange[1] - this->zoomrange[0]);
    int scaledval = (int) (sqrt(normalized) * 1000.0f);
    XmScaleSetValue( this->zoomslider, scaledval );
  }
} // zoomfromchanged()

/*!
  static callback
*/

void
SoXtFullViewer::zoomfromchangedCB( // static
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->zoomfromchanged();
} // zoomfromchangedCB()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::zoomsliderchanged(
  void )
{
  Boolean editable = True;
  XtVaGetValues( this->zoomslider, XmNsensitive, &editable, NULL );
  if ( ! editable ) {
    float value = (float) atof( XmTextGetString( this->zoomvalue ) );
    if ( this->zoomrange[0] == this->zoomrange[1] ) {
      XmScaleSetValue( this->zoomslider, 0 );
    } else {
      float normalized = (value - this->zoomrange[0]) /
        (this->zoomrange[1] - this->zoomrange[0]);
      int scaledval = (int) (sqrt(normalized) * 1000.0f);
      XmScaleSetValue( this->zoomslider, scaledval );
    }
    return;
  }

  int intval = 0;
  XmScaleGetValue( this->zoomslider, &intval );

  float expval = (float) (intval * intval) / 1000000.0f;
  float realval =
    ((this->zoomrange[1] - this->zoomrange[0]) * expval) + this->zoomrange[0];

  float curzoom = this->getCameraZoom();
  if ( curzoom != realval ) {
    char buf[16];
    sprintf( buf, "%.1f", realval );
    XmTextSetString( this->zoomvalue, buf );
    XmTextSetCursorPosition( this->zoomvalue, (long) strlen(buf) );
    this->setCameraZoom( realval );
  }
} // zoomsliderchanged()

/*!
  static callback
*/

void
SoXtFullViewer::zoomsliderchangedCB(
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->zoomsliderchanged();
} // zoomsliderchangedCB()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::zoomtochanged(
  void )
{
  float value = atof( XmTextGetString( this->zoomto ) );
  if ( value > 180.0f )
    value = 180.0f;
  else if ( value < this->zoomrange[0] )
    value = this->zoomrange[0];

  char buf[16];
  sprintf( buf, "%.1f", value );
  XmTextSetString( this->zoomto, buf );
  XmTextSetCursorPosition( this->zoomto, (long) strlen(buf) );

  this->zoomrange[1] = value;
  float zoom = this->getCameraZoom();
  if ( zoom < this->zoomrange[0] ) {
    zoom = this->zoomrange[0];
    this->setCameraZoom( zoom );
    sprintf( buf, "%.1f", zoom );
    XmTextSetString( this->zoomvalue, buf );
    XmTextSetCursorPosition( this->zoomvalue, (long) strlen(buf) );
    this->setCameraZoom( value );
  } else if ( zoom > this->zoomrange[1] ) {
    zoom = this->zoomrange[1];
    this->setCameraZoom( zoom );
    sprintf( buf, "%.1f", zoom );
    XmTextSetString( this->zoomvalue, buf );
    XmTextSetCursorPosition( this->zoomvalue, (long) strlen(buf) );
    this->setCameraZoom( value );
  }

  if ( this->zoomrange[0] == this->zoomrange[1] ) {
    XmScaleSetValue( this->zoomslider, 0 );
  } else {
    float normalized = (zoom - this->zoomrange[0]) /
      (this->zoomrange[1] - this->zoomrange[0]);
    int scaledval = (int) (sqrt(normalized) * 1000.0f);
    XmScaleSetValue( this->zoomslider, scaledval );
  }
} // zoomtochanged()

/*!
  static callback
*/

void
SoXtFullViewer::zoomtochangedCB( // static
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->zoomtochanged();
} // zoomtochangedCB()

/*!
*/

void
SoXtFullViewer::zoomvaluechanged(
  void )
{
  float value = atof( XmTextGetString( this->zoomvalue ) );
  if ( value < this->zoomrange[0] )
    value = this->zoomrange[0];
  else if ( value > this->zoomrange[1] )
    value = this->zoomrange[1];

  char buf[16];
  sprintf( buf, "%.1f", value );
  XmTextSetString( this->zoomvalue, buf );
  XmTextSetCursorPosition( this->zoomvalue, (long) strlen(buf) );

  if ( this->zoomrange[0] == this->zoomrange[1] ) {
    XmScaleSetValue( this->zoomslider, 0 );
  } else {
    float normalized = (value - this->zoomrange[0]) /
      (this->zoomrange[1] - this->zoomrange[0]);
    int scaledval = (int) (sqrt(normalized) * 1000.0f);
    XmScaleSetValue( this->zoomslider, scaledval );
  }

  this->setCameraZoom( value );
} // zoomvaluechanged()

/*!
  static callback
*/

void
SoXtFullViewer::zoomvaluechangedCB(
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->zoomvaluechanged();
} // zoomvaluechangedCB()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::speedInc(
  void )
{
  SOXT_STUB_ONCE();
} // speedInc()

/*!
  static callback
*/

void
SoXtFullViewer::speedIncCB(
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->speedInc();
} // speedIncCB()

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::speedDec(
  void )
{
  SOXT_STUB_ONCE();
} // speedDec()

/*!
  static callback
*/

void
SoXtFullViewer::speedDecCB(
  Widget,
  XtPointer closure,
  XtPointer )
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->speedDec();
} // speedDecCB()

// *************************************************************************

/*!
*/

void
SoXtFullViewer::sizeChanged(
  const SbVec2s size )
{
  SbVec2s newsize( size );
  if ( this->isDecoration() ) {
    newsize[0] = size[0] - 2 * 30;
    newsize[1] = size[1] - 30;
  }
  inherited::sizeChanged( newsize );
} // sizeChanged()

// *************************************************************************

/*!
*/

SbBool
SoXtFullViewer::processSoEvent( // virtual, protected
  const SoEvent * const event )
{
  if ( common->processSoEvent(event) ) return TRUE;
  return inherited::processSoEvent(event);
} // processSoEvent()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtFullViewerRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG
