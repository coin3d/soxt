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

#ifndef SOXT_H
#define SOXT_H

#include <X11/Intrinsic.h>
#include <Xm/Xm.h>

#include <Inventor/SbBasic.h>
#include <Inventor/SbLinear.h>

class SbPList;

// *************************************************************************

class SoXt {
public:
  static void getVersionInfo( int * const major = NULL,
      int * const minor = NULL, int * const micro = NULL );
  static const char * getVersionString(void);

  static Widget init( const char * const appName,
      const char * const className = "SoXt" );
  static Widget init( int & argc, char ** argv, const char * const appName,
      const char * const className = "SoXt" );
  static void init( Widget toplevel );

  static void mainLoop(void);
  static void nextEvent( XtAppContext, XEvent * );
  static Boolean dispatchEvent( XEvent * event );

  static XtAppContext getAppContext(void);
  static Display * getDisplay(void);
  static Widget getTopLevelWidget(void);

  static void show( Widget widget );
  static void hide( Widget widget );

  static XmString encodeString( const char * const string );
  static char * decodeString( XmString xstring );

  static void setWidgetSize( Widget widget, const SbVec2s size );
  static SbVec2s getWidgetSize( Widget widget );

  static Widget getShellWidget( Widget widget );

  static void createSimpleErrorDialog( Widget widget, char * title,
      char * string1, char * string2 = NULL );
  static void getPopupArgs(
      Display * display, int screen, ArgList args, int * n );

  static void registerColormapLoad( Widget widget, Widget shell );
  static void addColormapToShell( Widget widget, Widget shell );
  static void removeColormapFromShell( Widget widget, Widget shell );

public:
  static void addExtensionEventHandler( Widget widget,
                  int eventType, XtEventHandler proc,
                  XtPointer clientData );
  static void removeExtensionEventHandler( Widget widget,
                  int eventType, XtEventHandler proc,
                  XtPointer clientData );

  static Widget getwidget( unsigned int what );

  static const char * getAppName(void);
  static const char * getAppClass(void);

protected:
  static void getExtensionEventHandler( XEvent * event, Widget & widget,
                  XtEventHandler & proc, XtPointer & clientData );

private:
  static void selectBestVisual(
    Display *& dpy, Visual *& visual, Colormap & cmap, int & depth );

  static Display * display;
  static XtAppContext xtAppContext;
  static Widget mainWidget;

  static void timerSensorCB( XtPointer, XtIntervalId * );
  static XtIntervalId timerSensorId;
  static SbBool timerSensorActive;

  static void delaySensorCB( XtPointer, XtIntervalId * );
  static XtIntervalId delaySensorId;
  static SbBool delaySensorActive;

  static Boolean idleSensorCB( XtPointer );
  static XtIntervalId idleSensorId;
  static SbBool idleSensorActive;

  static void sensorQueueChanged( void * user );

  static char * appName;
  static char * appClass;

  static SbPList * eventHandlers;

  static String fallback_resources[];

}; // class SoXt

// *************************************************************************

#endif // ! SOXT_H
