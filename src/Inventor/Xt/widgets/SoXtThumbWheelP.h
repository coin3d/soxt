#ifndef SOXT_THUMBWHEEL_H_PRIVATE
#define SOXT_THUMBWHEEL_H_PRIVATE

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
