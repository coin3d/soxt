/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

// ************************************************************************

// Class is documented in common/viewers/SoGuiFullViewer.cpp.in.

// ************************************************************************

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

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
#include <Inventor/Xt/SoXtInternal.h>
#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtResource.h>
#include <Inventor/Xt/widgets/SoXtThumbWheel.h>
#include <Inventor/Xt/widgets/SoXtPopupMenu.h>

#include <Inventor/Xt/viewers/SoXtFullViewer.h>
#include <Inventor/Xt/viewers/SoXtFullViewerP.h>

#if HAVE_LIBXPM
#include <X11/xpm.h>
#include <Inventor/Xt/common/pixmaps/pick.xpm>
#include <Inventor/Xt/common/pixmaps/view.xpm>
#include <Inventor/Xt/common/pixmaps/home.xpm>
#include <Inventor/Xt/common/pixmaps/set_home.xpm>
#include <Inventor/Xt/common/pixmaps/view_all.xpm>
#include <Inventor/Xt/common/pixmaps/seek.xpm>
#endif // HAVE_LIBXPM

// *************************************************************************

#define PRIVATE(o) (o->pimpl)
#define PUBLIC(o) (o->pub)

// *************************************************************************

SOXT_OBJECT_ABSTRACT_SOURCE(SoXtFullViewer);

// *************************************************************************

enum DefaultViewerButtons {
  INTERACT_BUTTON = 0,
  EXAMINE_BUTTON,
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

  this->popupEnabled = (flags & SoXtFullViewer::BUILD_POPUP) ? TRUE : FALSE;
  PRIVATE(this)->decorations = (flags & SoXtFullViewer::BUILD_DECORATION) ? TRUE : FALSE;

  PRIVATE(this)->appButtonsList = new SbPList;
  this->viewerButtonWidgets = new SbPList;

  this->prefmenu = NULL;

  PRIVATE(this)->pixmaps.pick = 0;
  PRIVATE(this)->pixmaps.pick_ins = 0;
  PRIVATE(this)->pixmaps.view = 0;
  PRIVATE(this)->pixmaps.view_ins = 0;
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

  if ( (PRIVATE(this)->decorations != enable) &&
       (PRIVATE(this)->viewerbase != (Widget) NULL) ) {
    PRIVATE(this)->showDecorationWidgets(enable);
  }
  PRIVATE(this)->decorations = enable;

  if (this->isTopLevelShell() || XtIsShell(XtParent(this->getBaseWidget()))) {
    Widget shell = this->getShellWidget();
    Dimension minwidth = SOXT_VIEWER_MIN_WIDTH;
    Dimension minheight = SOXT_VIEWER_MIN_HEIGHT_BASE +
      30 * this->viewerButtonWidgets->getLength();
    if ( enable ) {
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
    } else {
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
  PRIVATE(this)->resetAppPushButtons();
  PRIVATE(this)->appButtonsList->append(button);
  PRIVATE(this)->layoutAppPushButtons();
}

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::insertAppPushButton(Widget button,
                                    int idx)
{
  PRIVATE(this)->resetAppPushButtons();
  PRIVATE(this)->appButtonsList->insert(button, idx);
  PRIVATE(this)->layoutAppPushButtons();
}

/*!
  FIXME: write doc
*/

void
SoXtFullViewer::removeAppPushButton(Widget button)
{
  int idx = PRIVATE(this)->appButtonsList->find(button);
  assert(idx != -1 && "tried to remove non-existant button");
  PRIVATE(this)->resetAppPushButtons();
  PRIVATE(this)->appButtonsList->remove(idx);
  PRIVATE(this)->layoutAppPushButtons();
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

  // build application buttons
  PRIVATE(this)->appButtonsForm = PRIVATE(this)->buildAppButtonsForm(trim);
  XtVaSetValues(PRIVATE(this)->appButtonsForm,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    NULL);

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

#ifndef DOXYGEN_SKIP_THIS
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
#endif DOXYGEN_SKIP_THIS

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

const char *
SoXtFullViewer::getRightWheelString() const
{
  return this->rightWheelStr;
}

// *************************************************************************

#ifndef DOXYGEN_SKIP_THIS
void
SoXtFullViewerP::resetAppPushButtons(void)
{
  int i;
  const int widgets = this->appButtonsList->getLength();
  for ( i = 0; i < widgets; i++ ) {
    Widget w = (Widget) (*(this->appButtonsList))[i];
    XtUnmanageChild(w);
  }
}

Widget
SoXtFullViewerP::buildAppButtonsForm(Widget parent)
{
  Widget form = XtVaCreateManagedWidget("appbuttons",
                                        xmFormWidgetClass, parent,
                                        NULL);
  return form;
}

void
SoXtFullViewerP::layoutAppPushButtons(void)
{
  int i;
  const int widgets = this->appButtonsList->getLength();
  Widget prev = NULL;
  for ( i = 0; i < widgets; i++ ) {
    Widget w = (Widget) (*(this->appButtonsList))[i];
    if ( i == 0 ) {
      XtVaSetValues(w,
        XmNhighlightThickness, 0,
        XmNtopAttachment, XmATTACH_FORM,
        XmNtopOffset, 0,
        XmNleftAttachment, XmATTACH_FORM,
        XmNleftOffset, 0,
        XmNrightAttachment, XmATTACH_FORM,
        XmNrightOffset, 0,
        XmNbottomOffset, 0,
        XmNwidth, 28,
        XmNheight, 28,
        NULL);
    } else {
      XtVaSetValues(w,
        XmNhighlightThickness, 0,
        XmNtopAttachment, XmATTACH_WIDGET,
        XmNtopWidget, prev,
        XmNleftAttachment, XmATTACH_FORM,
        XmNleftOffset, 0,
        XmNrightAttachment, XmATTACH_FORM,
        XmNrightOffset, 0,
        XmNbottomOffset, 0,
        XmNwidth, 28,
        XmNheight, 28,
        NULL);
    }
    XtManageChild(w);
    prev = w;
  }
  // SOXT_STUB();
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

  if ( enable ) {
    XtVaSetValues(this->canvas,
                  XmNtopOffset, 0,
                  XmNleftOffset, 30,
                  XmNrightOffset, 30,
                  XmNbottomOffset, 30,
                  NULL);
    if ( XtWindow(PUBLIC(this)->leftDecoration) != 0 ) {
      XtMapWidget(PUBLIC(this)->leftDecoration);
      XtManageChild(PUBLIC(this)->leftDecoration);
    }
    if ( XtWindow(PUBLIC(this)->rightDecoration) != 0 ) {
      XtMapWidget(PUBLIC(this)->rightDecoration);
      XtManageChild(PUBLIC(this)->rightDecoration);
    }
    if ( XtWindow(PUBLIC(this)->bottomDecoration) != 0 ) {
      XtMapWidget(PUBLIC(this)->bottomDecoration);
      XtManageChild(PUBLIC(this)->bottomDecoration);
    }
  } else {
    if ( XtWindow(PUBLIC(this)->leftDecoration) != 0 ) {
      XtUnmapWidget(PUBLIC(this)->leftDecoration);
    }
    if ( XtWindow(PUBLIC(this)->rightDecoration) != 0 ) {
      XtUnmapWidget(PUBLIC(this)->rightDecoration);
    }
    if ( XtWindow(PUBLIC(this)->bottomDecoration) != 0 ) {
      XtUnmapWidget(PUBLIC(this)->bottomDecoration);
    }
    XtVaSetValues(this->canvas,
                  XmNtopOffset, 0,
                  XmNleftOffset, 0,
                  XmNrightOffset, 0,
                  XmNbottomOffset, 0,
                  NULL);
  }
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
#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

// doc in super
void
SoXtFullViewer::sizeChanged(const SbVec2s & size)
{
  SbVec2s newsize(size);
  if ( this->isDecoration() ) {
    newsize[0] = size[0] - 2 * 30;
    newsize[1] = size[1] - 30;
  }
  inherited::sizeChanged(newsize);
}

// *************************************************************************

#ifndef DOXYGEN_SKIP_THIS
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
#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

#undef PRIVATE
#undef PUBLIC
