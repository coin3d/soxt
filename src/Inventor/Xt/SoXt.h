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

#ifndef SOXT_H
#define SOXT_H

#include <X11/Intrinsic.h>
#include <Xm/Xm.h>

#include <Inventor/SbBasic.h>
#include <Inventor/SbLinear.h>

#include <Inventor/Xt/SoXtBasic.h>

class SbPList;

// *************************************************************************

class SOXT_DLL_API SoXt {
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
  static void exitMainLoop(void);

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

  static void selectBestVisual(
    Display * & dpy, Visual * & visual, Colormap & cmap, int & depth );

protected:
  static void getExtensionEventHandler( XEvent * event, Widget & widget,
                  XtEventHandler & proc, XtPointer & clientData );

private:
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
