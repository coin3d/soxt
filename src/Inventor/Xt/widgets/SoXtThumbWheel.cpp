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

static const char rcsid[] =
  "$Id$";

#ifdef SOXT_THUMBWHEELTEST

#include <Xm/XmP.h>

#include <Inventor/errors/SoDebugError.h>

#include <Inventor/Xt/widgets/SoXtThumbWheelP.h>

// *************************************************************************
// RESOURCES

static XtResource resources[] = {
}; // resources

// *************************************************************************
// ACTION FUNCTION DECLARATIONS

// *************************************************************************
// ACTIONS TABLE

// *************************************************************************
// DEFAULT TRANSLATION TABLE

// *************************************************************************
// METHOD FUNCTION DECLARATIONS

// *************************************************************************
// CLASS RECORD INITIALIZATION

SoXtThumbWheelClassRec soXtThumbWheelClassRec = {
  { // core part
        /* superclass            */ (WidgetClass) &xmPrimitiveClassRec,
        /* class_name            */ "SoXtThumbWheel",
        /* widget_size           */ sizeof(SoXtThumbWheelRec),
        /* class_initialize      */ class_initialize,
        /* class_part_initialize */ class_part_initialize,
        /* class_inited          */ False,
        /* initialize            */ initialize,
        /* initialize_hook       */ NULL,
        /* realize               */ XtInheritRealize,
        /* actions               */ actions,
        /* num_actions           */ XtNumber(actions),
        /* resources             */ resources,
        /* num_resources         */ XtNumber(resources),
        /* xrm_class             */ NULLQUARK,
        /* compress_motion       */ True,
        /* compress_exposure     */ XtExposeCompressMaximal,
        /* compress_enterleave   */ True,
        /* visible_interest      */ False,
        /* destroy               */ destroy,
        /* resize                */ resize,
        /* expose                */ expose,
        /* set_values            */ set_values,
        /* set_values_hook       */ NULL,
        /* set_values_almost     */ XtInheritSetValuesAlmost, /* FIX ME */
        /* get_values_hook       */ NULL,
        /* accept_focus          */ NULL,
        /* version               */ XtVersionDontCheck,
        /* callback offsets      */ NULL,
        /* tm_table              */ NULL,
        /* query_geometry        */ query_geometry,
        /* display_accelerator   */ NULL,
        /* extension             */ (XtPointer)&_XmLabelCoreClassExtRec
  },
  { // primitive part
        /* border_highlight      */ XmInheritBorderHighlight,
        /* border_unhighlight    */ XmInheritBorderUnhighlight,
        /* translations          */ XtInheritTranslations,
        /* arm_and_activate_proc */ NULL,
        /* Synthetic Resources   */ syn_resources,
        /* num syn res           */ XtNumber(syn_resources),
        /* extension             */ (XtPointer)&_XmLabelPrimClassExtRec
  },
  { // thumbwheel part
        /* setOverrideCallback */ set_override_callback,
        /* menuProcs           */ NULL,
        /* translations        */ NULL,
        /* extension           */ NULL

  }
}; // soXtThumbWheelClassRec

WidgetClass soXtThumbWheelWidgetClass = (WidgetClass) &soXtThumbWheelClassRec;

// *************************************************************************
// METHOD FUNCTION DEFINITIONS

// *************************************************************************
// ACTION FUNCTION DEFINITIONS

// *************************************************************************

#endif // SOXT_THUMBWHEELTEST
