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

#include <Inventor/Xt/SoXtBasic.h>

class SbPList;
class SoXtSliderSetModule;

// *************************************************************************

typedef void SoXtSliderCB( void * closure, char * title, float value );

class SoXtSlider {
public:
  SoXtSlider( const char * const title );
  ~SoXtSlider(void);

  void setValue( float val );
  float getValue(void) const;

  void setRange( float min, float max );
  void getRange( float & min, float & max ) const;

  void addCallback( SoXtSliderCB * callback, void * closure = NULL );
  void removeCallback( SoXtSliderCB * callback, void * closure = NULL );

  Widget buildSimpleWidget( const Widget parent );
  Widget buildRangedWidget( const Widget parent );
  Widget buildFullWidget( const Widget parent );
  Widget buildOrderlyWidget( const Widget parent );

protected:
  void invokeCallbacks(void);

private:
  float current, minimum, maximum;
  char * title;

  // simple
  Widget s_form, s_value, s_slider;
  // ranged
  Widget r_form, r_value, r_slider, r_minValue, r_maxValue;
  // full
  Widget f_form, f_value, f_slider, f_minValue, f_min, f_maxValue, f_max,
         f_label;
  // orderly
  Widget o_form, o_value, o_slider, o_label;

  SbPList * callbacks;

private:
  static void slider_cb( Widget, XtPointer, XtPointer );
  static void value_cb( Widget, XtPointer, XtPointer );
  static void min_value_cb( Widget, XtPointer, XtPointer );
  static void min_cb( Widget, XtPointer, XtPointer );
  static void max_value_cb( Widget, XtPointer, XtPointer );
  static void max_cb( Widget, XtPointer, XtPointer );

}; // class SoXtSlider

// *************************************************************************

#endif // ! SOXT_SLIDER_H
