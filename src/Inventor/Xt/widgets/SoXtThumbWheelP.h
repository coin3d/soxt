/**************************************************************************
 *
 *  This file is part of the Coin SoXt GUI binding library.
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License version
 *  2.1 as published by the Free Software Foundation.  See the file
 *  LICENSE.LGPL at the root directory of the distribution for all the
 *  details.
 *
 *  If you want to use Coin SoXt for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

/* $Id$ */

#ifndef SOXT_THUMBWHEEL_H_PRIVATE
#define SOXT_THUMBWHEEL_H_PRIVATE

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

#define SoXtNrefresh "refresh"
#define SoXtCRefresh "refresh"

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! SOXT_THUMBWHEEL_H_PRIVATE */
