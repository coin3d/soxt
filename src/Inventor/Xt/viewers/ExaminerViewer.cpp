/**************************************************************************\
 *
 *  This file is part of the Coin family of 3D visualization libraries.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and / or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use this library in software that is incompatible
 *  with the GNU GPL, and / or you would like to take advantage of the
 *  additional benefits with regard to our support services, please
 *  contact Systems in Motion about acquiring a Coin Professional
 *  Edition License.  See <URL:http://www.coin3d.org> for more
 *  information.
 *
 *  Systems in Motion, Abels gate 5, Teknobyen, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

// Class documentation in common/viewers/SoGuiExaminerViewer.cpp.in.

// *************************************************************************

#ifdef HAVE_CONFIG_H
#include <config.h> // for HAVE_LIBXPM
#endif // HAVE_CONFIG_H

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
#include <Inventor/Xt/SoXtInternal.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXtResource.h>
#include <Inventor/Xt/widgets/SoXtPopupMenu.h>
#include <Inventor/Xt/widgets/SoXtThumbWheel.h>

#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewerP.h>

#if HAVE_LIBXPM
#include <Inventor/Xt/common/pixmaps/ortho.xpm>
#include <Inventor/Xt/common/pixmaps/perspective.xpm>
#endif // HAVE_LIBXPM

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->pub)

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

#undef PRIVATE
#undef PUBLIC

