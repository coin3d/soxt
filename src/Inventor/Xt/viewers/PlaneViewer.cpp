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

// *************************************************************************

// Class is documented in common/viewers/SoGuiPlaneViewer.cpp.in.

// *************************************************************************

#ifdef HAVE_CONFIG_H
#include <config.h> // for HAVE_LIBXPM
#endif // HAVE_CONFIG_H

#include <string.h>

#include <X11/keysym.h>

#include <Xm/PushB.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtInternal.h>
#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtResource.h>
#include <Inventor/Xt/widgets/XtNativePopupMenu.h>

#include <Inventor/Xt/viewers/SoXtPlaneViewer.h>
#include <Inventor/Xt/viewers/SoXtPlaneViewerP.h>

#if HAVE_LIBXPM
#include <Inventor/Xt/common/pixmaps/ortho.xpm>
#include <Inventor/Xt/common/pixmaps/perspective.xpm>
#include <Inventor/Xt/common/pixmaps/x.xpm>
#include <Inventor/Xt/common/pixmaps/y.xpm>
#include <Inventor/Xt/common/pixmaps/z.xpm>
#else // !HAVE_LIBXPM
static const char ** ortho_xpm = NULL;
static const char ** perspective_xpm = NULL;
static const char ** x_xpm = NULL;
static const char ** y_xpm = NULL;
static const char ** z_xpm = NULL;
#endif // !HAVE_LIBXPM

// ************************************************************************

#ifndef DOXYGEN_SKIP_THIS

struct SoXtViewerButton {
  const char * keyword;
  const char * label;
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

  const int numbuttons = sizeof(SoXtPlaneViewerButtons) / sizeof(SoXtViewerButton);
  this->buttons = new SoXtViewerButton[numbuttons];
  memcpy(this->buttons, SoXtPlaneViewerButtons, sizeof(SoXtPlaneViewerButtons));

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
  const int numbuttons = sizeof(SoXtPlaneViewerButtons) / sizeof(SoXtViewerButton);
  for (int button = 0; button < numbuttons; button++) {
    if (this->buttons[button].bwidget == widget) return button;
  }
  return -1;
}

#endif // DOXYGEN_SKIP_THIS

// ************************************************************************

// Documented in superclass.
Widget
SoXtPlaneViewer::buildWidget(Widget parent)
{
  // FIXME: is there _really_ nothing to do here? See for instance
  // what is done in SoQtPlaneViewer. 20020113 mortene.
  return inherited::buildWidget(parent);
}

// *************************************************************************

// Documented in superclass.
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
    char * zoomString = NULL;
    if (
        (rsc.getResource("dollyString", XmRString, dollyString) && dollyString != NULL)
        &&
        (rsc.getResource("zoomString", XmRString, zoomString) && zoomString != NULL)
        && strcmp(zoomString,this->getRightWheelString())== 0
        )
      this->setRightWheelString(dollyString);
  } else if (camera->isOfType(SoOrthographicCamera::getClassTypeId())) {
    pixmap = PRIVATE(this)->pixmaps.ortho;
    pixmap_ins = PRIVATE(this)->pixmaps.ortho_ins;
    SoXtResource rsc(this->getRightWheelLabelWidget());
    char * dollyString = NULL;
    char * zoomString = NULL;
    if (
        (rsc.getResource("dollyString", XmRString, dollyString) && dollyString != NULL)
        &&
        (rsc.getResource("zoomString", XmRString, zoomString) && zoomString != NULL)
        && strcmp(dollyString,this->getRightWheelString())== 0
        )
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

// Documented in superclass. Overridden to add the x, y, z, and
// camera-toggle buttons to the list of viewer buttons.
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

#undef PRIVATE
#undef PUBLIC
