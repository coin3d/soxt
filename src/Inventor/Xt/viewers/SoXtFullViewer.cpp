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

#if HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <assert.h>
#include <string.h>
#include <stdlib.h>

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

#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/widgets/SoXtThumbWheel.h>
#include <Inventor/Xt/widgets/SoAnyPopupMenu.h>

#include <Inventor/Xt/viewers/SoAnyFullViewer.h>
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

#define VIEWERBUTTON(button) ((Widget) ((*this->viewerButtonsList)[button]))

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
, common( new SoAnyFullViewer( this ) )
, popupTitle( NULL )
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

  this->seekdistance = 50.0f;
  this->seekdistaspercentage = TRUE;

  this->zoomrange = SbVec2f( 1.0f, 140.0f );

  this->popupEnabled = (flags & SoXtFullViewer::BUILD_POPUP) ? TRUE : FALSE;
  this->decorations = (flags & SoXtFullViewer::BUILD_DECORATION) ? TRUE : FALSE;

  this->appButtonsList = new SbPList;
  this->viewerButtonsList = new SbPList;

  this->setSize( SbVec2s( 500, 390 ) );
  this->setClassName( "SoXtFullViewer" );

  this->prefshell = NULL;
  this->prefmenu = NULL;

  pixmaps.pick = 0;
  pixmaps.view = 0;
  pixmaps.help = 0;
  pixmaps.home = 0;
  pixmaps.set_home = 0;
  pixmaps.view_all = 0;
  pixmaps.seek = 0;

  if ( build != FALSE )
    this->buildWidget( parent );
} // SoXtFullViewer()

/*!
  The destructor.
*/

SoXtFullViewer::~SoXtFullViewer( // protected
  void )
{
  delete this->common;
  delete this->appButtonsList;
  delete this->viewerButtonsList;
} // ~SoXtFullViewer()

// *************************************************************************

static const int SOXT_VIEWER_MIN_HEIGHT_BASE = 30 + 90 + 8;
static const int SOXT_VIEWER_MIN_WIDTH = 300;

/*!
*/

void
SoXtFullViewer::setDecoration(
  const SbBool enable )
{
  if ( (this->decorations != enable) && (this->viewerbase != (Widget) NULL) )
    this->showDecorationWidgets( enable );
  this->decorations = enable;
  if ( this->prefmenu )
    this->prefmenu->SetMenuItemMarked( DECORATION_ITEM, enable );
  Widget shell = this->baseWidget();
  while ( shell != NULL && ! XtIsWMShell( shell ) )
    shell = XtParent( shell );

  if ( shell ) {
    Dimension curminwidth, curwidth, curminheight, curheight;
    Dimension minwidth = SOXT_VIEWER_MIN_WIDTH;
    Dimension minheight = SOXT_VIEWER_MIN_HEIGHT_BASE +
       30 * this->viewerButtonsList->getLength();
    XtVaGetValues( shell,
      XmNminHeight, &curminheight,
      XmNheight, &curheight,
      XmNminWidth, &curminwidth,
      XmNwidth, &curwidth,
      NULL );

    if ( enable ) {
      if ( curminwidth == 0 ) {
        XtVaSetValues( shell, XmNminWidth, minwidth, NULL );
        if ( curwidth < minwidth )
          XtVaSetValues( shell, XmNwidth, minwidth, NULL );
      }
      if ( curminheight == 0 ) {
        XtVaSetValues( shell, XmNminHeight, minheight, NULL );
        if ( curheight < minheight )
          XtVaSetValues( shell, XmNheight, minheight, NULL );
      }
    } else {
      if ( curminwidth == minwidth )
        XtVaSetValues( shell, XmNminWidth, 0, NULL );
      if ( curminheight == minheight )
        XtVaSetValues( shell, XmNminHeight, 0, NULL );
    }
  }
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
  this->popupEnabled = enable;
} // setPopupMenuEnabled()

/*!
*/

SbBool
SoXtFullViewer::isPopupMenuEnabled(
  void ) const
{
  return this->popupEnabled;
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
  SOXT_STUB();
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
      XmNleftAttachment, XmATTACH_FORM,
      XmNleftOffset, 30,
      XmNtopAttachment, XmATTACH_FORM,
      XmNbottomAttachment, XmATTACH_FORM,
      XmNbottomOffset, 30,
      XmNrightAttachment, XmATTACH_FORM,
      XmNrightOffset, 30,
      NULL );
  this->setBorder( FALSE );

  XtManageChild( this->canvas );
  this->setBaseWidget( this->viewerbase );

  Widget shell = this->viewerbase;
  while ( shell && ! XtIsWMShell( shell ) )
    shell = XtParent( shell );
  if ( shell && this->decorations != FALSE ) {
    int existing = 0, current = 0;
    XtVaGetValues( shell,
      XmNminHeight, &existing,
      XmNheight, &current,
      NULL );
    Dimension minheight =
      30 + 90 + 30 * this->viewerButtonsList->getLength() + 8;
    if ( existing > minheight ) minheight = existing;
    XtVaSetValues( shell,
      XmNminWidth, 300,
      XmNminHeight, minheight,
      NULL );
    if ( current < minheight )
      XtVaSetValues( shell,
        XmNheight, minheight,
        NULL );
  }
  return this->viewerbase;
} // buildWidget()

// *************************************************************************

/*!
*/

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
  This method created the bottom decoration form.
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
      XmNtopAttachment, XmATTACH_OPPOSITE_FORM,
      XmNtopOffset, -30,
      NULL );

  const char * string;

  string = this->wheelstrings[LEFTDECORATION].getString();
  this->wheellabels[LEFTDECORATION] = XtVaCreateManagedWidget( "Roty",
    xmLabelWidgetClass, trim,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNleftOffset, 5,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      string, strlen(string) + 1,
    NULL );

  string = this->wheelstrings[BOTTOMDECORATION].getString();
  this->wheellabels[BOTTOMDECORATION] = XtVaCreateManagedWidget( "Rotx",
    xmLabelWidgetClass, trim,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, this->wheellabels[LEFTDECORATION],
    XmNtopAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNleftOffset, 5,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      string, strlen(string) + 1,
    NULL );

  // add bottom thumb wheel
  this->wheels[BOTTOMDECORATION] = XtVaCreateManagedWidget( "BottomWheel",
    soxtThumbWheelWidgetClass, trim,
    XmNorientation, XmHORIZONTAL,
    XmNshadowType, XmSHADOW_OUT,
    XmNhighlightThickness, 0,
    XmNshadowThickness, 2,
    XmNtraversalOn, False,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, this->wheellabels[BOTTOMDECORATION],
    XmNleftOffset, 2,
    XmNtopAttachment, XmATTACH_FORM,
    XmNtopOffset, 2,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNbottomOffset, 2,
    XmNwidth, 90,
    NULL );

  XtAddCallback( this->wheels[BOTTOMDECORATION],
    XmNarmCallback, SoXtFullViewer::bottomWheelStartCB, this );
  XtAddCallback( this->wheels[BOTTOMDECORATION],
    XmNdisarmCallback, SoXtFullViewer::bottomWheelFinishCB, this );
  XtAddCallback( this->wheels[BOTTOMDECORATION],
    XmNvalueChangedCallback, SoXtFullViewer::bottomWheelMotionCB, this );

  string = this->wheelstrings[RIGHTDECORATION].getString();
  this->wheellabels[RIGHTDECORATION] =
    XtVaCreateManagedWidget( "rightwheellabel",
    xmLabelWidgetClass, trim,
    XmNtopAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XmNrightOffset, 5,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      string, strlen(string) + 1,
    NULL );

  return trim;
} // buildBottomTrim()

/*!
  This method creates the left decoration form.
*/

Widget
SoXtFullViewer::buildLeftTrim( // virtual
  Widget parent )
{
  Widget trim = XtVaCreateManagedWidget( "LeftTrim",
      xmFormWidgetClass, parent,
      XmNleftAttachment, XmATTACH_FORM,
      XmNtopAttachment, XmATTACH_FORM,
      XmNbottomAttachment, XmATTACH_WIDGET,
      XmNbottomWidget, this->decorform[BOTTOMDECORATION],
      XmNrightAttachment, XmATTACH_OPPOSITE_FORM,
      XmNrightOffset, -30,
      NULL );

  // build application buttons
  this->appButtonsForm = this->buildAppButtonsForm( trim );

  XtVaSetValues( this->appButtonsForm,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    NULL );
  XtManageChild( this->appButtonsForm );

  // add right thumb wheel
  this->wheels[LEFTDECORATION] = XtVaCreateManagedWidget( "LeftWheel",
    soxtThumbWheelWidgetClass, trim,
    XmNorientation, XmVERTICAL,
    XmNshadowType, XmSHADOW_OUT,
    XmNhighlightThickness, 0,
    XmNshadowThickness, 2,
    XmNtraversalOn, False,
    XmNleftAttachment, XmATTACH_FORM,
    XmNleftOffset, 2,
    XmNrightAttachment, XmATTACH_FORM,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNbottomOffset, 2,
    XmNheight, 90,
    NULL );

  XtAddCallback( this->wheels[LEFTDECORATION],
    XmNarmCallback, SoXtFullViewer::leftWheelStartCB, this );
  XtAddCallback( this->wheels[LEFTDECORATION],
    XmNdisarmCallback, SoXtFullViewer::leftWheelFinishCB, this );
  XtAddCallback( this->wheels[LEFTDECORATION],
    XmNvalueChangedCallback, SoXtFullViewer::leftWheelMotionCB, this );

  return trim;
} // buildLeftTrim()

/*!
  This method creates the right decoration form.
*/

Widget
SoXtFullViewer::buildRightTrim( // virtual
  Widget parent )
{
  Widget trim = XtVaCreateManagedWidget( "RightTrim",
      xmFormWidgetClass, parent,
      XmNtopAttachment, XmATTACH_FORM,
      XmNrightAttachment, XmATTACH_FORM,
      XmNbottomAttachment, XmATTACH_WIDGET,
      XmNbottomWidget, this->decorform[BOTTOMDECORATION],
      XmNleftAttachment, XmATTACH_OPPOSITE_FORM,
      XmNleftOffset, -30,
      NULL );

  Widget buttonForm = this->buildViewerButtons( trim );
  XtManageChild( buttonForm );

  // add right thumb wheel
  this->wheels[RIGHTDECORATION] = XtVaCreateManagedWidget( "RightWheel",
    soxtThumbWheelWidgetClass, trim,
    XmNorientation, XmVERTICAL,
    XmNshadowType, XmSHADOW_OUT,
    XmNhighlightThickness, 0,
    XmNshadowThickness, 2,
    XmNtraversalOn, False,
    XmNleftAttachment, XmATTACH_FORM,
    XmNleftOffset, 2,
    XmNrightAttachment, XmATTACH_FORM,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNbottomOffset, 2,
    XmNheight, 90,
    NULL );

  XtAddCallback( this->wheels[RIGHTDECORATION],
    XmNarmCallback, SoXtFullViewer::rightWheelStartCB, this );
  XtAddCallback( this->wheels[RIGHTDECORATION],
    XmNdisarmCallback, SoXtFullViewer::rightWheelFinishCB, this );
  XtAddCallback( this->wheels[RIGHTDECORATION],
    XmNvalueChangedCallback, SoXtFullViewer::rightWheelMotionCB, this );

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
  if ( this->prefmenu ) {
    this->prefmenu->SetMenuItemMarked( EXAMINING_ITEM, enable );
    this->prefmenu->SetMenuItemEnabled( SEEK_ITEM, enable );
  }

  XtUnmanageChild( this->viewerbuttons.view );
  XtSetSensitive( this->viewerbuttons.view, enable ? False : True );
  XtVaSetValues( this->viewerbuttons.view,
                 XmNset, enable ? True : False,
                 XmNwidth, 30,
                 XmNheight, 30,
                 NULL );
  XtManageChild( this->viewerbuttons.view );

  XtUnmanageChild( this->viewerbuttons.pick );
  XtSetSensitive( this->viewerbuttons.pick, enable ? True : False );
  XtVaSetValues( this->viewerbuttons.pick,
                 XmNset, enable ? False : True,
                 XmNwidth, 30,
                 XmNheight, 30,
                 NULL );
  XtManageChild( this->viewerbuttons.pick );

  XtUnmanageChild( this->viewerbuttons.seek );
  XtSetSensitive( this->viewerbuttons.seek,
    enable ? True : False );
  XtVaSetValues( this->viewerbuttons.seek,
    XmNwidth, 30,
    XmNheight, 30,
    NULL );
  XtManageChild( this->viewerbuttons.seek );
} // setViewing()

/*!
*/

void
SoXtFullViewer::setHeadlight( // virtual
  SbBool enable )
{
  inherited::setHeadlight( enable );
  if ( this->prefmenu )
    this->prefmenu->SetMenuItemMarked( HEADLIGHT_ITEM, enable );
} // setHeadlight()

/*!
*/

void
SoXtFullViewer::setDrawStyle( // virtual
  SoXtViewer::DrawType type,
  SoXtViewer::DrawStyle style )
{
  inherited::setDrawStyle( type, style );
  if ( this->prefmenu )
    common->setDrawStyleMenuActivation( type, style );
} // setDrawStyle()

/*!
*/

void
SoXtFullViewer::setBufferingType( // virtual
  SoXtViewer::BufferType type )
{
  inherited::setBufferingType( type );

  if ( this->prefmenu ) {
    switch ( type ) {
    case SoXtViewer::BUFFER_SINGLE:
      this->prefmenu->SetMenuItemMarked( SINGLE_BUFFER_ITEM, TRUE );
      break;
    case SoXtViewer::BUFFER_DOUBLE:
      this->prefmenu->SetMenuItemMarked( DOUBLE_BUFFER_ITEM, TRUE );
      break;
    case SoXtViewer::BUFFER_INTERACTIVE:
      this->prefmenu->SetMenuItemMarked( INTERACTIVE_BUFFER_ITEM, TRUE );
      break;
    default:
      assert( 0 && "unsupported buffer type" );
      break;
    } // switch ( type )
  }
} // setBufferingType()

/*!
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

Widget
SoXtFullViewer::buildViewerButtons(
  Widget parent )
{
  Widget form = XtVaCreateManagedWidget( "ViewerButtons",
      xmFormWidgetClass, parent, NULL );

  this->createViewerButtons( form, this->viewerButtonsList );

  const int buttons = this->viewerButtonsList->getLength();

  XtVaSetValues( form,
    XmNx, 0, XmNy, 0,
    XmNleftAttachment, XmATTACH_POSITION,
    XmNtopAttachment, XmATTACH_POSITION,
    XmNwidth, 30,
    XmNheight, buttons * 30,
    XmNfractionBase, buttons,
    NULL );

  for ( int i = 0; i < this->viewerButtonsList->getLength(); i++ ) {
    Widget button = (Widget) (*this->viewerButtonsList)[i];
    XtVaSetValues( button,
       XmNwidth, 30, XmNheight, 30,
       XmNleftAttachment, XmATTACH_POSITION,
       XmNtopAttachment, XmATTACH_POSITION,
       XmNleftPosition, 0, XmNtopPosition, i,
       NULL );
  }

  XtUnmanageChild( this->viewerbuttons.view );
  XtSetSensitive( this->viewerbuttons.view, this->isViewing() ? False : True );
  XtVaSetValues( this->viewerbuttons.view,
                 XmNset, this->isViewing() ? True : False,
                 XmNwidth, 30,
                 XmNheight, 30,
                 NULL );
  XtManageChild( this->viewerbuttons.view );

  XtUnmanageChild( this->viewerbuttons.pick );
  XtSetSensitive( this->viewerbuttons.pick, this->isViewing() ? True : False );
  XtVaSetValues( this->viewerbuttons.pick,
                 XmNset, this->isViewing() ? False : True,
                 XmNwidth, 30,
                 XmNheight, 30,
                 NULL );
  XtManageChild( this->viewerbuttons.pick );

  return form;
} // buildViewerButtons()

// *************************************************************************

/*
  \internal
  Does nothing if libXpm use hasn't been enabled.
*/

Pixmap
SoXtFullViewer::createPixmapFromXpmData(
  Widget button,
  char ** xpm )
{
#if HAVE_LIBXPM
  Pixel bg;
  XtVaGetValues( button, XmNbackground, &bg, NULL );
  Display * dpy = SoXt::getDisplay();
  XImage * image, * stencil;
  int error = XpmCreateImageFromData( dpy, xpm, &image, &stencil, NULL );
  if ( error != XpmSuccess ) return 0;
  for ( int x = 0; x < image->width; x++ ) {
    for ( int y = 0; y < image->height; y++ ) {
      Pixel pixel = XGetPixel( stencil, x, y );
      if ( pixel == 0 ) // background must be set in image
        XPutPixel( image, x, y, bg );
    }
  }
  const int width = 24, height = 24;
  Drawable draw = RootWindow( dpy, DefaultScreen(dpy) );
  Pixmap retval = XCreatePixmap( dpy, draw, width, height, image->depth );
  GC gc = XCreateGC( dpy, draw, 0, NULL );
  XPutImage( dpy, retval, gc, image, 0, 0, 0, 0, width, height );
  XFreeGC( dpy, gc );
  XDestroyImage( image );
  XDestroyImage( stencil );
  return retval;
#endif // HAVE_LIBXPM
  return (Pixmap) 0;
} // ButtonPixmap()

/*!
  \internal
  Does nothing if libXpm use hasn't been enabled.
*/

Pixmap
SoXtFullViewer::createInsensitivePixmapFromXpmData(
  Widget button,
  char ** xpm )
{
#if HAVE_LIBXPM
  Pixel bg;
  XtVaGetValues( button, XmNbackground, &bg, NULL );
  Display * dpy = SoXt::getDisplay();
  XImage * image, * stencil;
  int error = XpmCreateImageFromData( dpy, xpm, &image, &stencil, NULL );
  if ( error != XpmSuccess ) return 0;
  for ( int x = 0; x < image->width; x++ ) {
    for ( int y = 0; y < image->height; y++ ) {
      Pixel pixel = XGetPixel( stencil, x, y );
      if ( (pixel == 0) || (((x+y) & 1) == 1) )
        XPutPixel( image, x, y, bg );
    }
  }
  const int width = 24, height = 24;
  Drawable draw = RootWindow( dpy, DefaultScreen(dpy) );
  Pixmap retval = XCreatePixmap( dpy, draw, width, height, image->depth );
  GC gc = XCreateGC( dpy, draw, 0, NULL );
  XPutImage( dpy, retval, gc, image, 0, 0, 0, 0, width, height );
  XFreeGC( dpy, gc );
  XDestroyImage( image );
  XDestroyImage( stencil );
  return retval;
#endif // HAVE_LIBXPM
  return (Pixmap) 0;
} // createInsensitivePixmapFromXpmData()

/*!
*/

void
SoXtFullViewer::createViewerButtons(
  Widget parent,
  SbPList * buttonlist )
{
  assert( buttonlist != NULL );

  int viewerbutton;
  for ( viewerbutton = 0; viewerbutton <= SEEK_BUTTON; viewerbutton++ ) {
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
        XmNshadowType, XmSHADOW_OUT,
        XmNhighlightThickness, 0,
        XmNshadowThickness, 2,
        XmNtraversalOn, False,
        XmNwidth, 30,
        XmNheight, 30,
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
        XmNshadowType, XmSHADOW_OUT,
        XmNhighlightThickness, 0,
        XmNshadowThickness, 2,
        XmNtraversalOn, False,
        XmNwidth, 30,
        XmNheight, 30,
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
      pixmap = pixmap_ins = pixmaps.pick =
        createPixmapFromXpmData( button, pick_xpm );
      break;
    case EXAMINE_BUTTON:
      pixmap = pixmap_ins = pixmaps.view =
        createPixmapFromXpmData( button, view_xpm );
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
    XtVaSetValues( button,
      XmNlabelType, XmPIXMAP,
      XmNlabelPixmap, pixmap,
      XmNlabelInsensitivePixmap, pixmap_ins,
      XmNselectPixmap, pixmap,
      XmNselectInsensitivePixmap, pixmap_ins,
      XmNwidth, 30, XmNheight, 30,
      NULL );
#endif // HAVE_LIBXPM

    if ( proc != NULL ) {
      if ( viewerbutton == INTERACT_BUTTON || viewerbutton == EXAMINE_BUTTON ) {
        XtAddCallback( button, XmNdisarmCallback, proc, this );
      } else {
        XtAddCallback( button, XmNactivateCallback, proc, this );
      }
    }
    buttonlist->append( button );
  }
} // createViewerButtons()

/*!
*/

void
SoXtFullViewer::buildPopupMenu(
  void )
{
  if ( this->prefmenu == NULL )
    this->prefmenu = common->setupStandardPopupMenu();

  this->setDrawStyle(
    SoXtViewer::STILL, this->getDrawStyle( SoXtViewer::STILL ) );
  this->setDrawStyle(
    SoXtViewer::INTERACTIVE, this->getDrawStyle( SoXtViewer::INTERACTIVE ) );
  this->setBufferingType( this->getBufferingType() );

  this->prefmenu->SetMenuItemMarked( EXAMINING_ITEM, this->isViewing() );
  this->prefmenu->SetMenuItemMarked( DECORATION_ITEM, this->decorations );
  this->prefmenu->SetMenuItemMarked( HEADLIGHT_ITEM, this->isHeadlight() );
  this->prefmenu->SetMenuItemEnabled( SEEK_ITEM, this->isViewing() );
} // buildPopupMenu()

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

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  leftWheelStart )
{
  this->leftWheelStart();
} // leftWheelStart()

/*!
*/

void
SoXtFullViewer::leftWheelMotion( // virtual
  float value )
{
  this->wheelvalues[LEFTDECORATION] = value;
} // leftWheelMotion()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  leftWheelMotion )
{
  SoXtThumbWheelCallbackData * data = (SoXtThumbWheelCallbackData *) call_data;
  this->leftWheelMotion( data->current );
} // leftWheelMotionCB()

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

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  leftWheelFinish )
{
  this->leftWheelFinish();
} // leftWheelFinishCB()

/*!
*/

float
SoXtFullViewer::getLeftWheelValue(
  void ) const
{
  return this->wheelvalues[LEFTDECORATION];
} // getLeftWheelValue()

/*!
*/

void
SoXtFullViewer::setLeftWheelValue(
  const float value )
{
  this->wheelvalues[LEFTDECORATION] = value;
  SoXtThumbWheelSetValue( this->wheels[LEFTDECORATION], value );
} // setLeftWheelValue()

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

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  bottomWheelStart )
{
  this->bottomWheelStart();
} // bottomWheelStart()

/*!
*/

void
SoXtFullViewer::bottomWheelMotion( // virtual
  float value )
{
  this->wheelvalues[BOTTOMDECORATION] = value;
} // bottomWheelMode()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  bottomWheelMotion )
{
  SoXtThumbWheelCallbackData * data = (SoXtThumbWheelCallbackData *) call_data;
  this->bottomWheelMotion( data->current );
} // bottomWheelStart()

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

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  bottomWheelFinish )
{
  this->bottomWheelFinish();
} // bottomWheelStart()

/*!
*/

float
SoXtFullViewer::getBottomWheelValue(
  void ) const
{
  return this->wheelvalues[BOTTOMDECORATION];
} // getBottomWheelValue()

/*!
*/

void
SoXtFullViewer::setBottomWheelValue(
  const float value )
{
  this->wheelvalues[BOTTOMDECORATION] = value;
  SoXtThumbWheelSetValue( this->wheels[BOTTOMDECORATION], value );
} // setBottomWheelValue()

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

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  rightWheelStart )
{
  this->rightWheelStart();
} // rightWheelStartCB()

/*!
*/

void
SoXtFullViewer::rightWheelMotion( // virtual
  float value )
{
  this->wheelvalues[RIGHTDECORATION] = value;
} // rightWheelMotion()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  rightWheelMotion )
{
  SoXtThumbWheelCallbackData * data = (SoXtThumbWheelCallbackData *) call_data;
  this->rightWheelMotion( data->current );
} // rightWheelMotionCB()

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

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  rightWheelFinish )
{
  this->rightWheelFinish();
} // rightWheelFinishCB()

/*!
*/

float
SoXtFullViewer::getRightWheelValue(
  void ) const
{
  return this->wheelvalues[RIGHTDECORATION];
} // getRightWheelValue()

/*!
*/

void
SoXtFullViewer::setRightWheelValue(
  const float value )
{
  this->wheelvalues[RIGHTDECORATION] = value;
  SoXtThumbWheelSetValue( this->wheels[RIGHTDECORATION], value );
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
  this->wheelstrings[LEFTDECORATION] = string;
  if ( this->wheellabels[LEFTDECORATION] != NULL )
    XtVaSetValues( this->wheellabels[LEFTDECORATION],
      XtVaTypedArg,
        XmNlabelString, XmRString,
        string, strlen(string) + 1,
      NULL );
} // setLeftWheelString()

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
  this->wheelstrings[BOTTOMDECORATION] = string;
  if ( this->wheellabels[BOTTOMDECORATION] != NULL )
    XtVaSetValues( this->wheellabels[BOTTOMDECORATION],
      XtVaTypedArg,
        XmNlabelString, XmRString,
        string, strlen(string) + 1,
      NULL );
} // setBottomWheelString()

/*!
  This method sets the label text that is displayed under the right decoration
  thumb wheel.
*/

void
SoXtFullViewer::setRightWheelString(
  const char * const string )
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtFullViewer::setRightWheelString",
    "setting right wheel label" );
#endif // SOXT_DEBUG
  this->wheelstrings[RIGHTDECORATION] = string;
  if ( this->wheellabels[RIGHTDECORATION] != NULL )
    XtVaSetValues( this->wheellabels[RIGHTDECORATION],
      XtVaTypedArg,
        XmNlabelString, XmRString,
        string, strlen(string) + 1,
      NULL );
} // setRightWheelString()

// *************************************************************************

/*!
  Default implementation does nothing.
*/

void
SoXtFullViewer::openViewerHelpCard( // virtual
  void )
{
  SOXT_STUB();
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

  if ( enable ) {
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
*/

void
SoXtFullViewer::createPrefSheet( // protected, virtual
  void )
{
/*¡
  Method could be made abstract.
*/
} // createPrefSheet()

/*!
*/

void
SoXtFullViewer::createPrefSheetShellAndForm(  // protected
  Widget & shell,
  Widget & form )
{
  assert( this->prefshell == NULL );

  if ( ! this->prefstring )
    this->setPrefSheetString( "Xt Viewer Preferences" );

  shell = XtVaAppCreateShell( NULL, "SoXt",
    topLevelShellWidgetClass, SoXt::getDisplay(),
    XtNtitle, this->prefstring,
    NULL );
  XtAddCallback( shell, XmNdestroyCallback,
    prefSheetDestroyCB, (XtPointer) this );
  this->prefshell = shell;

  form = XtVaCreateWidget( "form", xmFormWidgetClass, shell, NULL );
  this->prefsheet = form;
} // createPrefSheetShellAndForm()

/*!
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
  if ( (widgets[num] = this->createFramedClippingPrefSheetGuts( form )) )
    num++;
  if ( (widgets[num] = this->createFramedStereoPrefSheetGuts( form )) )
    num++;
  // NOTE: SpeedPrefs is set up on SoXtFullViewer level, but is only used
  // by WalkViewer and FlyViewer, so it is not included as default...
} // createDefaultPrefSheetParts()

/*!
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

  XmString labelstring;
  labelstring = SoXt::encodeString( "Seek Settings" );
  XtVaCreateManagedWidget( "title",
    xmLabelGadgetClass, frame,
    XmNchildType, XmFRAME_TITLE_CHILD,
    XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
    XmNlabelString, labelstring,
    NULL );
  XtFree( (char *) labelstring );

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
  XmString labelstring;
  labelstring = SoXt::encodeString( "Seek Distance Settings" );
  XtVaCreateManagedWidget( "seekdisttitle",
    xmLabelGadgetClass, frame,
    XmNchildType, XmFRAME_TITLE_CHILD,
    XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
    XmNlabelString, labelstring,
    NULL );
  XtFree( (char *) labelstring );

  Widget form = XtVaCreateManagedWidget( "form",
    xmFormWidgetClass, frame,
    NULL );

  this->createSeekDistPrefSheetGuts( form );

  return frame;
} // createFramedSeekDistPrefSheetGuts()

/*!
*/

Widget
SoXtFullViewer::createFramedZoomPrefSheetGuts( // protected
  Widget parent )
{
  Widget frame = XtVaCreateManagedWidget( "zoomframe",
    xmFrameWidgetClass, parent,
    NULL );

  XmString labelstring = SoXt::encodeString( "Zoom Settings" );
  XtVaCreateManagedWidget( "title",
    xmLabelGadgetClass, frame,
    XmNchildType, XmFRAME_TITLE_CHILD,
    XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
    XmNlabelString, labelstring,
    NULL );
  XtFree( (char *) labelstring );

  this->createZoomPrefSheetGuts( frame );

  return frame;
} // createFramedZoomPrefSheetGuts()

/*!
*/

Widget
SoXtFullViewer::createFramedClippingPrefSheetGuts(
  Widget parent )
{
  Widget frame = XtVaCreateManagedWidget( "clippingframe",
    xmFrameWidgetClass, parent,
    NULL );

  XmString labelstring = SoXt::encodeString( "Clipping Plane Settings" );
  XtVaCreateManagedWidget( "title",
    xmLabelGadgetClass, frame,
    XmNchildType, XmFRAME_TITLE_CHILD,
    XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
    XmNlabelString, labelstring,
    NULL );
  XtFree( (char *) labelstring );

  this->createClippingPrefSheetGuts( frame );
  return frame;
} // createFramedClippingPrefSheetGuts()

// *************************************************************************

/*!
*/

Widget
SoXtFullViewer::createFramedStereoPrefSheetGuts(
  Widget parent )
{
  Widget frame = XtVaCreateManagedWidget( "stereoframe",
    xmFrameWidgetClass, parent, NULL );

  XmString labelstring = SoXt::encodeString( "Stereo Viewing Settings" );
  Widget label = XtVaCreateManagedWidget( "stereoframelabel",
    xmLabelGadgetClass, frame,
    XmNchildType, XmFRAME_TITLE_CHILD,
    XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
    XmNlabelString, labelstring,
    NULL );

  this->createStereoPrefSheetGuts( frame );

  return frame;
} // createFramedStereoPrefSheetGuts()

/*!
*/

Widget
SoXtFullViewer::createFramedSpeedPrefSheetGuts(
  Widget parent )
{
  Widget frame = XtVaCreateManagedWidget( "speedframe",
    xmFrameWidgetClass, parent, NULL );

  XmString labelstring = SoXt::encodeString( "Speed Settings" );
  Widget label = XtVaCreateManagedWidget( "speedframelabel",
    xmLabelGadgetClass, frame,
    XmNchildType, XmFRAME_TITLE_CHILD,
    XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
    XmNlabelString, labelstring,
    NULL );

  this->createSpeedPrefSheetGuts( frame );

  return frame;
} // createFramedSpeedPrefSheetGuts()

// *************************************************************************

/*!
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

  XmString labelstring;
  labelstring = SoXt::encodeString( "Seek animation time: " );
  Widget label = XtVaCreateManagedWidget( "seektime",
    xmLabelWidgetClass, line1,
    XmNlabelString, labelstring,
    XmNtopAttachment, XmATTACH_FORM,
    XmNleftAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    NULL );
  XtFree( (char *) labelstring );

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
  XmTextSetCursorPosition( this->seektimefield, strlen(buffer) );

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

  labelstring = SoXt::encodeString( "Seek to: " );
  Widget tolabel = XtVaCreateManagedWidget( "tolabel",
    xmLabelWidgetClass, line2,
    XmNlabelString, labelstring,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    NULL );
  XtFree( (char *) labelstring );

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

  XmString labelstring;
  labelstring = SoXt::encodeString( "Seek distance: " );
  Widget label = XtVaCreateManagedWidget( "seekdistlabel",
    xmLabelWidgetClass, line1,
    XmNtopAttachment, XmATTACH_FORM,
    XmNleftAttachment, XmATTACH_FORM,
    XmNlabelString, labelstring,
    XmNbottomAttachment, XmATTACH_FORM,
    NULL );
  XtFree( (char *) labelstring );

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
  XmTextSetCursorPosition( this->seekdistancefield, strlen(buffer) );

  Widget line2 = XtVaCreateManagedWidget( "line 2",
    xmFormWidgetClass, form,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, line1,
    XmNleftAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    NULL );

  labelstring = SoXt::encodeString( "percentage" );
  this->percenttoggle = XtVaCreateManagedWidget( "percentagetoggle",
    xmToggleButtonWidgetClass, line2,
    XmNtopAttachment, XmATTACH_FORM,
    XmNleftAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNlabelString, labelstring,
    XmNindicatorType, XmONE_OF_MANY,
    XmNset, this->seekdistaspercentage ? True : False,
    NULL );
  XtFree( (char *) labelstring );
  XtAddCallback( this->percenttoggle, XmNvalueChangedCallback,
    SoXtFullViewer::percenttoggledCB, (XtPointer) this );

  labelstring = SoXt::encodeString( "absolute" );
  this->absolutetoggle = XtVaCreateManagedWidget( "absolutetoggle",
    xmToggleButtonWidgetClass, line2,
    XmNtopAttachment, XmATTACH_FORM,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, this->percenttoggle,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNindicatorType, XmONE_OF_MANY,
    XmNlabelString, labelstring,
    XmNset, this->seekdistaspercentage ? False : True,
    NULL );
  XtFree( (char *) labelstring );
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
  XmTextSetCursorPosition( this->zoomfrom, strlen(buf) );

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
  XmTextSetCursorPosition( this->zoomvalue, strlen(buf) );

  XtVaSetValues( this->zoomvalue,
    XmNsensitive, enable,
    XmNeditable, enable,
    XmNcursorPositionVisible, enable,
    NULL );

  XtAddCallback( this->zoomvalue, XmNactivateCallback,
    SoXtFullViewer::zoomvaluechangedCB, (XtPointer) this );
  XtAddCallback( this->zoomvalue, XmNlosingFocusCallback,
    SoXtFullViewer::zoomvaluechangedCB, (XtPointer) this );

  XmString labelstring = SoXt::encodeString( " Value: " );
  Widget valuelabel = XtVaCreateManagedWidget( "valuelabel",
    xmLabelWidgetClass, form,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_WIDGET,
    XmNrightWidget, zoomvalue,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNlabelString, labelstring,
    NULL );
  XtFree( (char *) labelstring );

  this->zoomto = XtVaCreateManagedWidget( "to",
    xmTextFieldWidgetClass, form,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_WIDGET,
    XmNrightWidget, valuelabel,
    XmNwidth, 50,
    NULL );

  sprintf( buf, "%.1f", this->zoomrange[1] );
  XmTextSetString( this->zoomto, buf );
  XmTextSetCursorPosition( this->zoomto, strlen(buf) );

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
  XmString labelstring;

  Widget form = XtVaCreateManagedWidget( "clippingprefs",
    xmFormWidgetClass, parent, NULL );

  labelstring = SoXt::encodeString( "auto" );
  this->autocliptoggle = XtVaCreateManagedWidget( "autocliptoggle",
    xmToggleButtonWidgetClass, form,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNset, this->isAutoClipping() ? True : False,
    XmNlabelString, labelstring,
    NULL );
  XtFree( (char *) labelstring );

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
  XmTextSetCursorPosition( this->farvalue, strlen(buf) );

  XtAddCallback( this->farvalue, XmNactivateCallback,
    SoXtFullViewer::farvaluechangedCB, (XtPointer) this );
  XtAddCallback( this->farvalue, XmNlosingFocusCallback,
    SoXtFullViewer::farvaluechangedCB, (XtPointer) this );

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

  labelstring = SoXt::encodeString( "Far plane:" );
  Widget farlabel = XtVaCreateManagedWidget( "farlabel",
    xmLabelWidgetClass, form,
    XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNleftWidget, this->farvalue,
    XmNrightAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNrightWidget, this->farvalue,
    XmNbottomAttachment, XmATTACH_WIDGET,
    XmNbottomWidget, this->farwheel,
    XmNbottomOffset, 2,
    XmNlabelString, labelstring,
    NULL );
  XtFree( (char *) labelstring );

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
  XmTextSetCursorPosition( this->nearvalue, strlen(buf) );

  XtAddCallback( this->nearvalue, XmNactivateCallback,
    SoXtFullViewer::nearvaluechangedCB, (XtPointer) this );
  XtAddCallback( this->nearvalue, XmNlosingFocusCallback,
    SoXtFullViewer::nearvaluechangedCB, (XtPointer) this );

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

  labelstring = SoXt::encodeString( "Near plane:" );
  Widget nearlabel = XtVaCreateManagedWidget( "nearlabel",
    xmLabelWidgetClass, form,
    XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNleftWidget, this->nearvalue,
    XmNrightAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNrightWidget, this->nearvalue,
    XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNbottomWidget, farlabel,
    XmNlabelString, labelstring,
    NULL );
  XtFree( (char *) labelstring );

  return form;
} // createClippingPrefSheetGuts()

// *************************************************************************

/*!
*/

Widget
SoXtFullViewer::createStereoPrefSheetGuts( // protected
  Widget parent )
{
  Widget form = XtVaCreateManagedWidget( "stereoguts",
    xmFormWidgetClass, parent, NULL );

  XmString labelstring = SoXt::encodeString( "stereo viewing" );
  this->stereotoggle = XtVaCreateManagedWidget( "stereotoggle",
    xmToggleButtonWidgetClass, form,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNset, False,
    XmNlabelString, labelstring,
    NULL );
  XtFree( (char *) labelstring );

  XtAddCallback( this->stereotoggle, XmNvalueChangedCallback,
    SoXtFullViewer::stereotoggledCB, (XtPointer) this );

  return form;
} // createStereoPrefSheetGuts()

// *************************************************************************

/*!
*/

Widget
SoXtFullViewer::createSpeedPrefSheetGuts( // protected
  Widget parent )
{
  Widget form = XtVaCreateManagedWidget( "speedform",
    xmFormWidgetClass, parent, NULL );

  XmString labelstring = SoXt::encodeString( "Viewer speed: " );
  Widget label = XtVaCreateManagedWidget( "speedlabel",
    xmLabelWidgetClass, form,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNlabelString, labelstring,
    NULL );
  XtFree( (char *) labelstring );

  labelstring = SoXt::encodeString( "increase" );
  this->incspeedbutton = XtVaCreateManagedWidget( "incbutton",
    xmPushButtonWidgetClass, form,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, label,
    XmNtopAttachment, XmATTACH_FORM,
    XmNlabelString, labelstring,
    NULL );
  XtFree( (char *) labelstring );

  XtAddCallback( this->incspeedbutton, XmNactivateCallback,
    SoXtFullViewer::speedIncCB, (XtPointer) this );

  labelstring = SoXt::encodeString( "decrease" );
  this->decspeedbutton = XtVaCreateManagedWidget( "decbutton",
    xmPushButtonWidgetClass, form,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, this->incspeedbutton,
    XmNtopAttachment, XmATTACH_FORM,
    XmNlabelString, labelstring,
    NULL );
  XtFree( (char *) labelstring );

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

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  prefSheetDestroy )
{
  XtUnrealizeWidget( this->prefshell );
  this->prefshell = NULL;
  this->prefsheet = NULL;
} // prefSheetDestroy()

/*!
  \internal
  This callback is hooked up to the "seconds" preferences sheet text field.
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  seekPrefSheetField )
{
  SOXT_STUB();
} // seekPrefSheetFieldCB()

/*!
  \internal

  This callback is hooked up to the seek-to-point-or-object toggling
  widgets.
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  seekPrefSheetToggle1 )
{
  SOXT_STUB();
} // seekPrefSheetToggle1CB()

/*!
  \internal
  This callback is hooked up to the seek-percentage-or-distance toggle
  widgets.
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  seekPrefSheetToggle2 )
{
  SOXT_STUB();
} // seekPrefSheetToggle2CB()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  autocliptoggled )
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
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  nearwheelvaluechanged )
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
  XmTextSetCursorPosition( this->nearvalue, strlen(valuestring) );

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
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  farwheelvaluechanged )
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
  XmTextSetCursorPosition( this->farvalue, strlen(valuestring) );

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
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  nearvaluechanged )
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
  XmTextSetCursorPosition( this->nearvalue, strlen(buf) );

  SoCamera * const camera = this->getCamera();
  if ( ! camera ) {
    SoDebugError::postInfo( "SoXtFullViewer::nearvaluechanged",
      "no camera!" );
    return;
  }
  camera->nearDistance = val;
} // nearvaluechanged()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  farvaluechanged )
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
  XmTextSetCursorPosition( this->farvalue, strlen(buf) );

  SoCamera * const camera = this->getCamera();
  if ( ! camera ) {
    SoDebugError::postInfo( "SoXtFullViewer::nearvaluechanged",
      "no camera!" );
    return;
  }
  camera->farDistance = val;
} // farvaluechanged()

// *************************************************************************
  
/*!
*/

void
SoXtFullViewer::selectedViewing(
  void )
{
  this->setViewing( this->isViewing() ? FALSE : TRUE );
} // selectedViewing()

/*!
*/

void
SoXtFullViewer::selectedDecoration(
  void )
{
  this->setDecoration( this->isDecoration() ? FALSE : TRUE );
} // selectedDecoration();

/*!
*/

void
SoXtFullViewer::selectedHeadlight(
  void )
{
  SOXT_STUB();
} // selectedHeadlight();

/*!
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
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  interactbutton )
{
  this->interactbuttonClicked( TRUE );
/*
  XmToggleButtonCallbackStruct * data =
    (XmToggleButtonCallbackStruct *) call_data;
  if ( ! data->set ) {
    
  } else {
    ((SoXtFullViewer *) client_data)->interactbuttonClicked( data->set );
  }
*/
} // interactbuttonCB()

/*!
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
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  examinebutton )
{
  this->examinebuttonClicked( TRUE );
} // examinebutton()

/*
void
SoXtFullViewer::examinebuttonCB(
  Widget,
  XtPointer client_data,
  XtPointer call_data )
{
  XmToggleButtonCallbackStruct * data =
    (XmToggleButtonCallbackStruct *) call_data;
  ((SoXtFullViewer *) client_data)->examinebuttonClicked( data->set );
} // examinebuttonCB()
*/

/*!
*/

void
SoXtFullViewer::helpbuttonClicked(
  void )
{
  this->openViewerHelpCard();
} // helpbuttonClicked()

/*!
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
*/

void
SoXtFullViewer::homebuttonClicked(
  void )
{
  this->resetToHomePosition();
} // homebuttonClicked()

/*!
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
*/

void
SoXtFullViewer::sethomebuttonClicked(
  void )
{
  this->saveHomePosition();
} // sethomebuttonClicked()

/*!
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
*/

void
SoXtFullViewer::viewallbuttonClicked(
  void )
{
  this->viewAll();
} // viewallbuttonClicked()

/*!
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
*/

void
SoXtFullViewer::copyviewSelected(
  void )
{
  this->copyView( SbTime::getTimeOfDay() );
} // copyviewSelected()

/*!
*/

void
SoXtFullViewer::pasteviewSelected(
  void )
{
  this->pasteView( SbTime::getTimeOfDay() );
} // pasteviewSelected()

/*!
*/

void
SoXtFullViewer::drawstyleActivated(
  int item )
{
  SOXT_STUB();
} // drawstyleActivated()

// *************************************************************************

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  increaseInteractiveCount )
{
  this->interactiveCountInc();
} // increaseInteractiveCount()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  decreaseInteractiveCount )
{
  this->interactiveCountDec();
} // increaseInteractiveCount()

// *************************************************************************

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  seektimechanged )
{
  float newtime = (float) atof( XmTextGetString( this->seektimefield ) );
  this->setSeekTime( newtime );
  char buf[16];
  sprintf( buf, "%g", newtime );
  XmTextSetString( this->seektimefield, buf );
  XmTextSetCursorPosition( this->seektimefield, strlen(buf) );
} // seektimechanged()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  pointtoggled )
{
  XtVaSetValues( this->pointtoggle, XmNset, True, NULL );
  XtVaSetValues( this->objecttoggle, XmNset, False, NULL );
  if ( ! this->isDetailSeek() )
    this->setDetailSeek( TRUE );
} // pointtoggled()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  objecttoggled )
{
  XtVaSetValues( this->pointtoggle, XmNset, False, NULL );
  XtVaSetValues( this->objecttoggle, XmNset, True, NULL );
  if ( this->isDetailSeek() )
    this->setDetailSeek( FALSE );
} // objecttoggled()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  seekdistancechanged )
{
  float newdist = (float) atof( XmTextGetString( this->seekdistancefield ) );
  if ( newdist < 0.0f )
    newdist = 0.0f;
  this->seekdistance = newdist;
  char buf[16];
  sprintf( buf, "%g", newdist );
  XmTextSetString( this->seekdistancefield, buf );
  XmTextSetCursorPosition( this->seekdistancefield, strlen(buf) );
} // seekdistancechanged()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  percenttoggled )
{
  XtVaSetValues( this->percenttoggle, XmNset, True, NULL );
  XtVaSetValues( this->absolutetoggle, XmNset, False, NULL );
  this->seekdistaspercentage = TRUE;
} // percenttoggled()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  absolutetoggled )
{
  XtVaSetValues( this->percenttoggle, XmNset, False, NULL );
  XtVaSetValues( this->absolutetoggle, XmNset, True, NULL );
  this->seekdistaspercentage = FALSE;
} // absolutetoggled()

// *************************************************************************

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  stereotoggled )
{
  Boolean enable = False;
  XtVaGetValues( this->stereotoggle, XmNset, &enable, NULL );
  this->setStereoViewing( enable ? TRUE : FALSE );
  XtVaSetValues( this->stereotoggle,
    XmNset, this->isStereoViewing() ? True : False,
    NULL );
} // stereotoggled()

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
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  zoomfromchanged )
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
  XmTextSetCursorPosition( this->zoomfrom, strlen(buf) );

  float zoom = this->getCameraZoom();
  if ( zoom < this->zoomrange[0] ) {
    zoom = this->zoomrange[0];
    this->setCameraZoom( zoom );
    sprintf( buf, "%.1f", zoom );
    XmTextSetString( this->zoomvalue, buf );
    XmTextSetCursorPosition( this->zoomvalue, strlen(buf) );
  } else if ( zoom > this->zoomrange[1] ) {
    zoom = this->zoomrange[1];
    this->setCameraZoom( zoom );
    sprintf( buf, "%.1f", zoom );
    XmTextSetString( this->zoomvalue, buf );
    XmTextSetCursorPosition( this->zoomvalue, strlen(buf) );
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
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  zoomsliderchanged )
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
    XmTextSetCursorPosition( this->zoomvalue, strlen(buf) );
    this->setCameraZoom( realval );
  }
} // zoomsliderchanged()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  zoomtochanged )
{
  float value = atof( XmTextGetString( this->zoomto ) );
  if ( value > 180.0f )
    value = 180.0f;
  else if ( value < this->zoomrange[0] )
    value = this->zoomrange[0];

  char buf[16];
  sprintf( buf, "%.1f", value );
  XmTextSetString( this->zoomto, buf );
  XmTextSetCursorPosition( this->zoomto, strlen(buf) );

  this->zoomrange[1] = value;
  float zoom = this->getCameraZoom();
  if ( zoom < this->zoomrange[0] ) {
    zoom = this->zoomrange[0];
    this->setCameraZoom( zoom );
    sprintf( buf, "%.1f", zoom );
    XmTextSetString( this->zoomvalue, buf );
    XmTextSetCursorPosition( this->zoomvalue, strlen(buf) );
    this->setCameraZoom( value );
  } else if ( zoom > this->zoomrange[1] ) {
    zoom = this->zoomrange[1];
    this->setCameraZoom( zoom );
    sprintf( buf, "%.1f", zoom );
    XmTextSetString( this->zoomvalue, buf );
    XmTextSetCursorPosition( this->zoomvalue, strlen(buf) );
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
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  zoomvaluechanged )
{
  float value = atof( XmTextGetString( this->zoomvalue ) );
  if ( value < this->zoomrange[0] )
    value = this->zoomrange[0];
  else if ( value > this->zoomrange[1] )
    value = this->zoomrange[1];

  char buf[16];
  sprintf( buf, "%.1f", value );
  XmTextSetString( this->zoomvalue, buf );
  XmTextSetCursorPosition( this->zoomvalue, strlen(buf) );

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

// *************************************************************************

/*!
*/

Widget
SoXtFullViewer::getThumbWheel(
  int num )
{
  assert( num >= FIRSTDECORATION && num <= LASTDECORATION );
  return this->wheels[num];
} // getThumbWheel()

// *************************************************************************

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  speedInc )
{
  SOXT_STUB_ONCE();
} // speedInc()

/*!
*/

SOXT_WIDGET_CALLBACK_IMPLEMENTATION(
  SoXtFullViewer,
  speedDec )
{
  SOXT_STUB_ONCE();
} // speedDec()

// *************************************************************************
