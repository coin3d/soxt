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

