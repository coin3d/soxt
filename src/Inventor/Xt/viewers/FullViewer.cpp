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
#include <stdlib.h>
#include <math.h>

#include <X11/IntrinsicP.h>

#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/LabelG.h>
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>
#include <Xm/DialogS.h>
#include <Xm/RowColumn.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/Scale.h>

#include <Inventor/misc/SoBasic.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>

#include <soxtdefs.h>
#include <../SoXtInternal.h>
#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtResource.h>
#include <Inventor/Xt/widgets/SoXtThumbWheel.h>
#include <Inventor/Xt/widgets/SoXtPopupMenu.h>

#include <Inventor/Xt/viewers/SoXtFullViewer.h>
#include <Inventor/Xt/viewers/SoXtFullViewerP.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#if HAVE_LIBXPM
#include <X11/xpm.h>
#include <Inventor/Xt/common/pixmaps/pick.xpm>
#include <Inventor/Xt/common/pixmaps/view.xpm>
#include <Inventor/Xt/common/pixmaps/help.xpm>
#include <Inventor/Xt/common/pixmaps/home.xpm>
#include <Inventor/Xt/common/pixmaps/set_home.xpm>
#include <Inventor/Xt/common/pixmaps/view_all.xpm>
#include <Inventor/Xt/common/pixmaps/seek.xpm>
#endif // HAVE_LIBXPM

// *************************************************************************

/*!
  \class SoXtFullViewer Inventor/Xt/SoXtFullViewer.h
  \brief The SoXtFullViewer class adds GUI decorations to the viewer component.
  \ingroup components viewers
*/

// *************************************************************************

#define PRIVATE(o) (o->pimpl)
#define PUBLIC(o) (o->pub)

// *************************************************************************

SOXT_OBJECT_ABSTRACT_SOURCE(SoXtFullViewer);

// *************************************************************************

enum DefaultViewerButtons {
  INTERACT_BUTTON = 0,
  EXAMINE_BUTTON,
  HELP_BUTTON,
  HOME_BUTTON,
  SET_HOME_BUTTON,
  VIEW_ALL_BUTTON,
  SEEK_BUTTON,

  FIRST_BUTTON = INTERACT_BUTTON,
  LAST_BUTTON = SEEK_BUTTON
};

#define VIEWERBUTTON(button) ((Widget) ((*this->viewerButtonWidgets)[button]))

// *************************************************************************

/*!
  \var float SoXtFullViewer::leftWheelVal

  The value of the left thumb wheel.  Do not set directly, use access method.
*/

/*!
  \var Widget SoXtFullViewer::leftWheel

  The widget for the left thumb wheel.
*/

/*!
  \var Widget SoXtFullViewer::leftWheelLabel

  The widget for the label below the left thumb wheel.
*/

/*!
  \var char * SoXtFullViewer::leftWheelStr

  The string used in the label under the left thumb wheel.
*/

/*!
  \var float SoXtFullViewer::bottomWheelVal

  The value of the bottom thumb wheel.  Do not set directly, use access method.
*/

/*!
  \var Widget SoXtFullViewer::bottomWheel

  The widget for the bottom thumb wheel.
*/

/*!
  \var Widget SoXtFullViewer::bottomWheelLabel

  The widget for the label besides the bottom thumb wheel.
*/

/*!
  \var char * SoXtFullViewer::bottomWheelStr

  The string used in the label besides the bottom thumb wheel.
*/

/*!
  \var float SoXtFullViewer::rightWheelVal

  The value of the right thumb wheel.  Do not set directly, use access method.
*/

/*!
  \var Widget SoXtFullViewer::rightWheel

  The widget for the right thumb wheel.
*/

/*!
  \var Widget SoXtFullViewer::rightWheelLabel

  The widget for the label below the right thumb wheel.
*/

/*!
  \var char * SoXtFullViewer::rightWheelStr

  The string used in the label below the right thumb wheel.
*/

/*!
  \enum SoXtFullViewer::BuildFlag

  Flags for building various parts of the viewer.
*/

/*!
  \var SbPList * SoXtFullViewer::viewerButtonWidgets

  The list of button widgets for the right viewer decoration.
*/

/*!
  \var Widget SoXtFullViewer::prefsheet

  The preferences sheet.
*/

/*!
  \var Widget SoXtFullViewer::prefshell

  The shell for the preferences window.
*/

/*!
  \var char * SoXtFullViewer::prefstring

  The title string for the preferences window.
*/

/*!
  \var SbBool SoXtFullViewer::popupEnabled

  Whether the popup menu is enabled or not.
*/

/*!
  \var SoXtPopupMenu * SoXtFullViewer::prefmenu

  The popup menu manager object.
*/

/*!
  \var char * SoXtFullViewer::popupTitle

  The title for the popup menu,
*/


// *************************************************************************

/*!
  Constructor.
*/

SoXtFullViewer::SoXtFullViewer(Widget parent,
                               const char * name,
                               SbBool embed,
                               BuildFlag flags,
                               Type type,
                               SbBool build)
  : inherited(parent, name, embed, type, FALSE),
    popupTitle(NULL)
{
  PRIVATE(this) = new SoXtFullViewerP(this);

  // initialization of protected members
  this->leftWheel = NULL;
  this->leftWheelStr = strcpy(new char [sizeof("Motion X")+1], "Motion X");
  this->leftWheelVal = 0.0f;
  this->leftWheelLabel = NULL;

  this->bottomWheel = NULL;
  this->bottomWheelStr = strcpy(new char [sizeof("Motion Y")+1], "Motion Y");
  this->bottomWheelVal = 0.0f;
  this->bottomWheelLabel = NULL;

  this->rightWheel = NULL;
  this->rightWheelStr = strcpy(new char [sizeof("Motion Z")+1], "Motion Z");
  this->rightWheelVal = 0.0f;
  this->rightWheelLabel = NULL;

  // initialization of private members
  this->leftDecoration = NULL;
  this->rightDecoration = NULL;
  this->bottomDecoration = NULL;

  PRIVATE(this)->viewerbase = NULL;
  PRIVATE(this)->canvas = NULL;

  PRIVATE(this)->seekdistance = 50.0f;
  PRIVATE(this)->seekdistaspercentage = TRUE;

  PRIVATE(this)->zoomrange = SbVec2f(1.0f, 140.0f);

  this->popupEnabled = (flags & SoXtFullViewer::BUILD_POPUP) ? TRUE : FALSE;
  PRIVATE(this)->decorations = (flags & SoXtFullViewer::BUILD_DECORATION) ? TRUE : FALSE;

  PRIVATE(this)->appButtonsList = new SbPList;
  this->viewerButtonWidgets = new SbPList;

  this->prefshell = NULL;
  this->prefmenu = NULL;
  this->prefstring = NULL;

  PRIVATE(this)->pixmaps.pick = 0;
  PRIVATE(this)->pixmaps.pick_ins = 0;
  PRIVATE(this)->pixmaps.view = 0;
  PRIVATE(this)->pixmaps.view_ins = 0;
  PRIVATE(this)->pixmaps.help = 0;
  PRIVATE(this)->pixmaps.help_ins = 0;
  PRIVATE(this)->pixmaps.home = 0;
  PRIVATE(this)->pixmaps.home_ins = 0;
  PRIVATE(this)->pixmaps.set_home = 0;
  PRIVATE(this)->pixmaps.set_home_ins = 0;
  PRIVATE(this)->pixmaps.view_all = 0;
  PRIVATE(this)->pixmaps.view_all_ins = 0;
  PRIVATE(this)->pixmaps.seek = 0;
  PRIVATE(this)->pixmaps.seek_ins = 0;

  if (build != FALSE) {
    this->setClassName("SoXtFullViewer");
    Widget viewer = this->buildWidget(this->getParentWidget());
    this->setBaseWidget(viewer);
    this->fitSize(SbVec2s(500, 300));
  }
}

/*!
  The destructor.
*/

SoXtFullViewer::~SoXtFullViewer()
{
  delete PRIVATE(this)->appButtonsList;
  delete this->viewerButtonWidgets;

  delete PRIVATE(this);
}

// *************************************************************************

static const int SOXT_VIEWER_MIN_HEIGHT_BASE = 30 + /* 90 */ 122 + 8;
static const int SOXT_VIEWER_MIN_WIDTH = 300;

/*!
  This method sets whether the decorations should be shown or not.
*/

void
SoXtFullViewer::setDecoration(const SbBool enable)
{
  if (!!PRIVATE(this)->decorations && !!enable) { return; }

  if ((PRIVATE(this)->decorations != enable) && (PRIVATE(this)->viewerbase != (Widget) NULL))
    PRIVATE(this)->showDecorationWidgets(enable);
  PRIVATE(this)->decorations = enable;

  if (this->isTopLevelShell() || XtIsShell(XtParent(this->getBaseWidget()))) {
    Widget shell = this->getShellWidget();
    Dimension minwidth = SOXT_VIEWER_MIN_WIDTH;
    Dimension minheight = SOXT_VIEWER_MIN_HEIGHT_BASE +
      30 * this->viewerButtonWidgets->getLength();
    if (enable) {
      Dimension width, height;
      XtVaGetValues(shell, 
                    XmNwidth, &width,
                    XmNheight, &height,
                    NULL);
      width = SoXtMax(width, minwidth);
      height = SoXtMax(height, minheight);
      XtVaSetValues(shell,
                    XmNminWidth, minwidth,
                    XmNminHeight, minheight,
                    XmNwidth, width,
                    XmNheight, height,
                    NULL);
    }
    else {
      XtVaSetValues(shell,
                    XmNminWidth, 0,
                    XmNminHeight, 0,
                    NULL);
    }
  }
}

/*!
  This method returns whether the decorations are shown or not.
*/

SbBool
SoXtFullViewer::isDecoration(void) const
{
  return PRIVATE(this)->decorations;
}

// *************************************************************************

/*!
  This method sets whether the popup menu should be enabled or not.
*/

void
SoXtFullViewer::setPopupMenuEnabled(const SbBool enable)
{
  this->popupEnabled = enable;
}

/*!
  This method returns whether the popup menu is enabled or not.
*/

SbBool
SoXtFullViewer::isPopupMenuEnabled(void) const
{
  return this->popupEnabled;
}


// *************************************************************************

/*!
  Return value is either NULL or a widget of type xmFormWidgetClass (Xm/Form.h)

  AppPushButtons are on the left decoration.
*/

Widget
SoXtFullViewer::getAppPushButtonParent(void) const
{
  return PRIVATE(this)->appButtonsForm;
}

/*!
  Not implemented.

  Each button will be set to 30x30 pixels, and they will appear on the left
  viewer decoration.
*/

void
SoXtFullViewer::addAppPushButton(Widget button)
{
  PRIVATE(this)->appButtonsList->append(button);
  PRIVATE(this)->layoutAppPushButtons(this->getAppPushButtonParent());
}

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::insertAppPushButton(Widget button,
                                    int idx)
{
  PRIVATE(this)->appButtonsList->insert(button, idx);
  PRIVATE(this)->layoutAppPushButtons(this->getAppPushButtonParent());
}

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::removeAppPushButton(Widget button)
{
  int idx = PRIVATE(this)->appButtonsList->find(button);
  assert(idx != -1 && "tried to remove non-existant button");
  PRIVATE(this)->appButtonsList->remove(idx);
  PRIVATE(this)->layoutAppPushButtons(this->getAppPushButtonParent());
}

/*!
  FIXME: write doc
*/

int
SoXtFullViewer::findAppPushButton(Widget button) const
{
  return PRIVATE(this)->appButtonsList->find(button);
}

/*!
  FIXME: write doc
*/

int
SoXtFullViewer::lengthAppPushButton(void) const
{
  return PRIVATE(this)->appButtonsList->getLength();
}

/*!
  FIXME: write doc
*/

void
SoXtFullViewerP::layoutAppPushButtons(Widget parent)
{
  SOXT_STUB();
}

// *************************************************************************

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::getRenderAreaWidget(void) const
{
  return PRIVATE(this)->canvas;
}

// *************************************************************************

// Documented in superclass.
Widget
SoXtFullViewer::buildWidget(Widget parent)
{
  int depth = 0;
  XtVaGetValues(parent, XmNdepth, &depth, NULL);

  PRIVATE(this)->viewerbase = XtVaCreateManagedWidget(this->getWidgetName(),
                                                      xmFormWidgetClass, parent,
                                                      NULL);
  this->registerWidget(PRIVATE(this)->viewerbase);

  char * titleString = NULL;
  SoXtResource rsc(PRIVATE(this)->viewerbase);
  rsc.getResource("title", XmRString, titleString);
  if (titleString != NULL)
    this->setTitle(titleString);

  PRIVATE(this)->canvas = inherited::buildWidget(PRIVATE(this)->viewerbase);
  XtVaSetValues(PRIVATE(this)->canvas,
                XmNleftAttachment, XmATTACH_FORM,
                XmNleftOffset, 30,
                XmNtopAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_FORM,
                XmNrightOffset, 30,
                XmNbottomAttachment, XmATTACH_FORM,
                XmNbottomOffset, 30,
                NULL);

  this->buildDecoration(PRIVATE(this)->viewerbase);
  
  if (this->isTopLevelShell() &&
      PRIVATE(this)->decorations != FALSE) {
    Widget shell = this->getShellWidget();
    if (shell != NULL) {
      Dimension minheight =
        30 + 122 + 30 * this->viewerButtonWidgets->getLength() + 8;
      Dimension width, height;
      XtVaGetValues(shell,
                    XmNwidth, &width,
                    XmNheight, &height,
                    NULL);
      width = SoXtMax(width, (Dimension) 300);
      height = SoXtMax(height, minheight);
      XtVaSetValues(shell,
                    XmNminWidth, 300,
                    XmNminHeight, minheight,
                    XmNwidth, width,
                    XmNheight, height,
                    NULL);
    }
  }
  return PRIVATE(this)->viewerbase;
}

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::buildDecoration(Widget parent)
{
  this->leftDecoration = this->buildLeftTrim(parent);
  XtVaSetValues(this->leftDecoration,
                XmNleftAttachment, XmATTACH_FORM,
                XmNtopAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_OPPOSITE_FORM,
                XmNrightOffset, -30,
                XmNbottomAttachment, XmATTACH_FORM,
                XmNbottomOffset, 30,
                NULL);

  this->rightDecoration = this->buildRightTrim(parent);
  XtVaSetValues(this->rightDecoration,
                XmNleftAttachment, XmATTACH_OPPOSITE_FORM,
                XmNleftOffset, -30,
                XmNtopAttachment, XmATTACH_FORM,
                XmNrightAttachment, XmATTACH_FORM,
                XmNbottomAttachment, XmATTACH_FORM,
                XmNbottomOffset, 30,
                NULL);

  this->bottomDecoration = this->buildBottomTrim(parent);
  XtVaSetValues(this->bottomDecoration,
                XmNleftAttachment, XmATTACH_FORM,
                XmNtopAttachment, XmATTACH_OPPOSITE_FORM,
                XmNtopOffset, -30,
                XmNrightAttachment, XmATTACH_FORM,
                XmNbottomAttachment, XmATTACH_FORM,
                NULL);
}

// *************************************************************************

/*!
  This method creates the left decoration form.

  Return value is a Motif widget of the xmFormWidgetClass type.
  The left decoration is 30 pixels wide.
*/

Widget
SoXtFullViewer::buildLeftTrim(Widget parent)
{
  Widget trim = XtVaCreateManagedWidget("LeftTrim",
      xmFormWidgetClass, parent,
      NULL);

#if 0
  // build application buttons
  this->appButtonsForm = this->buildAppButtonsForm(trim);
  XtVaSetValues(this->appButtonsForm,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    NULL);
#endif

  // add left thumb wheel
  this->leftWheel = XtVaCreateManagedWidget("LeftWheel",
    soxtThumbWheelWidgetClass, trim,
    XmNleftAttachment, XmATTACH_FORM,
    XmNleftOffset, 2,
    XmNrightAttachment, XmATTACH_FORM,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNbottomOffset, 2,
    XmNheight, 122,
    XmNorientation, XmVERTICAL,
    XmNshadowType, XmSHADOW_OUT,
    XmNhighlightThickness, 0,
    XmNshadowThickness, 2,
    XmNtraversalOn, False,
    NULL);

  XtAddCallback(this->leftWheel,
                XmNarmCallback, SoXtFullViewerP::leftWheelStartCB, this);
  XtAddCallback(this->leftWheel,
                XmNdisarmCallback, SoXtFullViewerP::leftWheelFinishCB, this);
  XtAddCallback(this->leftWheel,
                XmNvalueChangedCallback, SoXtFullViewerP::leftWheelMotionCB, this);

  return trim;
}

// *************************************************************************

/*!
  This method creates the right decoration form.

  Returned value is a Motif widget of the xmFormWidgetClass type.
  The form is 30 pixels wide.
*/

Widget
SoXtFullViewer::buildRightTrim(Widget parent)
{
  Widget trim = XtVaCreateManagedWidget("RightTrim",
    xmFormWidgetClass, parent,
    NULL);

  Widget buttons = this->buildViewerButtons(trim);

  XtVaSetValues(buttons,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_OPPOSITE_FORM,
    XmNbottomOffset, 0 - this->viewerButtonWidgets->getLength() * 30,
    NULL);

  // add right thumb wheel
  this->rightWheel = XtVaCreateManagedWidget("RightWheel",
    soxtThumbWheelWidgetClass, trim,
    XmNleftAttachment, XmATTACH_FORM,
    XmNleftOffset, 2,
    XmNrightAttachment, XmATTACH_FORM,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNbottomOffset, 2,
    XmNheight, 122,
    XmNorientation, XmVERTICAL,
    XmNshadowType, XmSHADOW_OUT,
    XmNhighlightThickness, 0,
    XmNshadowThickness, 2,
    XmNtraversalOn, False,
    NULL);

  XtAddCallback(this->rightWheel,
                XmNarmCallback, SoXtFullViewerP::rightWheelStartCB, this);
  XtAddCallback(this->rightWheel,
                XmNdisarmCallback, SoXtFullViewerP::rightWheelFinishCB, this);
  XtAddCallback(this->rightWheel,
                XmNvalueChangedCallback, SoXtFullViewerP::rightWheelMotionCB, this);

  return trim;
}

// *************************************************************************

/*!
  This method created the bottom decoration form.

  Return value is a Motif widget of the xmFormWidgetClass type.
  It is 30 pixels high.
*/

Widget
SoXtFullViewer::buildBottomTrim(Widget parent)
{
  Widget trim = XtVaCreateManagedWidget("BottomTrim",
                                        xmFormWidgetClass, parent,
                                        NULL);

  this->leftWheelLabel = XtVaCreateManagedWidget("LeftWheelLabel",
                                                 xmLabelWidgetClass, trim,
                                                 XmNleftAttachment, XmATTACH_FORM,
                                                 XmNtopAttachment, XmATTACH_FORM,
                                                 XmNbottomAttachment, XmATTACH_FORM,
                                                 XmNleftOffset, 5,
                                                 XtVaTypedArg,
                                                 XmNlabelString, XmRString,
                                                 this->leftWheelStr, strlen(this->leftWheelStr)+1,
                                                 NULL);

  this->bottomWheelLabel = XtVaCreateManagedWidget("BottomWheelLabel",
                                                   xmLabelWidgetClass, trim,
                                                   XmNleftAttachment, XmATTACH_WIDGET,
                                                   XmNleftWidget, this->leftWheelLabel,
                                                   XmNtopAttachment, XmATTACH_FORM,
                                                   XmNbottomAttachment, XmATTACH_FORM,
                                                   XmNleftOffset, 5,
                                                   XtVaTypedArg,
                                                   XmNlabelString, XmRString,
                                                   this->bottomWheelStr, strlen(this->bottomWheelStr)+1,
                                                   NULL);

  // add bottom thumb wheel
  this->bottomWheel = XtVaCreateManagedWidget("BottomWheel",
                                              soxtThumbWheelWidgetClass, trim,
                                              XmNorientation, XmHORIZONTAL,
                                              XmNshadowType, XmSHADOW_OUT,
                                              XmNhighlightThickness, 0,
                                              XmNshadowThickness, 2,
                                              XmNtraversalOn, False,
                                              XmNleftAttachment, XmATTACH_WIDGET,
                                              XmNleftWidget, this->bottomWheelLabel,
                                              XmNleftOffset, 2,
                                              XmNtopAttachment, XmATTACH_FORM,
                                              XmNtopOffset, 2,
                                              XmNbottomAttachment, XmATTACH_FORM,
                                              XmNbottomOffset, 2,
                                              XmNwidth, 122,
                                              NULL);

  XtAddCallback(this->bottomWheel,
                XmNarmCallback, SoXtFullViewerP::bottomWheelStartCB, this);
  XtAddCallback(this->bottomWheel,
                XmNdisarmCallback, SoXtFullViewerP::bottomWheelFinishCB, this);
  XtAddCallback(this->bottomWheel,
                XmNvalueChangedCallback, SoXtFullViewerP::bottomWheelMotionCB, this);

  this->rightWheelLabel = XtVaCreateManagedWidget("RightWheelLabel",
                                                  xmLabelWidgetClass, trim,
                                                  XmNleftAttachment, XmATTACH_WIDGET,
                                                  XmNleftWidget, this->bottomWheel,
                                                  XmNalignment, XmALIGNMENT_END,
                                                  XmNtopAttachment, XmATTACH_OPPOSITE_FORM,
                                                  XmNtopOffset, -30,
                                                  XmNbottomAttachment, XmATTACH_FORM,
                                                  XmNrightAttachment, XmATTACH_FORM,
                                                  XmNrightOffset, 5,
                                                  XtVaTypedArg,
                                                  XmNlabelString, XmRString,
                                                  this->rightWheelStr, strlen(this->rightWheelStr)+1,
                                                  NULL);

  return trim;
}

// *************************************************************************

// Documented in superclass.
void
SoXtFullViewer::setViewing(SbBool enable)
{
  if ((this->isViewing() && enable) || (!this->isViewing() && !enable)) {
#if SOXT_DEBUG
    SoDebugError::postWarning("SoXtFullViewer::setViewing",
                              "current state already %s", enable ? "TRUE" : "FALSE");
#endif // SOXT_DEBUG
    return;
  }

  inherited::setViewing(enable);

  if (PRIVATE(this)->viewerbuttons.pick != 0) {
    XtSetSensitive(PRIVATE(this)->viewerbuttons.pick, enable ? True : False);
    XtVaSetValues(PRIVATE(this)->viewerbuttons.pick,
                  XmNset, enable ? False : True,
                  NULL);
  }

  if (PRIVATE(this)->viewerbuttons.view != 0) {
    XtSetSensitive(PRIVATE(this)->viewerbuttons.view, enable ? False : True);
    XtVaSetValues(PRIVATE(this)->viewerbuttons.view,
                  XmNset, enable ? True : False,
                  NULL);
  }

  if (PRIVATE(this)->viewerbuttons.seek != 0) {
    XtSetSensitive(PRIVATE(this)->viewerbuttons.seek, enable ? True : False);
  }
}

// Documented in superclass.
void
SoXtFullViewer::setCamera(SoCamera * camera)
{
  inherited::setCamera(camera);

  if (this->prefshell != NULL) {
    Boolean enable = False;
    if (camera) {
      SoType camtype = camera->getTypeId();
      if (camtype.isDerivedFrom(SoPerspectiveCamera::getClassTypeId()))
        enable = True;
    }
    XtVaSetValues(PRIVATE(this)->zoomfrom,
                  XmNsensitive, enable,
                  XmNeditable, enable,
                  XmNcursorPositionVisible, enable,
                  NULL);
    XtVaSetValues(PRIVATE(this)->zoomslider,
                  XmNsensitive, enable,
                  XmNeditable, enable,
                  NULL);
    XtVaSetValues(PRIVATE(this)->zoomto,
                  XmNsensitive, enable,
                  XmNeditable, enable,
                  XmNcursorPositionVisible, enable,
                  NULL);
    XtVaSetValues(PRIVATE(this)->zoomvalue,
                  XmNsensitive, enable,
                  XmNeditable, enable,
                  XmNcursorPositionVisible, enable,
                  NULL);
  }
}

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::hide(void)
{
  inherited::hide();
//  if (this->prefwindow)
//    SoXt::hide(this->prefwindow);
}

// *************************************************************************

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::buildViewerButtons(Widget parent)
{
  Widget buttons = XtVaCreateWidget("ViewerButtons",
                                    xmRowColumnWidgetClass, parent,
                                    XmNrowColumnType, XmWORK_AREA,
                                    XmNpacking, XmPACK_COLUMN,
                                    XmNnumColumns, 1,
                                    XmNspacing, 0,
                                    XmNmarginWidth, 0,
                                    XmNmarginHeight, 0,
                                    NULL);

  this->createViewerButtons(buttons, this->viewerButtonWidgets);

  XtSetSensitive(PRIVATE(this)->viewerbuttons.pick, this->isViewing() ? True : False);
  XtVaSetValues(PRIVATE(this)->viewerbuttons.pick,
                XmNset, this->isViewing() ? False : True,
                NULL);

  XtSetSensitive(PRIVATE(this)->viewerbuttons.view, this->isViewing() ? False : True);
  XtVaSetValues(PRIVATE(this)->viewerbuttons.view,
                XmNset, this->isViewing() ? True : False,
                NULL);

  const int numbuttons = this->viewerButtonWidgets->getLength();
  for (int i = 0; i < numbuttons; i++) {
    Widget button = (Widget) (*this->viewerButtonWidgets)[i];
    XtVaSetValues(button,
                  XmNshadowType, XmSHADOW_OUT,
                  XmNhighlightThickness, 0,
                  XmNshadowThickness, 2,
                  XmNtraversalOn, False,
                  XmNmarginWidth, 0,
                  XmNmarginHeight, 0,
                  XmNmarginLeft, 0,
                  XmNmarginTop, 0,
                  XmNmarginRight, 0,
                  XmNmarginBottom, 0,
                  XmNrecomputeSize, False,
                  XmNwidth, 28,
                  XmNheight, 28,
                  NULL);
  }

/*
  XtVaSetValues(buttons,
    XmNwidth, 30,
    XmNheight, numbuttons * 31,
    NULL);
*/

  XtManageChild(buttons);
  return buttons;
}

// *************************************************************************

// Documented in common/viewers/SoGuiFullViewer.cpp.in.
void
SoXtFullViewer::createViewerButtons(Widget parent,
                                    SbPList * buttonlist)
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtFullViewer::createViewerButtons", "[enter]");
#endif // SOXT_DEBUG

  int viewerbutton;
  for (viewerbutton = FIRST_BUTTON; viewerbutton <= LAST_BUTTON; viewerbutton++) {

    XtCallbackProc proc = NULL;
    char label[2];
    label[1] = '\0';
    switch (viewerbutton) {
    case INTERACT_BUTTON:
      proc = SoXtFullViewerP::interactbuttonCB;
      label[0] = 'I';
      break;
    case EXAMINE_BUTTON:
      proc = SoXtFullViewerP::examinebuttonCB;
      label[0] = 'E';
      break;
    case HELP_BUTTON:
      proc = SoXtFullViewerP::helpbuttonCB;
      label[0] = '?';
      break;
    case HOME_BUTTON:
      proc = SoXtFullViewerP::homebuttonCB;
      label[0] = 'H';
      break;
    case SET_HOME_BUTTON:
      proc = SoXtFullViewerP::sethomebuttonCB;
      label[0] = 'N';
      break;
    case VIEW_ALL_BUTTON:
      proc = SoXtFullViewerP::viewallbuttonCB;
      label[0] = 'V';
      break;
    case SEEK_BUTTON:
      proc = SoXtFullViewerP::seekbuttonCB;
      label[0] = 'S';
      break;
    default:
      assert(0);
      break;
    }
    Widget button;
    if (viewerbutton == EXAMINE_BUTTON || viewerbutton == INTERACT_BUTTON) {
      button = XtVaCreateManagedWidget(label,
                                       xmToggleButtonWidgetClass, parent,
                                       XmNindicatorOn, False,
                                       NULL);
      if (viewerbutton == EXAMINE_BUTTON) {
        XtVaSetValues(button, XmNset, this->isViewing() ? True : False, NULL);
      }
      if (viewerbutton == INTERACT_BUTTON) {
        XtVaSetValues(button, XmNset, this->isViewing() ? False : True, NULL);
      }
    } else {
      button = XtVaCreateManagedWidget(label,
                                       xmPushButtonWidgetClass, parent,
                                       NULL);
    }

    switch (viewerbutton) {
    case INTERACT_BUTTON:
      PRIVATE(this)->viewerbuttons.pick = button;
      break;
    case EXAMINE_BUTTON:
      PRIVATE(this)->viewerbuttons.view = button;
      break;
    case HELP_BUTTON:
      PRIVATE(this)->viewerbuttons.help = button;
      break;
    case HOME_BUTTON:
      PRIVATE(this)->viewerbuttons.home = button;
      break;
    case SET_HOME_BUTTON:
      PRIVATE(this)->viewerbuttons.set_home = button;
      break;
    case VIEW_ALL_BUTTON:
      PRIVATE(this)->viewerbuttons.view_all = button;
      break;
    case SEEK_BUTTON:
      PRIVATE(this)->viewerbuttons.seek = button;
      break;
    default:
      assert(0 && "impossible");
      break;
    }
#if HAVE_LIBXPM
    Pixmap pixmap, pixmap_ins;
    switch (viewerbutton) {
    case INTERACT_BUTTON:
      pixmap = PRIVATE(this)->pixmaps.pick =
        SoXtInternal::createPixmapFromXpm(button, pick_xpm);
      pixmap_ins = PRIVATE(this)->pixmaps.pick_ins =
        SoXtInternal::createPixmapFromXpm(button, pick_xpm, TRUE);
      break;
    case EXAMINE_BUTTON:
      pixmap = PRIVATE(this)->pixmaps.view =
        SoXtInternal::createPixmapFromXpm(button, view_xpm);
      pixmap_ins = PRIVATE(this)->pixmaps.view_ins =
        SoXtInternal::createPixmapFromXpm(button, view_xpm, TRUE);
      break;
    case HELP_BUTTON:
      pixmap = pixmap_ins = PRIVATE(this)->pixmaps.help =
        SoXtInternal::createPixmapFromXpm(button, help_xpm);
      break;
    case HOME_BUTTON:
      pixmap = pixmap_ins = PRIVATE(this)->pixmaps.home =
        SoXtInternal::createPixmapFromXpm(button, home_xpm);
      break;
    case SET_HOME_BUTTON:
      pixmap = pixmap_ins = PRIVATE(this)->pixmaps.set_home =
        SoXtInternal::createPixmapFromXpm(button, set_home_xpm);
      break;
    case VIEW_ALL_BUTTON:
      pixmap = pixmap_ins = PRIVATE(this)->pixmaps.view_all =
        SoXtInternal::createPixmapFromXpm(button, view_all_xpm);
      break;
    case SEEK_BUTTON:
      pixmap = PRIVATE(this)->pixmaps.seek =
        SoXtInternal::createPixmapFromXpm(button, seek_xpm);
      pixmap_ins = PRIVATE(this)->pixmaps.seek_ins =
        SoXtInternal::createPixmapFromXpm(button, seek_xpm, TRUE);
      break;
    default:
      assert(0 && "impossible");
      break;
    }
    if (pixmap && pixmap_ins) {
      XtVaSetValues(button,
                    XmNlabelType, XmPIXMAP,
                    XmNlabelPixmap, pixmap,
                    XmNlabelInsensitivePixmap, pixmap_ins,
                    XmNselectPixmap, pixmap,
                    XmNselectInsensitivePixmap, pixmap_ins,
                    NULL);
    }
#endif // HAVE_LIBXPM

    if (proc != NULL) {
      if (viewerbutton == INTERACT_BUTTON || viewerbutton == EXAMINE_BUTTON) {
        XtAddCallback(button, XmNdisarmCallback, proc, this);
      } else {
        XtAddCallback(button, XmNactivateCallback, proc, this);
      }
    }
    if (buttonlist) buttonlist->append(button);
  }
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtFullViewer::createViewerButtons", "[exit]");
#endif // SOXT_DEBUG
}

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::buildPopupMenu(void)
{
  if (this->prefmenu == NULL)
    this->prefmenu = PRIVATE(this)->setupStandardPopupMenu();
}

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::buildFunctionsSubmenu(Widget popup)
{
  SOXT_STUB();
  return (Widget) NULL;
}

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::buildDrawStyleSubmenu(Widget popup)
{
  SOXT_STUB();
  return (Widget) NULL;
}

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::openPopupMenu(const SbVec2s position)
{
  if (! this->isPopupMenuEnabled())
    return;
  if (this->prefmenu == NULL)
    this->buildPopupMenu();
  assert(this->prefmenu != NULL);
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtFullViewer::openPopupMenu", "[invoked]");
#endif // SOXT_DEBUG && 0

  int x = position[0] + 2;
  int y = this->getGLSize()[1] - position[1] + 2;

  PRIVATE(this)->prepareMenu(this->prefmenu);
  this->prefmenu->popUp(this->getGLWidget(), x, y);
}

// *************************************************************************

void
SoXtFullViewerP::leftWheelStartCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->leftWheelStart();
}

void
SoXtFullViewerP::leftWheelMotionCB(Widget, XtPointer closure, XtPointer call_data)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  SoXtThumbWheelCallbackData * data = (SoXtThumbWheelCallbackData *) call_data;
  viewer->leftWheelMotion(data->current);
}

void
SoXtFullViewerP::leftWheelFinishCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->leftWheelFinish();
}

void
SoXtFullViewerP::bottomWheelStartCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->bottomWheelStart();
}

void
SoXtFullViewerP::bottomWheelMotionCB(Widget, XtPointer closure, XtPointer call_data)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  SoXtThumbWheelCallbackData * data = (SoXtThumbWheelCallbackData *) call_data;
  viewer->bottomWheelMotion(data->current);
}

void
SoXtFullViewerP::bottomWheelFinishCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->bottomWheelFinish();
}

void
SoXtFullViewerP::rightWheelStartCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->rightWheelStart();
}

void
SoXtFullViewerP::rightWheelMotionCB(Widget, XtPointer closure, XtPointer call_data)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  SoXtThumbWheelCallbackData * data = (SoXtThumbWheelCallbackData *) call_data;
  viewer->rightWheelMotion(data->current);
}

void
SoXtFullViewerP::rightWheelFinishCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->rightWheelFinish();
}

// *************************************************************************

/*!
  This method sets the label text displayed below the thumb wheel on the
  left decoration.
*/

void
SoXtFullViewer::setLeftWheelString(const char * const string)
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtFullViewer::setLeftWheelString",
                         "setting left wheel label");
#endif // SOXT_DEBUG
  delete [] this->leftWheelStr;
  this->leftWheelStr = strcpy(new char [strlen(string)+1], string);
  if (this->leftWheelLabel != NULL)
    XtVaSetValues(this->leftWheelLabel,
      XtVaTypedArg,
        XmNlabelString, XmRString,
        this->leftWheelStr, strlen(this->leftWheelStr) + 1,
      NULL);
}

/*!
  This method sets the label text that is displayed to the left og the
  bottom decoration thumb wheel.
*/

void
SoXtFullViewer::setBottomWheelString(const char * const string)
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtFullViewer::setBottomWheelString",
                         "setting bottom wheel label");
#endif // SOXT_DEBUG
  delete [] this->bottomWheelStr;
  this->bottomWheelStr = strcpy(new char [strlen(string)+1], string);
  if (this->bottomWheelLabel != NULL)
    XtVaSetValues(this->bottomWheelLabel,
      XtVaTypedArg,
        XmNlabelString, XmRString,
        this->bottomWheelStr, strlen(this->bottomWheelStr) + 1,
      NULL);
}

/*!
  This method sets the label text that is displayed under the right decoration
  thumb wheel.
*/

void
SoXtFullViewer::setRightWheelString(const char * const string)
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtFullViewer::setRightWheelString", "[enter]");
#endif // SOXT_DEBUG

  delete [] this->rightWheelStr;
  this->rightWheelStr = strcpy(new char [strlen(string)+1], string);

  if (this->rightWheelLabel != NULL)
    XtVaSetValues(this->rightWheelLabel,
      XtVaTypedArg,
        XmNlabelString, XmRString,
        this->rightWheelStr, strlen(this->rightWheelStr) + 1,
      NULL);

#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtFullViewer::setRightWheelString", "[exit]");
#endif // SOXT_DEBUG
}

// *************************************************************************

/*!
  Not implemented.
*/

Widget
SoXtFullViewer::buildAppButtonsForm(Widget parent)
{
  Widget form = XtVaCreateManagedWidget("appbuttons",
                                        xmFormWidgetClass, parent,
                                        NULL);

  SOXT_STUB();
  return form;
}

// *************************************************************************

// This method shows or hides the decoration widgets, depending on the
// \a enable argument.
void
SoXtFullViewerP::showDecorationWidgets(SbBool enable)
{
  if (!this->canvas) return;
  assert(PUBLIC(this)->leftDecoration != (Widget) NULL);
  assert(PUBLIC(this)->rightDecoration != (Widget) NULL);
  assert(PUBLIC(this)->bottomDecoration != (Widget) NULL);

  if (enable) {
    XtVaSetValues(this->canvas,
                  XmNleftOffset, 30,
                  XmNrightOffset, 30,
                  XmNbottomOffset, 30,
                  NULL);
    if (XtWindow(PUBLIC(this)->leftDecoration) != 0) {
      XtMapWidget(PUBLIC(this)->leftDecoration);
      XtManageChild(PUBLIC(this)->leftDecoration);
      XtMapWidget(PUBLIC(this)->rightDecoration);
      XtManageChild(PUBLIC(this)->rightDecoration);
      XtMapWidget(PUBLIC(this)->bottomDecoration);
      XtManageChild(PUBLIC(this)->bottomDecoration);
    }
  } else {
    if (XtWindow(PUBLIC(this)->leftDecoration) != 0) {
      XtUnmapWidget(PUBLIC(this)->leftDecoration);
      XtUnmapWidget(PUBLIC(this)->rightDecoration);
      XtUnmapWidget(PUBLIC(this)->bottomDecoration);
    }
    XtVaSetValues(this->canvas,
                  XmNleftOffset, 0,
                  XmNrightOffset, 0,
                  XmNbottomOffset, 0,
                  NULL);
  }
}

// *************************************************************************

/*!
  This method sets the title text for the viewer preferences window.
*/

void
SoXtFullViewer::setPrefSheetString(const char * name)
{
  if (this->prefstring)
    delete [] this->prefstring;
  this->prefstring = NULL;
  if (name)
    this->prefstring = strcpy(new char [strlen(name) + 1], name);

  if (this->prefshell)
    XtVaSetValues(this->prefshell, XtNtitle, this->prefstring, NULL);
}

// Documented in common/viewers/SoGuiFullViewer.cpp.in.
void
SoXtFullViewer::createPrefSheet(void)
{
}

/*!
  This method creates the preferences sheet shell.
*/

void
SoXtFullViewer::createPrefSheetShellAndForm(Widget & shell,
                                            Widget & form)
{
  assert(this->prefshell == NULL);

  if (! this->prefstring)
    this->setPrefSheetString("Xt Viewer Preferences");

  Widget pshell = this->getBaseWidget();
  while (pshell && ! XtIsShell(pshell))
    pshell = XtParent(pshell);
  assert(pshell != NULL);

  Visual * visual = NULL;
  int depth = 0;
  Colormap colormap = 0;

  XtVaGetValues(pshell,
    XmNvisual, &visual,
    XmNdepth, &depth,
    XmNcolormap, &colormap,
    NULL);

  shell = XtVaAppCreateShell(NULL, "SoXt",
    topLevelShellWidgetClass, SoXt::getDisplay(),
    XmNvisual, visual,
    XmNdepth, depth,
    XmNcolormap, colormap,
    XmNtitle, this->prefstring,
    NULL);

  XtAddCallback(shell, XmNdestroyCallback,
                SoXtFullViewerP::prefSheetDestroyCB, (XtPointer) this);
  this->prefshell = shell;

  form = XtVaCreateWidget("form", xmFormWidgetClass, shell, NULL);

  this->prefsheet = form;
}

/*!
  This method creates all the preference sheel parts.
*/

void
SoXtFullViewer::createDefaultPrefSheetParts(Widget * widgets,
                                            int & num,
                                            Widget form)
{
  assert(widgets != NULL);
  num = 0;

  if ((widgets[num] = this->createFramedSeekPrefSheetGuts(form)))
    num++;
  if ((widgets[num] = this->createFramedSeekDistPrefSheetGuts(form)))
    num++;
  if ((widgets[num] = this->createFramedZoomPrefSheetGuts(form)))
    num++;
// FIXME: this barfs on 8-bit displays...
  if ((widgets[num] = this->createFramedClippingPrefSheetGuts(form)))
    num++;
  if ((widgets[num] = this->createFramedStereoPrefSheetGuts(form)))
    num++;
  // NOTE: SpeedPrefs is set up on SoXtFullViewer level, but is only used
  // by WalkViewer and FlyViewer, so it is not included as default...
}

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::layoutPartsAndMapPrefSheet(Widget * widgets,
                                           int num,
                                           Widget form,
                                           Widget shell)
{
  assert(widgets != NULL);
  for (int i = 0; i < num; i++) {
    int attachment_top, attachment_bottom;
    if (i == 0) {
      attachment_top = XmATTACH_FORM;
      attachment_bottom = XmATTACH_NONE;
    } else if (i == (num - 1)) {
      attachment_top = XmATTACH_WIDGET;
      attachment_bottom = XmATTACH_NONE; // FORM;
    } else {
      attachment_top = XmATTACH_WIDGET;
      attachment_bottom = XmATTACH_NONE;
    }
    XtVaSetValues(widgets[i],
      XmNtopAttachment, attachment_top,
      XmNleftAttachment, XmATTACH_FORM,
      XmNrightAttachment, XmATTACH_FORM,
      XmNbottomAttachment, attachment_bottom,
      NULL);
    if (attachment_top == XmATTACH_WIDGET)
      XtVaSetValues(widgets[i], XmNtopWidget, widgets[i-1], NULL);
  }
  XtManageChild(form);
  XtPopup(shell, XtGrabNone);
}
 
// *************************************************************************

/*!
  This method is a wrapper for the createSeekPrefSheetGuts() and
  createSeekDistPrefSheetGuts() methods, to place them both inside
  one frame.
*/

Widget
SoXtFullViewer::createFramedSeekPrefSheetGuts(Widget parent)
{
  Widget frame = XtVaCreateManagedWidget("seekframe",
    xmFrameWidgetClass, parent,
    NULL);

  XtVaCreateManagedWidget("title",
    xmLabelGadgetClass, frame,
    XmNchildType, XmFRAME_TITLE_CHILD,
    XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Seek Settings", strlen("Seek Settings") + 1,
    NULL);

  Widget form = XtVaCreateManagedWidget("form",
    xmFormWidgetClass, frame,
    NULL);

  this->createSeekPrefSheetGuts(form);

  return frame;
}

/*!
  This method is a wrapper for the createSeekDistPrefSheetGuts() method,
  to wrap a frame around the widgets.
*/

Widget
SoXtFullViewer::createFramedSeekDistPrefSheetGuts(Widget parent)
{
  Widget frame = XtVaCreateManagedWidget("seekdistframe",
    xmFrameWidgetClass, parent,
    NULL);
  XtVaCreateManagedWidget("seekdisttitle",
    xmLabelGadgetClass, frame,
    XmNchildType, XmFRAME_TITLE_CHILD,
    XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Seek Distance Settings", strlen("Seek Distance Settings") + 1,
    NULL);

  Widget form = XtVaCreateManagedWidget("form",
    xmFormWidgetClass, frame,
    NULL);

  this->createSeekDistPrefSheetGuts(form);

  return frame;
}

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::createFramedZoomPrefSheetGuts(Widget parent)
{
  Widget frame = XtVaCreateManagedWidget("zoomframe",
    xmFrameWidgetClass, parent,
    NULL);

  XtVaCreateManagedWidget("title",
    xmLabelGadgetClass, frame,
    XmNchildType, XmFRAME_TITLE_CHILD,
    XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Zoom Settings", strlen("Zoom Settings") + 1,
    NULL);

  this->createZoomPrefSheetGuts(frame);

  return frame;
}

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::createFramedClippingPrefSheetGuts(Widget parent)
{
  Widget frame = XtVaCreateManagedWidget("clippingframe",
    xmFrameWidgetClass, parent,
    NULL);

  XtVaCreateManagedWidget("title",
    xmLabelGadgetClass, frame,
    XmNchildType, XmFRAME_TITLE_CHILD,
    XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Clipping Plane Settings", strlen("Clipping Plane Settings") + 1,
    NULL);

  this->createClippingPrefSheetGuts(frame);

  return frame;
}

// *************************************************************************

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::createFramedStereoPrefSheetGuts(Widget parent)
{
  Widget frame = XtVaCreateManagedWidget("stereoframe",
    xmFrameWidgetClass, parent, NULL);

  Widget label = XtVaCreateManagedWidget("stereoframelabel",
    xmLabelGadgetClass, frame,
    XmNchildType, XmFRAME_TITLE_CHILD,
    XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Stereo Viewing Settings", strlen("Stereo Viewing Settings") + 1,
    NULL);

  this->createStereoPrefSheetGuts(frame);

  return frame;
}

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::createFramedSpeedPrefSheetGuts(Widget parent)
{
  Widget frame = XtVaCreateManagedWidget("speedframe",
    xmFrameWidgetClass, parent, NULL);

  Widget label = XtVaCreateManagedWidget("speedframelabel",
    xmLabelGadgetClass, frame,
    XmNchildType, XmFRAME_TITLE_CHILD,
    XmNchildVerticalAlignment, XmALIGNMENT_CENTER,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Speed Settings", strlen("Speed Settings") + 1,
    NULL);

  this->createSpeedPrefSheetGuts(frame);

  return frame;
}

// *************************************************************************

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::createSeekPrefSheetGuts(Widget parent)
{
  //  Seek animation time  |NNN|  seconds
  //  Seek to:   <> point   <> object

  Widget form = XtVaCreateManagedWidget("seekprefs",
                                        xmFormWidgetClass, parent, NULL);

  Widget line1 = XtVaCreateManagedWidget("line 1",
                                         xmFormWidgetClass, form,
                                         XmNtopAttachment, XmATTACH_FORM,
                                         XmNleftAttachment, XmATTACH_FORM,
                                         XmNrightAttachment, XmATTACH_FORM,
                                         NULL);

  Widget label = XtVaCreateManagedWidget("seektime",
                                         xmLabelWidgetClass, line1,
                                         XmNtopAttachment, XmATTACH_FORM,
                                         XmNleftAttachment, XmATTACH_FORM,
                                         XmNbottomAttachment, XmATTACH_FORM,
                                         XtVaTypedArg,
                                         XmNlabelString, XmRString,
                                         "Seek animation time: ", strlen("Seek animation time: ") + 1, 
                                         NULL);

  PRIVATE(this)->seektimefield = XtVaCreateManagedWidget("seektimeinput",
                                                         xmTextFieldWidgetClass, line1,
                                                         XmNtopAttachment, XmATTACH_FORM,
                                                         XmNleftAttachment, XmATTACH_WIDGET,
                                                         XmNleftWidget, label,
                                                         XmNbottomAttachment, XmATTACH_FORM,
                                                         XmNwidth, 60,
                                                         NULL);

  XtAddCallback(PRIVATE(this)->seektimefield, XmNactivateCallback,
                SoXtFullViewerP::seektimechangedCB, (XtPointer) this);
  XtAddCallback(PRIVATE(this)->seektimefield, XmNlosingFocusCallback,
                SoXtFullViewerP::seektimechangedCB, (XtPointer) this);
  char buffer[16];
  sprintf(buffer, "%g", this->getSeekTime());
  XmTextSetString(PRIVATE(this)->seektimefield, buffer);
  XmTextSetCursorPosition(PRIVATE(this)->seektimefield, (long) strlen(buffer));

  Widget fieldlabed = XtVaCreateManagedWidget("seconds",
                                              xmLabelWidgetClass, line1,
                                              XmNtopAttachment, XmATTACH_FORM,
                                              XmNleftAttachment, XmATTACH_WIDGET,
                                              XmNleftWidget, PRIVATE(this)->seektimefield,
                                              XmNbottomAttachment, XmATTACH_FORM,
                                              XmNbottomWidget, PRIVATE(this)->seektimefield,
                                              NULL);

  Widget line2 = XtVaCreateManagedWidget("line 2",
                                         xmFormWidgetClass, form,
                                         XmNleftAttachment, XmATTACH_FORM,
                                         XmNrightAttachment, XmATTACH_FORM,
                                         XmNtopAttachment, XmATTACH_WIDGET,
                                         XmNtopWidget, line1,
                                         XmNbottomAttachment, XmATTACH_FORM,
                                         NULL);

  Widget tolabel = XtVaCreateManagedWidget("tolabel",
                                           xmLabelWidgetClass, line2,
                                           XmNleftAttachment, XmATTACH_FORM,
                                           XmNtopAttachment, XmATTACH_FORM,
                                           XmNbottomAttachment, XmATTACH_FORM,
                                           XtVaTypedArg,
                                           XmNlabelString, XmRString,
                                           "Seek to: ", strlen("Seek to: ") + 1,
                                           NULL);

  PRIVATE(this)->pointtoggle = XtVaCreateManagedWidget("point",
                                                       xmToggleButtonWidgetClass, line2,
                                                       XmNindicatorType, XmONE_OF_MANY,
                                                       XmNleftAttachment, XmATTACH_WIDGET,
                                                       XmNleftWidget, tolabel,
                                                       XmNtopAttachment, XmATTACH_FORM,
                                                       XmNbottomAttachment, XmATTACH_FORM,
                                                       XmNset, this->isDetailSeek() ? True : False,
                                                       NULL);

  XtAddCallback(PRIVATE(this)->pointtoggle, XmNvalueChangedCallback,
                SoXtFullViewerP::pointtoggledCB, (XtPointer) this);

  PRIVATE(this)->objecttoggle = XtVaCreateManagedWidget("object",
                                                        xmToggleButtonWidgetClass, line2,
                                                        XmNindicatorType, XmONE_OF_MANY,
                                                        XmNleftAttachment, XmATTACH_WIDGET,
                                                        XmNleftWidget, PRIVATE(this)->pointtoggle,
                                                        XmNtopAttachment, XmATTACH_FORM,
                                                        XmNbottomAttachment, XmATTACH_FORM,
                                                        XmNset, this->isDetailSeek() ? False : True,
                                                        NULL);

  XtAddCallback(PRIVATE(this)->objecttoggle, XmNvalueChangedCallback,
                SoXtFullViewerP::objecttoggledCB, (XtPointer) this);

  return form;
}

// *************************************************************************

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::createSeekDistPrefSheetGuts(Widget parent)
{
  //   Seek distance:   |||||||||||||   |NN.NNN|
  //      <> percentage   <> absolute

  Widget form = XtVaCreateManagedWidget("seekdistprefs",
                                        xmFormWidgetClass, parent, NULL);

  Widget line1 = XtVaCreateManagedWidget("seekdistprefs",
                                         xmFormWidgetClass, form,
                                         XmNleftAttachment, XmATTACH_FORM,
                                         XmNrightAttachment, XmATTACH_FORM,
                                         XmNtopAttachment, XmATTACH_FORM,
                                         NULL);

  Widget label = XtVaCreateManagedWidget("seekdistlabel",
                                         xmLabelWidgetClass, line1,
                                         XmNtopAttachment, XmATTACH_FORM,
                                         XmNleftAttachment, XmATTACH_FORM,
                                         XmNbottomAttachment, XmATTACH_FORM,
                                         XtVaTypedArg,
                                         XmNlabelString, XmRString,
                                         "Seek distance: ", strlen("Seek distance: ") + 1,
                                         NULL);

  Widget wheel = XtVaCreateManagedWidget("distance",
                                         soxtThumbWheelWidgetClass, line1,
                                         XmNtopAttachment, XmATTACH_FORM,
                                         XmNtopOffset, 2,
                                         XmNleftAttachment, XmATTACH_WIDGET,
                                         XmNleftWidget, label,
                                         XmNbottomAttachment, XmATTACH_FORM,
                                         XmNbottomOffset, 2,
                                         XmNorientation, XmHORIZONTAL,
                                         XmNshadowType, XmSHADOW_OUT,
                                         XmNshadowThickness, 2,
                                         XmNtraversalOn, False,
                                         XmNwidth, 90,
                                         NULL);

  PRIVATE(this)->seekdistancefield = XtVaCreateManagedWidget("seekdistancefield",
                                                             xmTextFieldWidgetClass, line1,
                                                             XmNtopAttachment, XmATTACH_FORM,
                                                             XmNleftAttachment, XmATTACH_WIDGET,
                                                             XmNleftWidget, wheel,
                                                             XmNwidth, 80,
                                                             NULL);
  XtAddCallback(PRIVATE(this)->seekdistancefield, XmNlosingFocusCallback,
                SoXtFullViewerP::seekdistancechangedCB, (XtPointer) this);
  XtAddCallback(PRIVATE(this)->seekdistancefield, XmNactivateCallback,
                SoXtFullViewerP::seekdistancechangedCB, (XtPointer) this);
  char buffer[16];
  sprintf(buffer, "%g", PRIVATE(this)->seekdistance);
  XmTextSetString(PRIVATE(this)->seekdistancefield, buffer);
  XmTextSetCursorPosition(PRIVATE(this)->seekdistancefield, (long) strlen(buffer));

  Widget line2 = XtVaCreateManagedWidget("line 2",
                                         xmFormWidgetClass, form,
                                         XmNtopAttachment, XmATTACH_WIDGET,
                                         XmNtopWidget, line1,
                                         XmNleftAttachment, XmATTACH_FORM,
                                         XmNrightAttachment, XmATTACH_FORM,
                                         NULL);

  PRIVATE(this)->percenttoggle = XtVaCreateManagedWidget("percentagetoggle",
                                                         xmToggleButtonWidgetClass, line2,
                                                         XmNtopAttachment, XmATTACH_FORM,
                                                         XmNleftAttachment, XmATTACH_FORM,
                                                         XmNbottomAttachment, XmATTACH_FORM,
                                                         XtVaTypedArg,
                                                         XmNlabelString, XmRString,
                                                         "percentage", strlen("percentage") + 1,
                                                         XmNindicatorType, XmONE_OF_MANY,
                                                         XmNset, PRIVATE(this)->seekdistaspercentage ? True : False,
                                                         NULL);

  XtAddCallback(PRIVATE(this)->percenttoggle, XmNvalueChangedCallback,
                SoXtFullViewerP::percenttoggledCB, (XtPointer) this);

  PRIVATE(this)->absolutetoggle = XtVaCreateManagedWidget("absolutetoggle",
                                                          xmToggleButtonWidgetClass, line2,
                                                          XmNtopAttachment, XmATTACH_FORM,
                                                          XmNleftAttachment, XmATTACH_WIDGET,
                                                          XmNleftWidget, PRIVATE(this)->percenttoggle,
                                                          XmNbottomAttachment, XmATTACH_FORM,
                                                          XmNindicatorType, XmONE_OF_MANY,
                                                          XtVaTypedArg,
                                                          XmNlabelString, XmRString,
                                                          "absolute", strlen("absolute") + 1,
                                                          XmNset, PRIVATE(this)->seekdistaspercentage ? False : True,
                                                          NULL);

  XtAddCallback(PRIVATE(this)->absolutetoggle, XmNvalueChangedCallback,
                SoXtFullViewerP::absolutetoggledCB, (XtPointer) this);

  return form;
}

// *************************************************************************

/*!
  This method creates and lays out the widgets in the Zoom preferences sheet
  frame.
*/

Widget
SoXtFullViewer::createZoomPrefSheetGuts(Widget parent)
{
  //  Camera zoom  |          ||||        |     |NN.NN|
  //  Zoom slider ranges from:  |N.N|  to |N.N|

  Widget form = XtVaCreateManagedWidget("zoomprefs",
                                        xmFormWidgetClass, parent,
                                        NULL);

  PRIVATE(this)->zoomfrom = XtVaCreateManagedWidget("from",
                                                    xmTextFieldWidgetClass, form,
                                                    XmNtopAttachment, XmATTACH_FORM,
                                                    XmNleftAttachment, XmATTACH_FORM,
                                                    XmNwidth, 50,
                                                    NULL);

  SoCamera * const camera = this->getCamera();
  Boolean enable = False;
  if (camera) {
    SoType camtype = camera->getTypeId();
    if (camtype.isDerivedFrom(SoPerspectiveCamera::getClassTypeId()))
      enable = True;
  }

  char buf[16];
  sprintf(buf, "%.1f", PRIVATE(this)->zoomrange[0]);
  XmTextSetString(PRIVATE(this)->zoomfrom, buf);
  XmTextSetCursorPosition(PRIVATE(this)->zoomfrom, (long) strlen(buf));

  XtVaSetValues(PRIVATE(this)->zoomfrom,
                XmNsensitive, enable,
                XmNeditable, enable,
                XmNcursorPositionVisible, enable,
                NULL);

  XtAddCallback(PRIVATE(this)->zoomfrom, XmNactivateCallback,
                SoXtFullViewerP::zoomfromchangedCB, (XtPointer) this);
  XtAddCallback(PRIVATE(this)->zoomfrom, XmNlosingFocusCallback,
                SoXtFullViewerP::zoomfromchangedCB, (XtPointer) this);

  PRIVATE(this)->zoomvalue = XtVaCreateManagedWidget("zoomvalue",
                                                     xmTextFieldWidgetClass, form,
                                                     XmNtopAttachment, XmATTACH_FORM,
                                                     XmNrightAttachment, XmATTACH_FORM,
                                                     XmNwidth, 60,
                                                     NULL);

  sprintf(buf, "%.1f", PRIVATE(this)->getCameraZoom());
  XmTextSetString(PRIVATE(this)->zoomvalue, buf);
  XmTextSetCursorPosition(PRIVATE(this)->zoomvalue, (long) strlen(buf));

  XtVaSetValues(PRIVATE(this)->zoomvalue,
                XmNsensitive, enable,
                XmNeditable, enable,
                XmNcursorPositionVisible, enable,
                NULL);

  XtAddCallback(PRIVATE(this)->zoomvalue, XmNactivateCallback,
                SoXtFullViewerP::zoomvaluechangedCB, (XtPointer) this);
  XtAddCallback(PRIVATE(this)->zoomvalue, XmNlosingFocusCallback,
                SoXtFullViewerP::zoomvaluechangedCB, (XtPointer) this);

  Widget valuelabel = XtVaCreateManagedWidget("valuelabel",
                                              xmLabelWidgetClass, form,
                                              XmNtopAttachment, XmATTACH_FORM,
                                              XmNrightAttachment, XmATTACH_WIDGET,
                                              XmNrightWidget, PRIVATE(this)->zoomvalue,
                                              XmNbottomAttachment, XmATTACH_FORM,
                                              XtVaTypedArg,
                                              XmNlabelString, XmRString,
                                              " Value: ", strlen(" Value: ") + 1,
                                              NULL);

  PRIVATE(this)->zoomto = XtVaCreateManagedWidget("to",
                                                  xmTextFieldWidgetClass, form,
                                                  XmNtopAttachment, XmATTACH_FORM,
                                                  XmNrightAttachment, XmATTACH_WIDGET,
                                                  XmNrightWidget, valuelabel,
                                                  XmNwidth, 50,
                                                  NULL);

  sprintf(buf, "%.1f", PRIVATE(this)->zoomrange[1]);
  XmTextSetString(PRIVATE(this)->zoomto, buf);
  XmTextSetCursorPosition(PRIVATE(this)->zoomto, (long) strlen(buf));

  XtVaSetValues(PRIVATE(this)->zoomto,
                XmNsensitive, enable,
                XmNeditable, enable,
                XmNcursorPositionVisible, enable,
                NULL);

  XtAddCallback(PRIVATE(this)->zoomto, XmNactivateCallback,
                SoXtFullViewerP::zoomtochangedCB, (XtPointer) this);
  XtAddCallback(PRIVATE(this)->zoomto, XmNlosingFocusCallback,
                SoXtFullViewerP::zoomtochangedCB, (XtPointer) this);

  PRIVATE(this)->zoomslider = XtVaCreateManagedWidget("zoomslider",
                                                      xmScaleWidgetClass, form,
                                                      XmNtopAttachment, XmATTACH_FORM,
                                                      XmNtopOffset, 6,
                                                      XmNleftAttachment, XmATTACH_WIDGET,
                                                      XmNleftWidget, PRIVATE(this)->zoomfrom,
                                                      XmNrightAttachment, XmATTACH_WIDGET,
                                                      XmNrightWidget, PRIVATE(this)->zoomto,
                                                      XmNorientation, XmHORIZONTAL,
                                                      XmNminimum, 0,
                                                      XmNmaximum, 1000,
                                                      XmNdecimalPoints, 1,
                                                      XmNshowValue, False,
                                                      NULL);

  float zoomvalue = PRIVATE(this)->getCameraZoom();
  if (zoomvalue == 0.0f || (PRIVATE(this)->zoomrange[0] == PRIVATE(this)->zoomrange[1])) {
    XmScaleSetValue(PRIVATE(this)->zoomslider, 0);
  } else {
    float normalized = (zoomvalue - PRIVATE(this)->zoomrange[0]) /
      (PRIVATE(this)->zoomrange[1] - PRIVATE(this)->zoomrange[0]);
    int scaledval = (int) (sqrt(normalized) * 1000.0f);
    XmScaleSetValue(PRIVATE(this)->zoomslider, scaledval);
  }

  XtVaSetValues(PRIVATE(this)->zoomslider,
                XmNsensitive, enable,
                XmNeditable, enable,
                NULL);

  XtAddCallback(PRIVATE(this)->zoomslider, XmNvalueChangedCallback,
                SoXtFullViewerP::zoomsliderchangedCB, (XtPointer) this);
  XtAddCallback(PRIVATE(this)->zoomslider, XmNdragCallback,
                SoXtFullViewerP::zoomsliderchangedCB, (XtPointer) this);

  return form;
}

// *************************************************************************

/*!
  \internal
  This method sets up the clipping related part of the preferences window,
  excluding the surrounding frame.
*/

Widget
SoXtFullViewer::createClippingPrefSheetGuts(Widget parent)
{
  Widget form = XtVaCreateManagedWidget("clippingprefs",
                                        xmFormWidgetClass, parent, NULL);

#if 0
  this->autocliptoggle = XtVaCreateManagedWidget("autocliptoggle",
                                                 xmToggleButtonWidgetClass, form,
                                                 XmNleftAttachment, XmATTACH_FORM,
                                                 XmNtopAttachment, XmATTACH_FORM,
                                                 XmNset, this->isAutoClipping() ? True : False,
                                                 XtVaTypedArg,
                                                 XmNlabelString, XmRString,
                                                 "auto", strlen("auto") + 1,
                                                 NULL);

  XtAddCallback(this->autocliptoggle, XmNvalueChangedCallback,
                SoXtFullViewer::autocliptoggledCB, (XtPointer) this);

  SoCamera * const camera = this->getCamera();

  this->farvalue = XtVaCreateManagedWidget("farvalue",
                                           xmTextFieldWidgetClass, form,
                                           XmNrightAttachment, XmATTACH_FORM,
                                           XmNrightOffset, 2,
                                           XmNbottomAttachment, XmATTACH_FORM,
                                           XmNbottomOffset, 2,
                                           XmNwidth, 100,
                                           XmNsensitive, this->isAutoClipping() ? False : True,
                                           XmNeditable, this->isAutoClipping() ? False : True,
                                           XmNcursorPositionVisible, this->isAutoClipping() ? False : True,
                                           NULL);

  float fardistance = 0.002f;
  if (camera != NULL)
    fardistance = camera->farDistance.getValue();

  char buf[16];
  sprintf(buf, "%g", fardistance);
  XmTextSetString(this->farvalue, buf);
  XmTextSetCursorPosition(this->farvalue, (long) strlen(buf));

  XtAddCallback(this->farvalue, XmNactivateCallback,
                SoXtFullViewer::farvaluechangedCB, (XtPointer) this);
  XtAddCallback(this->farvalue, XmNlosingFocusCallback,
                SoXtFullViewer::farvaluechangedCB, (XtPointer) this);

/*
  this->farwheel = XtVaCreateManagedWidget("farwheel",
    soxtThumbWheelWidgetClass, form,
    XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNleftWidget, this->farvalue,
    XmNleftOffset, 2,
    XmNrightAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNrightWidget, this->farvalue,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_WIDGET,
    XmNbottomWidget, this->farvalue,
    XmNbottomOffset, 2,
    XmNheight, 28,
    XmNsensitive, this->isAutoClipping() ? False : True,
    XmNorientation, XmHORIZONTAL,
    NULL);

  XtAddCallback(this->farwheel, XmNvalueChangedCallback,
    SoXtFullViewer::farwheelvaluechangedCB, (XtPointer) this);
  XtAddCallback(this->farwheel, XmNarmCallback,
    SoXtFullViewer::increaseInteractiveCountCB, (XtPointer) this);
  XtAddCallback(this->farwheel, XmNdisarmCallback,
    SoXtFullViewer::decreaseInteractiveCountCB, (XtPointer) this);

  SoXtThumbWheelSetValue(this->farwheel, fardistance);

  Widget farlabel = XtVaCreateManagedWidget("farlabel",
    xmLabelWidgetClass, form,
    XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNleftWidget, this->farvalue,
    XmNrightAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNrightWidget, this->farvalue,
    XmNbottomAttachment, XmATTACH_WIDGET,
    XmNbottomWidget, this->farwheel,
    XmNbottomOffset, 2,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Far plane:", strlen("Far plane:") + 1,
    NULL);
*/

  float neardistance = 0.001f;
  if (camera != NULL)
    neardistance = camera->nearDistance.getValue();

  this->nearvalue = XtVaCreateManagedWidget("nearvalue",
                                            xmTextFieldWidgetClass, form,
                                            XmNrightAttachment, XmATTACH_WIDGET,
                                            XmNrightWidget, this->farvalue,
                                            XmNrightOffset, 2,
                                            XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
                                            XmNbottomWidget, this->farvalue,
                                            XmNwidth, 100,
                                            XmNsensitive, this->isAutoClipping() ? False : True,
                                            XmNeditable, this->isAutoClipping() ? False : True,
                                            XmNcursorPositionVisible, this->isAutoClipping() ? False : True,
                                            NULL);

  sprintf(buf, "%g", neardistance);
  XmTextSetString(this->nearvalue, buf);
  XmTextSetCursorPosition(this->nearvalue, (long) strlen(buf));

  XtAddCallback(this->nearvalue, XmNactivateCallback,
                SoXtFullViewer::nearvaluechangedCB, (XtPointer) this);
  XtAddCallback(this->nearvalue, XmNlosingFocusCallback,
                SoXtFullViewer::nearvaluechangedCB, (XtPointer) this);

/*
  this->nearwheel = XtVaCreateManagedWidget("nearwheel",
    soxtThumbWheelWidgetClass, form,
    XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNleftWidget, this->nearvalue,
    XmNleftOffset, 2,
    XmNrightAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNrightWidget, this->nearvalue,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNbottomWidget, this->farwheel,
    XmNsensitive, this->isAutoClipping() ? False : True,
    XmNorientation, XmHORIZONTAL,
    XmNheight, 28,
    NULL);

  SoXtThumbWheelSetValue(this->nearwheel, neardistance);

  XtAddCallback(this->nearwheel, XmNvalueChangedCallback,
    SoXtFullViewer::nearwheelvaluechangedCB, (XtPointer) this);
  XtAddCallback(this->nearwheel, XmNarmCallback,
    SoXtFullViewer::increaseInteractiveCountCB, (XtPointer) this);
  XtAddCallback(this->nearwheel, XmNdisarmCallback,
    SoXtFullViewer::decreaseInteractiveCountCB, (XtPointer) this);

  Widget nearlabel = XtVaCreateManagedWidget("nearlabel",
    xmLabelWidgetClass, form,
    XmNleftAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNleftWidget, this->nearvalue,
    XmNrightAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNrightWidget, this->nearvalue,
    XmNbottomAttachment, XmATTACH_OPPOSITE_WIDGET,
    XmNbottomWidget, farlabel,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "Near plane:", strlen("Near plane:") + 1,
    NULL);
*/
#endif

  return form;
}

// *************************************************************************

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::createStereoPrefSheetGuts(Widget parent)
{
  Widget form = XtVaCreateManagedWidget("stereoguts",
                                        xmFormWidgetClass, parent, NULL);

  PRIVATE(this)->stereotoggle = XtVaCreateManagedWidget("stereotoggle",
                                                        xmToggleButtonWidgetClass, form,
                                                        XmNleftAttachment, XmATTACH_FORM,
                                                        XmNtopAttachment, XmATTACH_FORM,
                                                        XmNset, False,
                                                        XtVaTypedArg,
                                                        XmNlabelString, XmRString,
                                                        "stereo viewing", strlen("stereo viewing") + 1,
                                                        NULL);

  XtAddCallback(PRIVATE(this)->stereotoggle, XmNvalueChangedCallback,
                SoXtFullViewerP::stereotoggledCB, (XtPointer) this);

  return form;
}

// *************************************************************************

/*!
  FIXME: write doc
*/

Widget
SoXtFullViewer::createSpeedPrefSheetGuts(Widget parent)
{
  Widget form = XtVaCreateManagedWidget("speedform",
                                        xmFormWidgetClass, parent, NULL);

  Widget label = XtVaCreateManagedWidget("speedlabel",
                                         xmLabelWidgetClass, form,
                                         XmNleftAttachment, XmATTACH_FORM,
                                         XmNtopAttachment, XmATTACH_FORM,
                                         XmNbottomAttachment, XmATTACH_FORM,
                                         XtVaTypedArg,
                                         XmNlabelString, XmRString,
                                         "Viewer speed: ", strlen("Viewer speed: ") + 1,
                                         NULL);

  PRIVATE(this)->incspeedbutton = XtVaCreateManagedWidget("incbutton",
                                                          xmPushButtonWidgetClass, form,
                                                          XmNleftAttachment, XmATTACH_WIDGET,
                                                          XmNleftWidget, label,
                                                          XmNtopAttachment, XmATTACH_FORM,
                                                          XtVaTypedArg,
                                                          XmNlabelString, XmRString,
                                                          "increase", strlen("increase") + 1,
                                                          NULL);

  XtAddCallback(PRIVATE(this)->incspeedbutton, XmNactivateCallback,
                SoXtFullViewerP::speedIncCB, (XtPointer) this);

  PRIVATE(this)->decspeedbutton = XtVaCreateManagedWidget("decbutton",
                                                          xmPushButtonWidgetClass, form,
                                                          XmNleftAttachment, XmATTACH_WIDGET,
                                                          XmNleftWidget, PRIVATE(this)->incspeedbutton,
                                                          XmNtopAttachment, XmATTACH_FORM,
                                                          XtVaTypedArg,
                                                          XmNlabelString, XmRString,
                                                          "decrease", strlen("decrease") + 1,
                                                          NULL);

  XtAddCallback(PRIVATE(this)->decspeedbutton, XmNactivateCallback,
                SoXtFullViewerP::speedDecCB, (XtPointer) this);

  return form;
}

// *************************************************************************
// preferences sheet hooks:

// This callback is hooked up to the destruction of the preferences
// sheet window.
void
SoXtFullViewerP::prefSheetDestroy(void)
{
  XtUnrealizeWidget(PUBLIC(this)->prefshell);
  PUBLIC(this)->prefshell = NULL;
  PUBLIC(this)->prefsheet = NULL;
}

void
SoXtFullViewerP::prefSheetDestroyCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  PRIVATE(viewer)->prefSheetDestroy();
}

// This callback is hooked up to the "seconds" preferences sheet text
// field.
void
SoXtFullViewerP::seekPrefSheetFieldCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  SOXT_STUB();
}

// This callback is hooked up to the seek-to-point-or-object toggling
// widgets.
void
SoXtFullViewerP::seekPrefSheetToggle1CB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  SOXT_STUB();
}

// This callback is hooked up to the seek-percentage-or-distance
// toggle widgets.
void
SoXtFullViewerP::seekPrefSheetToggle2CB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  SOXT_STUB();
}

void
SoXtFullViewerP::autocliptoggled(void)
{
  Boolean enable = False;
  XtVaGetValues(this->autocliptoggle, XmNset, &enable, NULL);
  PUBLIC(this)->setAutoClipping(enable ? TRUE : FALSE);

  XtVaSetValues(this->nearwheel,
                XmNsensitive, enable ? False : True,
                NULL);
  XtVaSetValues(this->farwheel,
                XmNsensitive, enable ? False : True,
                NULL);
  XtVaSetValues(this->nearvalue,
                XmNsensitive, enable ? False : True,
                XmNeditable, enable ? False : True,
                XmNcursorPositionVisible, enable ? False : True,
                NULL);
  XtVaSetValues(this->farvalue,
                XmNsensitive, enable ? False : True,
                XmNeditable, enable ? False : True,
                XmNcursorPositionVisible, enable ? False : True,
                NULL);

  SoCamera * const camera = PUBLIC(this)->getCamera();
  if (!camera) { return; }

  if (! enable) {
    float nearval = SoXtThumbWheelGetValue(this->nearwheel);
    float farval = SoXtThumbWheelGetValue(this->farwheel);
    camera->nearDistance = nearval;
    camera->farDistance = farval;
  }
}

void
SoXtFullViewerP::autocliptoggledCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  PRIVATE(viewer)->autocliptoggled();
}

void
SoXtFullViewerP::nearwheelvaluechanged(void)
{
  assert(this->nearwheel != NULL && this->farwheel != NULL);

  float val = SoXtThumbWheelGetValue(this->nearwheel);
  float farval = SoXtThumbWheelGetValue(this->farwheel);
  
  if (val < 0.001f) {
    val = 0.001f;
    SoXtThumbWheelSetValue(this->nearwheel, val);
  } else if (val >= farval) {
    val = farval - 0.001f;
    SoXtThumbWheelSetValue(this->nearwheel, val);
  }

  char valuestring[32];
  sprintf(valuestring, "%.3f", val);
  XmTextSetString(this->nearvalue, valuestring);
  XmTextSetCursorPosition(this->nearvalue, (long) strlen(valuestring));

  SoCamera * const cam = PUBLIC(this)->getCamera();
  if (!cam) { return; }

  // FIXME: cut off at 0.0? And against far clipping value? 990223 mortene.
  cam->nearDistance = val;
}

void
SoXtFullViewerP::nearwheelvaluechangedCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  PRIVATE(viewer)->nearwheelvaluechanged();
}

void
SoXtFullViewerP::farwheelvaluechanged(void)
{
  assert(this->nearwheel != NULL && this->farwheel != NULL);

  float val = SoXtThumbWheelGetValue(this->farwheel);
  float nearval = SoXtThumbWheelGetValue(this->nearwheel);

  if (val <= nearval) {
    val = nearval + 0.001f;
    SoXtThumbWheelSetValue(this->farwheel, val);
  }

  char valuestring[32];
  sprintf(valuestring, "%.3f", val);
  XmTextSetString(this->farvalue, valuestring);
  XmTextSetCursorPosition(this->farvalue, (long) strlen(valuestring));

  SoCamera * const cam = PUBLIC(this)->getCamera();
  if (!cam) { return; }

  cam->farDistance = val;
}

void
SoXtFullViewerP::farwheelvaluechangedCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  PRIVATE(viewer)->farwheelvaluechanged();
}

void
SoXtFullViewerP::nearvaluechanged(void)
{
  float val = atof(XmTextGetString(this->nearvalue));
  float farval = SoXtThumbWheelGetValue(this->farwheel);
  if (val >= farval)
    val = farval - 0.001f;
  float temp = SoXtThumbWheelGetValue(this->nearwheel);
  if (val != temp)
    SoXtThumbWheelSetValue(this->nearwheel, val);
  char buf[16];
  sprintf(buf, "%g", val);
  XmTextSetString(this->nearvalue, buf);
  XmTextSetCursorPosition(this->nearvalue, (long) strlen(buf));

  SoCamera * const camera = PUBLIC(this)->getCamera();
  if (!camera) { return; }
  camera->nearDistance = val;
}

void
SoXtFullViewerP::nearvaluechangedCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  PRIVATE(viewer)->nearvaluechanged();
}

void
SoXtFullViewerP::farvaluechanged(void)
{
  float nearval = SoXtThumbWheelGetValue(this->nearwheel);
  float val = atof(XmTextGetString(this->farvalue));
  if (val <= nearval)
    val = nearval + 0.001f;
  float temp = SoXtThumbWheelGetValue(this->farwheel);
  if (temp != val)
    SoXtThumbWheelSetValue(this->farwheel, val);
  char buf[16];
  sprintf(buf, "%g", val);
  XmTextSetString(this->farvalue, buf);
  XmTextSetCursorPosition(this->farvalue, (long) strlen(buf));

  SoCamera * const camera = PUBLIC(this)->getCamera();
  if (!camera) { return; }
  camera->farDistance = val;
}

void
SoXtFullViewerP::farvaluechangedCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  PRIVATE(viewer)->farvaluechanged();
}

// *************************************************************************
  
void
SoXtFullViewerP::selectedViewing(void)
{
  PUBLIC(this)->setViewing(PUBLIC(this)->isViewing() ? FALSE : TRUE);
}

void
SoXtFullViewerP::selectedDecoration(void)
{
  PUBLIC(this)->setDecoration(PUBLIC(this)->isDecoration() ? FALSE : TRUE);
}

void
SoXtFullViewerP::selectedHeadlight(void)
{
  SOXT_STUB();
}

void
SoXtFullViewerP::selectedPrefs(void)
{
  if (!PUBLIC(this)->prefshell)
    PUBLIC(this)->createPrefSheet();
/*
  else
    FIXME: de-iconify prefshell and push to front
*/
}

void
SoXtFullViewerP::interactbuttonCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *)closure;
  if (viewer->isViewing()) viewer->setViewing(FALSE);
}

void
SoXtFullViewerP::examinebuttonCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *)closure;
  if (!viewer->isViewing()) viewer->setViewing(TRUE);
}

void
SoXtFullViewerP::helpbuttonCB(Widget, XtPointer client_data, XtPointer)
{
  ((SoXtFullViewer *)client_data)->openViewerHelpCard();
}

void
SoXtFullViewerP::homebuttonCB(Widget, XtPointer client_data, XtPointer)
{
  ((SoXtFullViewer *)client_data)->resetToHomePosition();
}

void
SoXtFullViewerP::sethomebuttonCB(Widget, XtPointer client_data, XtPointer)
{
  ((SoXtFullViewer *)client_data)->saveHomePosition();
}

void
SoXtFullViewerP::viewallbuttonCB(Widget, XtPointer client_data, XtPointer)
{
  ((SoXtFullViewer *)client_data)->viewAll();
}

void
SoXtFullViewerP::seekbuttonCB(Widget, XtPointer client_data, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *)client_data;
  PRIVATE(viewer)->seekbuttonClicked();
}

void
SoXtFullViewerP::copyviewSelected(void)
{
  PUBLIC(this)->copyView(SbTime::getTimeOfDay());
}

void
SoXtFullViewerP::pasteviewSelected(void)
{
  PUBLIC(this)->pasteView(SbTime::getTimeOfDay());
}

// *************************************************************************

void
SoXtFullViewerP::increaseInteractiveCountCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->interactiveCountInc();
}

void
SoXtFullViewerP::decreaseInteractiveCountCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  viewer->interactiveCountDec();
}

// *************************************************************************

void
SoXtFullViewerP::seektimechanged(void)
{
  float newtime = (float) atof(XmTextGetString(this->seektimefield));
  PUBLIC(this)->setSeekTime(newtime);
  char buf[16];
  sprintf(buf, "%g", newtime);
  XmTextSetString(this->seektimefield, buf);
  XmTextSetCursorPosition(this->seektimefield, (long) strlen(buf));
}

void
SoXtFullViewerP::seektimechangedCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  PRIVATE(viewer)->seektimechanged();
}

void
SoXtFullViewerP::pointtoggled(void)
{
  XtVaSetValues(this->pointtoggle, XmNset, True, NULL);
  XtVaSetValues(this->objecttoggle, XmNset, False, NULL);
  if (!PUBLIC(this)->isDetailSeek()) PUBLIC(this)->setDetailSeek(TRUE);
}

void
SoXtFullViewerP::pointtoggledCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  PRIVATE(viewer)->pointtoggled();
}

void
SoXtFullViewerP::objecttoggled(void)
{
  XtVaSetValues(this->pointtoggle, XmNset, False, NULL);
  XtVaSetValues(this->objecttoggle, XmNset, True, NULL);
  if (PUBLIC(this)->isDetailSeek()) PUBLIC(this)->setDetailSeek(FALSE);
}

void
SoXtFullViewerP::objecttoggledCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  PRIVATE(viewer)->objecttoggled();
}

void
SoXtFullViewerP::seekdistancechanged(void)
{
  float newdist = (float) atof(XmTextGetString(this->seekdistancefield));
  if (newdist < 0.0f)
    newdist = 0.0f;
  this->seekdistance = newdist;
  char buf[16];
  sprintf(buf, "%g", newdist);
  XmTextSetString(this->seekdistancefield, buf);
  XmTextSetCursorPosition(this->seekdistancefield, (long) strlen(buf));
}

void
SoXtFullViewerP::seekdistancechangedCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  PRIVATE(viewer)->seekdistancechanged();
}

void
SoXtFullViewerP::percenttoggled(void)
{
  XtVaSetValues(this->percenttoggle, XmNset, True, NULL);
  XtVaSetValues(this->absolutetoggle, XmNset, False, NULL);
  this->seekdistaspercentage = TRUE;
}

void
SoXtFullViewerP::percenttoggledCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  PRIVATE(viewer)->percenttoggled();
}

void
SoXtFullViewerP::absolutetoggled(void)
{
  XtVaSetValues(this->percenttoggle, XmNset, False, NULL);
  XtVaSetValues(this->absolutetoggle, XmNset, True, NULL);
  this->seekdistaspercentage = FALSE;
}

void
SoXtFullViewerP::absolutetoggledCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  PRIVATE(viewer)->absolutetoggled();
}

void
SoXtFullViewerP::stereotoggled(void)
{
  Boolean enable = False;
  XtVaGetValues(this->stereotoggle, XmNset, &enable, NULL);
  PUBLIC(this)->setStereoViewing(enable ? TRUE : FALSE);
  XtVaSetValues(this->stereotoggle,
                XmNset, PUBLIC(this)->isStereoViewing() ? True : False,
                NULL);
}

void
SoXtFullViewerP::stereotoggledCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  PRIVATE(viewer)->stereotoggled();
}

// *************************************************************************

/*!
  This method returns the current camera zoom value (degrees used in the
  camera field of view) or 0.0f if "zoom" has no meaning (no camera or a
  non-perspective camera).
*/

float
SoXtFullViewerP::getCameraZoom(void)
{
  SoCamera * const camera = PUBLIC(this)->getCamera();
  if (! camera) {
#if SOXT_DEBUG
    SoDebugError::postInfo("SoXtFullViewer::getCameraZoom",
                           "no camera!");
#endif // SOXT_DEBUG
    return 0.0f;
  }

  const SoType camtype(camera->getTypeId());
  if (! camtype.isDerivedFrom(SoPerspectiveCamera::getClassTypeId())) {
#if SOXT_DEBUG
    SoDebugError::postInfo("SoXtFullViewer::getCameraZoom",
                           "not using a perspective camera!");
#endif // SOXT_DEBUG
    return 0.0f;
  }

  SoPerspectiveCamera * const pCamera = (SoPerspectiveCamera *) camera;
  return pCamera->heightAngle.getValue() * 180 / M_PI;
}

/*!
  This method sets the camera zoom value (given the scene graph camera is a
  perspecive camera).  \a zoom must be within the range between 0 and 180
  (degrees to use as field of view).
*/

void
SoXtFullViewerP::setCameraZoom(float zoom)
{
  SoCamera * const camera = PUBLIC(this)->getCamera();
  if (camera == NULL) {
#if SOXT_DEBUG
    SoDebugError::postInfo("SoXtFullViewer::setCameraZoom",
                           "no camera!");
#endif // SOXT_DEBUG
    return;
  }

  const SoType camtype(camera->getTypeId());
  if (! camtype.isDerivedFrom(SoPerspectiveCamera::getClassTypeId())) {
#if SOXT_DEBUG
    SoDebugError::postInfo("SoXtFullViewer::getCameraZoom",
                           "not using a perspective camera!");
#endif // SOXT_DEBUG
    return;
  }

  SoPerspectiveCamera * const pCamera = (SoPerspectiveCamera *) camera;
  pCamera->heightAngle = zoom * M_PI / 180.0f;
}

// *************************************************************************

void
SoXtFullViewerP::zoomfromchanged(void)
{
  float value = atof(XmTextGetString(this->zoomfrom));
  if (value < 0.001f)
    value = 0.001f;
  else if (value > this->zoomrange[1])
    value = this->zoomrange[1];
  this->zoomrange[0] = value;

  char buf[16];
  sprintf(buf, "%.1f", value);
  XmTextSetString(this->zoomfrom, buf);
  XmTextSetCursorPosition(this->zoomfrom, (long) strlen(buf));

  float zoom = this->getCameraZoom();
  if (zoom < this->zoomrange[0]) {
    zoom = this->zoomrange[0];
    this->setCameraZoom(zoom);
    sprintf(buf, "%.1f", zoom);
    XmTextSetString(this->zoomvalue, buf);
    XmTextSetCursorPosition(this->zoomvalue, (long) strlen(buf));
  } else if (zoom > this->zoomrange[1]) {
    zoom = this->zoomrange[1];
    this->setCameraZoom(zoom);
    sprintf(buf, "%.1f", zoom);
    XmTextSetString(this->zoomvalue, buf);
    XmTextSetCursorPosition(this->zoomvalue, (long) strlen(buf));
  }

  if (this->zoomrange[0] == this->zoomrange[1]) {
    XmScaleSetValue(this->zoomslider, 0);
  } else {
    float normalized = (zoom - this->zoomrange[0]) /
      (this->zoomrange[1] - this->zoomrange[0]);
    int scaledval = (int) (sqrt(normalized) * 1000.0f);
    XmScaleSetValue(this->zoomslider, scaledval);
  }
}

void
SoXtFullViewerP::zoomfromchangedCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  PRIVATE(viewer)->zoomfromchanged();
}

void
SoXtFullViewerP::zoomsliderchanged(void)
{
  Boolean editable = True;
  XtVaGetValues(this->zoomslider, XmNsensitive, &editable, NULL);
  if (! editable) {
    float value = (float) atof(XmTextGetString(this->zoomvalue));
    if (this->zoomrange[0] == this->zoomrange[1]) {
      XmScaleSetValue(this->zoomslider, 0);
    } else {
      float normalized = (value - this->zoomrange[0]) /
        (this->zoomrange[1] - this->zoomrange[0]);
      int scaledval = (int) (sqrt(normalized) * 1000.0f);
      XmScaleSetValue(this->zoomslider, scaledval);
    }
    return;
  }

  int intval = 0;
  XmScaleGetValue(this->zoomslider, &intval);

  float expval = (float) (intval * intval) / 1000000.0f;
  float realval =
    ((this->zoomrange[1] - this->zoomrange[0]) * expval) + this->zoomrange[0];

  float curzoom = this->getCameraZoom();
  if (curzoom != realval) {
    char buf[16];
    sprintf(buf, "%.1f", realval);
    XmTextSetString(this->zoomvalue, buf);
    XmTextSetCursorPosition(this->zoomvalue, (long) strlen(buf));
    this->setCameraZoom(realval);
  }
}

void
SoXtFullViewerP::zoomsliderchangedCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  PRIVATE(viewer)->zoomsliderchanged();
}

void
SoXtFullViewerP::zoomtochanged(void)
{
  float value = atof(XmTextGetString(this->zoomto));
  if (value > 180.0f)
    value = 180.0f;
  else if (value < this->zoomrange[0])
    value = this->zoomrange[0];

  char buf[16];
  sprintf(buf, "%.1f", value);
  XmTextSetString(this->zoomto, buf);
  XmTextSetCursorPosition(this->zoomto, (long) strlen(buf));

  this->zoomrange[1] = value;
  float zoom = this->getCameraZoom();
  if (zoom < this->zoomrange[0]) {
    zoom = this->zoomrange[0];
    this->setCameraZoom(zoom);
    sprintf(buf, "%.1f", zoom);
    XmTextSetString(this->zoomvalue, buf);
    XmTextSetCursorPosition(this->zoomvalue, (long) strlen(buf));
    this->setCameraZoom(value);
  } else if (zoom > this->zoomrange[1]) {
    zoom = this->zoomrange[1];
    this->setCameraZoom(zoom);
    sprintf(buf, "%.1f", zoom);
    XmTextSetString(this->zoomvalue, buf);
    XmTextSetCursorPosition(this->zoomvalue, (long) strlen(buf));
    this->setCameraZoom(value);
  }

  if (this->zoomrange[0] == this->zoomrange[1]) {
    XmScaleSetValue(this->zoomslider, 0);
  } else {
    float normalized = (zoom - this->zoomrange[0]) /
      (this->zoomrange[1] - this->zoomrange[0]);
    int scaledval = (int) (sqrt(normalized) * 1000.0f);
    XmScaleSetValue(this->zoomslider, scaledval);
  }
}

void
SoXtFullViewerP::zoomtochangedCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  PRIVATE(viewer)->zoomtochanged();
}

void
SoXtFullViewerP::zoomvaluechanged(void)
{
  float value = atof(XmTextGetString(this->zoomvalue));
  if (value < this->zoomrange[0])
    value = this->zoomrange[0];
  else if (value > this->zoomrange[1])
    value = this->zoomrange[1];

  char buf[16];
  sprintf(buf, "%.1f", value);
  XmTextSetString(this->zoomvalue, buf);
  XmTextSetCursorPosition(this->zoomvalue, (long) strlen(buf));

  if (this->zoomrange[0] == this->zoomrange[1]) {
    XmScaleSetValue(this->zoomslider, 0);
  } else {
    float normalized = (value - this->zoomrange[0]) /
      (this->zoomrange[1] - this->zoomrange[0]);
    int scaledval = (int) (sqrt(normalized) * 1000.0f);
    XmScaleSetValue(this->zoomslider, scaledval);
  }

  this->setCameraZoom(value);
}

void
SoXtFullViewerP::zoomvaluechangedCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  PRIVATE(viewer)->zoomvaluechanged();
}

// *************************************************************************

void
SoXtFullViewerP::speedIncCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  SOXT_STUB_ONCE();
}

void
SoXtFullViewerP::speedDecCB(Widget, XtPointer closure, XtPointer)
{
  SoXtFullViewer * viewer = (SoXtFullViewer *) closure;
  SOXT_STUB_ONCE();
}

// *************************************************************************

// doc in super
void
SoXtFullViewer::sizeChanged(const SbVec2s & size)
{
  SbVec2s newsize(size);
  if (this->isDecoration()) {
    newsize[0] = size[0] - 2 * 30;
    newsize[1] = size[1] - 30;
  }
  inherited::sizeChanged(newsize);
}

// *************************************************************************

SoXtFullViewerP::SoXtFullViewerP(SoXtFullViewer * publ)
  : SoGuiFullViewerP(publ)
{
}

SoXtFullViewerP::~SoXtFullViewerP()
{
}

void
SoXtFullViewerP::seekbuttonClicked(void)
{
  assert(PUBLIC(this)->isViewing() && "seek button should not be enabled in interaction mode");
  PUBLIC(this)->setSeekMode(PUBLIC(this)->isSeekMode() ? FALSE : TRUE);
}

void
SoXtFullViewerP::setThumbWheelValue(void * wheel, float val)
{
  SoXtThumbWheelSetValue((Widget)wheel, val);
}

// *************************************************************************
