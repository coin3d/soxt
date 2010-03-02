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
