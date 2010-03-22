/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

// $Id$

#ifndef SOXT_SLIDER_H
#define SOXT_SLIDER_H

#include <X11/Intrinsic.h>

#include <Inventor/SbBasic.h>

#include <Inventor/Xt/SoXtBasic.h>

class SbPList;
class SoXtSliderSetModule;

// *************************************************************************

typedef void SoXtSliderCB(void * closure, char * title, float value);

class SoXtSlider {
public:
  SoXtSlider(const char * const title);
  ~SoXtSlider(void);

  void setValue(float val);
  float getValue(void) const;

  void setRange(float min, float max);
  void getRange(float & min, float & max) const;

  void addCallback(SoXtSliderCB * callback, void * closure = NULL);
  void removeCallback(SoXtSliderCB * callback, void * closure = NULL);

  Widget buildSimpleWidget(const Widget parent);
  Widget buildRangedWidget(const Widget parent);
  Widget buildFullWidget(const Widget parent);
  Widget buildOrderlyWidget(const Widget parent);

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
  static void slider_cb(Widget, XtPointer, XtPointer);
  static void value_cb(Widget, XtPointer, XtPointer);
  static void min_value_cb(Widget, XtPointer, XtPointer);
  static void min_cb(Widget, XtPointer, XtPointer);
  static void max_value_cb(Widget, XtPointer, XtPointer);
  static void max_cb(Widget, XtPointer, XtPointer);

}; // class SoXtSlider

// *************************************************************************

#endif // ! SOXT_SLIDER_H
