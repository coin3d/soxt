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

// *************************************************************************

/*!
  \class SoXtPlaneViewer Inventor/Xt/viewers/SoXtPlaneViewer.h
  \brief The SoXtPlaneViewer class is an alternative to the examiner viewer.
  \ingroup components viewers
*/

// *************************************************************************

SOXT_OBJECT_SOURCE(SoXtPlaneViewer);

// *************************************************************************

struct SoXtViewerButton {
  char * keyword;
  char * label;
  void (*pressed)(Widget, XtPointer, XtPointer);
  const char ** xpm_data;
  Widget bwidget;
  Widget lwidget;
};

struct SoXtViewerButton
SoXtPlaneViewer::SoXtPlaneViewerButtons[] = {
  {
    "x", "X",
    SoXtPlaneViewer::buttonCB,
    x_xpm,
    NULL, NULL
  },
  { // plane Y button
    "y", "Y",
    SoXtPlaneViewer::buttonCB,
    y_xpm,
    NULL, NULL
  },
  { // plane Z button
    "z", "Z",
    SoXtPlaneViewer::buttonCB,
    z_xpm,
    NULL, NULL
  },
  { // camera type button
    "camera", "C",
    SoXtPlaneViewer::buttonCB,
    perspective_xpm,
    NULL, NULL
  }
}; // SoXtPlaneViewerButtons[]

// *************************************************************************

/*!
  Constructor.
*/

SoXtPlaneViewer::SoXtPlaneViewer(
  Widget parent,
  const char * const name,
  SbBool embed,
  SoXtFullViewer::BuildFlag flag,
  SoXtViewer::Type type)
: inherited(parent, name, embed, flag, type, FALSE)
{
  this->constructor(TRUE);
} // SoXtPlaneViewer()

/*!
  Protected constructor for deriving classes.
*/

SoXtPlaneViewer::SoXtPlaneViewer(// protected
  Widget parent,
  const char * const name,
  SbBool embed,
  SoXtFullViewer::BuildFlag flag,
  SoXtViewer::Type type,
  SbBool build)
: inherited(parent, name, embed, flag, type, FALSE)
{
  this->constructor(build);
} // SoXtPlaneViewer()

/*!
  Common constructor code.
*/

void
SoXtPlaneViewer::constructor(
  SbBool build)
{
  this->commonConstructor(); // generic code

  const int buttons = sizeof(SoXtPlaneViewerButtons) / sizeof(SoXtViewerButton);
  this->buttons = new SoXtViewerButton [ buttons ];
  memcpy(this->buttons, SoXtPlaneViewerButtons, sizeof(SoXtPlaneViewerButtons));

  this->prefshell = this->prefsheet = (Widget) NULL;
  this->prefparts = NULL;
  this->numprefparts = 0;

  this->setClassName(this->getDefaultWidgetName());

  if (build) {
    Widget viewer = this->buildWidget(this->getParentWidget());
    this->setBaseWidget(viewer);
    this->fitSize(SbVec2s(500, 300));

    SoXtResource rsc(this->getRightWheelLabelWidget());
    char * dollyString = NULL;
    rsc.getResource("dollyString", XmRString, dollyString);
    if (dollyString != NULL)
      this->setRightWheelString(dollyString);
  }
} // constructor()

/*!
  Destructor.
*/

SoXtPlaneViewer::~SoXtPlaneViewer(
  void)
{
  delete [] this->buttons;
} // ~SoXtPlaneViewer()

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
SoXtPlaneViewer::setViewing(// virtual
  SbBool enable)
{
  inherited::setViewing(enable);
} // setViewing()

/*!
  FIXME: write doc
*/

void
SoXtPlaneViewer::setCamera(// virtual
  SoCamera * camera)
{
  Pixmap pixmap, pixmap_ins;
  if (camera == NULL) {
    // find better pixmaps for this...
    pixmap = this->pixmaps.ortho;
    pixmap_ins = this->pixmaps.ortho_ins;
  } else if (camera->isOfType(SoPerspectiveCamera::getClassTypeId())) {
    pixmap = this->pixmaps.perspective;
    pixmap_ins = this->pixmaps.perspective_ins;
    SoXtResource rsc(this->getRightWheelLabelWidget());
    char * dollyString = NULL;
    rsc.getResource("dollyString", XmRString, dollyString);
    if (dollyString != NULL)
      this->setRightWheelString(dollyString);
  } else if (camera->isOfType(SoOrthographicCamera::getClassTypeId())) {
    pixmap = this->pixmaps.ortho;
    pixmap_ins = this->pixmaps.ortho_ins;
    SoXtResource rsc(this->getRightWheelLabelWidget());
    char * zoomString = NULL;
    rsc.getResource("zoomString", XmRString, zoomString);
    if (zoomString != NULL)
      this->setRightWheelString(zoomString);
  } else {
    SoDebugError::postWarning("SoXtExaminerViewer::setCamera",
      "unknown camera type - got no pixmap");
    // find better pixmaps for this...
    pixmap = this->pixmaps.ortho;
    pixmap_ins = this->pixmaps.ortho_ins;
  }

#if HAVE_LIBXPM
  XtUnmanageChild(this->buttons[3].bwidget);
  XtVaSetValues(this->buttons[3].bwidget,
    XmNlabelType, XmPIXMAP,
    XmNlabelPixmap, pixmap,
    XmNselectPixmap, pixmap,
    XmNlabelInsensitivePixmap, pixmap_ins,
    XmNselectInsensitivePixmap, pixmap_ins,
    NULL);
  XtVaSetValues(this->buttons[3].bwidget,
    XmNwidth, 30, XmNheight, 30, NULL);
  XtManageChild(this->buttons[3].bwidget);
#endif // HAVE_LIBXPM

  inherited::setCamera(camera);
} // setCamera()

// *************************************************************************

/*!
  Overloads to add the x, y, z, and camera-toggle buttons to the list of
  viewer buttons.
*/

void
SoXtPlaneViewer::createViewerButtons(// virtual, protected
  Widget parent,
  SbPList * buttonlist)
{
  inherited::createViewerButtons(parent, buttonlist);

  const int buttons = sizeof(SoXtPlaneViewerButtons) / sizeof(SoXtViewerButton);
  for (int button = 0; button < buttons; button++) {
    Widget widget = 
      XtVaCreateManagedWidget(this->buttons[button].keyword,
      xmPushButtonWidgetClass, parent,
      XmNshadowType, XmSHADOW_OUT,
      XmNhighlightThickness, 0,
      XmNshadowThickness, 2,
      XmNtraversalOn, False,
      XmNwidth, 30,
      XmNheight, 30,
      XtVaTypedArg,
        XmNlabelString, XmRString,
        this->buttons[button].label, strlen(this->buttons[button].label) + 1,
      NULL);
    this->buttons[button].bwidget = widget;

#if HAVE_LIBXPM
    Pixmap pixmap = SoXtInternal::createPixmapFromXpm(widget, this->buttons[button].xpm_data);
    if (pixmap)
      XtVaSetValues(widget,
        XmNlabelType, XmPIXMAP,
        XmNlabelPixmap, pixmap,
        NULL);
#endif

    buttonlist->append(widget);

    XtAddCallback(widget, XmNactivateCallback,
      SoXtPlaneViewer::buttonCB, (XtPointer) this);
  } // for (button < buttons)

#if HAVE_LIBXPM
  this->pixmaps.ortho =
    SoXtInternal::createPixmapFromXpm(this->buttons[3].bwidget, ortho_xpm);
  this->pixmaps.ortho_ins =
    SoXtInternal::createPixmapFromXpm(this->buttons[3].bwidget, ortho_xpm, TRUE);
  this->pixmaps.perspective =
    SoXtInternal::createPixmapFromXpm(this->buttons[3].bwidget, perspective_xpm);
  this->pixmaps.perspective_ins =
    SoXtInternal::createPixmapFromXpm(this->buttons[3].bwidget, perspective_xpm, TRUE);
#endif // HAVE_LIBXPM
} // createViewerButtons()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtPlaneViewer::processEvent(
  XAnyEvent * event)
{
  if (SoXtViewer::processCommonEvents(event))
    return;

  if (event->type == ButtonPress) {
    // SoDebugError::postInfo("", "button %d", ((XButtonEvent *) event)->button);
    if (((XButtonEvent *) event)->button == 3) {
      if (this->isPopupMenuEnabled()) {
        int x = ((XButtonEvent *) event)->x_root;
        int y = ((XButtonEvent *) event)->y_root;
        if (! this->prefmenu)
          this->buildPopupMenu();
        this->prefmenu->popUp(this->getGLWidget(), x, y);
        return;
      }
    }
  }

  inherited::processEvent(event);
} // processEvent()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtPlaneViewer::actualRedraw(
  void)
{
  inherited::actualRedraw();
#if 0
  if (this->mode == ROTZ_MODE)
    this->drawRotateGraphics();
#endif
} // actualRedraw()

/*!
  FIXME: write doc
*/

void
SoXtPlaneViewer::openViewerHelpCard(// virtual
  void)
{
  this->openHelpCard("SoXtPlaneViewer.help");
} // openViewerHelpCard()

// *************************************************************************

/*!
  This method set up the bottom wheel to control camera translation in the
  horizontal direction.
*/

void
SoXtPlaneViewer::bottomWheelMotion(// virtual
  float value)
{
  this->translateX(value - this->getBottomWheelValue());
  inherited::bottomWheelMotion(value);
} // bottomWheelMotion()

/*!
  This method set up the left wheel to control camera translation in the
  vertical direction.
*/

void
SoXtPlaneViewer::leftWheelMotion(// virtual
  float value)
{
  this->translateY(value - this->getLeftWheelValue());
  inherited::leftWheelMotion(value);
} // leftWheelMotion()

/*!
  This method set up the right wheel to control camera movement in the
  inwards direction.
*/

void
SoXtPlaneViewer::rightWheelMotion(// virtual
  float value)
{
  this->zoom(this->getRightWheelValue() - value);
  inherited::rightWheelMotion(value);
} // rightWheelMotion()

// *************************************************************************

/*!
  This method creates the preferences sheet widgets for the Plane Viewer
  prefereences window.
*/

void
SoXtPlaneViewer::createPrefSheet(
  void)
{
  if (! this->prefshell) {
    this->prefparts = new Widget [ 16 ];
    this->createPrefSheetShellAndForm(this->prefshell, this->prefsheet);
    this->createDefaultPrefSheetParts(this->prefparts, this->numprefparts,
      this->prefsheet);
    // add parts specific for derived viewer
  }
  this->layoutPartsAndMapPrefSheet(this->prefparts, this->numprefparts,
    this->prefsheet, this->prefshell);
} // createPrefSheet()

// *************************************************************************

/*!
  FIXME: write doc
*/

int
SoXtPlaneViewer::findButton(// private
  Widget widget) const
{
  const int buttons = sizeof(SoXtPlaneViewerButtons) / sizeof(SoXtViewerButton);
  for (int button = 0; button < buttons; button++) {
    if (this->buttons[button].bwidget == widget) return button;
  }
  return -1;
} // findButton()

/*!
  static callback for the plane viewer buttons
*/

void
SoXtPlaneViewer::buttonCB(// static, private
  Widget widget,
  XtPointer closure,
  XtPointer)
{
  assert(closure != NULL);
  SoXtPlaneViewer * viewer = (SoXtPlaneViewer *) closure;
  const int idx = viewer->findButton(widget);
  if (idx == -1) {
    SoDebugError::post("SoXtPlaneViewer::buttonCB", "unknown button");
  } else if (strcmp(viewer->buttons[idx].keyword, "x") == 0) {
    viewer->viewPlaneX();
  } else if (strcmp(viewer->buttons[idx].keyword, "y") == 0) {
    viewer->viewPlaneY();
  } else if (strcmp(viewer->buttons[idx].keyword, "z") == 0) {
    viewer->viewPlaneZ();
  } else if (strcmp(viewer->buttons[idx].keyword, "camera") == 0) {
    viewer->toggleCameraType();
  } else {
    SoDebugError::post("SoXtPlaneViewer::buttonCB", "unsupported button");
  }
} /// camerabuttonCB()

// *************************************************************************
