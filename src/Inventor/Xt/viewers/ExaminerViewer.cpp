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

// Class documentation in common/viewers/SoGuiExaminerViewer.cpp.in.

// *************************************************************************

#include <string.h>
#include <stdlib.h> // atoi()

#include <X11/keysym.h>

#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/projectors/SbSphereSheetProjector.h>
#include <Inventor/projectors/SbSpherePlaneProjector.h>
#include <Inventor/sensors/SoTimerSensor.h>

#include <soxtdefs.h>
#include <../SoXtInternal.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXtResource.h>
#include <Inventor/Xt/widgets/SoXtPopupMenu.h>
#include <Inventor/Xt/widgets/SoXtThumbWheel.h>

#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewerP.h>

#if HAVE_CONFIG_H
#include <config.h> // for HAVE_LIBXPM
#endif // HAVE_CONFIG_H

#if HAVE_LIBXPM
#include <Inventor/Xt/common/pixmaps/ortho.xpm>
#include <Inventor/Xt/common/pixmaps/perspective.xpm>
#endif // HAVE_LIBXPM

// *************************************************************************

// SoXtExaminerViewerP "private implementation" class.

SoXtExaminerViewerP::SoXtExaminerViewerP(SoXtExaminerViewer * publ)
  : SoGuiExaminerViewerP(publ)
{
}

SoXtExaminerViewerP::~SoXtExaminerViewerP()
{
}

// *************************************************************************

SOXT_OBJECT_SOURCE(SoXtExaminerViewer);

// *************************************************************************

/*!
  Public constructor.
*/

SoXtExaminerViewer::SoXtExaminerViewer(Widget parent,
                                       const char * name,
                                       SbBool embed,
                                       SoXtFullViewer::BuildFlag flag,
                                       SoXtViewer::Type type)
  : inherited(parent, name, embed, flag, type, FALSE)
{
  PRIVATE(this) = new SoXtExaminerViewerP(this);
  PRIVATE(this)->constructor(TRUE);
}

/*!
  Protected constructor for derived viewers.
*/

SoXtExaminerViewer::SoXtExaminerViewer(Widget parent,
                                       const char * name,
                                       SbBool embed,
                                       SoXtFullViewer::BuildFlag flag,
                                       SoXtViewer::Type type,
                                       SbBool build)
  : inherited(parent, name, embed, flag, type, FALSE)
{
  PRIVATE(this) = new SoXtExaminerViewerP(this);
  PRIVATE(this)->constructor(build);
}

/*!
  Common constructor code.
*/

void
SoXtExaminerViewerP::constructor(const SbBool build)
{
  this->genericConstructor();

//  this->prefshell = this->prefsheet = (Widget) NULL;
  this->prefparts = NULL;
  this->numprefparts = 0;

  PUBLIC(this)->setClassName(PUBLIC(this)->getWidgetName());
  this->camerabutton = (Widget) NULL;

  if (build) {
    Widget viewer = PUBLIC(this)->buildWidget(PUBLIC(this)->getParentWidget());
    PUBLIC(this)->setBaseWidget(viewer);
    PUBLIC(this)->fitSize(SbVec2s(500, 300));

    char * dollyString = NULL;
    SoXtResource rsc(PUBLIC(this)->getRightWheelLabelWidget());
    if (rsc.getResource("dollyString", XmRString, dollyString) &&
         dollyString != NULL)
      PUBLIC(this)->setRightWheelString(dollyString);
  }
}

/*!
  The destructor.
*/

SoXtExaminerViewer::~SoXtExaminerViewer()
{
  PRIVATE(this)->genericDestructor();
  delete [] PRIVATE(this)->prefparts;
  delete PRIVATE(this);
}

// *************************************************************************

// Documented in superclass.
void
SoXtExaminerViewer::createViewerButtons(Widget parent, SbPList * buttonlist)
{
  assert(PRIVATE(this)->camerabutton == (Widget) NULL);

  inherited::createViewerButtons(parent, buttonlist);

  PRIVATE(this)->camerabutton = XtVaCreateManagedWidget("C",
                                               xmPushButtonWidgetClass, parent,
                                               NULL);

  XtAddCallback(PRIVATE(this)->camerabutton,
                XmNdisarmCallback, SoXtExaminerViewerP::camerabuttonCB, this);

  buttonlist->append(PRIVATE(this)->camerabutton);

#if HAVE_LIBXPM
  PRIVATE(this)->camerapixmaps.ortho =
    SoXtInternal::createPixmapFromXpm(PRIVATE(this)->camerabutton, ortho_xpm);
  PRIVATE(this)->camerapixmaps.ortho_ins =
    SoXtInternal::createPixmapFromXpm(PRIVATE(this)->camerabutton, ortho_xpm, TRUE);
  PRIVATE(this)->camerapixmaps.perspective =
    SoXtInternal::createPixmapFromXpm(PRIVATE(this)->camerabutton, perspective_xpm);
  PRIVATE(this)->camerapixmaps.perspective_ins =
    SoXtInternal::createPixmapFromXpm(PRIVATE(this)->camerabutton, perspective_xpm, TRUE);
#endif // HAVE_LIBXPM

}

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewerP::camerabuttonClicked(void)
{
  PUBLIC(this)->toggleCameraType();
}

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewerP::camerabuttonCB(Widget w,
                                    XtPointer client_data,
                                    XtPointer call_data)
{
  SoXtExaminerViewer * viewer = (SoXtExaminerViewer *) client_data;
  PRIVATE(viewer)->camerabuttonClicked();
}

// *************************************************************************

// Documented in superclass.
void
SoXtExaminerViewer::setCamera(SoCamera * camera)
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtExaminerViewer::setCamera", "[enter]");
#endif // SOXT_DEBUG
  Pixmap pixmap, pixmap_ins;
  if (camera == NULL) {
    // find better pixmaps for this...
    pixmap = PRIVATE(this)->camerapixmaps.ortho;
    pixmap_ins = PRIVATE(this)->camerapixmaps.ortho_ins;
  } else if (camera->isOfType(SoPerspectiveCamera::getClassTypeId())) {
    pixmap = PRIVATE(this)->camerapixmaps.perspective;
    pixmap_ins = PRIVATE(this)->camerapixmaps.perspective_ins;
    SoXtResource rsc(this->getRightWheelLabelWidget());
    char * dollyString = NULL;
    if (rsc.getResource("dollyString", XmRString, dollyString) &&
         dollyString != NULL)
      this->setRightWheelString(dollyString);
  } else if (camera->isOfType(SoOrthographicCamera::getClassTypeId())) {
    pixmap = PRIVATE(this)->camerapixmaps.ortho;
    pixmap_ins = PRIVATE(this)->camerapixmaps.ortho_ins;
    SoXtResource rsc(this->getRightWheelLabelWidget());
    char * zoomString = NULL;
    if (rsc.getResource("zoomString", XmRString, zoomString) &&
         zoomString != NULL)
      this->setRightWheelString(zoomString);
  } else {
    SoDebugError::postWarning("SoXtExaminerViewer::setCamera",
      "unknown camera type - got no pixmap");
    // find better pixmaps for this...
    pixmap = PRIVATE(this)->camerapixmaps.ortho;
    pixmap_ins = PRIVATE(this)->camerapixmaps.ortho_ins;
  }

#if HAVE_LIBXPM
  SbBool extra = XtIsRealized(PRIVATE(this)->camerabutton) ? TRUE : FALSE;

  if (extra) {
    XtUnmapWidget(PRIVATE(this)->camerabutton);
    XtUnrealizeWidget(PRIVATE(this)->camerabutton);
  }
  if (pixmap) {
    XtVaSetValues(PRIVATE(this)->camerabutton,
                  XmNlabelType, XmPIXMAP,
                  XmNlabelPixmap, pixmap,
                  XmNselectPixmap, pixmap,
                  XmNlabelInsensitivePixmap, pixmap_ins,
                  XmNselectInsensitivePixmap, pixmap_ins,
                  NULL);
    XtVaSetValues(PRIVATE(this)->camerabutton,
                  XmNwidth, 30,
                  XmNheight, 30,
                  NULL);
  }
  if (extra) {
    XtRealizeWidget(PRIVATE(this)->camerabutton);
    XtMapWidget(PRIVATE(this)->camerabutton);
  }
#endif // HAVE_LIBXPM

  inherited::setCamera(camera);
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtExaminerViewer::setCamera", "[exit]");
#endif // SOXT_DEBUG
}

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::setAnimationEnabled(const SbBool enable)
{
  PRIVATE(this)->setGenericAnimationEnabled(enable);
  if (PRIVATE(this)->spinanimtoggle) {
    Boolean enabled = False;
    XtVaGetValues(PRIVATE(this)->spinanimtoggle,
                  XmNset, &enabled,
                  NULL);
    if (enable != enabled)
      XtVaSetValues(PRIVATE(this)->spinanimtoggle,
                    XmNset, enable ? True : False,
                    NULL);
  }
}

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::setFeedbackSize(const int size)
{
  if (PRIVATE(this)->axessizefield) {
    char buf[8];
    sprintf(buf, "%d", size);
    XmTextSetString(PRIVATE(this)->axessizefield, buf);
    XmTextSetCursorPosition(PRIVATE(this)->axessizefield, (long) strlen(buf));
  }
  PRIVATE(this)->setGenericFeedbackSize(size);
}

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::createPrefSheet(void)
{
  if (! this->prefshell) {
#if SOXT_DEBUG && 0
    SoDebugError::postInfo("SoXtExaminerViewer::createPrefSheet",
                           "creating preferences window");
#endif // SOXT_DEBUG
    PRIVATE(this)->prefparts = new Widget [ 16 ];
    this->createPrefSheetShellAndForm(this->prefshell, this->prefsheet);
    this->createDefaultPrefSheetParts(PRIVATE(this)->prefparts, PRIVATE(this)->numprefparts,
                                      this->prefsheet);
#if SOXT_DEBUG && 0
    SoDebugError::postInfo("SoXtExaminerViewer::createPrefSheet",
                           "numparts = %d", PRIVATE(this)->numprefparts);
#endif // SOXT_DEBUG

    // add parts specific for derived viewer
    PRIVATE(this)->prefparts[PRIVATE(this)->numprefparts] =
      PRIVATE(this)->createFramedSpinAnimPrefSheetGuts(this->prefsheet);
    if (PRIVATE(this)->prefparts[PRIVATE(this)->numprefparts] != NULL) PRIVATE(this)->numprefparts++;

    PRIVATE(this)->prefparts[PRIVATE(this)->numprefparts] =
      PRIVATE(this)->createFramedRotAxisPrefSheetGuts(this->prefsheet);
    if (PRIVATE(this)->prefparts[PRIVATE(this)->numprefparts] != NULL) PRIVATE(this)->numprefparts++;
  }
  this->layoutPartsAndMapPrefSheet(PRIVATE(this)->prefparts, PRIVATE(this)->numprefparts,
                                   this->prefsheet, this->prefshell);
}

// *************************************************************************

/*!
  FIXME: write doc
*/

Widget
SoXtExaminerViewerP::createFramedSpinAnimPrefSheetGuts(Widget parent)
{
  Widget frame = XtVaCreateManagedWidget("spinanimframe",
                                         xmFrameWidgetClass, parent, NULL);

  Widget label = XtVaCreateManagedWidget("spinanimlabel",
                                         xmLabelGadgetClass, frame,
                                         XmNchildType, XmFRAME_TITLE_CHILD,
                                         XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
                                         XtVaTypedArg,
                                         XmNlabelString, XmRString,
                                         "Spin Animation Settings", strlen("Spin Animation Settings") + 1,
                                         NULL);

  this->createSpinAnimPrefSheetGuts(frame);

  return frame;
}

/*!
  FIXME: write doc
*/

Widget
SoXtExaminerViewerP::createSpinAnimPrefSheetGuts(Widget parent)
{
  //  [] enable spin animation
  Widget form = XtVaCreateManagedWidget("spinanimguts",
                                        xmFormWidgetClass, parent, NULL);

  this->spinanimtoggle =
    XtVaCreateManagedWidget("spinanimtoggle",
                            xmToggleButtonWidgetClass, form,
                            XmNtopAttachment, XmATTACH_FORM,
                            XmNleftAttachment, XmATTACH_FORM,
                            XmNbottomAttachment, XmATTACH_FORM,
                            XtVaTypedArg,
                            XmNlabelString, XmRString,
                            "enable spin animation", strlen("enable spin animation") + 1,
                            XmNset, PUBLIC(this)->isAnimationEnabled(),
                            NULL);

  XtAddCallback(this->spinanimtoggle, XmNvalueChangedCallback,
                SoXtExaminerViewerP::spinanimtoggledCB, (XtPointer) this);

  return form;
}

void
SoXtExaminerViewerP::spinanimtoggled(void)
{
  Boolean enable = False;
  XtVaGetValues(this->spinanimtoggle, XmNset, &enable, NULL);
  PUBLIC(this)->setAnimationEnabled(enable ? TRUE : FALSE);
  if (! enable && PUBLIC(this)->isAnimating())
    PUBLIC(this)->stopAnimating();
}

void
SoXtExaminerViewerP::spinanimtoggledCB(Widget,
                                      XtPointer closure,
                                      XtPointer)
{
  SoXtExaminerViewer * viewer = (SoXtExaminerViewer *) closure;
  PRIVATE(viewer)->spinanimtoggled();
}

// *************************************************************************

/*!
  This method is a wrapper for createRotAxisPrefSheetGuts() to include a
  titled frame around the widgets.
*/

Widget
SoXtExaminerViewerP::createFramedRotAxisPrefSheetGuts(Widget parent)
{
  Widget frame = XtVaCreateManagedWidget("rotaxisframe",
                                         xmFrameWidgetClass, parent, NULL);

  Widget label = XtVaCreateManagedWidget("rotaxislabel",
                                         xmLabelGadgetClass, frame,
                                         XmNchildType, XmFRAME_TITLE_CHILD,
                                         XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
                                         XtVaTypedArg,
                                         XmNlabelString, XmRString,
                                         "Rotation Point Axes Settings", strlen("Rotation Point Axes Settings") + 1,
                                         NULL);

  this->createRotAxisPrefSheetGuts(frame);

  return frame;
}

/*!
  FIXME: write doc
*/

Widget
SoXtExaminerViewerP::createRotAxisPrefSheetGuts(Widget parent)
{
  //  [] display point of rotation
  //  axes size   |||||||||||||||||||||||||  |NN| pixels

  Widget form = XtVaCreateManagedWidget("rotaxisguts",
                                        xmFormWidgetClass, parent, NULL);

#if 0

  this->rotpointaxestoggle = XtVaCreateManagedWidget("rotpointaxestoggle",
                                                     xmToggleButtonWidgetClass, form,
                                                     XmNtopAttachment, XmATTACH_FORM,
                                                     XmNleftAttachment, XmATTACH_FORM,
                                                     XtVaTypedArg,
                                                     XmNlabelString, XmRString,
                                                     "display rotation point", strlen("display rotation point") + 1,
                                                     XmNset, PUBLIC(this)->isFeedbackVisible(),
                                                     NULL);

  XtAddCallback(this->rotpointaxestoggle, XmNvalueChangedCallback,
                SoXtExaminerViewerP::rotpointtoggledCB, (XtPointer) this);

  this->rotaxesoverlaytoggle = XtVaCreateManagedWidget("rotaxesoverlaytoggle",
                                                       xmToggleButtonWidgetClass, form,
                                                       XmNtopAttachment, XmATTACH_FORM,
                                                       XmNrightAttachment, XmATTACH_FORM,
                                                       XtVaTypedArg,
                                                       XmNlabelString, XmRString,
                                                       "overlay graphics", strlen("overlay graphics") + 1,
                                                       XmNset, True,
                                                       NULL);

  XtAddCallback(this->rotaxesoverlaytoggle, XmNvalueChangedCallback,
                SoXtExaminerViewerP::rotaxesoverlaytoggledCB, (XtPointer) this);

  Widget pixelslabel = XtVaCreateWidget("pixelslabel",
                                        xmLabelWidgetClass, form,
                                        XtVaTypedArg,
                                        XmNlabelString, XmRString,
                                        "pixels", strlen("pixels") + 1,
                                        NULL);

  Dimension width;
  XtVaGetValues(pixelslabel, XmNwidth, &width, NULL);
  this->axessizefield = XtVaCreateManagedWidget("axessizefield",
                                                xmTextFieldWidgetClass, form,
                                                XmNtopAttachment, XmATTACH_WIDGET,
                                                XmNtopWidget, this->rotpointaxestoggle,
                                                XmNrightAttachment, XmATTACH_FORM,
                                                XmNrightOffset, width + 5,
                                                XmNwidth, 40,
                                                XmNsensitive, PUBLIC(this)->isFeedbackVisible() ? True : False,
                                                XmNeditable, PUBLIC(this)->isFeedbackVisible() ? True : False,
                                                XmNcursorPositionVisible, PUBLIC(this)->isFeedbackVisible() ? True : False,
                                                NULL);

  XmTextSetMaxLength(this->axessizefield, 3);
  char buffer[16];
  sprintf(buffer, "%d", PUBLIC(this)->getFeedbackSize());
  XmTextSetString(this->axessizefield, buffer);
  XmTextSetCursorPosition(this->axessizefield, (long) strlen(buffer));

  XtAddCallback(this->axessizefield, XmNactivateCallback,
                SoXtExaminerViewerP::axesfieldchangedCB, (XtPointer) this);
  XtAddCallback(this->axessizefield, XmNlosingFocusCallback,
                SoXtExaminerViewerP::axesfieldchangedCB, (XtPointer) this);

  XtVaSetValues(pixelslabel,
                XmNrightAttachment, XmATTACH_FORM,
                XmNrightOffset, 2,
                XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                XmNtopWidget, this->axessizefield,
                XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
                XmNbottomWidget, this->axessizefield,
                NULL);
  XtManageChild(pixelslabel);

  this->axessizewheel = XtVaCreateManagedWidget("axeswheel",
                                                soxtThumbWheelWidgetClass, form,
                                                XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                                                XmNtopWidget, this->axessizefield,
                                                XmNtopOffset, 2,
                                                XmNrightAttachment, XmATTACH_WIDGET,
                                                XmNrightWidget, this->axessizefield,
                                                XmNrightOffset, 2,
                                                XmNheight, 26,
                                                XmNwidth, 90,
                                                XmNorientation, XmHORIZONTAL,
                                                XmNsensitive, PUBLIC(this)->isFeedbackVisible() ? True : False,
                                                NULL);
  XtAddCallback(this->axessizewheel, XmNvalueChangedCallback,
                SoXtExaminerViewerP::axeswheelmovedCB, (XtPointer) this);

  Widget label = XtVaCreateManagedWidget("axeslabel",
                                         xmLabelWidgetClass, form,
                                         XmNtopAttachment, XmATTACH_OPPOSITE_WIDGET,
                                         XmNtopWidget, this->axessizewheel,
                                         XmNrightAttachment, XmATTACH_WIDGET,
                                         XmNrightWidget, this->axessizewheel,
                                         XmNrightOffset, 2,
                                         XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
                                         XmNbottomWidget, this->axessizewheel,
                                         XtVaTypedArg,
                                         XmNlabelString, XmRString,
                                         "axes size", strlen("axes size") + 1,
                                         NULL);

#endif // 0

  return form;
}

void
SoXtExaminerViewerP::rotpointtoggled(void)
{
  Boolean enable = False;
  XtVaGetValues(this->rotpointaxestoggle, XmNset, &enable, NULL);

  XtVaSetValues(this->axessizewheel,
                XmNsensitive, enable,
                NULL);
  XtVaSetValues(this->axessizefield,
                XmNsensitive, enable,
                XmNeditable, enable,
                XmNcursorPositionVisible, enable,
                NULL);

  PUBLIC(this)->setFeedbackVisibility(enable ? TRUE : FALSE);
}

void
SoXtExaminerViewerP::rotpointtoggledCB(Widget,
                                      XtPointer closure,
                                      XtPointer)
{
  SoXtExaminerViewer * viewer = (SoXtExaminerViewer *) closure;
  PRIVATE(viewer)->rotpointtoggled();
}

void
SoXtExaminerViewerP::axeswheelmoved(int ticks)
{
  int size = PUBLIC(this)->getFeedbackSize() + ticks;
  if (size < 3)
    size = 3;
  else if (size > 200)
    size = 200;
  PUBLIC(this)->setFeedbackSize(size);
}

void
SoXtExaminerViewerP::axeswheelmovedCB(Widget,
                                      XtPointer closure,
                                      XtPointer call_data)
{
  SoXtExaminerViewer * viewer = (SoXtExaminerViewer *) closure;
  SoXtThumbWheelCallbackData * data = (SoXtThumbWheelCallbackData *) call_data;
  PRIVATE(viewer)->axeswheelmoved(data->ticks);
}

void
SoXtExaminerViewerP::axesfieldchanged(void)
{
  int size = atoi(XmTextGetString(this->axessizefield));
  if (size < 3)
    size = 3;
  else if (size > 200)
    size = 200;
  PUBLIC(this)->setFeedbackSize(size);
}

void
SoXtExaminerViewerP::axesfieldchangedCB(Widget,
                                       XtPointer closure,
                                       XtPointer)
{
  SoXtExaminerViewer * viewer = (SoXtExaminerViewer *) closure;
  PRIVATE(viewer)->axesfieldchanged();
}

// This callback should toggle wether the point of rotation should be
// embedded in the model Z-buffer wise, or be printed as overlay data.
void
SoXtExaminerViewerP::rotaxesoverlaytoggled(void)
{
#if SOXT_DEBUG
  SOXT_STUB_ONCE();
#endif // SOXT_DEBUG
  XtVaSetValues(this->rotaxesoverlaytoggle, XmNset, True, NULL);
}

void
SoXtExaminerViewerP::rotaxesoverlaytoggledCB(Widget,
                                             XtPointer closure,
                                             XtPointer)
{
  SoXtExaminerViewer * viewer = (SoXtExaminerViewer *) closure;
  PRIVATE(viewer)->rotaxesoverlaytoggled();
}

// *************************************************************************

Widget
SoXtExaminerViewer::makeSubPreferences(Widget parent)
{
  // FIXME: not activated in SoXtFullViewer yet. 20020111 mortene.
//    return inherited::makeSubPreferences(parent);
  return NULL;
}
