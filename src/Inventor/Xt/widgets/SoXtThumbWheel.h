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

/* $Id$ */

#ifndef SOXT_THUMBWHEEL_H
#define SOXT_THUMBWHEEL_H

#include <Xm/Xm.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

extern WidgetClass soxtThumbWheelWidgetClass;

typedef struct _SoXtThumbWheelRec *       SoXtThumbWheelWidget;
typedef struct _SoXtThumbWheelClassRec *  SoXtThumbWheelWidgetClass;

typedef struct _SoXtThumbWheelCallbackData {
  int reason;
  XEvent * event;
  float current;
  float previous;
  int ticks;
} SoXtThumbWheelCallbackData;

/* Callback reasons */
#define SoXtCR_ARM      0x01
#define SoXtCR_DISARM   0x02
#define SoXtCR_MOVE     0x03

void SoXtThumbWheelSetValue(Widget w, float value);
float SoXtThumbWheelGetValue(Widget w);

#ifndef XtIsSoXtThumbWheel
#define XtIsSoXtThumbWheel(w) XtIsSubclass(w, soxtThumbWheelWidgetClass)
#endif /* ! SoXtIsThumbWheel */

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! SOXT_THUMBWHEEL_H */
