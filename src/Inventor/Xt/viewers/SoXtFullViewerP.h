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

#ifndef SOXTFULLVIEWERP_H
#define SOXTFULLVIEWERP_H

#include <Inventor/SbBasic.h>
#include <Inventor/Xt/viewers/SoGuiFullViewerP.h>

class SoXtFullViewer;

// ************************************************************************

// This class contains private data and methods used within the
// SoXtFullViewer class.

class SoXtFullViewerP : public SoGuiFullViewerP
{
public:
  SoXtFullViewerP(SoXtFullViewer * publ);
  ~SoXtFullViewerP();

  static void setThumbWheelValue(void * wheel, float val);

  static void leftWheelStartCB(Widget, XtPointer, XtPointer);
  static void leftWheelMotionCB(Widget, XtPointer, XtPointer);
  static void leftWheelFinishCB(Widget, XtPointer, XtPointer);

  static void bottomWheelStartCB(Widget, XtPointer, XtPointer);
  static void bottomWheelMotionCB(Widget, XtPointer, XtPointer);
  static void bottomWheelFinishCB(Widget, XtPointer, XtPointer);

  static void rightWheelStartCB(Widget, XtPointer, XtPointer);
  static void rightWheelMotionCB(Widget, XtPointer, XtPointer);
  static void rightWheelFinishCB(Widget, XtPointer, XtPointer);

  void selectedViewing(void);
  void selectedDecoration(void);
  void selectedHeadlight(void);

  static void interactbuttonCB(Widget, XtPointer, XtPointer);
  static void examinebuttonCB(Widget, XtPointer, XtPointer);
  static void homebuttonCB(Widget, XtPointer, XtPointer);
  static void sethomebuttonCB(Widget, XtPointer, XtPointer);
  static void viewallbuttonCB(Widget, XtPointer, XtPointer);
  static void seekbuttonCB(Widget, XtPointer, XtPointer);

  void seekbuttonClicked(void);

  static void increaseInteractiveCountCB(Widget, XtPointer, XtPointer);
  static void decreaseInteractiveCountCB(Widget, XtPointer, XtPointer);

  Widget incspeedbutton, decspeedbutton;
  static void speedIncCB(Widget, XtPointer, XtPointer);
  static void speedDecCB(Widget, XtPointer, XtPointer);

  SbBool decorations;

  Widget viewerbase, canvas;

  Widget mainlayout;
  void showDecorationWidgets(SbBool enable);

  Widget buildAppButtonsForm(Widget parent);
  Widget appButtonsForm;
  SbPList * appButtonsList;
  void resetAppPushButtons(void);
  void layoutAppPushButtons(void);

  Widget viewerButtonsForm;

  struct {
    Widget pick, view, home, set_home, view_all, seek;
  } viewerbuttons;

  struct {
    Pixmap pick, pick_ins;
    Pixmap view, view_ins;
    Pixmap home, home_ins;
    Pixmap set_home, set_home_ins;
    Pixmap view_all, view_all_ins;
    Pixmap seek, seek_ins;
  } pixmaps;

  struct {
    Widget leftwheel, bottomwheel, rightwheel;
  } labels;
};

// ************************************************************************

#endif // ! SOXTFULLVIEWERP_H
