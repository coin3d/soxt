/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
