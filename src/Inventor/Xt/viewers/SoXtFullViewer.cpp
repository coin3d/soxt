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

#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoBasic.h>

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

// *************************************************************************

/*!
  Constructor.
*/

SoXtFullViewer::SoXtFullViewer( // protected
  Widget parent,
  const char * name,
  SbBool inParent,
  BuildFlag flag,
  Type type,
  SbBool build )
: inherited( parent, name, inParent, type, FALSE )
{
  this->setClassName( "SoXtFullViewer" );
  this->setSize( SbVec2s( 500, 390 ) );
  this->viewerButtons = new SbPList;
  this->appButtons = new SbPList;
  if ( build != FALSE )
    this->buildWidget( parent );
} // SoXtFullViewer()

/*!
  The destructor.
*/

SoXtFullViewer::~SoXtFullViewer( // protected
  void )
{
  delete this->viewerButtons;
  delete this->appButtons;
} // ~SoXtFullViewer()

// *************************************************************************

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
      XmNleftAttachment, XmATTACH_WIDGET,
      XmNleftWidget, this->decorform[LEFTDECORATION],
      XmNbottomAttachment, XmATTACH_WIDGET,
      XmNbottomWidget, this->decorform[BOTTOMDECORATION],
      XmNrightAttachment, XmATTACH_WIDGET,
      XmNrightWidget, this->decorform[RIGHTDECORATION],
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
      xmFormWidgetClass, parent, XmNshadowType, XmSHADOW_OUT, NULL );
  XtVaSetValues( trim,
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
      xmFormWidgetClass, parent, NULL );
  XtVaSetValues( trim,
      XmNwidth, 30,
      XmNleftAttachment, XmATTACH_FORM,
      XmNtopAttachment, XmATTACH_FORM,
      XmNbottomAttachment, XmATTACH_WIDGET,
      XmNbottomWidget, this->decorform[BOTTOMDECORATION],
      NULL );
  return trim;
} // buildLeftTrim()


/*!
*/

Widget
SoXtFullViewer::buildRightTrim( // virtual
  Widget parent )
{
  Widget trim = XtVaCreateManagedWidget( "RightTrim",
      xmFormWidgetClass, parent, NULL );

  XtVaSetValues( trim,
      XmNwidth, 30,
      XmNtopAttachment, XmATTACH_FORM,
      XmNrightAttachment, XmATTACH_FORM,
      XmNbottomAttachment, XmATTACH_WIDGET,
      XmNbottomWidget, this->decorform[BOTTOMDECORATION],
      NULL );

  Widget buttonForm = this->buildViewerButtons( trim );
  XtManageChild( buttonForm );

  // add right thumb wheel

  return trim;
} // buildRightTrim()

// *************************************************************************

void
SoXtFullViewer::setViewing( // virtual
  SbBool enable )
{
  inherited::setViewing( enable );
  COIN_STUB();
}

void
SoXtFullViewer::setHeadlight( // virtual
  SbBool enable )
{
  inherited::setHeadlight( enable );
#if 0
  if ( this->prefmenu )
    this->prefmenu->setItemChecked( HEADLIGHT_ITEM, enable );
#endif // 0
} // setHeadlight()

void
SoXtFullViewer::setDrawStyle( // virtual
  SoXtViewer::DrawType type,
  SoXtViewer::DrawStyle style )
{
  COIN_STUB();
}

void
SoXtFullViewer::setBufferingType( // virtual
  SoXtViewer::BufferType type )
{
  COIN_STUB();
}

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

void
SoXtFullViewer::hide( // virtual
  void )
{
  COIN_STUB();
}

// *************************************************************************

SbBool
SoXtFullViewer::eventFilter( // virtual
  Widget obj,
  XAnyEvent * event )
{
  COIN_STUB();
  return FALSE;
} // eventFilter()

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

  this->createViewerButtons( form, this->viewerButtons );

  for ( int i = 0; i < this->viewerButtons->getLength(); i++ ) {
    Widget button = (Widget) (*this->viewerButtons)[i];
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
         XmNtopWidget, (Widget) (*this->viewerButtons)[i-1],
         XmNrightAttachment, XmATTACH_FORM,
         XmNwidth, 30, XmNheight, 30,
         NULL );
  }

  return form;
} // buildViewerButtons()

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
}

void
SoXtFullViewer::buildPopupMenu(
  void )
{
  COIN_STUB();
}

Widget
SoXtFullViewer::makeSubPreferences(
  Widget parent )
{
  COIN_STUB();
  return (Widget) NULL;
}

void
SoXtFullViewer::leftWheelStart(
  void )
{
  COIN_STUB();
}

void
SoXtFullViewer::leftWheelMotion(
  float )
{
  COIN_STUB();
}

void
SoXtFullViewer::leftWheelFinish(void)
{
  COIN_STUB();
}

void
SoXtFullViewer::bottomWheelStart(void)
{
  COIN_STUB();
}

void
SoXtFullViewer::bottomWheelMotion(float)
{
  COIN_STUB();
}

void
SoXtFullViewer::bottomWheelFinish(void)
{
  COIN_STUB();
}

void
SoXtFullViewer::rightWheelStart(void)
{
  COIN_STUB();
}

void
SoXtFullViewer::rightWheelMotion(float)
{
  COIN_STUB();
}

void
SoXtFullViewer::rightWheelFinish(void)
{
  COIN_STUB();
}

void
SoXtFullViewer::openViewerHelpCard(void)
{
  COIN_STUB();
}

