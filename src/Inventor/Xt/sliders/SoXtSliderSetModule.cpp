/**************************************************************************
 *
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
 **************************************************************************/

#if SOXT_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOXT_DEBUG

#include <assert.h>

#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>

#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXtResource.h>
#include <Inventor/Xt/widgets/SoXtSlider.h>

#include <Inventor/Xt/sliders/SoXtSliderSetModule.h>

/*!
  \class SoXtSliderSetModule Inventor/Xt/sliders/SoXtSliderSetModule.h
  \brief The SoXtSliderSetModule class is yet to be documented.

  This class is internal to SoXt and may change interface without notice.
*/

// *************************************************************************

/*!
  Public constructor.
*/

SoXtSliderSetModule::SoXtSliderSetModule(
  const Widget parent,
  const char * const name,
  const SbBool embed,
  SoNode * const node )
: inherited( parent, name, embed, node )
{
  this->constructor( TRUE );
} // SoXtSliderSetModule()

/*!
  Protected constructor used by derived classes.
*/

SoXtSliderSetModule::SoXtSliderSetModule( // protected
  const Widget parent,
  const char * const name,
  const SbBool embed,
  SoNode * const node,
  const SbBool build )
: inherited( parent, name, embed, node )
{
  this->constructor( build );
} // SoXtSliderSetModule()

/*!
*/

void
SoXtSliderSetModule::constructor( // private
  const SbBool build )
{
  this->form = NULL;
  this->head = NULL;
  this->styleToggle = NULL;
  this->foldingToggle = NULL;
  this->body = NULL;

  this->numSliders = 1;
  this->sliders = NULL;
  this->folded = TRUE;

  this->setClassName( this->getDefaultWidgetName() );

  if ( build ) {
    Widget module = this->buildWidget( this->getParentWidget() );
    this->setBaseWidget( module );
  }
} // constructor()

/*!
  Destructor.
*/

SoXtSliderSetModule::~SoXtSliderSetModule( // protected
  void )
{
} // ~SoXtSliderSetModule()

// *************************************************************************

/*!
*/

int
SoXtSliderSetModule::getNumSliders(
  void ) const
{
  return this->numSliders;
} // getSliderSetSize()

/*!
*/

void
SoXtSliderSetModule::setFolding(
  const SbBool enable )
{
  SoDebugError::postInfo( "SoXtSliderSetModule::setFolding",
    "invoked (%s)", enable ? "TRUE" : "FALSE" );
  this->folded = enable;
  int i;
  if ( enable ) {
    for ( i = 0; i < this->_numSubComponents; i++ ) {
      XtManageChild( this->_subComponentArray[i]->getBaseWidget() );
    }
  } else {
    for ( i = 0; i < this->_numSubComponents; i++ ) {
      XtUnmanageChild( this->_subComponentArray[i]->getBaseWidget() );
    }
  }
  this->updateLayout();
  // update 
} // setFolding()

/*!
*/

SbBool
SoXtSliderSetModule::isFolded(
  void ) const
{
  return this->folded;
} // isFolded()

// *************************************************************************

/*!
*/

Widget
SoXtSliderSetModule::buildWidget(
  Widget parent )
{
  assert( this->sliders == NULL );

  this->form = XtVaCreateManagedWidget( this->getWidgetName(),
    xmFormWidgetClass, parent,
//    XmNleftAttachment, XmATTACH_FORM,
//    XmNtopAttachment, XmATTACH_FORM,
//    XmNrightAttachment, XmATTACH_FORM,
//    XmNbottomAttachment, XmATTACH_FORM,
    XmNbackground, BlackPixelOfScreen(XtScreen(parent)),
    NULL );
  this->registerWidget( this->form );

  this->head = XtVaCreateManagedWidget( "head",
    xmFormWidgetClass, this->form,
    XmNleftAttachment, XmATTACH_FORM,
    XmNleftOffset, 2,
    XmNtopAttachment, XmATTACH_FORM,
    XmNtopOffset, 2,
    XmNrightAttachment, XmATTACH_FORM,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_NONE,
    XmNbackground, BlackPixelOfScreen(XtScreen(this->form)),
    NULL );

  this->styleToggle = XtVaCreateManagedWidget( "style",
    xmPushButtonWidgetClass, this->head,
    XmNleftAttachment, XmATTACH_NONE,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_FORM,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNhighlightThickness, 0,
    XmNshadowType, XmSHADOW_IN,
    XmNtraversalOn, False,
    XtVaTypedArg,
      XmNlabelString, XmRString, " Style ", strlen( " Style " ) + 1,
    NULL );

  XtAddCallback( this->styleToggle, XmNactivateCallback,
    SoXtSliderSetModule::toggleStyleCB, (XtPointer) this );

  this->foldingToggle = XtVaCreateManagedWidget( "folding",
    xmToggleButtonWidgetClass, this->head,
    XmNleftAttachment, XmATTACH_FORM,
    XmNtopAttachment, XmATTACH_FORM,
    XmNrightAttachment, XmATTACH_WIDGET,
    XmNrightWidget, this->styleToggle,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNtraversalOn, False,
    XmNindicatorOn, False,
    XmNhighlightThickness, 0,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      "INTENSITY", strlen( "INTENSITY" ) + 1,
    NULL );

  XtAddCallback( this->foldingToggle, XmNvalueChangedCallback,
    SoXtSliderSetModule::toggleFoldingCB, (XtPointer) this );

  this->body = XtVaCreateManagedWidget( "body",
    xmFormWidgetClass, this->form,
    XmNleftAttachment, XmATTACH_FORM,
    XmNleftOffset, 2,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, this->head,
    XmNtopOffset, 2,
    XmNrightAttachment, XmATTACH_FORM,
    XmNrightOffset, 2,
    XmNbottomAttachment, XmATTACH_NONE,
    XmNbottomOffset, 2,
    XmNbackground, BlackPixelOfScreen(XtScreen(this->form)),
    NULL );

  this->sliders = new SoXtSlider * [this->numSliders];
  for ( int i = 0; i < this->numSliders; i++ ) {
    this->sliders[i] = new SoXtSlider( this->body, this );
    Widget slider = this->sliders[i]->getBaseWidget();
    assert( slider != NULL );
    XtVaSetValues( slider,
      XmNleftAttachment, XmATTACH_FORM,
      XmNrightAttachment, XmATTACH_FORM,
      NULL );
    if ( i == 0 ) {
      XtVaSetValues( slider,
        XmNtopAttachment, XmATTACH_FORM,
        NULL );
    } else {
      XtVaSetValues( slider,
        XmNtopAttachment, XmATTACH_WIDGET,
        XmNtopWidget, this->sliders[i-1]->getBaseWidget(),
        NULL );
    }
    if ( i == (this->numSliders - 1) ) {
      XtVaSetValues( slider,
        XmNbottomAttachment, XmATTACH_FORM,
        XmNbottomOffset, 2,
        NULL );
    }
  }

  return this->form;
} // buildWidget()

// *************************************************************************

/*!
*/

void
SoXtSliderSetModule::getLayoutSize( // virtual, protected
  int & width,
  int & height )
{
  width = 1;
  if ( this->isFolded() ) {
    height = 1;
  } else {
    height = this->getNumSliders() + 1;
  }
} // getLayoutSize()

/*!
*/

const char *
SoXtSliderSetModule::getDefaultWidgetName( // virtual
  void ) const
{
  static const char defaultWidgetName[] = "SoXtSliderSetModule";
  return defaultWidgetName;
} // getDefaultWidgetName()

// *************************************************************************

/*!
  \fn int getNumSliders(void) const = 0
*/

/*!
*/

void
SoXtSliderSetModule::valueChanged(
  float value, int slider )
{
  SOXT_STUB();
} // valueChanged()

/*!
*/

void
SoXtSliderSetModule::valueChangedCB(
  void * user,
  float value,
  int slider )
{
  assert( user != NULL );
  SoXtSliderSetModule * const module = (SoXtSliderSetModule *) user;
  module->valueChanged( value, slider );
} // valueChangedCB()

/*!
*/

void
SoXtSliderSetModule::toggleFoldingCB( // static, private
  Widget widget,
  XtPointer client_data,
  XtPointer call_data )
{
  SoXtSliderSetModule * const module = (SoXtSliderSetModule *) client_data;
  module->setFolding( XmToggleButtonGetState(widget) ? TRUE : FALSE );
} // foldingToggledCB()

/*!
*/

void
SoXtSliderSetModule::toggleStyleCB( // static, private
  Widget widget,
  XtPointer client_data,
  XtPointer call_data )
{
  SoXtSliderSetModule * const module = (SoXtSliderSetModule *) client_data;
  int i;
  for ( i = 0; i < module->numSliders; i++ )
    module->sliders[i]->changeLayoutStyle();
} // toggleStyleCB()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtSliderSetModuleRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

