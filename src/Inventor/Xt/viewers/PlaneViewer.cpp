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

// *************************************************************************

// Class is documented in common/viewers/SoGuiPlaneViewer.cpp.in.

// *************************************************************************

#include <string.h>

#include <X11/keysym.h>

#include <Xm/PushB.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>

#include <soxtdefs.h>
#include <../SoXtInternal.h>
#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtResource.h>
#include <Inventor/Xt/widgets/XtNativePopupMenu.h>

#include <Inventor/Xt/viewers/SoXtPlaneViewer.h>
#include <Inventor/Xt/viewers/SoXtPlaneViewerP.h>

#if HAVE_CONFIG_H
#include <config.h> // for HAVE_LIBXPM
#endif // HAVE_CONFIG_H

#if HAVE_LIBXPM
#include <Inventor/Xt/common/pixmaps/ortho.xpm>
#include <Inventor/Xt/common/pixmaps/perspective.xpm>
#include <Inventor/Xt/common/pixmaps/x.xpm>
#include <Inventor/Xt/common/pixmaps/y.xpm>
#include <Inventor/Xt/common/pixmaps/z.xpm>
#else // !HAVE_LIBXPM
static char ** ortho_xpm = NULL;
static char ** perspective_xpm = NULL;
static char ** x_xpm = NULL;
static char ** y_xpm = NULL;
static char ** z_xpm = NULL;
#endif // !HAVE_LIBXPM

// ************************************************************************

struct SoXtViewerButton {
  char * keyword;
  char * label;
  void (*pressed)(Widget, XtPointer, XtPointer);
  const char ** xpm_data;
  Widget bwidget;
  Widget lwidget;
};

struct SoXtViewerButton
SoXtPlaneViewerP::SoXtPlaneViewerButtons[] = {
  {
    "x", "X",
    SoXtPlaneViewerP::buttonCB,
    x_xpm,
    NULL, NULL
  },
  { // plane Y button
    "y", "Y",
    SoXtPlaneViewerP::buttonCB,
    y_xpm,
    NULL, NULL
  },
  { // plane Z button
    "z", "Z",
    SoXtPlaneViewerP::buttonCB,
    z_xpm,
    NULL, NULL
  },
  { // camera type button
    "camera", "C",
    SoXtPlaneViewerP::buttonCB,
    perspective_xpm,
    NULL, NULL
  }
};

// ************************************************************************

// SoXtPlaneViewerP "private implementation" class.

#define PUBLIC(ptr) (ptr->pub)
#define PRIVATE(ptr) (ptr->pimpl)

#define THIS (PRIVATE(this))

SoXtPlaneViewerP::SoXtPlaneViewerP(SoXtPlaneViewer * publ)
  : SoGuiPlaneViewerP(publ)
{
}

SoXtPlaneViewerP::~SoXtPlaneViewerP()
{
  delete[] this->buttons;
}

void
SoXtPlaneViewerP::constructor(SbBool build)
{
  this->commonConstructor(); // generic code

  const int buttons = sizeof(SoXtPlaneViewerButtons) / sizeof(SoXtViewerButton);
  this->buttons = new SoXtViewerButton[buttons];
  memcpy(this->buttons, SoXtPlaneViewerButtons, sizeof(SoXtPlaneViewerButtons));

  this->prefshell = this->prefsheet = (Widget) NULL;
  this->prefparts = NULL;
  this->numprefparts = 0;

  PUBLIC(this)->setClassName(PUBLIC(this)->getDefaultWidgetName());

  if (build) {
    Widget viewer = PUBLIC(this)->buildWidget(PUBLIC(this)->getParentWidget());
    PUBLIC(this)->setBaseWidget(viewer);
    PUBLIC(this)->fitSize(SbVec2s(500, 300));

    SoXtResource rsc(PUBLIC(this)->getRightWheelLabelWidget());
    char * dollystring = NULL;
    rsc.getResource("dollyString", XmRString, dollystring);
    if (dollystring != NULL)
      PUBLIC(this)->setRightWheelString(dollystring);
  }
}

// static callback for the plane viewer buttons
void
SoXtPlaneViewerP::buttonCB(Widget widget, XtPointer closure, XtPointer)
{
  assert(closure != NULL);
  SoXtPlaneViewer * viewer = (SoXtPlaneViewer *) closure;
  const int idx = PRIVATE(viewer)->findButton(widget);
  if (idx == -1) {
    SoDebugError::post("SoXtPlaneViewerP::buttonCB", "unknown button");
  }
  else if (strcmp(PRIVATE(viewer)->buttons[idx].keyword, "x") == 0) {
    PRIVATE(viewer)->viewPlaneX();
  }
  else if (strcmp(PRIVATE(viewer)->buttons[idx].keyword, "y") == 0) {
    PRIVATE(viewer)->viewPlaneY();
  }
  else if (strcmp(PRIVATE(viewer)->buttons[idx].keyword, "z") == 0) {
    PRIVATE(viewer)->viewPlaneZ();
  }
  else if (strcmp(PRIVATE(viewer)->buttons[idx].keyword, "camera") == 0) {
    viewer->toggleCameraType();
  }
  else {
    SoDebugError::post("SoXtPlaneViewerP::buttonCB", "unsupported button");
  }
}

int
SoXtPlaneViewerP::findButton(Widget widget) const
{
  const int buttons = sizeof(SoXtPlaneViewerButtons) / sizeof(SoXtViewerButton);
  for (int button = 0; button < buttons; button++) {
    if (this->buttons[button].bwidget == widget) return button;
  }
  return -1;
}

// *************************************************************************

// Documented in common/viewers/SoGuiPlaneViewer.cpp.in.
SoXtPlaneViewer::SoXtPlaneViewer(Widget parent,
                                 const char * const name,
                                 SbBool embed,
                                 SoXtFullViewer::BuildFlag flag,
                                 SoXtViewer::Type type)
  : inherited(parent, name, embed, flag, type, FALSE)
{
  PRIVATE(this) = new SoXtPlaneViewerP(this);
  PRIVATE(this)->constructor(TRUE);
}

// Documented in common/viewers/SoGuiPlaneViewer.cpp.in.
SoXtPlaneViewer::SoXtPlaneViewer(Widget parent,
                                 const char * const name,
                                 SbBool embed,
                                 SoXtFullViewer::BuildFlag flag,
                                 SoXtViewer::Type type,
                                 SbBool build)
  : inherited(parent, name, embed, flag, type, FALSE)
{
  PRIVATE(this) = new SoXtPlaneViewerP(this);
  PRIVATE(this)->constructor(build);
}

// Documented in common/viewers/SoGuiPlaneViewer.cpp.in.
SoXtPlaneViewer::~SoXtPlaneViewer()
{
  delete PRIVATE(this);
}

// ************************************************************************

/*!
  FIXME: write doc
*/
Widget
SoXtPlaneViewer::buildWidget(Widget parent)
{
  // FIXME: is there _really_ nothing to do here? See for instance
  // what is done in SoQtPlaneViewer. 20020113 mortene.
  return inherited::buildWidget(parent);
}

// *************************************************************************

/*!
  FIXME: write doc
*/
void
SoXtPlaneViewer::setCamera(SoCamera * camera)
{
  Pixmap pixmap, pixmap_ins;
  if (camera == NULL) {
    // find better pixmaps for this...
    pixmap = PRIVATE(this)->pixmaps.ortho;
    pixmap_ins = PRIVATE(this)->pixmaps.ortho_ins;
  } else if (camera->isOfType(SoPerspectiveCamera::getClassTypeId())) {
    pixmap = PRIVATE(this)->pixmaps.perspective;
    pixmap_ins = PRIVATE(this)->pixmaps.perspective_ins;
    SoXtResource rsc(this->getRightWheelLabelWidget());
    char * dollyString = NULL;
    rsc.getResource("dollyString", XmRString, dollyString);
    if (dollyString != NULL)
      this->setRightWheelString(dollyString);
  } else if (camera->isOfType(SoOrthographicCamera::getClassTypeId())) {
    pixmap = PRIVATE(this)->pixmaps.ortho;
    pixmap_ins = PRIVATE(this)->pixmaps.ortho_ins;
    SoXtResource rsc(this->getRightWheelLabelWidget());
    char * zoomString = NULL;
    rsc.getResource("zoomString", XmRString, zoomString);
    if (zoomString != NULL)
      this->setRightWheelString(zoomString);
  } else {
    SoDebugError::postWarning("SoXtExaminerViewer::setCamera",
      "unknown camera type - got no pixmap");
    // find better pixmaps for this...
    pixmap = PRIVATE(this)->pixmaps.ortho;
    pixmap_ins = PRIVATE(this)->pixmaps.ortho_ins;
  }

#if HAVE_LIBXPM
  XtUnmanageChild(PRIVATE(this)->buttons[3].bwidget);
  XtVaSetValues(PRIVATE(this)->buttons[3].bwidget,
    XmNlabelType, XmPIXMAP,
    XmNlabelPixmap, pixmap,
    XmNselectPixmap, pixmap,
    XmNlabelInsensitivePixmap, pixmap_ins,
    XmNselectInsensitivePixmap, pixmap_ins,
    NULL);
  XtVaSetValues(PRIVATE(this)->buttons[3].bwidget,
    XmNwidth, 30, XmNheight, 30, NULL);
  XtManageChild(PRIVATE(this)->buttons[3].bwidget);
#endif // HAVE_LIBXPM

  inherited::setCamera(camera);
}

// *************************************************************************

/*!
  Overloads to add the x, y, z, and camera-toggle buttons to the list of
  viewer buttons.
*/
void
SoXtPlaneViewer::createViewerButtons(Widget parent,
                                     SbPList * buttonlist)
{
  inherited::createViewerButtons(parent, buttonlist);

  const int buttons =
    sizeof(SoXtPlaneViewerP::SoXtPlaneViewerButtons) / sizeof(SoXtViewerButton);

  for (int button = 0; button < buttons; button++) {
    Widget widget = 
      XtVaCreateManagedWidget(PRIVATE(this)->buttons[button].keyword,
                              xmPushButtonWidgetClass, parent,
                              XmNshadowType, XmSHADOW_OUT,
                              XmNhighlightThickness, 0,
                              XmNshadowThickness, 2,
                              XmNtraversalOn, False,
                              XmNwidth, 30,
                              XmNheight, 30,
                              XtVaTypedArg,
                              XmNlabelString, XmRString,
                              PRIVATE(this)->buttons[button].label, strlen(PRIVATE(this)->buttons[button].label) + 1,
                              NULL);
    PRIVATE(this)->buttons[button].bwidget = widget;

#if HAVE_LIBXPM
    Pixmap pixmap = SoXtInternal::createPixmapFromXpm(widget, PRIVATE(this)->buttons[button].xpm_data);
    if (pixmap) {
      XtVaSetValues(widget,
                    XmNlabelType, XmPIXMAP,
                    XmNlabelPixmap, pixmap,
                    NULL);
    }
#endif

    buttonlist->append(widget);

    XtAddCallback(widget, XmNactivateCallback,
                  SoXtPlaneViewerP::buttonCB, (XtPointer) this);
  } // for (button < buttons)

#if HAVE_LIBXPM
  PRIVATE(this)->pixmaps.ortho =
    SoXtInternal::createPixmapFromXpm(PRIVATE(this)->buttons[3].bwidget, ortho_xpm);
  PRIVATE(this)->pixmaps.ortho_ins =
    SoXtInternal::createPixmapFromXpm(PRIVATE(this)->buttons[3].bwidget, ortho_xpm, TRUE);
  PRIVATE(this)->pixmaps.perspective =
    SoXtInternal::createPixmapFromXpm(PRIVATE(this)->buttons[3].bwidget, perspective_xpm);
  PRIVATE(this)->pixmaps.perspective_ins =
    SoXtInternal::createPixmapFromXpm(PRIVATE(this)->buttons[3].bwidget, perspective_xpm, TRUE);
#endif // HAVE_LIBXPM
}

// *************************************************************************

/*!
  FIXME: write doc
*/
void
SoXtPlaneViewer::openViewerHelpCard(void)
{
  this->openHelpCard("SoXtPlaneViewer.help");
}

// *************************************************************************

/*!
  This method creates the preferences sheet widgets for the Plane Viewer
  prefereences window.
*/
void
SoXtPlaneViewer::createPrefSheet(void)
{
  if (! this->prefshell) {
    PRIVATE(this)->prefparts = new Widget [ 16 ];
    this->createPrefSheetShellAndForm(this->prefshell, this->prefsheet);
    this->createDefaultPrefSheetParts(PRIVATE(this)->prefparts,
                                      PRIVATE(this)->numprefparts,
                                      this->prefsheet);
    // add parts specific for derived viewer
  }
  this->layoutPartsAndMapPrefSheet(PRIVATE(this)->prefparts,
                                   PRIVATE(this)->numprefparts,
                                   this->prefsheet, this->prefshell);
}

// *************************************************************************
