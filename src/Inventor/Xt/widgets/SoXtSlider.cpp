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

#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Scale.h>
#include <Xm/Text.h>
#include <Xm/PushB.h>

#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>
#include <Inventor/Xt/sliders/SoXtSliderSetModule.h>

#include <Inventor/Xt/widgets/SoXtSlider.h>

/*!
  \class SoXtSlider Inventor/Xt/widgets/compound/SoXtSlider.h
  \brief The SoXtSlider class manages a compound widget containing a
  generic slider.

  This class is internal to SoXt and may change interface without notice.
*/

// *************************************************************************

/*!
*/

SoXtSlider::SoXtSlider(
  const Widget parent,
  SoXtSliderSetModule * const _sliderset )
: sliderset( _sliderset )
{
  this->layoutStyle = DEFAULT_LAYOUT_STYLE;

  this->form = NULL;
  this->value = NULL;
  this->slider = NULL;
  this->min = NULL;
  this->minValue = NULL;
  this->max = NULL;
  this->maxValue = NULL;

  this->current = 0.0f;
  this->minimum = 0.0f;
  this->maximum = 1.0f;

  this->buildWidget( parent );

  switch ( this->layoutStyle ) {
  case SIMPLE:   this->layoutSimple(TRUE);   break;
  case RANGED:   this->layoutRanged(TRUE);   break;
  case FULL:     this->layoutFull(TRUE);     break;
  case ORDERLY:  this->layoutOrderly(TRUE);  break;
  default:       assert( 0 );            break;
  } // switch ( this->layoutStyle )
} // SoXtSlider()

/*!
*/

SoXtSlider::~SoXtSlider(
  void )
{
} // ~SoXtSlider()

// *************************************************************************

/*!
*/

Widget
SoXtSlider::getBaseWidget(
  void ) const
{
  return this->form;
} // getBaseWidget()

/*!
*/

void
SoXtSlider::changeLayoutStyle(
  void )
{
  if ( this->layoutStyle == LAST_LAYOUT_STYLE )
    this->layoutStyle = FIRST_LAYOUT_STYLE;
  else
    this->layoutStyle = (SoXtSliderLayoutStyle) (((int) this->layoutStyle) + 1);

  switch ( this->layoutStyle ) {
  case SIMPLE:   this->layoutSimple();   break;
  case RANGED:   this->layoutRanged();   break;
  case FULL:     this->layoutFull();     break;
  case ORDERLY:  this->layoutOrderly();  break;
  default:       assert( 0 && "impossible" );
  } // switch ( this->layoutStyle )
} // changeLayoutStyle()

// *************************************************************************

/*!
*/

void
SoXtSlider::layoutSimple(
  const SbBool initial ) const
{
#if SOXT_DEBUG
  SoDebugError::postInfo( "SoXtSlider::layoutSimple", "invoked" );
#endif // SOXT_DEBUG

  XtVaSetValues( this->value,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_NONE,
    XmNbottomAttachment, XmATTACH_NONE,
    XmNwidth, 60,
    NULL );

  XtVaSetValues( this->slider,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, this->value,
    XmNleftOffset, 2,
    XmNtopAttachment, XmATTACH_FORM,
    XmNtopOffset, 2,
    XmNrightAttachment, XmATTACH_FORM,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNbottomWidget, this->value,
    XmNbottomOffset, 2,
    XtVaTypedArg,
      XmNtitleString, XmRString, "", 0,
    NULL );

  if ( XtIsRealized( this->min ) ) {
    XtUnmapWidget( this->min );
    XtUnrealizeWidget( this->min );
  }
  if ( XtIsRealized( this->minValue ) ) {
    XtUnmapWidget( this->minValue );
    XtUnrealizeWidget( this->minValue );
  }
  if ( XtIsRealized( this->max ) ) {
    XtUnmapWidget( this->max );
    XtUnrealizeWidget( this->max );
  }
  if ( XtIsRealized( this->maxValue ) ) {
    XtUnmapWidget( this->maxValue );
    XtUnrealizeWidget( this->maxValue );
  }
  if ( ! initial ) {
    if ( ! XtIsRealized( this->value ) ) {
      XtRealizeWidget( this->value );
      XtMapWidget( this->value );
      XtManageChild( this->value );
    }
    if ( ! XtIsRealized( this->slider ) ) {
      XtRealizeWidget( this->slider );
      XtMapWidget( this->slider );
      XtManageChild( this->slider );
    }
  }
  XtManageChild( this->form );
} // layoutSimple()

// *************************************************************************

/*!
*/

void
SoXtSlider::layoutRanged(
  const SbBool initial ) const
{
#if SOXT_DEBUG
  SoDebugError::postInfo( "SoXtSlider::layoutRanged", "invoked" );
#endif // SOXT_DEBUG

  XtVaSetValues( this->value,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_NONE,
    XmNbottomAttachment, XmATTACH_NONE,
    XmNwidth, 60,
    NULL );

  XtVaSetValues( this->minValue,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, this->value,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_NONE,
    XmNbottomAttachment, XmATTACH_NONE,
    XmNwidth, 50,
    NULL );

  XtVaSetValues( this->maxValue,
    XmNleftAttachment, XmATTACH_NONE,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_NONE,
    XmNwidth, 50,
    NULL );

  XtVaSetValues( this->slider,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, this->minValue,
    XmNleftOffset, 2,
    XmNtopAttachment, XmATTACH_FORM,
    XmNtopOffset, 2,
    XmNrightAttachment, XmATTACH_WIDGET,
    XmNrightWidget, this->maxValue,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNbottomWidget, this->minValue,
    XmNbottomOffset, 2,
    XtVaTypedArg,
      XmNtitleString, XmRString, "", 0,
    NULL );

  if ( ! initial ) {
    if ( ! XtIsRealized( this->value ) ) {
      XtRealizeWidget( this->value );
      XtMapWidget( this->value );
      XtManageChild( this->value );
    }
    if ( ! XtIsRealized( this->minValue ) ) {
      XtRealizeWidget( this->minValue );
      XtMapWidget( this->minValue );
      XtManageChild( this->minValue );
    }
    if ( ! XtIsRealized( this->slider ) ) {
      XtRealizeWidget( this->slider );
      XtMapWidget( this->slider );
      XtManageChild( this->slider );
    }
    if ( ! XtIsRealized( this->maxValue ) ) {
      XtRealizeWidget( this->maxValue );
      XtMapWidget( this->maxValue );
      XtManageChild( this->maxValue );
    }
  }
  if ( XtIsRealized( this->min ) ) {
    XtUnmapWidget( this->min );
    XtUnrealizeWidget( this->min );
  }
  if ( XtIsRealized( this->max ) ) {
    XtUnmapWidget( this->max );
    XtUnrealizeWidget( this->max );
  }
//  XtManageChild( this->form );
} // layoutRanged()

// *************************************************************************

/*!
*/

void
SoXtSlider::layoutFull(
  const SbBool initial ) const
{
#if SOXT_DEBUG
  SoDebugError::postInfo( "SoXtSlider::layoutFull", "invoked" );
#endif // SOXT_DEBUG

  if ( ! initial ) {
    if ( ! XtIsRealized( this->minValue ) ) {
      XtRealizeWidget( this->minValue );
      XtMapWidget( this->minValue );
      XtManageChild( this->minValue );
    }
    if ( ! XtIsRealized( this->value ) ) {
      XtRealizeWidget( this->value );
      XtMapWidget( this->value );
      XtManageChild( this->value );
    }
    if ( ! XtIsRealized( this->maxValue ) ) {
      XtRealizeWidget( this->maxValue );
      XtMapWidget( this->maxValue );
      XtManageChild( this->maxValue );
    }
    if ( ! XtIsRealized( this->min ) ) {
      XtRealizeWidget( this->min );
      XtMapWidget( this->min );
      XtManageChild( this->min );
    }
    if ( ! XtIsRealized( this->slider ) ) {
      XtRealizeWidget( this->slider );
      XtMapWidget( this->slider );
      XtManageChild( this->slider );
    }
    if ( ! XtIsRealized( this->max ) ) {
      XtRealizeWidget( this->max );
      XtMapWidget( this->max );
      XtManageChild( this->max );
    }
  }

  XtVaSetValues( this->minValue,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_NONE,
    XmNbottomAttachment, XmATTACH_NONE,
    XmNwidth, 50,
    NULL );

  XtVaSetValues( this->maxValue,
    XmNleftAttachment, XmATTACH_NONE,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_NONE,
    XmNwidth, 50,
    NULL );

  XtVaSetValues( this->value,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, this->minValue,
    XmNleftOffset, 2,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_WIDGET,
    XmNrightWidget, this->maxValue,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_NONE,
    NULL );

  XtVaSetValues( this->min,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, this->minValue,
    XmNtopOffset, 2,
    XmNrightAttachment, XmATTACH_NONE,
    XmNbottomAttachment, XmATTACH_NONE,
    XmNwidth, 50,
    NULL );

  XtVaSetValues( this->max,
    XmNleftAttachment, XmATTACH_NONE,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, this->maxValue,
    XmNtopOffset, 2,
    XmNrightAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_NONE,
    XmNwidth, 50,
    NULL );

  XtVaSetValues( this->slider,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, this->min,
    XmNleftOffset, 2,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, this->value,
    XmNtopOffset, 2,
    XmNrightAttachment, XmATTACH_WIDGET,
    XmNrightWidget, this->max,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_NONE,
    XtVaTypedArg,
      XmNtitleString, XmRString, "Slider", 7,
    NULL );

  XtVaSetValues( this->min,
    XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNbottomWidget, this->slider,
    NULL );

  XtVaSetValues( this->max,
    XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNbottomWidget, this->slider,
    NULL );

  XtManageChild( this->form );
} // layoutFull()

// *************************************************************************

/*!
*/

void
SoXtSlider::layoutOrderly(
  const SbBool initial ) const
{
#if SOXT_DEBUG
  SoDebugError::postInfo( "SoXtSlider::layoutOrderly", "invoked" );
#endif // SOXT_DEBUG

  XtVaSetValues( this->value,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_NONE,
    XmNbottomOffset, 0,
    NULL );

  XtVaSetValues( this->slider,
    XmNleftAttachment, XmATTACH_FORM,
    XmNleftOffset, 2,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, this->value,
    XmNtopOffset, 2,
    XmNrightAttachment, XmATTACH_FORM,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_NONE,
    XmNbottomOffset, 2,
    XtVaTypedArg,
      XmNtitleString, XmRString, "Slider", 7,
    NULL );

  if ( XtIsRealized( this->min ) ) {
    XtUnmapWidget( this->min );
    XtUnrealizeWidget( this->min );
  }
  if ( XtIsRealized( this->minValue ) ) {
    XtUnmapWidget( this->minValue );
    XtUnrealizeWidget( this->minValue );
  }
  if ( XtIsRealized( this->max ) ) {
    XtUnmapWidget( this->max );
    XtUnrealizeWidget( this->max );
  }
  if ( XtIsRealized( this->maxValue ) ) {
    XtUnmapWidget( this->maxValue );
    XtUnrealizeWidget( this->maxValue );
  }
  if ( ! initial ) {
    if ( ! XtIsRealized( this->value ) ) {
      XtRealizeWidget( this->value );
      XtMapWidget( this->value );
      XtManageChild( this->value );
    }
    if ( ! XtIsRealized( this->slider ) ) {
      XtRealizeWidget( this->slider );
      XtMapWidget( this->slider );
      XtManageChild( this->slider );
    }
  }
//  XtManageChild( this->form );
} // layoutOrderly()

// *************************************************************************

/*!
*/

Widget
SoXtSlider::buildWidget( // protected
  Widget parent )
{
  assert( this->form == (Widget) NULL );

  this->form = XtVaCreateManagedWidget( "form",
    xmFormWidgetClass, parent,
//    XmNleftAttachment, XmATTACH_FORM,
//    XmNtopAttachment, XmATTACH_FORM,
//    XmNrightAttachment, XmATTACH_FORM,
//    XmNbottomAttachment, XmATTACH_FORM,
    NULL );

  this->slider = XtVaCreateManagedWidget( "slider",
    xmScaleWidgetClass, this->form,
    XmNorientation, XmHORIZONTAL,
    XmNtraversalOn, False,
    XmNminimum, 0,
    XmNvalue, 0,
    XmNmaximum, 999,
    XmNshowValue, False,
    XmNhighlightThickness, 0,
    XtVaTypedArg,
      XmNtitleString, XmRString, "", 0,
    NULL );

  XtAddCallback( this->slider,
    XmNdragCallback, SoXtSlider::widgetEventCB, this );
  XtAddCallback( this->slider,
    XmNvalueChangedCallback, SoXtSlider::widgetEventCB, this );

  this->value = XtVaCreateManagedWidget( "value",
    xmTextWidgetClass, this->form,
    XmNhighlightThickness, 0,
    NULL );

  char text[40];

  sprintf( text, "%g", this->current );
  XmTextSetString( this->value, text );
  XmTextSetCursorPosition( this->value, strlen(text) );

  XtAddCallback( this->value,
    XmNvalueChangedCallback, SoXtSlider::widgetEventCB, this );
  XtAddCallback( this->value,
    XmNlosingFocusCallback, SoXtSlider::widgetEventCB, this );

  this->min = XtVaCreateManagedWidget( "min",
    xmPushButtonWidgetClass, this->form,
    XmNtraversalOn, False,
    NULL );

  XtAddCallback( this->min,
    XmNactivateCallback, SoXtSlider::widgetEventCB, this );

  this->minValue = XtVaCreateManagedWidget( "minValue",
    xmTextWidgetClass, this->form,
    XmNhighlightThickness, 0,
    NULL );

  sprintf( text, "%g", this->minimum );
  XmTextSetString( this->minValue, text );
  XmTextSetCursorPosition( this->minValue, strlen(text) );

  XtAddCallback( this->minValue,
    XmNvalueChangedCallback, SoXtSlider::widgetEventCB, this );
  XtAddCallback( this->minValue,
    XmNlosingFocusCallback, SoXtSlider::widgetEventCB, this );

  this->max = XtVaCreateManagedWidget( "max",
    xmPushButtonWidgetClass, this->form,
    XmNtraversalOn, False,
    NULL );

  XtAddCallback( this->max,
    XmNactivateCallback, SoXtSlider::widgetEventCB, this );

  this->maxValue = XtVaCreateManagedWidget( "maxValue",
    xmTextWidgetClass, this->form,
    XmNhighlightThickness, 0,
    NULL );

  sprintf( text, "%g", this->maximum );
  XmTextSetString( this->maxValue, text );
  XmTextSetCursorPosition( this->maxValue, strlen(text) );

  XtAddCallback( this->maxValue,
    XmNvalueChangedCallback, SoXtSlider::widgetEventCB, this );
  XtAddCallback( this->maxValue,
    XmNlosingFocusCallback, SoXtSlider::widgetEventCB, this );

/*
  if ( XtIsRealized( this->maxValue ) )
    XtUnmapWidget( this->maxValue );
  if ( XtIsRealized( this->max ) )
    XtUnmapWidget( this->max );
  if ( XtIsRealized( this->minValue ) )
    XtUnmapWidget( this->minValue );
  if ( XtIsRealized( this->slider ) )
    XtUnmapWidget( this->slider );
  if ( XtIsRealized( this->value ) )
    XtUnmapWidget( this->value );
  if ( XtIsRealized( this->min ) )
    XtUnmapWidget( this->min );
*/

  return this->form;
} // buildWidget()

// *************************************************************************

void
SoXtSlider::widgetEventCB(
  Widget widget,
  XtPointer client_data,
  XtPointer call_data )
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtSlider::widgetEventCB", "invoked" );
#endif // SOXT_DEBUG
  SoXtSlider * const slider = (SoXtSlider *) client_data;

  if ( widget == slider->slider ) {
    // slider dragged
    int pos = 0;
    XmScaleGetValue( slider->slider, &pos );
    slider->current = slider->minimum +
      ((float(pos) / 999.0f) * (slider->maximum - slider->minimum));
    char text[16];
    sprintf( text, "%.3g", slider->current );
    XmTextSetString( slider->value, text );
    XmTextSetCursorPosition( slider->value, strlen(text) );
  } else if ( widget == slider->value ) {
    // value entered
  } else if ( widget == slider->min ) {
    // minimum button pressed
  } else if ( widget == slider->minValue ) {
    // minimum value entered
  } else if ( widget == slider->max ) {
    // maximum button pressed
  } else if ( widget == slider->maxValue ) {
    // maximum value entered
  } else {
    assert( 0 && "impossible" );
  }
} // widgetEventCB()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtSliderRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

