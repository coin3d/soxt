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

#include <Xm/Xm.h>
#include <Xm/Label.h>

#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/widgets/SoXtThumbWheel.h>
#include <Inventor/Xt/widgets/SoXtPopupMenu.h>
#include <Inventor/Xt/viewers/SoXtWalkViewer.h>

// *************************************************************************

/*!
  \class SoXtWalkViewer Inventor/Xt/viewers/SoXtWalkViewer.h
  \brief The SoXtWalkViewer class is a viewer for giving a "walk"-like
  experience in the scene.
  \ingroup components viewers

  FIXME: proper class doc. 20020117 mortene.
*/

// *************************************************************************

SOXT_OBJECT_SOURCE(SoXtWalkViewer);

// *************************************************************************

/*!
  Public constructor.
*/
SoXtWalkViewer::SoXtWalkViewer(Widget parent,
                               const char * name,
                               SbBool embed,
                               SoXtFullViewer::BuildFlag flag,
                               SoXtViewer::Type type)
  : inherited(parent, name, embed, flag, type, FALSE)
{
  this->constructor(TRUE);
}

/*!
  Protected constructor for derived classes.
*/
SoXtWalkViewer::SoXtWalkViewer(Widget parent,
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
SoXtWalkViewer::constructor(SbBool build)
{
  this->prefparts = NULL;
  this->prefshell = NULL;
  this->prefsheet = NULL;
  this->numprefparts = 0;
  this->heightwheel = NULL;
  this->heightvalue = 0.0f;

  this->setClassName(this->getDefaultWidgetName());
  if (build) {
    Widget viewer = this->buildWidget(this->getParentWidget());
    this->setBaseWidget(viewer);
    this->fitSize(SbVec2s(500, 300));
    // resources
  }
}

/*!
  Destructor.
*/
SoXtWalkViewer::~SoXtWalkViewer()
{
}

// *************************************************************************

// Documented in superclass.
void
SoXtWalkViewer::setViewing(SbBool enable)
{
  inherited::setViewing(enable);
}

// Documented in superclass.
void
SoXtWalkViewer::setCamera(SoCamera * camera)
{
  inherited::setCamera(camera);
}

// Documented in superclass.
void
SoXtWalkViewer::setCursorEnabled(SbBool enable)
{
  inherited::setCursorEnabled(enable);
}

// Documented in superclass.
void
SoXtWalkViewer::setCameraType(SoType type)
{
  SOXT_STUB();
}

// *************************************************************************

// Documented in superclass.
const char *
SoXtWalkViewer::getDefaultWidgetName(void) const
{
  static const char defaultWidgetName[] = "SoXtWalkViewer";
  return defaultWidgetName;
}

// Documented in superclass.
const char *
SoXtWalkViewer::getDefaultTitle(void) const
{
  static const char defaultTitle[] = "Walk Viewer";
  return defaultTitle;
}

// Documented in superclass.
const char *
SoXtWalkViewer::getDefaultIconTitle(void) const
{
  static const char defaultIconTitle[] = "Walk Viewer";
  return defaultIconTitle;
}

// *************************************************************************

// Documented in superclass.
void
SoXtWalkViewer::processEvent(XAnyEvent * event)
{
  if (this->isViewing() && event->type == ButtonPress) {
    XButtonEvent * bevent = (XButtonEvent *) event;
    if (bevent->button == 3 && this->isPopupMenuEnabled()) {
      if (! this->prefmenu)
        this->buildPopupMenu();
      this->prefmenu->popUp(this->getParentWidget(),
                            bevent->x_root, bevent->y_root);
      return;
    }
  }

  inherited::processEvent(event);
}

// Documented in superclass.
void
SoXtWalkViewer::setSeekMode(SbBool enable)
{
  inherited::setSeekMode(enable);
}

// Documented in superclass.
void
SoXtWalkViewer::actualRedraw(void)
{
  inherited::actualRedraw();
  // add walk viewer gfx here
}

// Documented in superclass. Hooks up the right wheel to dolly the
// camera back and forth.
void
SoXtWalkViewer::rightWheelMotion(float value)
{
  this->dollyCamera(value - this->getRightWheelValue());
  inherited::rightWheelMotion(value);
}

// *************************************************************************

// Documented in superclass.
Widget
SoXtWalkViewer::buildLeftTrim(Widget parent)
{
  Widget form = inherited::buildLeftTrim(parent);

//  this->tiltwheel = this->getLeftThumbWheel();

  Widget label = XtVaCreateManagedWidget("label",
                                         xmLabelWidgetClass, form,
                                         XmNleftAttachment, XmATTACH_FORM,
                                         XmNrightAttachment, XmATTACH_FORM,
                                         XmNbottomAttachment, XmATTACH_WIDGET,
                                         XmNbottomWidget, this->leftWheel,
                                         XtVaTypedArg,
                                         XmNlabelString, XmRString,
                                         "H", 2,
                                         NULL);

  this->heightwheel = XtVaCreateManagedWidget("heightwheel",
                                              soxtThumbWheelWidgetClass, form,
                                              XmNleftAttachment, XmATTACH_FORM,
                                              XmNleftOffset, 2,
                                              XmNrightAttachment, XmATTACH_FORM,
                                              XmNrightOffset, 2,
                                              XmNbottomAttachment, XmATTACH_WIDGET,
                                              XmNbottomWidget, label,
                                              XmNheight, 90,
                                              XmNorientation, XmVERTICAL,
                                              NULL);

  XtAddCallback(this->heightwheel, XmNarmCallback,
                SoXtWalkViewer::wheelarmedCB, (XtPointer) this);
  XtAddCallback(this->heightwheel, XmNdisarmCallback,
                SoXtWalkViewer::wheeldisarmedCB, (XtPointer) this);
  XtAddCallback(this->heightwheel, XmNvalueChangedCallback,
                SoXtWalkViewer::wheelchangedCB, (XtPointer) this);

  return form;
}

// Documented in superclass.
void
SoXtWalkViewer::createPrefSheet(void)
{
  if (! this->prefshell) {
    this->prefparts = new Widget [ 16 ];
    this->createPrefSheetShellAndForm(this->prefshell, this->prefsheet);
    this->createDefaultPrefSheetParts(this->prefparts, this->numprefparts,
                                      this->prefsheet);
    this->prefparts[this->numprefparts] =
      this->createFramedSpeedPrefSheetGuts(this->prefsheet);
    if (this->prefparts[this->numprefparts] != NULL) this->numprefparts++;
  }
  this->layoutPartsAndMapPrefSheet(this->prefparts, this->numprefparts,
                                   this->prefsheet, this->prefshell);
}

// Documented in superclass.
void
SoXtWalkViewer::openViewerHelpCard(void)
{
  this->openHelpCard("SoXtWalkViewer.help");
}

// *************************************************************************

/*!
  FIXME: write doc.
*/
void
SoXtWalkViewer::leftWheel2Start(void)
{
}

/*!
  FIXME: write doc.
*/
void
SoXtWalkViewer::leftWheel2Motion(float value)
{
  this->elevateCamera(value - this->getLeftWheel2Value());
  this->heightvalue = value;
}

/*!
  FIXME: write doc.
*/
void
SoXtWalkViewer::leftWheel2Finish(void)
{
}

/*!
  FIXME: write doc.
*/
float
SoXtWalkViewer::getLeftWheel2Value(void) const
{
  return this->heightvalue;
}

/*!
  FIXME: write doc
*/
void
SoXtWalkViewer::setLeftWheel2String(char * str)
{
  SOXT_STUB();
}

/*!
  FIXME: write doc.
*/
void
SoXtWalkViewer::wheelarmed(void)
{
  this->leftWheel2Start();
}

// static callback
void
SoXtWalkViewer::wheelarmedCB(Widget, XtPointer closure, XtPointer)
{
  SoXtWalkViewer * viewer = (SoXtWalkViewer *) closure;
  viewer->wheelarmed();
}

// private
void
SoXtWalkViewer::wheelchanged(float value)
{
  this->leftWheel2Motion(value);
}

// static callback
void
SoXtWalkViewer::wheelchangedCB(Widget, XtPointer closure, XtPointer call_data)
{
  SoXtWalkViewer * viewer = (SoXtWalkViewer *) closure;
  SoXtThumbWheelCallbackData * data = (SoXtThumbWheelCallbackData *) call_data;
  viewer->wheelchanged(data->current);
}

// private
void
SoXtWalkViewer::wheeldisarmed(void)
{
  this->leftWheel2Finish();
}

// static callback
void
SoXtWalkViewer::wheeldisarmedCB(Widget, XtPointer closure, XtPointer)
{
  SoXtWalkViewer * viewer = (SoXtWalkViewer *) closure;
  viewer->wheeldisarmed();
}

// *************************************************************************
