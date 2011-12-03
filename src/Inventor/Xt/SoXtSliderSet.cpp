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

#if SOXT_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOXT_DEBUG

#include <X11/Xlib.h>

#include <Xm/Xm.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/ToggleB.h>
#include <Xm/PushB.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SbPList.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoMFColor.h>
#include <Inventor/fields/SoMFVec3f.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtResource.h>
#include <Inventor/Xt/widgets/SoXtSlider.h>

#include <Inventor/Xt/SoXtSliderSet.h>

// *************************************************************************

/*!
  \class SoXtSliderSet Inventor/Xt/SoXtSliderSet.h
  \brief The SoXtSliderSet class is yet to be documented.
  \ingroup components slidersets

  This class is not implemented (not compatible with InventorXt at least).
*/

// *************************************************************************

SOXT_OBJECT_ABSTRACT_SOURCE(SoXtSliderSet);

// *************************************************************************

struct SoXtSliderSetClosure {
  SoXtSliderSet * sliderset;
  SbBool folded;
  Widget f_button, f_heading;
  Widget u_button, u_style, u_heading;
  int style;
  int sliders;
  Widget * s_slider, * r_slider, * f_slider, * o_slider;
  char ** title;
  char ** field;
}; // SoXtSliderSetClosure

// *************************************************************************

/*!
  Constructor.
*/

SoXtSliderSet::SoXtSliderSet(
  const Widget parent,
  const char * const name,
  const SbBool embed,
  SoNode * const node)
: inherited(parent, name, embed, node)
{
  this->constructor(TRUE);
} // SoXtSliderSet()
  
/*!
  Constructor.
*/

SoXtSliderSet::SoXtSliderSet(// protected
  const Widget parent,
  const char * const name,
  const SbBool embed,
  SoNode * const node,
  const SbBool build)
: inherited(parent, name, embed, node)
{
  this->constructor(build);
} // SoXtSliderSet()

/*!
  Common constructor code.
*/

void
SoXtSliderSet::constructor(// private
  const SbBool build)
{
  this->closures = NULL;
  this->numSliders = 0;
  this->sliders = NULL;

  if (build) {
    this->setClassName("SoXtSliderSet");
    Widget sliderset = this->buildWidget(this->getParentWidget());
    this->setBaseWidget(sliderset);
    this->fitSize(SbVec2s(300, 0));
  }
} // constructor()

/*!
  Destructor.
*/

SoXtSliderSet::~SoXtSliderSet(// protected
  void)
{
  if (this->closures != NULL) {
    int num = this->closures->getLength();
    for (int i = 0; i < num; i++) {
      SoXtSliderSetClosure * closure =
        (SoXtSliderSetClosure *) (*this->closures)[i];
      delete [] closure->s_slider;
      delete [] closure->r_slider;
      delete [] closure->f_slider;
      delete [] closure->o_slider;
      delete [] closure->title;
      delete [] closure->field;
      delete closure;
    }
    delete closures;
  }
} // ~SoXtSliderSet()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtSliderSet::setNode(// virtual
  SoNode * node)
{
  SoNode * const prev = this->getNode();
  if (prev != NULL) {
  }
  inherited::setNode(node);
} // setNode()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtSliderSet::updateLayout(
  void)
{
  SOXT_STUB();
}

// *************************************************************************

/*!
  FIXME: write doc
*/

Widget
SoXtSliderSet::buildWidget(// protected
  Widget parent)
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtSliderSet::buildWidget",
    "widget name: %s", this->getWidgetName());
#endif // SOXT_DEBUG

  Widget sliderset = XtVaCreateManagedWidget(this->getWidgetName(),
    xmRowColumnWidgetClass, parent,
    XmNrowColumnType, XmWORK_AREA, // ugly!
    XmNorientation, XmVERTICAL,
    XmNnumColumns, 1,
    XmNpacking, XmNO_PACKING,
     // XmNO_PACKING, XmPACK_TIGHT, XmPACK_COLUMN, XmPACK_NONE
    XmNspacing, 2,
    XmNmarginWidth, 1,
    XmNmarginHeight, 1,
    XmNbackground, 0,
    NULL);

  this->registerWidget(sliderset);

  char * value;
  SoXtResource rsc(sliderset);
  if (rsc.getResource("sliderset1", XmRString, value)) {
#if SOXT_DEBUG && 0
    SoDebugError::postInfo("SoXtSliderSet::buildWidget", "slider set module");
#endif // SOXT_DEBUG
    this->buildSliderModule(sliderset);
  } else if (rsc.getResource("slider1Title", XmRString, value)) {
#if SOXT_DEBUG && 0
    SoDebugError::postInfo("SoXtSliderSet::buildWidget", "slider set");
#endif // SOXT_DEBUG
    this->buildSliderSet(sliderset);
  } else {
#if SOXT_DEBUG
    SoDebugError::post("SoXtSliderSet::buildWidget",
      "unknown slider set type");
#endif // SOXT_DEBUG
  }
  return sliderset;
} // buildWidget()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtSliderSet::buildSliderModule(
  const Widget rowcolumn)
{
  char * value = NULL;
  char resource[32];
  char name[64];
  int sliderset = 1;
  sprintf(resource, "sliderset%d", sliderset);
  SoXtResource rsc(rowcolumn);
  while (rsc.getResource(resource, XmRString, value)) {
    sprintf(name, "SoXt%sSliderModule", value);
    Widget rowcol = XtVaCreateManagedWidget(name,
      xmRowColumnWidgetClass, rowcolumn,
      XmNrowColumnType, XmWORK_AREA,
      XmNorientation, XmVERTICAL,
      XmNnumColumns, 1,
      XmNpacking, XmNO_PACKING,
      NULL);
    this->registerWidget(rowcol);
    this->buildSliderSet(rowcol);
    sliderset++;
    sprintf(resource, "sliderset%d", sliderset);
  }
} // buildSliderModule()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtSliderSet::buildSliderSet(
  const Widget rowcolumn)
{
  SoXtResource rsc(rowcolumn);

  char * title;
  if (! rsc.getResource("title", XmRString, title)) {
    SoDebugError::postInfo("SoXtSliderSet::buildSliderSet",
      "no slider set title");
    return;
  }

  SoXtSliderSetClosure * closure = new SoXtSliderSetClosure;
  if (this->closures == NULL)
    this->closures = new SbPList;
  this->closures->append(closure);

  // HEADING
  closure->sliderset = this;
  closure->folded = TRUE;
  closure->style = 0;

  closure->f_heading = XtVaCreateManagedWidget("",
    xmFormWidgetClass, rowcolumn,
    NULL);

  closure->f_button = XtVaCreateManagedWidget("f_button",
    xmToggleButtonWidgetClass, closure->f_heading,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNshadowThickness, 0,
    XmNhighlightThickness, 0,
    XmNset, False,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      title, strlen(title)+1,
    NULL);

  XtAddCallback(closure->f_button, XmNvalueChangedCallback,
    SoXtSliderSet::folding_cb, (XtPointer) closure);

  closure->u_heading = XtVaCreateManagedWidget("",
    xmFormWidgetClass, rowcolumn,
    NULL);

  closure->u_style = XtVaCreateManagedWidget("u_style",
    xmPushButtonWidgetClass, closure->u_heading,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNshadowThickness, 0,
    XmNhighlightThickness, 0,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      " Style ", sizeof(" Style ")+1,
    NULL);

  XtAddCallback(closure->u_style, XmNactivateCallback,
    SoXtSliderSet::style_cb, (XtPointer) closure);

  closure->u_button = XtVaCreateManagedWidget("u_button",
    xmToggleButtonWidgetClass, closure->u_heading,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_WIDGET,
    XmNrightWidget, closure->u_style,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNshadowThickness, 0,
    XmNhighlightThickness, 0,
    XmNset, True,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      title, strlen(title)+1,
    NULL);

  XtAddCallback(closure->u_button, XmNvalueChangedCallback,
    SoXtSliderSet::folding_cb, (XtPointer) closure);

  char * value;
  char resource[32];
  int slider = 1;
  int sliders = 1;

  sprintf(resource, "slider%dTitle", sliders);
  while (rsc.getResource(resource, XmRString, value)) {
    sliders++;
    sprintf(resource, "slider%dTitle", sliders);
  }
  closure->sliders = sliders - 1;

  closure->s_slider = new Widget[sliders];
  closure->r_slider = new Widget[sliders];
  closure->f_slider = new Widget[sliders];
  closure->o_slider = new Widget[sliders];
  closure->field    = new char * [sliders];
  closure->title    = new char * [sliders];

  sprintf(resource, "slider%dTitle", slider);
  while (rsc.getResource(resource, XmRString, value)) {
    SoXtSlider * sliderwidget = NULL;
    sliderwidget = new SoXtSlider(value);
    sliderwidget->addCallback(SoXtSliderSet::slider_cb, closure);
    closure->s_slider[slider-1] = sliderwidget->buildSimpleWidget(rowcolumn);
    closure->r_slider[slider-1] = sliderwidget->buildRangedWidget(rowcolumn);
    closure->f_slider[slider-1] = sliderwidget->buildFullWidget(rowcolumn);
    closure->o_slider[slider-1] = sliderwidget->buildOrderlyWidget(rowcolumn);

    if (rsc.getResource(resource, XmRString, value)) {
      closure->title[slider-1] = strcpy(new char [strlen(value)+1], value);
    } else {
      closure->title[slider-1] = NULL;
    }

    sprintf(resource, "slider%dField", slider);
    if (rsc.getResource(resource, XmRString, value)) {
      closure->field[slider-1] = strcpy(new char [strlen(value)+1], value);
    } else {
      closure->field[slider-1] = NULL;
    }
    if (closure->folded)
      XtUnmanageChild(closure->s_slider[slider-1]);
    XtUnmanageChild(closure->r_slider[slider-1]);
    XtUnmanageChild(closure->f_slider[slider-1]);
    XtUnmanageChild(closure->o_slider[slider-1]);

    slider++;
    sprintf(resource, "slider%dTitle", slider);
  }

  if (closure->folded) {
    XtUnmanageChild(closure->u_heading);
  } else {
    XtUnmanageChild(closure->f_heading);
  }
} // buildSliderModule()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtSliderSet::folding(// private
  SoXtSliderSetClosure * closure,
  Widget button)
{
  assert(closure != NULL);
  if (XtParent(button) == closure->f_heading) {
    XtUnmanageChild(closure->f_heading);
    XtManageChild(closure->u_heading);
    XtVaSetValues(button, XmNset, False, NULL);
    for (int i = 0; i < closure->sliders; i++) {
      switch (closure->style) {
      case 0: XtManageChild(closure->s_slider[i]); break;
      case 1: XtManageChild(closure->r_slider[i]); break;
      case 2: XtManageChild(closure->f_slider[i]); break;
      case 3: XtManageChild(closure->o_slider[i]); break;
      default:
        SoDebugError::postInfo("SoXtSliderSet::folding", "invalid style");
        return;
      }
    }
  } else if (XtParent(button) == closure->u_heading) {
    XtUnmanageChild(closure->u_heading);
    XtManageChild(closure->f_heading);
    XtVaSetValues(button, XmNset, True, NULL);
    for (int i = 0; i < closure->sliders; i++) {
      switch (closure->style) {
      case 0: XtUnmanageChild(closure->s_slider[i]); break;
      case 1: XtUnmanageChild(closure->r_slider[i]); break;
      case 2: XtUnmanageChild(closure->f_slider[i]); break;
      case 3: XtUnmanageChild(closure->o_slider[i]); break;
      default:
        SoDebugError::postInfo("SoXtSliderSet::folding", "invalid style");
        return;
      }
    }
  } else {
#if SOXT_DEBUG
    SoDebugError::post("SoXtSliderSet::folding", "unknown button");
#endif // SOXT_DEBUG
    return;
  }
  Widget sliderset = closure->sliderset->getBaseWidget();
  Dimension height;
  XtVaGetValues(sliderset, XmNheight, &height, NULL);
  SbVec2s orig = closure->sliderset->getSize();
  closure->sliderset->fitSize(SbVec2s(orig[0], 0));
} // folding()

/*!
  FIXME: write doc
*/

void
SoXtSliderSet::folding_cb(// private, static
  Widget button,
  XtPointer closure,
  XtPointer call_data)
{
  assert(closure != NULL);
  SoXtSliderSetClosure * info = (SoXtSliderSetClosure *) closure;
  info->sliderset->folding(info, button);
} // folding_cb()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtSliderSet::style(// private
  SoXtSliderSetClosure * closure)
{
  assert(closure != NULL);
  int i;
  switch (closure->style) {
  case 0:
    for (i = 0; i < closure->sliders; i++)
      XtUnmanageChild(closure->s_slider[i]);
    for (i = 0; i < closure->sliders; i++)
      XtManageChild(closure->r_slider[i]);
    break;
  case 1:
    for (i = 0; i < closure->sliders; i++)
      XtUnmanageChild(closure->r_slider[i]);
    for (i = 0; i < closure->sliders; i++)
      XtManageChild(closure->f_slider[i]);
    break;
  case 2:
    for (i = 0; i < closure->sliders; i++)
      XtUnmanageChild(closure->f_slider[i]);
    for (i = 0; i < closure->sliders; i++)
      XtManageChild(closure->o_slider[i]);
    break;
  case 3:
    for (i = 0; i < closure->sliders; i++)
      XtUnmanageChild(closure->o_slider[i]);
    for (i = 0; i < closure->sliders; i++)
      XtManageChild(closure->s_slider[i]);
    break;
  default:
    SoDebugError::postInfo("SoXtSliderSet::style", "invalid style");
    return;
  }
  closure->style = (closure->style + 1) % 4;
  SbVec2s orig = closure->sliderset->getSize();
  closure->sliderset->fitSize(SbVec2s(orig[0], 0));
} // style()

/*!
  FIXME: write doc
*/

void
SoXtSliderSet::style_cb(// private, static
  Widget button,
  XtPointer closure,
  XtPointer call_data)
{
  assert(closure != NULL);
  SoXtSliderSetClosure * info = (SoXtSliderSetClosure *) closure;
  info->sliderset->style(info);
} // style_cb()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtSliderSet::slider(// private
  SoXtSliderSetClosure * closure,
  char * title,
  float value)
{
  assert(closure != NULL);
  SoNode * const node = this->getNode();
  if (! node) {
#if SOXT_DEBUG && 0
    SoDebugError::postInfo("SoXtSliderSet::slider", "no node");
#endif // SOXT_DEBUG
    return;
  }
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtSliderSet::slider",
    "[%s] set to %.2g", title, value);
#endif // SOXT_DEBUG

  SbBool found = FALSE;
  for (int i = 0; i < closure->sliders; i++) {
    if (strcmp(title, closure->title[i]) != 0)
      continue;
    found = TRUE;
#if SOXT_DEBUG && 0
    SoDebugError::postInfo("", "dragging slider %d [%s] (%s)", i,
      closure->title[i], closure->field[i]);
#endif // SOXT_DEBUG

    int fieldnamelen = strlen(closure->field[i]);
    char * fieldname = strcpy(new char [fieldnamelen+1], closure->field[i]);
    int subfield = -1;
    if (fieldname[fieldnamelen-3] == '[' &&
         fieldname[fieldnamelen-1] == ']' &&
         fieldname[fieldnamelen-2] >= '0' &&
         fieldname[fieldnamelen-2] <= '9') {
      subfield = fieldname[fieldnamelen-2] - '0';
      fieldname[fieldnamelen-3] = '\0';
    }

    SoField * const field = node->getField(SbName(fieldname));
    if (! field) {
#if SOXT_DEBUG
      SoDebugError::postInfo("SoXtSliderSet::slider", "no field '%s' in node",
        fieldname);
#endif // SOXT_DEBUG
      delete [] fieldname;
      continue;
    }

    SoType fieldType = field->getTypeId();
    if (fieldType.isDerivedFrom(SoMField::getClassTypeId())) {
      if (subfield == -1) {
        if (fieldType.isDerivedFrom(SoMFFloat::getClassTypeId())) {
          SoMFFloat * somffloat = (SoMFFloat *) field;
          somffloat->set1Value(0, value);
        } else {
          SoDebugError::postInfo("SoXtSliderSet::slider",
            "unsupported field type '%s'", fieldType.getName().getString());
        }
      } else { 
        if (fieldType.isDerivedFrom(SoMFColor::getClassTypeId())) {
          SoMFColor * somfcolor = (SoMFColor *) field;
          if (subfield < 0 || subfield > 2) {
            SOXT_STUB();
            SoDebugError::postInfo("", "subfield = %d", subfield);
          } else {
            SbColor orig = (*somfcolor)[0];
            orig[subfield] = value;
            somfcolor->set1Value(0, orig);
          }
        } else if (fieldType.isDerivedFrom(SoMFVec3f::getClassTypeId())) {
          SoMFVec3f * somfvec3f = (SoMFVec3f *) field;
          if (subfield < 0 || subfield > 2) {
            SOXT_STUB();
            SoDebugError::postInfo("", "subfield = %d", subfield);
          } else {
            SbVec3f orig = (*somfvec3f)[0];
            orig[subfield] = value;
            somfvec3f->set1Value(0, orig);
          }
        } else {
          SOXT_STUB();
        }
      }
    } else if (fieldType.isDerivedFrom(SoSField::getClassTypeId())) {
      if (subfield == -1) {
        if (fieldType.isDerivedFrom(SoSFFloat::getClassTypeId())) {
          SoSFFloat * sffloat = (SoSFFloat *) field;
          *sffloat = value;
        } else {
          SOXT_STUB();
        }
      } else {
        if (fieldType.isDerivedFrom(SoSFVec3f::getClassTypeId())) {
          if (subfield < 0 || subfield > 2) {
            SOXT_STUB();
          } else {
            SoSFVec3f * sosfvec3f = (SoSFVec3f *) field;
            SbVec3f orig = sosfvec3f->getValue();
            orig[subfield] = value;
            *sosfvec3f = orig;
          }
        } else if (fieldType.isDerivedFrom(SoSFRotation::getClassTypeId())) {
          SOXT_STUB();
        } else {
          SOXT_STUB();
        }
      }
    } else {
      SOXT_STUB();
    }

    delete [] fieldname;
  }
#if SOXT_DEBUG && 0
  if (! found) {
    SoDebugError::postInfo("SoXtSliderSet::slider",
      "couldn't find field for %s", title);
  }
#endif // SOXT_DEBUG
} // slider()

/*!
  FIXME: write doc
*/

void
SoXtSliderSet::slider_cb(// static, private
  void * closure,
  char * title,
  float value)
{
  assert(closure != NULL);
  SoXtSliderSetClosure * info = (SoXtSliderSetClosure *) closure;
  info->sliderset->slider(info, title, value);
} // slider_cb()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtSliderSetRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

