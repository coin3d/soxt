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
#include <Inventor/Xt/SoXtCursor.h>
#include <Inventor/Xt/widgets/SoXtPopupMenu.h>
#include <Inventor/Xt/widgets/SoXtThumbWheel.h>

#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>

#if HAVE_CONFIG_H
#include <config.h> // for HAVE_LIBXPM
#endif // HAVE_CONFIG_H

#if HAVE_LIBXPM
#include <Inventor/Xt/common/pixmaps/ortho.xpm>
#include <Inventor/Xt/common/pixmaps/perspective.xpm>
#endif // HAVE_LIBXPM

// *************************************************************************

/*!
  \class SoXtExaminerViewer Inventor/Xt/viewers/SoXtExaminerViewer.h
  \brief The SoXtExaminerViewer class is the most used viewer component.
  \ingroup components viewers
*/

// *************************************************************************

SOXT_OBJECT_SOURCE(SoXtExaminerViewer);

// *************************************************************************

/*!
  \var Widget SoXtExaminerViewer::spinanimtoggle
  Widget for toggling spin animation.
*/

/*!
  \var Widget SoXtExaminerViewer::rotpointaxestoggle
  Widget for toggling rotation point axes graphics.
*/

/*!
  \var Widget SoXtExaminerViewer::rotaxesoverlaytoggle
  Widget for toggling if rotation point axes should be drawn in overlay
  graphics or not.
*/

/*!
  \var Widget SoXtExaminerViewer::axessizewheel
  Widget for wheel controlling axes size.
*/

/*!
  \var Widget SoXtExaminerViewer::axessizefield
  Widget for field for setting axes size.
*/

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
  this->constructor(TRUE);
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
  this->constructor(build);
}

/*!
  Common constructor code.
*/

void
SoXtExaminerViewer::constructor(const SbBool build)
{
  this->genericConstructor();

//  this->prefshell = this->prefsheet = (Widget) NULL;
  this->prefparts = NULL;
  this->numprefparts = 0;

  this->setClassName(this->getWidgetName());
  this->camerabutton = (Widget) NULL;

  if (build) {
    Widget viewer = this->buildWidget(this->getParentWidget());
    this->setBaseWidget(viewer);
    this->fitSize(SbVec2s(500, 300));

    char * dollyString = NULL;
    SoXtResource rsc(this->getRightWheelLabelWidget());
    if (rsc.getResource("dollyString", XmRString, dollyString) &&
         dollyString != NULL)
      this->setRightWheelString(dollyString);
  }
}

/*!
  The destructor.
*/

SoXtExaminerViewer::~SoXtExaminerViewer()
{
  this->genericDestructor();
  delete [] this->prefparts;
}

// *************************************************************************

/*!
  Stops the model from spin-animating.
*/

void
SoXtExaminerViewer::leftWheelStart(void)
{
  if (this->isAnimating())
    this->stopAnimating();
  inherited::leftWheelStart();
}

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::leftWheelMotion(float value)
{
  inherited::leftWheelMotion(
    this->rotXWheelMotion(value, this->getLeftWheelValue()));
}

/*!
  Stops the model from spin-animating.
*/

void
SoXtExaminerViewer::bottomWheelStart(void)
{
  if (this->isAnimating())
    this->stopAnimating();
  inherited::bottomWheelStart();
}

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::bottomWheelMotion(float value)
{
  inherited::bottomWheelMotion(
    this->rotYWheelMotion(value, this->getBottomWheelValue()));
}

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::rightWheelMotion(float value)
{
  this->zoom(this->getRightWheelValue() - value);
  inherited::rightWheelMotion(value);
}

// *************************************************************************

// Documented in superclass.  This method overridden from parent class
// to make sure the mouse pointer cursor is updated.
void
SoXtExaminerViewer::setViewing(SbBool enable)
{
  if ((this->isViewing() && enable) || (!this->isViewing() && !enable)) {
#if SOXT_DEBUG
    SoDebugError::postWarning("SoXtFullViewer::setViewing",
                              "current state already %s", enable ? "TRUE" : "FALSE");
#endif // SOXT_DEBUG
    return;
  }

  this->setMode(enable ?
                        SoXtExaminerViewer::EXAMINE :
                        SoXtExaminerViewer::INTERACT);
  inherited::setViewing(enable);
}

// *************************************************************************

/*!
  \internal

  Set cursor graphics according to mode.
*/

void
SoXtExaminerViewer::setCursorRepresentation(int mode)
{
  // FIXME: remember to set up / change the cursor gfx whenever needed
  // (we need to overload setCursorEnabled(), for instance). 20000426 mortene.


  // FIXME: with the new So@Gui@Cursor class, this has actually become
  // a possibly generic method for all So* toolkits. Move to common
  // code. 20011125 mortene.

  if (!this->isCursorEnabled()) {
    this->setComponentCursor(SoXtCursor::getBlankCursor());
    return;
  }

  switch (mode) {
  case SoXtExaminerViewer::INTERACT:
    this->setComponentCursor(SoXtCursor(SoXtCursor::DEFAULT));
    break;

  case SoXtExaminerViewer::EXAMINE:
  case SoXtExaminerViewer::DRAGGING:
    this->setComponentCursor(SoXtCursor::getRotateCursor());
    break;

  case SoXtExaminerViewer::ZOOMING:
    this->setComponentCursor(SoXtCursor::getZoomCursor());
    break;

  case SoXtExaminerViewer::WAITING_FOR_SEEK:
    this->setComponentCursor(SoXtCursor(SoXtCursor::CROSSHAIR));
    break;

  case SoXtExaminerViewer::WAITING_FOR_PAN:
  case SoXtExaminerViewer::PANNING:
    this->setComponentCursor(SoXtCursor::getPanCursor());
    break;

  default: assert(0); break;
  }
}

// *************************************************************************

// Documented in superclass.
void
SoXtExaminerViewer::createViewerButtons(Widget parent, SbPList * buttonlist)
{
  assert(this->camerabutton == (Widget) NULL);

  inherited::createViewerButtons(parent, buttonlist);

  this->camerabutton = XtVaCreateManagedWidget("C",
                                               xmPushButtonWidgetClass, parent,
                                               NULL);

  XtAddCallback(this->camerabutton,
    XmNdisarmCallback, SoXtExaminerViewer::camerabuttonCB, this);

  buttonlist->append(this->camerabutton);

#if HAVE_LIBXPM
  this->camerapixmaps.ortho =
    SoXtInternal::createPixmapFromXpm(this->camerabutton, ortho_xpm);
  this->camerapixmaps.ortho_ins =
    SoXtInternal::createPixmapFromXpm(this->camerabutton, ortho_xpm, TRUE);
  this->camerapixmaps.perspective =
    SoXtInternal::createPixmapFromXpm(this->camerabutton, perspective_xpm);
  this->camerapixmaps.perspective_ins =
    SoXtInternal::createPixmapFromXpm(this->camerabutton, perspective_xpm, TRUE);
#endif // HAVE_LIBXPM

}

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::camerabuttonClicked(void)
{
  this->toggleCameraType();
}

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::camerabuttonCB(Widget w,
                                   XtPointer client_data,
                                   XtPointer call_data)
{
  SoXtExaminerViewer * viewer = (SoXtExaminerViewer *) client_data;
  viewer->camerabuttonClicked();
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
    pixmap = this->camerapixmaps.ortho;
    pixmap_ins = this->camerapixmaps.ortho_ins;
  } else if (camera->isOfType(SoPerspectiveCamera::getClassTypeId())) {
    pixmap = this->camerapixmaps.perspective;
    pixmap_ins = this->camerapixmaps.perspective_ins;
    SoXtResource rsc(this->getRightWheelLabelWidget());
    char * dollyString = NULL;
    if (rsc.getResource("dollyString", XmRString, dollyString) &&
         dollyString != NULL)
      this->setRightWheelString(dollyString);
  } else if (camera->isOfType(SoOrthographicCamera::getClassTypeId())) {
    pixmap = this->camerapixmaps.ortho;
    pixmap_ins = this->camerapixmaps.ortho_ins;
    SoXtResource rsc(this->getRightWheelLabelWidget());
    char * zoomString = NULL;
    if (rsc.getResource("zoomString", XmRString, zoomString) &&
         zoomString != NULL)
      this->setRightWheelString(zoomString);
  } else {
    SoDebugError::postWarning("SoXtExaminerViewer::setCamera",
      "unknown camera type - got no pixmap");
    // find better pixmaps for this...
    pixmap = this->camerapixmaps.ortho;
    pixmap_ins = this->camerapixmaps.ortho_ins;
  }

#if HAVE_LIBXPM
  SbBool extra = XtIsRealized(this->camerabutton) ? TRUE : FALSE;

  if (extra) {
    XtUnmapWidget(this->camerabutton);
    XtUnrealizeWidget(this->camerabutton);
  }
  if (pixmap) {
    XtVaSetValues(this->camerabutton,
                  XmNlabelType, XmPIXMAP,
                  XmNlabelPixmap, pixmap,
                  XmNselectPixmap, pixmap,
                  XmNlabelInsensitivePixmap, pixmap_ins,
                  XmNselectInsensitivePixmap, pixmap_ins,
                  NULL);
    XtVaSetValues(this->camerabutton,
                  XmNwidth, 30,
                  XmNheight, 30,
                  NULL);
  }
  if (extra) {
    XtRealizeWidget(this->camerabutton);
    XtMapWidget(this->camerabutton);
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
  this->setGenericAnimationEnabled(enable);
  if (this->spinanimtoggle) {
    Boolean enabled = False;
    XtVaGetValues(this->spinanimtoggle,
      XmNset, &enabled,
      NULL);
    if (enable != enabled)
      XtVaSetValues(this->spinanimtoggle,
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
  if (this->axessizefield) {
    char buf[8];
    sprintf(buf, "%d", size);
    XmTextSetString(this->axessizefield, buf);
    XmTextSetCursorPosition(this->axessizefield, (long) strlen(buf));
  }
  this->setGenericFeedbackSize(size);
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
    this->prefparts = new Widget [ 16 ];
    this->createPrefSheetShellAndForm(this->prefshell, this->prefsheet);
    this->createDefaultPrefSheetParts(this->prefparts, this->numprefparts,
      this->prefsheet);
#if SOXT_DEBUG && 0
    SoDebugError::postInfo("SoXtExaminerViewer::createPrefSheet",
      "numparts = %d", this->numprefparts);
#endif // SOXT_DEBUG

    // add parts specific for derived viewer
    this->prefparts[this->numprefparts] =
      this->createFramedSpinAnimPrefSheetGuts(this->prefsheet);
    if (this->prefparts[this->numprefparts] != NULL) this->numprefparts++;

    this->prefparts[this->numprefparts] =
      this->createFramedRotAxisPrefSheetGuts(this->prefsheet);
    if (this->prefparts[this->numprefparts] != NULL) this->numprefparts++;
  }
  this->layoutPartsAndMapPrefSheet(this->prefparts, this->numprefparts,
    this->prefsheet, this->prefshell);
}

// *************************************************************************

/*!
  FIXME: write doc
*/

Widget
SoXtExaminerViewer::createFramedSpinAnimPrefSheetGuts(Widget parent)
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
SoXtExaminerViewer::createSpinAnimPrefSheetGuts(Widget parent)
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
                            XmNset, this->isAnimationEnabled(),
                            NULL);

  XtAddCallback(this->spinanimtoggle, XmNvalueChangedCallback,
                SoXtExaminerViewer::spinanimtoggledCB, (XtPointer) this);

  return form;
}

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::spinanimtoggled(void)
{
  Boolean enable = False;
  XtVaGetValues(this->spinanimtoggle, XmNset, &enable, NULL);
  this->setAnimationEnabled(enable ? TRUE : FALSE);
  if (! enable && this->isAnimating())
    this->stopAnimating();
}

/*!
  static callback
*/

void
SoXtExaminerViewer::spinanimtoggledCB(Widget,
                                      XtPointer closure,
                                      XtPointer)
{
  SoXtExaminerViewer * viewer = (SoXtExaminerViewer *) closure;
  viewer->spinanimtoggled();
}

// *************************************************************************

/*!
  This method is a wrapper for createRotAxisPrefSheetGuts() to include a
  titled frame around the widgets.
*/

Widget
SoXtExaminerViewer::createFramedRotAxisPrefSheetGuts(Widget parent)
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
SoXtExaminerViewer::createRotAxisPrefSheetGuts(Widget parent)
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
    XmNset, this->isFeedbackVisible(),
    NULL);

  XtAddCallback(this->rotpointaxestoggle, XmNvalueChangedCallback,
    SoXtExaminerViewer::rotpointtoggledCB, (XtPointer) this);

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
    SoXtExaminerViewer::rotaxesoverlaytoggledCB, (XtPointer) this);

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
    XmNsensitive, this->isFeedbackVisible() ? True : False,
    XmNeditable, this->isFeedbackVisible() ? True : False,
    XmNcursorPositionVisible, this->isFeedbackVisible() ? True : False,
    NULL);

  XmTextSetMaxLength(this->axessizefield, 3);
  char buffer[16];
  sprintf(buffer, "%d", this->getFeedbackSize());
  XmTextSetString(this->axessizefield, buffer);
  XmTextSetCursorPosition(this->axessizefield, (long) strlen(buffer));

  XtAddCallback(this->axessizefield, XmNactivateCallback,
    SoXtExaminerViewer::axesfieldchangedCB, (XtPointer) this);
  XtAddCallback(this->axessizefield, XmNlosingFocusCallback,
    SoXtExaminerViewer::axesfieldchangedCB, (XtPointer) this);

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
    XmNsensitive, this->isFeedbackVisible() ? True : False,
    NULL);
  XtAddCallback(this->axessizewheel, XmNvalueChangedCallback,
    SoXtExaminerViewer::axeswheelmovedCB, (XtPointer) this);

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

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::rotpointtoggled(void)
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

  this->setFeedbackVisibility(enable ? TRUE : FALSE);
}

/*!
  static callback
*/

void
SoXtExaminerViewer::rotpointtoggledCB(Widget,
                                      XtPointer closure,
                                      XtPointer)
{
  SoXtExaminerViewer * viewer = (SoXtExaminerViewer *) closure;
  viewer->rotpointtoggled();
}

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::axeswheelmoved(int ticks)
{
  int size = this->getFeedbackSize() + ticks;
  if (size < 3)
    size = 3;
  else if (size > 200)
    size = 200;
  this->setFeedbackSize(size);
}

/*!
  static callback
*/

void
SoXtExaminerViewer::axeswheelmovedCB(Widget,
                                     XtPointer closure,
                                     XtPointer call_data)
{
  SoXtExaminerViewer * viewer = (SoXtExaminerViewer *) closure;
  SoXtThumbWheelCallbackData * data = (SoXtThumbWheelCallbackData *) call_data;
  viewer->axeswheelmoved(data->ticks);
}

/*!
  FIXME: write doc
*/

void
SoXtExaminerViewer::axesfieldchanged(void)
{
  int size = atoi(XmTextGetString(this->axessizefield));
  if (size < 3)
    size = 3;
  else if (size > 200)
    size = 200;
  this->setFeedbackSize(size);
}

/*!
  static callback
*/

void
SoXtExaminerViewer::axesfieldchangedCB(Widget,
                                       XtPointer closure,
                                       XtPointer)
{
  SoXtExaminerViewer * viewer = (SoXtExaminerViewer *) closure;
  viewer->axesfieldchanged();
}

/*!
  This callback should toggle wether the point of rotation should be embedded
  in the model Z-buffer wise, or be printed as overlay data.
*/

void
SoXtExaminerViewer::rotaxesoverlaytoggled(void)
{
#if SOXT_DEBUG
  SOXT_STUB_ONCE();
#endif // SOXT_DEBUG
  XtVaSetValues(this->rotaxesoverlaytoggle, XmNset, True, NULL);
}

/*!
  static callback
*/

void
SoXtExaminerViewer::rotaxesoverlaytoggledCB(Widget,
                                            XtPointer closure,
                                            XtPointer)
{
  SoXtExaminerViewer * viewer = (SoXtExaminerViewer *) closure;
  viewer->rotaxesoverlaytoggled();
}

/*!
  Invoked when first mapped.
*/
void
SoXtExaminerViewer::afterRealizeHook(void)
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtExaminerViewer::afterRealizeHook",
                         "[invoked]");
#endif // SOXT_DEBUG
  inherited::afterRealizeHook();
  this->setCursorRepresentation(this->currentmode);
}

// *************************************************************************

Widget
SoXtExaminerViewer::makeSubPreferences(Widget parent)
{
  // FIXME: not activated in SoXtFullViewer yet. 20020111 mortene.
//    return inherited::makeSubPreferences(parent);
  return NULL;
}
