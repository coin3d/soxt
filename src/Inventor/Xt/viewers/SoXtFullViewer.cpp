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

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoBasic.h>

#include <Inventor/Xt/viewers/SoXtFullViewer.h>

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
  if ( build != FALSE )
    this->buildWidget( parent );
} // SoXtFullViewer()

/*!
  The destructor.
*/

SoXtFullViewer::~SoXtFullViewer( // protected
  void )
{
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
  XtManageChild( this->canvas );
  SoDebugError::postInfo( "buildWidget", "canvas = %p", (void *) canvas );

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

  this->setBaseWidget( this->viewerbase );
  return this->viewerbase;
} // buildWidget()

// *************************************************************************

void
SoXtFullViewer::buildDecoration( // virtual
  Widget parent )
{
  this->decorform[BOTTOMDECORATION] = this->buildBottomTrim( parent );
  this->decorform[LEFTDECORATION]   = this->buildLeftTrim( parent );
  this->decorform[RIGHTDECORATION]  = this->buildRightTrim( parent );
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
  return trim;
} // buildRightTrim()

// *************************************************************************

void
SoXtFullViewer::setViewing( // virtual
  SbBool enable )
{
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

bool
SoXtFullViewer::eventFilter( // virtual
  Widget obj,
  XAnyEvent * event )
{
  COIN_STUB();
} // eventFilter()

void
SoXtFullViewer::createViewerButtons(
  Widget parent,
  SbPList buttonlist )
{
  COIN_STUB();
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

