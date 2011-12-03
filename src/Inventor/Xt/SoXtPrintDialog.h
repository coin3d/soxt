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

// $Id$

#ifndef SOXT_PRINTDIALOG_H
#define SOXT_PRINTDIALOG_H

#include <X11/Intrinsic.h>
#include <Xm/Xm.h>

#include <Inventor/misc/SoCallbackList.h>

#include <Inventor/Xt/SoXtComponent.h>

class SoGLRenderAction;
class SoXtPrintDialog;
class SoPath;
class SoNode;

// *************************************************************************

typedef void SoXtPrintDialogCB(void * userdata, SoXtPrintDialog * dialog);

class SOXT_DLL_API SoXtPrintDialog : public SoXtComponent {
  SOXT_OBJECT_HEADER(SoXtPrintDialog, SoXtComponent);

public:
  SoXtPrintDialog(
    Widget parent = NULL,
    const char * const name = NULL,
    SbBool embed = TRUE);
  SoXtPrintDialog(
    SoGLRenderAction * const action,
    Widget parent = NULL,
    const char * const name = NULL,
    SbBool embed = TRUE);
  ~SoXtPrintDialog(void);

  void setSceneGraph(SoPath * path);
  void setSceneGraph(SoNode * root);

  SoNode * getSceneGraph(void);
  SoPath * getSceneGraphPath(void);

  void setGLRenderAction(const SoGLRenderAction * const action);
  SoGLRenderAction * getGLRenderAction(void);

  void setPrintSize(const SbVec2f & size);
  void setPrintSize(const SbVec2s size);

  void setBeforePrintCallback(
    SoXtPrintDialogCB * callback, void * user = NULL);
  void setAfterPrintCallback(
    SoXtPrintDialogCB * callback, void * user = NULL);

protected:
  SoXtPrintDialog(
    Widget parent,
    const char * const name,
    SbBool embed,
    SbBool build);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  SoNode * rootNode;
  SoPath * rootPath;
  SbBool printDone;
  SbBool highQuality;
  SbBool portraitFormat;
  SbBool printerOutput, postScriptOutput;
  SbBool nodeMostRecent;
  SbBool WYSIWYGflag;
  Widget messageWidget, printButton, quitButton;
  Widget messageLabelWidget, fileFormatWidget;
  Widget toPrinterWidget, toPostScriptFileWidget, toRGBFileWidget;
  Widget printerHorizSize, printerVertSize;
  Widget postScriptHorizSize, postScriptVertSize;
  Widget rgbHorizSize, rgbVertSize;
  Widget printerDPIField, postScriptDPIField;
  Widget rgbFilenameWidget, postScriptFilenameWidget;
  SbPList * printers;
  char * defaultPrinter;
  int whichPrinter;
  SbVec2f printSize;
  SbVec2s printRes;
  SoCallbackList beforeList, afterList;
  SbBool alreadyUpdated;

  void print(void);
  void getPrinterList(void);
  void printToPostScript(SoNode *, char *, int, int);

  void buildToPrinterWidget(Widget parent);
  void buildToPostScriptFileWidget(Widget parent);
  void buildToRGBFileWidget(Widget parent);
  void placeBottomOfDialog(SoXtPrintDialog *);
  void buildRadioButton(char *,  char *, char *, int, int,
    Widget, XtCallbackProc);
  void buildSizeFields(char *, int, Widget, Widget &, Widget &,
    XtCallbackProc, XtCallbackProc);
  void buildDPIField(int, Widget, Widget &, XtCallbackProc);
  void updateTextports(void);

// callbacks
  static void qualityCB(Widget, SoXtPrintDialog *, XmAnyCallbackStruct *);
  static void pageFormatCB(Widget, SoXtPrintDialog *, XmAnyCallbackStruct *);
  static void fileFormatCB(Widget, SoXtPrintDialog *, XmAnyCallbackStruct *);
  static void printerHorizSizeCB(
    Widget, SoXtPrintDialog *, XmAnyCallbackStruct *);
  static void printerVertSizeCB(
    Widget, SoXtPrintDialog *, XmAnyCallbackStruct *);
  static void postScriptHorizSizeCB(
    Widget, SoXtPrintDialog *, XmAnyCallbackStruct *);
  static void postScriptVertSizeCB(
    Widget, SoXtPrintDialog *, XmAnyCallbackStruct *);
  static void rgbHorizSizeCB(
    Widget, SoXtPrintDialog *, XmAnyCallbackStruct *);
  static void rgbVertSizeCB(Widget, SoXtPrintDialog *, XmAnyCallbackStruct *);
  static void printerDPICB(Widget, SoXtPrintDialog *, XmAnyCallbackStruct *);
  static void postScriptDPICB(
    Widget, SoXtPrintDialog *, XmAnyCallbackStruct *);
  static void outputCB(Widget, SoXtPrintDialog *, XmAnyCallbackStruct *);
  static void printCB(Widget, SoXtPrintDialog *, XmAnyCallbackStruct *);
  static void quitCB(Widget, SoXtPrintDialog *, XmAnyCallbackStruct *);
  static void listPick(Widget, SoXtPrintDialog *, XmAnyCallbackStruct *);

  Widget styleButton;
  int currentStyle;

private:
  void constructor(SbBool build);
  Widget buildWidget(Widget parent);

}; // class SoXtPrintDialog

// *************************************************************************

#endif // ! SOXT_PRINTDIALOG_H
