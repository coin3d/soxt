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

//  $Id$

#ifndef SOXT_FULLVIEWER_H
#define SOXT_FULLVIEWER_H

#include <Inventor/Xt/viewers/SoXtViewer.h>

class SoAnyFullViewer;
class SoAnyPopupMenu;

#define SOXT_WIDGET_CALLBACK( name ) \
  void name( Widget, XtPointer ); \
  static void SO__CONCAT(name,CB)( Widget, XtPointer, XtPointer )

// *************************************************************************

class SoXtFullViewer : public SoXtViewer {
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
  void addAppPushButton( Widget newButton );
  void insertAppPushButton( Widget newButton, int idx );
  void removeAppPushButton( Widget oldButton );
  int findAppPushButton( Widget oldButton ) const;
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
  SoXtFullViewer( Widget parent, const char * name, SbBool inParent,
      BuildFlag flag, Type type, SbBool build );
  ~SoXtFullViewer(void);

  Widget buildWidget( Widget parent );

  virtual void buildDecoration( Widget parent );
  virtual Widget buildLeftTrim( Widget parent );
  virtual Widget buildBottomTrim( Widget parent );
  virtual Widget buildRightTrim( Widget parent );
  Widget buildAppButtonsForm( Widget parent );
  Widget buildViewerButtons( Widget parent );
  virtual void createViewerButtons( Widget parent, SbPList * buttonlist );

  virtual void buildPopupMenu(void);
  void setPopupMenuString( const char * const title );
  Widget buildFunctionsSubmenu( Widget popup );
  Widget buildDrawStyleSubmenu( Widget popup );

  virtual void leftWheelStart(void);
  SOXT_WIDGET_CALLBACK( leftWheelStart );
  virtual void leftWheelMotion( float value );
  SOXT_WIDGET_CALLBACK( leftWheelMotion );
  virtual void leftWheelFinish(void);
  SOXT_WIDGET_CALLBACK( leftWheelFinish );
  float getLeftWheelValue(void) const;
  void setLeftWheelValue( const float value );

  virtual void bottomWheelStart(void);
  SOXT_WIDGET_CALLBACK( bottomWheelStart );
  virtual void bottomWheelMotion( float value );
  SOXT_WIDGET_CALLBACK( bottomWheelMotion );
  virtual void bottomWheelFinish(void);
  SOXT_WIDGET_CALLBACK( bottomWheelFinish );
  float getBottomWheelValue(void) const;
  void setBottomWheelValue( const float value );

  virtual void rightWheelStart(void);
  SOXT_WIDGET_CALLBACK( rightWheelStart );
  virtual void rightWheelMotion( float value );
  SOXT_WIDGET_CALLBACK( rightWheelMotion );
  virtual void rightWheelFinish(void);
  SOXT_WIDGET_CALLBACK( rightWheelFinish );
  float getRightWheelValue(void) const;
  void setRightWheelValue( const float value );

  void setLeftWheelString( const char * const name );
  Widget getLeftWheelLabelWidget(void) const;
  void setBottomWheelString( const char * const name );
  Widget getBottomWheelLabelWidget(void) const;
  void setRightWheelString( const char * const name );
  Widget getRightWheelLabelWidget(void) const;

  virtual void openViewerHelpCard(void);

  Widget getThumbWheel( int num );

  enum {
    LEFTDECORATION,
    BOTTOMDECORATION,
    RIGHTDECORATION,

    FIRSTDECORATION = LEFTDECORATION,
    LASTDECORATION = RIGHTDECORATION,
    NUMDECORATIONS = LASTDECORATION - FIRSTDECORATION + 1
  };

  Pixmap createPixmapFromXpmData( Widget button, char ** xpm );
  Pixmap createInsensitivePixmapFromXpmData( Widget button, char ** xpm );

  char * popupTitle;
  SbBool popupEnabled;

  SoAnyPopupMenu * prefmenu;

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

  SOXT_WIDGET_CALLBACK( prefSheetDestroy );
  SOXT_WIDGET_CALLBACK( seekPrefSheetField );
  SOXT_WIDGET_CALLBACK( seekPrefSheetToggle1 );
  SOXT_WIDGET_CALLBACK( seekPrefSheetToggle2 );

  char * prefstring;
  Widget prefshell;
  Widget prefsheet;

private:
  SbBool decorations;

  Widget viewerbase, canvas;
  float wheelvalues[NUMDECORATIONS];
  SbString wheelstrings[NUMDECORATIONS];
  Widget wheels[NUMDECORATIONS];
  Widget wheellabels[NUMDECORATIONS];
  Widget decorform[NUMDECORATIONS];

  Widget mainlayout;
  void showDecorationWidgets( SbBool enable );

  Widget appButtonsForm;
  SbPList * appButtonsList;
  void layoutAppPushButtons( Widget parent );

  Widget viewerButtonsForm;
  SbPList * viewerButtonsList;

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
  void examinebuttonClicked( Boolean set );
  void helpbuttonClicked(void);
  void homebuttonClicked(void);
  void sethomebuttonClicked(void);
  void viewallbuttonClicked(void);
  void seekbuttonClicked(void);
  SOXT_WIDGET_CALLBACK( interactbutton );
  SOXT_WIDGET_CALLBACK( examinebutton );
  static void helpbuttonCB( Widget, XtPointer, XtPointer );
  static void homebuttonCB( Widget, XtPointer, XtPointer );
  static void sethomebuttonCB( Widget, XtPointer, XtPointer );
  static void viewallbuttonCB( Widget, XtPointer, XtPointer );
  static void seekbuttonCB( Widget, XtPointer, XtPointer );
  void copyviewSelected(void);
  void pasteviewSelected(void);
  void drawstyleActivated( int );

  Widget seektimefield;
  Widget pointtoggle, objecttoggle;
  SOXT_WIDGET_CALLBACK( seektimechanged );
  SOXT_WIDGET_CALLBACK( pointtoggled );
  SOXT_WIDGET_CALLBACK( objecttoggled );
  Widget seekdistancefield;
  Widget percenttoggle, absolutetoggle;
  SOXT_WIDGET_CALLBACK( seekdistancechanged );
  SOXT_WIDGET_CALLBACK( percenttoggled );
  SOXT_WIDGET_CALLBACK( absolutetoggled );

  SbVec2f zoomrange;
  Widget zoomfrom, zoomslider, zoomto, zoomvalue;
  SOXT_WIDGET_CALLBACK( zoomfromchanged );
  SOXT_WIDGET_CALLBACK( zoomtochanged );
  SOXT_WIDGET_CALLBACK( zoomsliderchanged );
  SOXT_WIDGET_CALLBACK( zoomvaluechanged );

  void setCameraZoom( float zoom );
  float getCameraZoom(void);

  // clipping preferences widgets
  Widget autocliptoggle, nearwheel, farwheel, nearvalue, farvalue;

  SOXT_WIDGET_CALLBACK( autocliptoggled );
  SOXT_WIDGET_CALLBACK( nearwheelvaluechanged );
  SOXT_WIDGET_CALLBACK( farwheelvaluechanged );
  SOXT_WIDGET_CALLBACK( nearvaluechanged );
  SOXT_WIDGET_CALLBACK( farvaluechanged );

  Widget stereotoggle;
  SOXT_WIDGET_CALLBACK( stereotoggled );

  SOXT_WIDGET_CALLBACK( increaseInteractiveCount );
  SOXT_WIDGET_CALLBACK( decreaseInteractiveCount );

  Widget incspeedbutton, decspeedbutton;
  SOXT_WIDGET_CALLBACK( speedInc );
  SOXT_WIDGET_CALLBACK( speedDec );

private:
  SoAnyFullViewer * const common;

}; // class SoXtFullViewer

// *************************************************************************

#define SOXT_WIDGET_CALLBACK_IMPLEMENTATION( soxtclass, name ) \
void                                                           \
soxtclass::SO__CONCAT( name, CB )(                             \
  Widget widget,                                               \
  XtPointer client_data,                                       \
  XtPointer call_data )                                        \
{                                                              \
  soxtclass * const viewer = (soxtclass *) client_data;        \
  viewer->name( widget, call_data );                           \
}                                                              \
                                                               \
void                                                           \
soxtclass::name(                                               \
  Widget widget,                                               \
  XtPointer call_data )

// *************************************************************************

#endif // ! SOXT_FULLVIEWER_H
