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

#ifndef SOXT_THUMBWHEEL_H
#define SOXT_THUMBWHEEL_H

#include <Xm/Xm.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

struct SoXtThumbWheelCallbackData {
  float current;
  float previous;
  int ticks;
};

extern WidgetClass soxtThumbWheelWidgetClass;

typedef struct _SoXtThumbWheelRec *       SoXtThumbWheelWidget;
typedef struct _SoXtThumbWheelClassRec *  SoXtThumbWheelWidgetClass;

#ifndef XtIsSoXtThumbWheel
#define XtIsSoXtThumbWheel(w) XtIsSubclass(w, soxtThumbWheelWidgetClass)
#endif /* ! SoXtIsThumbWheel */

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! SOXT_THUMBWHEEL_H */
