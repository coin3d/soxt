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
