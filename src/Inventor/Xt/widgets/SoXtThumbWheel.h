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

void SoXtThumbWheelSetValue( Widget w, float value );
float SoXtThumbWheelGetValue( Widget w );

#ifndef XtIsSoXtThumbWheel
#define XtIsSoXtThumbWheel(w) XtIsSubclass(w, soxtThumbWheelWidgetClass)
#endif /* ! SoXtIsThumbWheel */

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! SOXT_THUMBWHEEL_H */
