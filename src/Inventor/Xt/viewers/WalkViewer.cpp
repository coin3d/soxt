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

// Class is documented in common/viewers/SoGuiWalkViewer.cpp.in.

// *************************************************************************

#include <Xm/Xm.h>
#include <Xm/Label.h>

#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/widgets/SoXtThumbWheel.h>
#include <Inventor/Xt/widgets/SoXtPopupMenu.h>
#include <Inventor/Xt/viewers/SoXtWalkViewer.h>
#include <Inventor/Xt/viewers/SoGuiWalkViewerP.h>

// *************************************************************************

SOXT_OBJECT_SOURCE(SoXtWalkViewer);

// ************************************************************************

class SoXtWalkViewerP : public SoGuiWalkViewerP {
public:
  SoXtWalkViewerP(SoXtWalkViewer * o)
    : SoGuiWalkViewerP(o)
  {
  }

  void constructor(void);
};

#define PRIVATE(ptr) ptr->pimpl
#define PUBLIC(ptr) ptr->pub

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
  PRIVATE(this) = new SoXtWalkViewerP(this);
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
  PRIVATE(this) = new SoXtWalkViewerP(this);
  this->constructor(build);
}

/*!
  Common constructor code.
*/
void
SoXtWalkViewer::constructor(SbBool build)
{
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
  delete PRIVATE(this);
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
  PRIVATE(this)->dollyCamera(value - this->getRightWheelValue());
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
  PRIVATE(this)->elevateCamera(value - this->getLeftWheel2Value());
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

#undef PRIVATE
#undef PUBLIC

