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

//  $Id$

#ifndef SOXT_SLIDER_H
#define SOXT_SLIDER_H

#include <X11/Intrinsic.h>

#include <Inventor/SbBasic.h>

class SoXtSliderSetModule;

// *************************************************************************

class SoXtSlider {
public:
  SoXtSlider( const Widget parent, SoXtSliderSetModule * const sliderset );
  ~SoXtSlider(void);

  Widget getBaseWidget(void) const;

  void changeLayoutStyle(void);

protected:
  void layoutSimple( const SbBool initial = FALSE ) const;
  void layoutRanged( const SbBool initial = FALSE ) const;
  void layoutFull( const SbBool initial = FALSE ) const;
  void layoutOrderly( const SbBool initial = FALSE ) const;

  Widget buildWidget( Widget parent );

private:
  enum SoXtSliderLayoutStyle {
    SIMPLE, RANGED, FULL, ORDERLY,
    FIRST_LAYOUT_STYLE = SIMPLE,
    DEFAULT_LAYOUT_STYLE = FULL,
    LAST_LAYOUT_STYLE = ORDERLY
  } layoutStyle;

  float current, minimum, maximum;

  Widget form;         // XmForm
  Widget value;        // XmText
  Widget slider;       // XmScale
  Widget min;          // XmPushButton
  Widget minValue;     // XmText
  Widget max;          // XmPushButton
  Widget maxValue;     // XmText

  SoXtSliderSetModule * const sliderset;

  static void widgetEventCB( Widget, XtPointer, XtPointer );

}; // class SoXtSlider

// *************************************************************************

#endif // ! SOXT_SLIDER_H
