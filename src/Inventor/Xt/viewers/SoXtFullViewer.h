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

#include <Inventor/Xt/viewers/SoAnyFullViewer.h>
#include <Inventor/Xt/viewers/SoXtViewer.h>

class SoAnyPopupMenu;

// *************************************************************************

class SoXtFullViewer :
  public SoXtViewer,
  public SoAnyFullViewer
{
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

  virtual void processEvent( XAnyEvent * event );
  virtual SbBool eventFilter( Widget widget, XAnyEvent * event );

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

  virtual Widget makeSubPreferences( Widget parent );
  void setPrefSheetString( const char * title );

  virtual void leftWheelStart(void);
  virtual void leftWheelMotion( float value );
  virtual void leftWheelFinish(void);
  float getLeftWheelValue(void) const;
  virtual void bottomWheelStart(void);
  virtual void bottomWheelMotion( float value );
  virtual void bottomWheelFinish(void);
  float getBottomWheelValue(void) const;
  virtual void rightWheelStart(void);
  virtual void rightWheelMotion( float value );
  virtual void rightWheelFinish(void);
  float getRightWheelValue(void) const;

  void setLeftWheelString( const char * const name );
  void setBottomWheelString( const char * const name );
  void setRightWheelString( const char * const name );

  virtual void openViewerHelpCard(void);

  Widget getThumbwheel( int num );

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

private:
  Widget makePreferencesWindow(void);
  Widget makeSeekPreferences( Widget parent );
  Widget makeSeekDistancePreferences( Widget parent );
  Widget makeZoomPreferences( Widget parent );
  Widget makeAutoclipPreferences( Widget parent );

  SbBool decorations;

  Widget viewerbase, canvas;
  float wheelvalues[NUMDECORATIONS];
  SbString wheelstrings[NUMDECORATIONS];
  Widget wheels[NUMDECORATIONS];
  Widget wheellabels[NUMDECORATIONS];
  Widget decorform[NUMDECORATIONS];

  SbString menutitle;
  SoAnyPopupMenu * prefmenu;
  SbBool menuenabled;

  Widget mainlayout;
  void showDecorationWidgets( SbBool enable );

  Widget appButtonsForm;
  SbPList * appButtonsList;
  void layoutAppPushButtons( Widget parent );

  Widget viewerButtonsForm;
  SbPList * viewerButtonsList;

  SbVec2f zoomrange;

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

}; // class SoXtFullViewer

// *************************************************************************

#endif // ! SOXT_FULLVIEWER_H
