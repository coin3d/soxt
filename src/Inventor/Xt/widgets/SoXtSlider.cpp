/**************************************************************************\
 *
 *  This file is part of the Coin family of 3D visualization libraries.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and / or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use this library in software that is incompatible
 *  with the GNU GPL, and / or you would like to take advantage of the
 *  additional benefits with regard to our support services, please
 *  contact Systems in Motion about acquiring a Coin Professional
 *  Edition License.  See <URL:http://www.coin3d.org> for more
 *  information.
 *
 *  Systems in Motion, Abels gate 5, Teknobyen, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#include <assert.h>
#include <stdlib.h>

#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/Scale.h>
#include <Xm/Text.h>
#include <Xm/PushB.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SbPList.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtBasic.h>

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
  const char * const title)
{
  this->s_form = NULL;
  this->s_value = NULL;
  this->s_slider = NULL;

  this->r_form = NULL;
  this->r_value = NULL;
  this->r_slider = NULL;
  this->r_minValue = NULL;
  this->r_maxValue = NULL;

  this->f_form = NULL;
  this->f_value = NULL;
  this->f_slider = NULL;
  this->f_min = NULL;
  this->f_minValue = NULL;
  this->f_max = NULL;
  this->f_maxValue = NULL;
  this->f_label = NULL;

  this->o_form = NULL;
  this->o_value = NULL;
  this->o_slider = NULL;
  this->o_label = NULL;

  if (title)
    this->title = strcpy(new char [strlen(title)+1], title);
  else
    this->title = NULL;
  this->callbacks = NULL;

  this->current = 0.0f;
  this->minimum = 0.0f;
  this->maximum = 1.0f;
} // SoXtSlider()

/*!
*/

SoXtSlider::~SoXtSlider(
  void)
{
  delete [] this->title;
} // ~SoXtSlider()

// *************************************************************************

/*!
*/

void
SoXtSlider::setValue(
  float value)
{
  this->current = value;
  // FIXME: update widgets
} // setValue()

/*!
*/

float
SoXtSlider::getValue(
  void) const
{
  return this->current;
} // getValue()

// *************************************************************************

/*!
*/

void
SoXtSlider::setRange(
  float min,
  float max)
{
  this->minimum = min;
  this->maximum = max;
} // setRange()

/*!
*/

void
SoXtSlider::getRange(
  float & min,
  float & max) const
{
  min = this->minimum;
  max = this->maximum;
} // getRange()

// *************************************************************************

struct SoXtSliderCallbackInfo {
  SoXtSliderCB * callback;
  void * closure;
};

/*!
*/

void
SoXtSlider::addCallback(
  SoXtSliderCB * callback,
  void * closure)
{
  if (this->callbacks == NULL)
    this->callbacks = new SbPList;
  SoXtSliderCallbackInfo * info = new SoXtSliderCallbackInfo;
  info->callback = callback;
  info->closure = closure;
  this->callbacks->append(info);
} // addCallback()

/*!
*/

void
SoXtSlider::removeCallback(
  SoXtSliderCB * callback,
  void * closure)
{
  if (this->callbacks == NULL) {
#if SOXT_DEBUG
    SoDebugError::postInfo("SoXtSlider::removeCallback",
      "no callbacks");
#endif // SOXT_DEBUG
    return;
  }
  const int num = this->callbacks->getLength();
  for (int i = 0; i < num; i++) {
    SoXtSliderCallbackInfo * info =
      (SoXtSliderCallbackInfo *) (*this->callbacks)[i];
    if (info->callback == callback && info->closure == closure) {
      this->callbacks->remove(i);
      return;
    }
  }
#if SOXT_DEBUG
  SoDebugError::postInfo("SoXtSlider::removeCallback",
    "no such callback");
#endif // SOXT_DEBUG
} // removeCallback()

/*!
*/

void
SoXtSlider::invokeCallbacks(
  void)
{
  if (this->callbacks != NULL) {
    const int num = this->callbacks->getLength();
    for (int i = 0; i < num; i++) {
      SoXtSliderCallbackInfo * info =
        (SoXtSliderCallbackInfo *) (*this->callbacks)[i];
      info->callback(info->closure, this->title, this->current);
    }
  }
} // invokeCallbacks()

// *************************************************************************

/*!
*/

Widget
SoXtSlider::buildSimpleWidget(// protected
  const Widget parent)
{
  if (this->s_form == (Widget) NULL) {
    this->s_form = XtVaCreateManagedWidget("simple",
      xmFormWidgetClass, parent,
      NULL);

    this->s_value = XtVaCreateManagedWidget("value",
      xmTextWidgetClass, this->s_form,
      XmNleftAttachment, XmATTACH_FORM,
      XmNtopAttachment, XmATTACH_FORM,
      XmNrightAttachment, XmATTACH_NONE,
      XmNbottomAttachment, XmATTACH_NONE,
      XmNwidth, 60,
      XmNhighlightThickness, 0,
      NULL);

    char text[16];
    sprintf(text, "%g", this->current);
    XmTextSetString(this->s_value, text);
    XmTextSetCursorPosition(this->s_value, strlen(text));

    XtAddCallback(this->s_value, XmNactivateCallback,
      SoXtSlider::value_cb, (XtPointer) this);
    XtAddCallback(this->s_value, XmNlosingFocusCallback,
      SoXtSlider::value_cb, (XtPointer) this);

    this->s_slider = XtVaCreateManagedWidget("slider",
      xmScaleWidgetClass, this->s_form,
      XmNleftAttachment, XmATTACH_WIDGET,
      XmNleftWidget, this->s_value,
      XmNleftOffset, 2,
      XmNtopAttachment, XmATTACH_FORM,
      XmNtopOffset, 2,
      XmNrightAttachment, XmATTACH_FORM,
      XmNrightOffset, 2,
      XmNbottomAttachment, XmATTACH_FORM,
      XmNbottomOffset, 2,
      XmNorientation, XmHORIZONTAL,
      XmNtraversalOn, False,
      XmNminimum, 0,
      XmNvalue, 0,
      XmNmaximum, 999,
      XmNshowValue, False,
      XmNhighlightThickness, 0,
      XtVaTypedArg,
        XmNtitleString, XmRString,
        "", 0,
      NULL);

    XtAddCallback(this->s_slider, XmNdragCallback,
      SoXtSlider::slider_cb, (XtPointer) this);
    XtAddCallback(this->s_slider, XmNvalueChangedCallback,
      SoXtSlider::slider_cb, (XtPointer) this);
  }
  return this->s_form;
} // buildSimpleWidget()

/*!
*/

Widget
SoXtSlider::buildRangedWidget(
  const Widget parent)
{
  if (this->r_form == (Widget) NULL) {
    this->r_form = XtVaCreateManagedWidget("ranged",
      xmFormWidgetClass, parent,
      NULL);

    this->r_slider = XtVaCreateManagedWidget("slider",
      xmScaleWidgetClass, this->r_form,
      XmNorientation, XmHORIZONTAL,
      XmNtraversalOn, False,
      XmNminimum, 0,
      XmNvalue, 0,
      XmNmaximum, 999,
      XmNshowValue, False,
      XmNhighlightThickness, 0,
      XtVaTypedArg,
        XmNtitleString, XmRString, "", 0,
      NULL);

    XtAddCallback(this->r_slider, XmNdragCallback,
      SoXtSlider::slider_cb, (XtPointer) this);
    XtAddCallback(this->r_slider, XmNvalueChangedCallback,
      SoXtSlider::slider_cb, (XtPointer) this);

    this->r_value = XtVaCreateManagedWidget("value",
      xmTextWidgetClass, this->r_form,
      XmNhighlightThickness, 0,
      NULL);

    char text[16];
    sprintf(text, "%g", this->current);
    XmTextSetString(this->r_value, text);
    XmTextSetCursorPosition(this->r_value, strlen(text));

    XtAddCallback(this->r_value, XmNactivateCallback,
      SoXtSlider::value_cb, (XtPointer) this);
    XtAddCallback(this->r_value, XmNlosingFocusCallback,
      SoXtSlider::value_cb, (XtPointer) this);

    this->r_minValue = XtVaCreateManagedWidget("minValue",
      xmTextWidgetClass, this->r_form,
      XmNhighlightThickness, 0,
      NULL);

    sprintf(text, "%g", this->minimum);
    XmTextSetString(this->r_minValue, text);
    XmTextSetCursorPosition(this->r_minValue, strlen(text));

    XtAddCallback(this->r_minValue, XmNactivateCallback,
      SoXtSlider::min_value_cb, (XtPointer) this);
    XtAddCallback(this->r_minValue, XmNlosingFocusCallback,
      SoXtSlider::min_value_cb, (XtPointer) this);

    this->r_maxValue = XtVaCreateManagedWidget("maxValue",
      xmTextWidgetClass, this->r_form,
      XmNhighlightThickness, 0,
      NULL);

    sprintf(text, "%g", this->maximum);
    XmTextSetString(this->r_maxValue, text);
    XmTextSetCursorPosition(this->r_maxValue, strlen(text));

    XtAddCallback(this->r_maxValue, XmNactivateCallback,
      SoXtSlider::max_value_cb, (XtPointer) this);
    XtAddCallback(this->r_maxValue, XmNlosingFocusCallback,
      SoXtSlider::max_value_cb, (XtPointer) this);

  XtVaSetValues(this->r_value,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_NONE,
    XmNbottomAttachment, XmATTACH_NONE,
    XmNwidth, 60,
    NULL);

  XtVaSetValues(this->r_minValue,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, this->r_value,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_NONE,
    XmNbottomAttachment, XmATTACH_NONE,
    XmNwidth, 50,
    NULL);

  XtVaSetValues(this->r_maxValue,
    XmNleftAttachment, XmATTACH_NONE,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_NONE,
    XmNwidth, 50,
    NULL);

  XtVaSetValues(this->r_slider,
    XmNleftAttachment, XmATTACH_WIDGET,
    XmNleftWidget, this->r_minValue,
    XmNleftOffset, 2,
    XmNtopAttachment, XmATTACH_FORM,
    XmNtopOffset, 2,
    XmNrightAttachment, XmATTACH_WIDGET,
    XmNrightWidget, this->r_maxValue,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNbottomWidget, this->r_minValue,
    XmNbottomOffset, 2,
    XtVaTypedArg,
      XmNtitleString, XmRString,
      "", 0,
    NULL);
  }
  return this->r_form;
} // buildRangedWidget()

/*!
*/

Widget
SoXtSlider::buildFullWidget(
  const Widget parent)
{
  if (this->f_form == (Widget) NULL) {
    char text[16];

    this->f_form = XtVaCreateManagedWidget("slider",
      xmFormWidgetClass, parent,
      NULL);

    this->f_minValue = XtVaCreateManagedWidget("minValue",
      xmTextWidgetClass, this->f_form,
      XmNleftAttachment, XmATTACH_FORM,
      XmNtopAttachment, XmATTACH_FORM,
      XmNrightAttachment, XmATTACH_NONE,
      XmNbottomAttachment, XmATTACH_NONE,
      XmNwidth, 50,
      XmNhighlightThickness, 0,
      NULL);

    sprintf(text, "%g", this->minimum);
    XmTextSetString(this->f_minValue, text);
    XmTextSetCursorPosition(this->f_minValue, strlen(text));

    XtAddCallback(this->f_minValue, XmNactivateCallback,
      SoXtSlider::min_value_cb, (XtPointer) this);
    XtAddCallback(this->f_minValue, XmNlosingFocusCallback,
      SoXtSlider::min_value_cb, (XtPointer) this);

    this->f_maxValue = XtVaCreateManagedWidget("maxValue",
      xmTextWidgetClass, this->f_form,
      XmNleftAttachment, XmATTACH_NONE,
      XmNtopAttachment, XmATTACH_FORM,
      XmNrightAttachment, XmATTACH_FORM,
      XmNbottomAttachment, XmATTACH_NONE,
      XmNwidth, 50,
      XmNhighlightThickness, 0,
      NULL);

    sprintf(text, "%g", this->maximum);
    XmTextSetString(this->f_maxValue, text);
    XmTextSetCursorPosition(this->f_maxValue, strlen(text));

    XtAddCallback(this->f_maxValue, XmNactivateCallback,
      SoXtSlider::max_value_cb, (XtPointer) this);
    XtAddCallback(this->f_maxValue, XmNlosingFocusCallback, 
      SoXtSlider::max_value_cb, (XtPointer) this);

    this->f_value = XtVaCreateManagedWidget("value",
      xmTextWidgetClass, this->f_form,
      XmNleftAttachment, XmATTACH_WIDGET,
      XmNleftWidget, this->f_minValue,
      XmNleftOffset, 2,
      XmNtopAttachment, XmATTACH_FORM,
      XmNrightAttachment, XmATTACH_WIDGET,
      XmNrightWidget, this->f_maxValue,
      XmNrightOffset, 2,
      XmNhighlightThickness, 0,
      NULL);

    sprintf(text, "%g", this->current);
    XmTextSetString(this->f_value, text);
    XmTextSetCursorPosition(this->f_value, strlen(text));

    XtAddCallback(this->f_value, XmNactivateCallback,
      SoXtSlider::value_cb, (XtPointer) this);
    XtAddCallback(this->f_value, XmNlosingFocusCallback,
      SoXtSlider::value_cb, (XtPointer) this);

    this->f_min = XtVaCreateManagedWidget("min",
      xmPushButtonWidgetClass, this->f_form,
      XmNleftAttachment, XmATTACH_FORM,
      XmNtopAttachment, XmATTACH_WIDGET,
      XmNtopWidget, this->f_minValue,
      XmNtopOffset, 2,
      XmNbottomOffset, 2,
      XmNwidth, 50,
      XmNtraversalOn, False,
      NULL);

    XtAddCallback(this->f_min, XmNactivateCallback,
      SoXtSlider::min_cb, (XtPointer) this);

    this->f_max = XtVaCreateManagedWidget("max",
      xmPushButtonWidgetClass, this->f_form,
      XmNtopAttachment, XmATTACH_WIDGET,
      XmNtopWidget, this->f_maxValue,
      XmNtopOffset, 2,
      XmNrightAttachment, XmATTACH_FORM,
      XmNbottomOffset, 2,
      XmNwidth, 50,
      XmNtraversalOn, False,
      NULL);

    XtAddCallback(this->f_max, XmNactivateCallback,
      SoXtSlider::max_cb, (XtPointer) this);

    this->f_slider = XtVaCreateManagedWidget("slider",
      xmScaleWidgetClass, this->f_form,
      XmNleftAttachment, XmATTACH_WIDGET,
      XmNleftWidget, this->f_minValue,
      XmNleftOffset, 2,
      XmNtopAttachment, XmATTACH_WIDGET,
      XmNtopWidget, this->f_value,
      XmNtopOffset, 2,
      XmNrightAttachment, XmATTACH_WIDGET,
      XmNrightWidget, this->f_maxValue,
      XmNrightOffset, 2,
      XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
      XmNbottomWidget, this->f_min,
      XmNbottomOffset, 2,
      XmNorientation, XmHORIZONTAL,
      XmNtraversalOn, False,
      XmNminimum, 0,
      XmNvalue, 0,
      XmNmaximum, 999,
      XmNshowValue, False,
      XmNhighlightThickness, 0,
      NULL);

    XtAddCallback(this->f_slider, XmNdragCallback,
      SoXtSlider::slider_cb, (XtPointer) this);
    XtAddCallback(this->f_slider, XmNvalueChangedCallback,
      SoXtSlider::slider_cb, (XtPointer) this);

    this->f_label = XtVaCreateManagedWidget("label",
      xmLabelWidgetClass, this->f_form,
      XmNleftAttachment, XmATTACH_WIDGET,
      XmNleftWidget, this->f_min,
      XmNtopAttachment, XmATTACH_WIDGET,
      XmNtopWidget, this->f_slider,
      XmNrightAttachment, XmATTACH_WIDGET,
      XmNrightWidget, this->f_max,
      XmNalignment, XmALIGNMENT_BEGINNING,
      XtVaTypedArg,
        XmNlabelString, XmRString,
        this->title, strlen(this->title)+1,
      NULL);

  }
  return this->f_form;
} // buildFullWidget()

/*!
*/

Widget
SoXtSlider::buildOrderlyWidget(
  const Widget parent)
{
  if (this->o_form == (Widget) NULL) {
    this->o_form = XtVaCreateManagedWidget("orderly",
      xmFormWidgetClass, parent,
      NULL);

    this->o_value = XtVaCreateManagedWidget("value",
      xmTextWidgetClass, this->o_form,
      XmNleftAttachment, XmATTACH_FORM,
      XmNtopAttachment, XmATTACH_FORM,
      XmNrightAttachment, XmATTACH_FORM,
      XmNhighlightThickness, 0,
      NULL);

    char text[16];
    sprintf(text, "%g", this->current);
    XmTextSetString(this->o_value, text);
    XmTextSetCursorPosition(this->o_value, strlen(text));

    XtAddCallback(this->o_value, XmNactivateCallback,
      SoXtSlider::value_cb, (XtPointer) this);
    XtAddCallback(this->o_value, XmNlosingFocusCallback,
      SoXtSlider::value_cb, (XtPointer) this);

    this->o_slider = XtVaCreateManagedWidget("slider",
      xmScaleWidgetClass, this->o_form,
      XmNleftAttachment, XmATTACH_FORM,
      XmNtopAttachment, XmATTACH_WIDGET,
      XmNtopWidget, this->o_value,
      XmNtopOffset, 2,
      XmNrightAttachment, XmATTACH_FORM,
      XmNorientation, XmHORIZONTAL,
      XmNtraversalOn, False,
      XmNminimum, 0,
      XmNvalue, 0,
      XmNmaximum, 999,
      XmNshowValue, False,
      XmNhighlightThickness, 0,
      NULL);

    XtAddCallback(this->o_slider, XmNdragCallback,
      SoXtSlider::slider_cb, (XtPointer) this);
    XtAddCallback(this->o_slider, XmNvalueChangedCallback,
      SoXtSlider::slider_cb, (XtPointer) this);

    this->o_label = XtVaCreateManagedWidget("label",
      xmLabelWidgetClass, this->o_form,
      XmNleftAttachment, XmATTACH_FORM,
      XmNtopAttachment, XmATTACH_WIDGET,
      XmNtopWidget, this->o_slider,
      XmNrightAttachment, XmATTACH_FORM,
      XmNbottomAttachment, XmATTACH_FORM,
      XtVaTypedArg,
        XmNlabelString, XmRString,
        this->title, strlen(this->title)+1,
      XmNalignment, XmALIGNMENT_BEGINNING,
      NULL);
  }
  return this->o_form;
} // buildOrderlyWidget()

// *************************************************************************

/*!
*/

void
SoXtSlider::slider_cb(
  Widget widget,
  XtPointer closure,
  XtPointer call_data)
{
  assert(closure != NULL);
  SoXtSlider * slider = (SoXtSlider *) closure;
  XmScaleCallbackStruct * data = (XmScaleCallbackStruct *) call_data;
  float previous = slider->current;
  slider->current = slider->minimum +
    (slider->maximum - slider->minimum) / 999 * (float) data->value;
  slider->current = SoXtMax(slider->current, slider->minimum);
  slider->current = SoXtMin(slider->current, slider->maximum);
  char text[16];
  
  sprintf(text, "%.2g", slider->current);
  const int pos = strlen(text);
  XmTextSetString(slider->s_value, text);
  XmTextSetString(slider->r_value, text);
  XmTextSetString(slider->f_value, text);
  XmTextSetString(slider->o_value, text);
  XmTextSetCursorPosition(slider->s_value, pos);
  XmTextSetCursorPosition(slider->r_value, pos);
  XmTextSetCursorPosition(slider->f_value, pos);
  XmTextSetCursorPosition(slider->o_value, pos);
  XmScaleSetValue(slider->s_slider, data->value);
  XmScaleSetValue(slider->r_slider, data->value);
  XmScaleSetValue(slider->f_slider, data->value);
  XmScaleSetValue(slider->o_slider, data->value);

  if (previous != slider->current)
    slider->invokeCallbacks();
} // slider_cb()

/*!
*/

void
SoXtSlider::value_cb(
  Widget widget,
  XtPointer closure,
  XtPointer call_data)
{
  assert(closure != NULL);
  SoXtSlider * slider = (SoXtSlider *) closure;

  float previous = slider->current;
  slider->current = atof(XmTextGetString(widget));
  slider->current = SoXtMax(slider->current, slider->minimum);
  slider->current = SoXtMin(slider->current, slider->maximum);
  
  char text[16];
  sprintf(text, "%.2g", slider->current);
  const int cpos = strlen(text);
  const int pos = (int) ((slider->current - slider->minimum) /
    (slider->maximum - slider->minimum) * 999.0f);

  XmTextSetString(slider->s_value, text);
  XmTextSetString(slider->r_value, text);
  XmTextSetString(slider->f_value, text);
  XmTextSetString(slider->o_value, text);
  XmTextSetCursorPosition(slider->s_value, cpos);
  XmTextSetCursorPosition(slider->r_value, cpos);
  XmTextSetCursorPosition(slider->f_value, cpos);
  XmTextSetCursorPosition(slider->o_value, cpos);
  XmScaleSetValue(slider->s_slider, pos);
  XmScaleSetValue(slider->r_slider, pos);
  XmScaleSetValue(slider->f_slider, pos);
  XmScaleSetValue(slider->o_slider, pos);

  if (previous != slider->current)
    slider->invokeCallbacks();
} // value_cb()

/*!
*/

void
SoXtSlider::min_cb(
  Widget widget,
  XtPointer closure,
  XtPointer call_data)
{
  assert(closure != NULL);
  SoXtSlider * slider = (SoXtSlider *) closure;
  slider->minimum = slider->current;
  char text[16];
  sprintf(text, "%.2g", slider->minimum);
  const int cpos = strlen(text);

  XmTextSetString(slider->r_minValue, text);
  XmTextSetString(slider->f_minValue, text);
  XmTextSetCursorPosition(slider->r_minValue, cpos);
  XmTextSetCursorPosition(slider->f_minValue, cpos);
  XmScaleSetValue(slider->r_slider, 0);
  XmScaleSetValue(slider->f_slider, 0);
} // min_cb()

/*!
*/

void
SoXtSlider::min_value_cb(
  Widget widget,
  XtPointer closure,
  XtPointer call_data)
{
  assert(closure != NULL);
  SoXtSlider * slider = (SoXtSlider *) closure;
  float previous = slider->current;
  float prevmax = slider->maximum;
  slider->minimum = atof(XmTextGetString(widget));
  slider->maximum = SoXtMax(slider->maximum, slider->minimum);
  slider->current = SoXtMax(slider->current, slider->minimum);

#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtSlider::min_value_cb", "[invoked]");
#endif // SOXT_DEBUG

  char text[16];
  sprintf(text, "%.2g", slider->minimum);
  int cpos = strlen(text);
  int pos = 0;
  if (slider->minimum != slider->maximum)
    pos = (int) ((slider->current - slider->minimum) /
      (slider->maximum - slider->minimum) * 999.0f);

  XmTextSetString(slider->r_minValue, text);
  XmTextSetString(slider->f_minValue, text);
  XmTextSetCursorPosition(slider->r_minValue, cpos);
  XmTextSetCursorPosition(slider->f_minValue, cpos);

  if (prevmax != slider->maximum) {
    sprintf(text, "%.2g", slider->maximum);
    cpos = strlen(text);
    XmTextSetString(slider->r_maxValue, text);
    XmTextSetString(slider->f_maxValue, text);
    XmTextSetCursorPosition(slider->r_maxValue, cpos);
    XmTextSetCursorPosition(slider->f_maxValue, cpos);
  }

  XmScaleSetValue(slider->s_slider, pos);
  XmScaleSetValue(slider->r_slider, pos);
  XmScaleSetValue(slider->f_slider, pos);
  XmScaleSetValue(slider->o_slider, pos);

  if (previous != slider->current) {
    sprintf(text, "%.2g", slider->current);
    cpos = strlen(text);
    XmTextSetString(slider->s_value, text);
    XmTextSetString(slider->r_value, text);
    XmTextSetString(slider->f_value, text);
    XmTextSetString(slider->o_value, text);
    XmTextSetCursorPosition(slider->s_value, cpos);
    XmTextSetCursorPosition(slider->r_value, cpos);
    XmTextSetCursorPosition(slider->f_value, cpos);
    XmTextSetCursorPosition(slider->o_value, cpos);
  }
  if (previous != slider->current)
    slider->invokeCallbacks();
} // min_value_cb()

/*!
*/

void
SoXtSlider::max_cb(
  Widget widget,
  XtPointer closure,
  XtPointer call_data)
{
  assert(closure != NULL);
  SoXtSlider * slider = (SoXtSlider *) closure;
  slider->maximum = slider->current;
  char text[16];
  sprintf(text, "%.2g", slider->maximum);
  const int cpos = strlen(text);
  int pos = 999;
  if (slider->minimum == slider->maximum)
    pos = 0;

  XmTextSetString(slider->r_maxValue, text);
  XmTextSetString(slider->f_maxValue, text);
  XmTextSetCursorPosition(slider->r_maxValue, cpos);
  XmTextSetCursorPosition(slider->f_maxValue, cpos);
  XmScaleSetValue(slider->r_slider, pos);
  XmScaleSetValue(slider->f_slider, pos);
} // max_cb()

/*!
*/

void
SoXtSlider::max_value_cb(
  Widget widget,
  XtPointer closure,
  XtPointer call_data)
{
  assert(closure != NULL);
  SoXtSlider * slider = (SoXtSlider *) closure;
  float previous = slider->current;
  float prevmin = slider->minimum;
  slider->maximum = atof(XmTextGetString(widget));
  slider->minimum = SoXtMin(slider->minimum, slider->maximum);
  slider->current = SoXtMin(slider->current, slider->maximum);

#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtSlider::max_value_cb", "[invoked]");
#endif // SOXT_DEBUG

  char text[16];
  sprintf(text, "%.2g", slider->maximum);
  int cpos = strlen(text);
  int pos = 0;
  if (slider->minimum != slider->maximum)
    pos = (int) ((slider->current - slider->minimum) /
      (slider->maximum - slider->minimum) * 999.0f);

  XmTextSetString(slider->r_maxValue, text);
  XmTextSetString(slider->f_maxValue, text);
  XmTextSetCursorPosition(slider->r_maxValue, cpos);
  XmTextSetCursorPosition(slider->f_maxValue, cpos);

  if (prevmin != slider->minimum) {
    sprintf(text, "%.2g", slider->minimum);
    cpos = strlen(text);
    XmTextSetString(slider->r_minValue, text);
    XmTextSetString(slider->f_minValue, text);
    XmTextSetCursorPosition(slider->r_minValue, cpos);
    XmTextSetCursorPosition(slider->f_minValue, cpos);
  }

  XmScaleSetValue(slider->s_slider, pos);
  XmScaleSetValue(slider->r_slider, pos);
  XmScaleSetValue(slider->f_slider, pos);
  XmScaleSetValue(slider->o_slider, pos);

  if (previous != slider->current) {
    sprintf(text, "%.2g", slider->current);
    cpos = strlen(text);
    XmTextSetString(slider->s_value, text);
    XmTextSetString(slider->r_value, text);
    XmTextSetString(slider->f_value, text);
    XmTextSetString(slider->o_value, text);
    XmTextSetCursorPosition(slider->s_value, cpos);
    XmTextSetCursorPosition(slider->r_value, cpos);
    XmTextSetCursorPosition(slider->f_value, cpos);
    XmTextSetCursorPosition(slider->o_value, cpos);
  }
  if (previous != slider->current)
    slider->invokeCallbacks();
} // max_value_cb()

// *************************************************************************

