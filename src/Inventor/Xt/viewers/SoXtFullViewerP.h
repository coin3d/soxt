/**************************************************************************\
 *
 *  This file is part of the Coin GUI toolkit libraries.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation.  See the
 *  file LICENSE.LGPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you want to use this library with software that is incompatible
 *  licensewise with the LGPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#ifndef SOXTFULLVIEWERP_H
#define SOXTFULLVIEWERP_H

#include <Inventor/SbBasic.h>
#include <SoGuiFullViewerP.h>

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
  void selectedPrefs(void);
  void copyviewSelected(void);
  void pasteviewSelected(void);

  static void interactbuttonCB(Widget, XtPointer, XtPointer);
  static void examinebuttonCB(Widget, XtPointer, XtPointer);
  static void helpbuttonCB(Widget, XtPointer, XtPointer);
  static void homebuttonCB(Widget, XtPointer, XtPointer);
  static void sethomebuttonCB(Widget, XtPointer, XtPointer);
  static void viewallbuttonCB(Widget, XtPointer, XtPointer);
  static void seekbuttonCB(Widget, XtPointer, XtPointer);

  void seekbuttonClicked(void);

  void prefSheetDestroy(void);
  static void prefSheetDestroyCB(Widget, XtPointer, XtPointer);
  static void seekPrefSheetFieldCB(Widget, XtPointer, XtPointer);
  static void seekPrefSheetToggle1CB(Widget, XtPointer, XtPointer);
  void seekPrefSheetToggle2(void);
  static void seekPrefSheetToggle2CB(Widget, XtPointer, XtPointer);

  Widget seektimefield;
  Widget pointtoggle, objecttoggle;
  void seektimechanged(void);
  static void seektimechangedCB(Widget, XtPointer, XtPointer);
  void pointtoggled(void);
  static void pointtoggledCB(Widget, XtPointer, XtPointer);
  void objecttoggled(void);
  static void objecttoggledCB(Widget, XtPointer, XtPointer);
  Widget seekdistancefield;
  Widget percenttoggle, absolutetoggle;
  void seekdistancechanged(void);
  static void seekdistancechangedCB(Widget, XtPointer, XtPointer);
  void percenttoggled(void);
  static void percenttoggledCB(Widget, XtPointer, XtPointer);
  void absolutetoggled(void);
  static void absolutetoggledCB(Widget, XtPointer, XtPointer);

  SbVec2f zoomrange;
  Widget zoomfrom, zoomslider, zoomto, zoomvalue;
  void zoomfromchanged(void);
  static void zoomfromchangedCB(Widget, XtPointer, XtPointer);
  void zoomtochanged(void);
  static void zoomtochangedCB(Widget, XtPointer, XtPointer);
  void zoomsliderchanged(void);
  static void zoomsliderchangedCB(Widget, XtPointer, XtPointer);
  void zoomvaluechanged(void);
  static void zoomvaluechangedCB(Widget, XtPointer, XtPointer);

  void setCameraZoom(float zoom);
  float getCameraZoom(void);

  // clipping preferences widgets
  Widget autocliptoggle, nearwheel, farwheel, nearvalue, farvalue;

  void autocliptoggled(void);
  static void autocliptoggledCB(Widget, XtPointer, XtPointer);
  void nearwheelvaluechanged(void);
  static void nearwheelvaluechangedCB(Widget, XtPointer, XtPointer);
  void farwheelvaluechanged(void);
  static void farwheelvaluechangedCB(Widget, XtPointer, XtPointer);
  void nearvaluechanged(void);
  static void nearvaluechangedCB(Widget, XtPointer, XtPointer);
  void farvaluechanged(void);
  static void farvaluechangedCB(Widget, XtPointer, XtPointer);

  Widget stereotoggle;
  void stereotoggled(void);
  static void stereotoggledCB(Widget, XtPointer, XtPointer);

  static void increaseInteractiveCountCB(Widget, XtPointer, XtPointer);
  static void decreaseInteractiveCountCB(Widget, XtPointer, XtPointer);

  Widget incspeedbutton, decspeedbutton;
  static void speedIncCB(Widget, XtPointer, XtPointer);
  static void speedDecCB(Widget, XtPointer, XtPointer);

  SbBool decorations;

  Widget viewerbase, canvas;

  Widget mainlayout;
  void showDecorationWidgets(SbBool enable);

  Widget appButtonsForm;
  SbPList * appButtonsList;
  void layoutAppPushButtons(Widget parent);

  Widget viewerButtonsForm;

  struct {
    Widget pick, view, help, home, set_home, view_all, seek;
  } viewerbuttons;

  struct {
    Pixmap pick, pick_ins;
    Pixmap view, view_ins;
    Pixmap help, help_ins;
    Pixmap home, home_ins;
    Pixmap set_home, set_home_ins;
    Pixmap view_all, view_all_ins;
    Pixmap seek, seek_ins;
  } pixmaps;

  struct {
    Widget leftwheel, bottomwheel, rightwheel;
  } labels;

  float seekdistance;
  SbBool seekdistaspercentage;
};

// ************************************************************************

#endif // ! SOXTFULLVIEWERP_H
