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

#include <Inventor/Xt/widgets/compound/SoXtSliderManager.h>

#include <Inventor/Xt/widgets/compound/SoXtSlider.h>

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
  SoXtSliderManager * const _manager )
: manager( _manager )
{
  this->layoutStyle = SIMPLE;
  this->form = NULL;
  this->value = NULL;
  this->slider = NULL;
  this->min = NULL;
  this->minValue = NULL;
  this->max = NULL;
  this->maxValue = NULL;

  this->buildWidget( parent );
  this->layoutSimple();
} // SoXtSlider

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
  void ) const
{
} // layoutSimple()

/*!
*/

void
SoXtSlider::layoutRanged(
  void ) const
{
} // layoutRanged()

/*!
*/

void
SoXtSlider::layoutFull(
  void ) const
{
} // layoutFull()

/*!
*/

void
SoXtSlider::layoutOrderly(
  void ) const
{
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
    NULL );

  this->slider = XtVaCreateWidget( "slider",
    xmScaleWidgetClass, this->form,
    NULL );

  this->value = XtVaCreateWidget( "value",
    xmTextWidgetClass, this->form,
    NULL );

  this->min = XtVaCreateWidget( "min",
    xmPushButtonWidgetClass, this->form,
    NULL );

  this->max = XtVaCreateWidget( "max",
    xmPushButtonWidgetClass, this->form,
    NULL );

  this->minValue = XtVaCreateWidget( "minValue",
    xmTextWidgetClass, this->form,
    NULL );

  this->maxValue = XtVaCreateWidget( "maxValue",
    xmTextWidgetClass, this->form,
    NULL );

  return this->form;
} // buildWidget()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtSliderRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG
