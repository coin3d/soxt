/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
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
