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

/*  $Id$  */

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

#define XmNrefresh "refresh"
#define XmCRefresh "refresh"

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! SOXT_THUMBWHEEL_H_PRIVATE */
