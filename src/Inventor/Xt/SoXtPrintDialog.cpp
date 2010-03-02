/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#if SOXT_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOXT_DEBUG

#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>

#include <Inventor/Xt/SoXtPrintDialog.h>

// *************************************************************************

/*!
  \class SoXtPrintDialog Inventor/Xt/SoXtPrintDialog.h
  \brief The SoXtPrintDialog class is yet to be documented.
  \ingroup components

  This class is not implemented.
*/

// *************************************************************************

SOXT_OBJECT_SOURCE(SoXtPrintDialog);

// *************************************************************************

/*!
  \var SoNode * SoXtPrintDialog::rootNode
  FIXME: write doc
*/

/*!
  \var SoPath * SoXtPrintDialog::rootPath
  FIXME: write doc
*/

/*!
  \var SbBool SoXtPrintDialog::printDone
  FIXME: write doc
*/

/*!
  \var SbBool SoXtPrintDialog::highQuality
  FIXME: write doc
*/

/*!
  \var SbBool SoXtPrintDialog::portraitFormat
  FIXME: write doc
*/

/*!
  \var SbBool SoXtPrintDialog::printerOutput
  FIXME: write doc
*/

/*!
  \var SbBool SoXtPrintDialog::postScriptOutput
  FIXME: write doc
*/

/*!
  \var SbBool SoXtPrintDialog::nodeMostRecent
  FIXME: write doc
*/

/*!
  \var SbBool SoXtPrintDialog::WYSIWYGflag
  FIXME: write doc
*/

/*!
  \var Widget SoXtPrintDialog::messageWidget
  FIXME: write doc
*/

/*!
  \var Widget SoXtPrintDialog::printButton
  FIXME: write doc
*/

/*!
  \var Widget SoXtPrintDialog::quitButton
  FIXME: write doc
*/

/*!
  \var Widget SoXtPrintDialog::messageLabelWidget
  FIXME: write doc
*/

/*!
  \var Widget SoXtPrintDialog::fileFormatWidget
  FIXME: write doc
*/

/*!
  \var Widget SoXtPrintDialog::toPrinterWidget
  FIXME: write doc
*/

/*!
  \var Widget SoXtPrintDialog::toPostScriptFileWidget
  FIXME: write doc
*/

/*!
  \var Widget SoXtPrintDialog::toRGBFileWidget
  FIXME: write doc
*/

/*!
  \var Widget SoXtPrintDialog::printerHorizSize
  FIXME: write doc
*/

/*!
  \var Widget SoXtPrintDialog::printerVertSize
  FIXME: write doc
*/

/*!
  \var Widget SoXtPrintDialog::postScriptHorizSize
  FIXME: write doc
*/

/*!
  \var Widget SoXtPrintDialog::postScriptVertSize
  FIXME: write doc
*/

/*!
  \var Widget SoXtPrintDialog::rgbHorizSize
  FIXME: write doc
*/

/*!
  \var Widget SoXtPrintDialog::rgbVertSize
  FIXME: write doc
*/

/*!
  \var Widget SoXtPrintDialog::printerDPIField
  FIXME: write doc
*/

/*!
  \var Widget SoXtPrintDialog::postScriptDPIField
  FIXME: write doc
*/

/*!
  \var Widget SoXtPrintDialog::rgbFilenameWidget
  FIXME: write doc
*/

/*!
  \var Widget SoXtPrintDialog::postScriptFilenameWidget
  FIXME: write doc
*/

/*!
  \var SbPList * SoXtPrintDialog::printers
  FIXME: write doc
*/

/*!
  \var char * SoXtPrintDialog::defaultPrinter
  FIXME: write doc
*/

/*!
  \var int SoXtPrintDialog::whichPrinter
  FIXME: write doc
*/

/*!
  \var SbVec2f SoXtPrintDialog::printSize
  FIXME: write doc
*/

/*!
  \var SbVec2s SoXtPrintDialog::printRes
  FIXME: write doc
*/

/*!
  \var SoCallbackList SoXtPrintDialog::beforeList
  FIXME: write doc
*/

/*!
  \var SoCallbackList SoXtPrintDialog::afterList
  FIXME: write doc
*/

/*!
  \var SbBool SoXtPrintDialog::alreadyUpdated
  FIXME: write doc
*/

/*!
  \var Widget SoXtPrintDialog::styleButton
  FIXME: write doc
*/

/*!
  \var int SoXtPrintDialog::currentStyle
  FIXME: write doc
*/

// *************************************************************************

/*!
  Constructor.
*/

SoXtPrintDialog::SoXtPrintDialog(
  Widget parent,
  const char * const name,
  SbBool embed)
: inherited(parent, name, embed)
{
  this->constructor(TRUE);
} // SoXtPrintDialog()

/*!
  Constructor.
*/

SoXtPrintDialog::SoXtPrintDialog(
  SoGLRenderAction * const action,
  Widget parent,
  const char * const name,
  SbBool embed)
: inherited(parent, name, embed)
{
  this->constructor(TRUE);
} // SoXtPrintDialog()

/*!
  Constructor.
*/

SoXtPrintDialog::SoXtPrintDialog(// protected
  Widget parent,
  const char * const name,
  SbBool embed,
  SbBool build)
: inherited(parent, name, embed)
{
  this->constructor(build);
} // SoXtPrintDialog()

/*!
  Common constructor code.
*/

void
SoXtPrintDialog::constructor(// private
  SbBool build)
{
  SOXT_STUB();
} // constructor()

/*!
  Destructor.
*/

SoXtPrintDialog::~SoXtPrintDialog(
  void)
{
} // ~SoXtPrintDialog()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::setSceneGraph(
  SoPath * path)
{
} // setSceneGraph()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::setSceneGraph(
  SoNode * root)
{
} // setSceneGraph()

/*!
  FIXME: write doc
*/

SoNode *
SoXtPrintDialog::getSceneGraph(
  void)
{
  return (SoNode *) NULL;
} // getSceneGraph()

/*!
  FIXME: write doc
*/

SoPath *
SoXtPrintDialog::getSceneGraphPath(
  void)
{
  return (SoPath *) NULL;
} // getSceneGraphPath()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::setGLRenderAction(
  const SoGLRenderAction * const action)
{
} // setGLRenderAction()

/*!
  FIXME: write doc
*/

SoGLRenderAction *
SoXtPrintDialog::getGLRenderAction(
  void)
{
  return (SoGLRenderAction *) NULL;
} // getGLRenderAction()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::setPrintSize(
  const SbVec2f & size)
{
} // setPrintSize()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::setPrintSize(
  const SbVec2s size)
{
} // setPrintSize()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::setBeforePrintCallback(
  SoXtPrintDialogCB * callback,
  void * user)
{
} // setBeforePrintCallback()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::setAfterPrintCallback(
  SoXtPrintDialogCB * callback,
  void * user)
{
} // setAfterPrintCallback()

// *************************************************************************

/*!
  FIXME: write doc
*/

const char *
SoXtPrintDialog::getDefaultWidgetName(// virtual, protected
  void) const
{
  static const char defaultWidgetName[] = "SoXtPrintDialog";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
  FIXME: write doc
*/

const char *
SoXtPrintDialog::getDefaultTitle(// virtual, protected
  void) const
{
  static const char defaultTitle[] = "Print";
  return defaultTitle;
} // getDefaultTitle()

/*!
  FIXME: write doc
*/

const char *
SoXtPrintDialog::getDefaultIconTitle(// virtual, protected
  void) const
{
  static const char defaultIconTitle[] = "Print";
  return defaultIconTitle;
} // getDefaultIconTitle()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::print(// protected
  void)
{
} // print()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::getPrinterList(// protected
  void)
{
} // getPrinterList()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::printToPostScript(// protected
  SoNode * node,
  char *,
  int,
  int)
{
} // printToPostScript()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::buildToPrinterWidget(// protected
  Widget parent)
{
} // buildToPrinterWidget()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::buildToPostScriptFileWidget(// protected
  Widget parent)
{
} // buildToPostScriptFileWidget()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::buildToRGBFileWidget(// protected
  Widget parent)
{
} // buildToRGBFileWidget()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::placeBottomOfDialog(// protected
  SoXtPrintDialog *)
{
} // placeBottomOfdialog()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::buildRadioButton(// protected
  char *,
  char *,
  char *,
  int,
  int,
  Widget,
  XtCallbackProc)
{
} // buildRadioButton()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::buildSizeFields(// protected
  char *,
  int,
  Widget,
  Widget &,
  Widget &,
  XtCallbackProc,
  XtCallbackProc)
{
} // buildSizeFields()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::buildDPIField(// protected
  int,
  Widget,
  Widget &,
  XtCallbackProc)
{
} // buildDPIField()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::updateTextports(// protected
  void)
{
} // updateTextports()

// *************************************************************************

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::qualityCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct *)
{
  SOXT_STUB();
} // qualityCB()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::pageFormatCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct *)
{
  SOXT_STUB();
} // pageFormatCB()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::fileFormatCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct *)
{
  SOXT_STUB();
} // fileFormatCB()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::printerHorizSizeCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct *)
{
  SOXT_STUB();
} // printerHorizCB()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::printerVertSizeCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct *)
{
  SOXT_STUB();
} // printerVertSizeCB()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::postScriptHorizSizeCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct *)
{
  SOXT_STUB();
} // postScriptHorizSizeCB()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::postScriptVertSizeCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct *)
{
  SOXT_STUB();
} // postScriptVertSizeCB()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::rgbHorizSizeCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct *)
{
  SOXT_STUB();
} // rgbHorizSizeCB()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::rgbVertSizeCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct *)
{
  SOXT_STUB();
} // rgbVertSizeCB()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::printerDPICB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct *)
{
  SOXT_STUB();
} // printerDPICB()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::postScriptDPICB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct *)
{
  SOXT_STUB();
} // postScriptDPICB()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::outputCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct *)
{
  SOXT_STUB();
} // outputCB()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::printCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct *)
{
  SOXT_STUB();
} // printCB()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::quitCB(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct *)
{
  SOXT_STUB();
} // quitCB()

/*!
  FIXME: write doc
*/

void
SoXtPrintDialog::listPick(
  Widget,
  SoXtPrintDialog *,
  XmAnyCallbackStruct *)
{
  SOXT_STUB();
} // listPick()

// *************************************************************************

/*!
  FIXME: write doc
*/

// why private?
Widget
SoXtPrintDialog::buildWidget(// private
  Widget parent)
{
  SOXT_STUB();
  return (Widget) NULL;
} // buildWidget()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtPrintDialogRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

