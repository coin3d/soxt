/**************************************************************************
 *
 *  This file is part of the Coin SoXt GUI binding library.
 *  Copyright (C) 2000 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License version
 *  2.1 as published by the Free Software Foundation.  See the file
 *  LICENSE.LGPL at the root directory of the distribution for all the
 *  details.
 *
 *  If you want to use Coin SoXt for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
 **************************************************************************/

// $Id$

#ifndef SOXT_FULLVIEWER_H
#define SOXT_FULLVIEWER_H

#include <Inventor/Xt/viewers/SoXtViewer.h>

class SoAnyFullViewer;
class SoAnyPopupMenu;

// *************************************************************************

class SOXT_DLL_EXPORT SoXtFullViewer : public SoXtViewer {
  SOXT_TYPED_ABSTRACT_OBJECT_HEADER(SoXtFullViewer);
  typedef SoXtViewer inherited;
  friend class SoAnyFullViewer;

public:
  enum BuildFlag {
    BUILD_NONE       = 0x00,
    BUILD_DECORATION = 0x01,
    BUILD_POPUP      = 0x02,
    BUILD_ALL        = 0xff
  };

  void setDecoration( const SbBool enable );
  SbBool isDecoration(void) const;

  void setPopupMenuEnabled( const SbBool enable );
  SbBool isPopupMenuEnabled(void) const;

  Widget getAppPushButtonParent(void) const;
  void addAppPushButton( Widget button );
  void insertAppPushButton( Widget button, int idx );
  void removeAppPushButton( Widget button );
  int findAppPushButton( Widget button ) const;
  int lengthAppPushButton(void) const;

  Widget getRenderAreaWidget(void) const;

  virtual void setViewing( SbBool enable );
  virtual void setHeadlight( SbBool enable );
  virtual void setDrawStyle( SoXtViewer::DrawType type,
                             SoXtViewer::DrawStyle style );
  virtual void setBufferingType( SoXtViewer::BufferType type );
  virtual void setCamera( SoCamera * camera );
  virtual void hide(void);

protected:
  SoXtFullViewer(
    Widget parent,
    const char * name,
    SbBool embed,
    BuildFlag flag,
    Type type,
    SbBool build );
  ~SoXtFullViewer(void);

  Widget buildWidget( Widget parent );

  virtual void buildDecoration( Widget parent );
  virtual Widget buildLeftTrim( Widget parent );
  virtual Widget buildBottomTrim( Widget parent );
  virtual Widget buildRightTrim( Widget parent );
  Widget buildAppButtonsForm( Widget parent );
  Widget buildViewerButtons( Widget parent );
  virtual void createViewerButtons( Widget parent, SbPList * buttonlist = 0);

  virtual void buildPopupMenu(void);
  void setPopupMenuString( const char * const title );
  Widget buildFunctionsSubmenu( Widget popup );
  Widget buildDrawStyleSubmenu( Widget popup );
  void openPopupMenu( const SbVec2s position );

  virtual void leftWheelStart(void);
  static void leftWheelStartCB( Widget, XtPointer, XtPointer );
  virtual void leftWheelMotion( float value );
  static void leftWheelMotionCB( Widget, XtPointer, XtPointer );
  virtual void leftWheelFinish(void);
  static void leftWheelFinishCB( Widget, XtPointer, XtPointer );
  float getLeftWheelValue(void) const;
  void setLeftWheelValue( const float value );

  virtual void bottomWheelStart(void);
  static void bottomWheelStartCB( Widget, XtPointer, XtPointer );
  virtual void bottomWheelMotion( float value );
  static void bottomWheelMotionCB( Widget, XtPointer, XtPointer );
  virtual void bottomWheelFinish(void);
  static void bottomWheelFinishCB( Widget, XtPointer, XtPointer );
  float getBottomWheelValue(void) const;
  void setBottomWheelValue( const float value );

  virtual void rightWheelStart(void);
  static void rightWheelStartCB( Widget, XtPointer, XtPointer );
  virtual void rightWheelMotion( float value );
  static void rightWheelMotionCB( Widget, XtPointer, XtPointer );
  virtual void rightWheelFinish(void);
  static void rightWheelFinishCB( Widget, XtPointer, XtPointer );
  float getRightWheelValue(void) const;
  void setRightWheelValue( const float value );

  void setLeftWheelString( const char * const name );
  Widget getLeftWheelLabelWidget(void) const;
  void setBottomWheelString( const char * const name );
  Widget getBottomWheelLabelWidget(void) const;
  void setRightWheelString( const char * const name );
  Widget getRightWheelLabelWidget(void) const;

  virtual void sizeChanged( const SbVec2s size );

  virtual void openViewerHelpCard(void);

  Pixmap createPixmapFromXpmData( Widget button, char ** xpm );
  Pixmap createInsensitivePixmapFromXpmData( Widget button, char ** xpm );

  char * popupTitle;
  SbBool popupEnabled;

  SoAnyPopupMenu * prefmenu;

  virtual SbBool processSoEvent( const SoEvent * const event );

protected: // preference sheet routines
  void setPrefSheetString( const char * name );
  virtual void createPrefSheet(void);
  void createPrefSheetShellAndForm(
    Widget & shell, Widget & form );
  void createDefaultPrefSheetParts(
    Widget * widgets, int & num, Widget form );
  void layoutPartsAndMapPrefSheet(
    Widget * widgets, int num, Widget form, Widget shell );

  Widget createSeekPrefSheetGuts( Widget parent );
  Widget createSeekDistPrefSheetGuts( Widget parent );
  Widget createZoomPrefSheetGuts( Widget parent );
  Widget createClippingPrefSheetGuts( Widget parent );
  Widget createStereoPrefSheetGuts( Widget parent );
  Widget createSpeedPrefSheetGuts( Widget parent );

  Widget createFramedSeekPrefSheetGuts( Widget parent );
  Widget createFramedSeekDistPrefSheetGuts( Widget parent );
  Widget createFramedZoomPrefSheetGuts( Widget parent );
  Widget createFramedClippingPrefSheetGuts( Widget parent );
  Widget createFramedStereoPrefSheetGuts( Widget parent );
  Widget createFramedSpeedPrefSheetGuts( Widget parent );

  void prefSheetDestroy(void);
  static void prefSheetDestroyCB( Widget, XtPointer, XtPointer );
  void seekPrefSheetField(void);
  static void seekPrefSheetFieldCB( Widget, XtPointer, XtPointer );
  void seekPrefSheetToggle1(void);
  static void seekPrefSheetToggle1CB( Widget, XtPointer, XtPointer );
  void seekPrefSheetToggle2(void);
  static void seekPrefSheetToggle2CB( Widget, XtPointer, XtPointer );

protected:
  char * prefstring;
  Widget prefshell;
  Widget prefsheet;

protected:
  SbPList * viewerButtonWidgets;
  
  Widget rightWheel;
  char * rightWheelStr;
  float  rightWheelVal;
  Widget rightWheelLabel;

  Widget bottomWheel;
  char * bottomWheelStr;
  float  bottomWheelVal;
  Widget bottomWheelLabel;

  Widget leftWheel;
  char * leftWheelStr;
  float  leftWheelVal;
  Widget leftWheelLabel;

private:
  SbBool decorations;

  Widget viewerbase, canvas;
  Widget leftDecoration;
  Widget rightDecoration;
  Widget bottomDecoration;

  Widget mainlayout;
  void showDecorationWidgets( SbBool enable );

  Widget appButtonsForm;
  SbPList * appButtonsList;
  void layoutAppPushButtons( Widget parent );

  Widget viewerButtonsForm;

private:
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

private:
  void selectedViewing(void);
  void selectedDecoration(void);
  void selectedHeadlight(void);
  void selectedPrefs(void);
  void interactbuttonClicked( Boolean set );
  static void interactbuttonCB( Widget, XtPointer, XtPointer );
  void examinebuttonClicked( Boolean set );
  static void examinebuttonCB( Widget, XtPointer, XtPointer );
  void helpbuttonClicked(void);
  static void helpbuttonCB( Widget, XtPointer, XtPointer );
  void homebuttonClicked(void);
  static void homebuttonCB( Widget, XtPointer, XtPointer );
  void sethomebuttonClicked(void);
  static void sethomebuttonCB( Widget, XtPointer, XtPointer );
  void viewallbuttonClicked(void);
  static void viewallbuttonCB( Widget, XtPointer, XtPointer );
  void seekbuttonClicked(void);
  static void seekbuttonCB( Widget, XtPointer, XtPointer );
  void copyviewSelected(void);
  void pasteviewSelected(void);
  void drawstyleActivated( int );

  Widget seektimefield;
  Widget pointtoggle, objecttoggle;
  void seektimechanged(void);
  static void seektimechangedCB( Widget, XtPointer, XtPointer );
  void pointtoggled(void);
  static void pointtoggledCB( Widget, XtPointer, XtPointer );
  void objecttoggled(void);
  static void objecttoggledCB( Widget, XtPointer, XtPointer );
  Widget seekdistancefield;
  Widget percenttoggle, absolutetoggle;
  void seekdistancechanged(void);
  static void seekdistancechangedCB( Widget, XtPointer, XtPointer );
  void percenttoggled(void);
  static void percenttoggledCB( Widget, XtPointer, XtPointer );
  void absolutetoggled(void);
  static void absolutetoggledCB( Widget, XtPointer, XtPointer );

  SbVec2f zoomrange;
  Widget zoomfrom, zoomslider, zoomto, zoomvalue;
  void zoomfromchanged(void);
  static void zoomfromchangedCB( Widget, XtPointer, XtPointer );
  void zoomtochanged(void);
  static void zoomtochangedCB( Widget, XtPointer, XtPointer );
  void zoomsliderchanged(void);
  static void zoomsliderchangedCB( Widget, XtPointer, XtPointer );
  void zoomvaluechanged(void);
  static void zoomvaluechangedCB( Widget, XtPointer, XtPointer );

  void setCameraZoom( float zoom );
  float getCameraZoom(void);

  // clipping preferences widgets
  Widget autocliptoggle, nearwheel, farwheel, nearvalue, farvalue;

  void autocliptoggled(void);
  static void autocliptoggledCB( Widget, XtPointer, XtPointer );
  void nearwheelvaluechanged(void);
  static void nearwheelvaluechangedCB( Widget, XtPointer, XtPointer );
  void farwheelvaluechanged(void);
  static void farwheelvaluechangedCB( Widget, XtPointer, XtPointer );
  void nearvaluechanged(void);
  static void nearvaluechangedCB( Widget, XtPointer, XtPointer );
  void farvaluechanged(void);
  static void farvaluechangedCB( Widget, XtPointer, XtPointer );

  Widget stereotoggle;
  void stereotoggled(void);
  static void stereotoggledCB( Widget, XtPointer, XtPointer );

  void increaseInteractiveCount(void);
  static void increaseInteractiveCountCB( Widget, XtPointer, XtPointer );
  void decreaseInteractiveCount(void);
  static void decreaseInteractiveCountCB( Widget, XtPointer, XtPointer );

  Widget incspeedbutton, decspeedbutton;
  void speedInc(void);
  static void speedIncCB( Widget, XtPointer, XtPointer );
  void speedDec(void);
  static void speedDecCB( Widget, XtPointer, XtPointer );

private:
  SoAnyFullViewer * const common;

}; // class SoXtFullViewer

// *************************************************************************

#endif // ! SOXT_FULLVIEWER_H
