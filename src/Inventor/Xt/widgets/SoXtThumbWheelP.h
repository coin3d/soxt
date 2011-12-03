#ifndef SOXT_THUMBWHEEL_H_PRIVATE
#define SOXT_THUMBWHEEL_H_PRIVATE

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

#include <Xm/Xm.h>
#include <Xm/PrimitiveP.h>

#include <Inventor/Xt/widgets/SoXtThumbWheel.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

typedef struct _SoXtThumbWheelClassPart {
  XtPointer                extension;
} SoXtThumbWheelClassPart;

typedef struct _SoXtThumbWheelClassRec {
  CoreClassPart            core_class;
  XmPrimitiveClassPart     primitive_class;
  SoXtThumbWheelClassPart  thumbwheel_class;
} SoXtThumbWheelClassRec;

/* ********************************************************************** */

typedef struct _SoXtThumbWheelPart {
  /* resources */
  int orientation;
//  Pixel sensitive_color; // ABGR format
//  Pixel insensitive_color; // ABGR format
  Boolean refresh;
  XtCallbackList arm_callback;
  XtCallbackList disarm_callback;
  XtCallbackList valuechanged_callback;

  /* private data */
  float value;
  GC context;
  Pixmap * pixmaps;
  int numpixmaps; // or null-terminate?
  int currentpixmap;
  Boolean armed;
  float arm_value;
  float prev_value;
  int arm_position;
  int prev_position;
  void * thumbwheel;

} SoXtThumbWheelPart;

typedef struct _SoXtThumbWheelRec {
  CorePart            core;
  XmPrimitivePart     primitive;
  SoXtThumbWheelPart  thumbwheel;
} SoXtThumbWheelRec;

#ifndef SoXtNrefresh
#define SoXtNrefresh "refresh"
#endif

#ifndef SoXtCRefresh
#define SoXtCRefresh "Refresh"
#endif

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! SOXT_THUMBWHEEL_H_PRIVATE */
