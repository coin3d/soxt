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

static const char rcsid[] =
  "$Id$";

#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtPrintDialog.h>

/*!
  \class SoXtPrintDialog Inventor/Xt/SoXtPrintDialog.h
  \brief The SoXtPrintDialog class is yet to be documented.
*/

// *************************************************************************

/*!
*/

SoXtPrintDialog::SoXtPrintDialog(
  Widget parent,
  const char * const name,
  SbBool inParent )
: inherited()
{
  this->constructor( TRUE );
} // SoXtPrintDialog()

/*!
*/

SoXtPrintDialog::SoXtPrintDialog(
  SoGLRenderAction * const action,
  Widget parent,
  const char * const name,
  SbBool inParent )
: inherited()
{
  this->constructor( TRUE );
} // SoXtPrintDialog()

/*!
*/

SoXtPrintDialog::SoXtPrintDialog( // protected
  Widget parent,
  const char * const name,
  SbBool inParent,
  SbBool build )
: inherited()
{
  this->constructor( build );
} // SoXtPrintDialog()

/*!
*/

void
SoXtPrintDialog::constructor( // private
  SbBool build )
{
  SOXT_STUB();
} // constructor()

/*!
*/

SoXtPrintDialog::~SoXtPrintDialog(
  void )
{
} // ~SoXtPrintDialog()

// *************************************************************************

/*!
*/

void
SoXtPrintDialog::setSceneGraph(
  SoPath * path )
{
} // setSceneGraph()

/*!
*/

void
SoXtPrintDialog::setSceneGraph(
  SoNode * root )
{
} // setSceneGraph()

/*!
*/

SoNode *
SoXtPrintDialog::getSceneGraph(
  void )
{
  return (SoNode *) NULL;
} // getSceneGraph()

/*!
*/

SoPath *
SoXtPrintDialog::getSceneGraphPath(
  void )
{
  return (SoPath *) NULL;
} // getSceneGraphPath()

/*!
*/

void
SoXtPrintDialog::setGLRenderAction(
  const SoGLRenderAction * const action )
{
} // setGLRenderAction()

/*!
*/

SoGLRenderAction *
SoXtPrintDialog::getGLRenderAction(
  void )
{
  return (SoGLRenderAction *) NULL;
} // getGLRenderAction()

/*!
*/

void
SoXtPrintDialog::setPrintSize(
  const SbVec2f & size )
{
} // setPrintSize()

/*!
*/

void
SoXtPrintDialog::setPrintSize(
  const SbVec2s size )
{
} // setPrintSize()

/*!
*/

void
SoXtPrintDialog::setBeforePrintCallback(
  SoXtPrintDialogCB * callback,
  void * user )
{
} // setBeforePrintCallback()

/*!
*/

void
SoXtPrintDialog::setAfterPrintCallback(
  SoXtPrintDialogCB * callback,
  void * user )
{
} // setAfterPrintCallback()

// *************************************************************************

/*!
*/

const char *
SoXtPrintDialog::getDefaultWidgetName( // virtual, protected
  void ) const
{
  static const char defaultWidgetName[] = "SoXtPrintDialog";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
*/

const char *
SoXtPrintDialog::getDefaultTitle( // virtual, protected
  void ) const
{
  static const char defaultTitle[] = "Print";
  return defaultTitle;
} // getDefaultTitle()

/*!
*/

const char *
SoXtPrintDialog::getDefaultIconTitle( // virtual, protected
  void ) const
{
  static const char defaultIconTitle[] = "Print";
  return defaultIconTitle;
} // getDefaultIconTitle()

// *************************************************************************

/*!
*/

void
SoXtPrintDialog::print( // protected
  void )
{
} // print()

/*!
*/

void
SoXtPrintDialog::getPrinterList( // protected
  void )
{
} // getPrinterList()

/*!
*/

void
SoXtPrintDialog::printToPostScript( // protected
  SoNode * node,
  char *,
  int,
  int )
{
} // printToPostScript()

// *************************************************************************

/*!
*/

void
SoXtPrintDialog::buildToPrinterWidget( // protected
  Widget parent )
{
} // buildToPrinterWidget()

/*!
*/

void
SoXtPrintDialog::buildToPostScriptFileWidget( // protected
  Widget parent )
{
} // buildToPostScriptFileWidget()

/*!
*/

void
SoXtPrintDialog::buildToRGBFileWidget( // protected
  Widget parent )
{
} // buildToRGBFileWidget()

/*!
*/

void
SoXtPrintDialog::placeBottomOfDialog( // protected
  SoXtPrintDialog * )
{
} // placeBottomOfdialog()

/*!
*/

void
SoXtPrintDialog::buildRadioButton( // protected
  char *,
  char *,
  char *,
  int,
  int,
  Widget,
  XtCallbackProc )
{
} // buildRadioButton()

/*!
*/

void
SoXtPrintDialog::buildSizeFields( // protected
  char *,
  int,
  Widget,
  Widget &,
  Widget &,
  XtCallbackProc,
  XtCallbackProc )
{
} // buildSizeFields()

/*!
*/

void
SoXtPrintDialog::buildDPIField( // protected
  int,
  Widget,
  Widget &,
  XtCallbackProc )
{
} // buildDPIField()

/*!
*/

void
SoXtPrintDialog::updateTextports( // protected
  void )
{
} // updateTextports()

// *************************************************************************

/*!
*/

void
SoXtPrintDialog::qualityCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct * )
{
  SOXT_STUB();
} // qualityCB()

/*!
*/

void
SoXtPrintDialog::pageFormatCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct * )
{
  SOXT_STUB();
} // pageFormatCB()

/*!
*/

void
SoXtPrintDialog::fileFormatCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct * )
{
  SOXT_STUB();
} // fileFormatCB()

/*!
*/

void
SoXtPrintDialog::printerHorizSizeCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct * )
{
  SOXT_STUB();
} // printerHorizCB()

/*!
*/

void
SoXtPrintDialog::printerVertSizeCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct * )
{
  SOXT_STUB();
} // printerVertSizeCB()

/*!
*/

void
SoXtPrintDialog::postScriptHorizSizeCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct * )
{
  SOXT_STUB();
} // postScriptHorizSizeCB()

/*!
*/

void
SoXtPrintDialog::postScriptVertSizeCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct * )
{
  SOXT_STUB();
} // postScriptVertSizeCB()

/*!
*/

void
SoXtPrintDialog::rgbHorizSizeCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct * )
{
  SOXT_STUB();
} // rgbHorizSizeCB()

/*!
*/

void
SoXtPrintDialog::rgbVertSizeCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct * )
{
  SOXT_STUB();
} // rgbVertSizeCB()

/*!
*/

void
SoXtPrintDialog::printerDPICB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct * )
{
  SOXT_STUB();
} // printerDPICB()

/*!
*/

void
SoXtPrintDialog::postScriptDPICB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct * )
{
  SOXT_STUB();
} // postScriptDPICB()

/*!
*/

void
SoXtPrintDialog::outputCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct * )
{
  SOXT_STUB();
} // outputCB()

/*!
*/

void
SoXtPrintDialog::printCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct * )
{
  SOXT_STUB();
} // printCB()

/*!
*/

void
SoXtPrintDialog::quitCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct * )
{
  SOXT_STUB();
} // quitCB()

/*!
*/

void
SoXtPrintDialog::listPick(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct * )
{
  SOXT_STUB();
} // listPick()

// *************************************************************************

/*!
*/

// why private?
Widget
SoXtPrintDialog::buildWidget( // private
  Widget parent )
{
  SOXT_STUB();
  return (Widget) NULL;
} // buildWidget()

// *************************************************************************
