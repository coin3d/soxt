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

static const char rcsid[] =
  "$Id$";

#include <X11/StringDefs.h>
#include <Xm/Xm.h>

#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/misc/SoBasic.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/sensors/SoSensorManager.h>

#include <common/soguidefs.h> // SOGUI_STUB()
#include <Inventor/Xt/SoXt.h>

// *************************************************************************
// the static SoXt:: variables

Display *     SoXt::display = NULL;
XtAppContext  SoXt::xtAppContext;
Widget        SoXt::mainWidget = (Widget) NULL;
XtIntervalId  SoXt::timerSensorId = 0;
SbBool        SoXt::timerSensorActive = FALSE;
XtIntervalId  SoXt::delaySensorId = 0;
SbBool        SoXt::delaySensorActive = FALSE;
XtIntervalId  SoXt::idleSensorId = 0;
SbBool        SoXt::idleSensorActive = FALSE;

// *************************************************************************

Widget
SoXt::init( // static
  const char * const appName,
  const char * const className )
{
  int argc = 1;
  char * argv[] = { (char *) appName, NULL };
  return SoXt::init( argc, argv, appName, className );
} // init()

Widget
SoXt::init( // static
  int & argc,
  char ** argv,
  const char * const appName,
  const char * const className )
{
  XtAppContext context;
  Widget toplevel = XtAppInitialize( &context, className,
                        NULL, 0, &argc, argv, NULL, NULL, 0 );
  SoXt::init( toplevel );
  return toplevel;
} // init()

void
SoXt::init( // static
  Widget toplevel )
{
  SoXt::mainWidget = toplevel;
  SoXt::display = XtDisplay( toplevel );
  SoXt::xtAppContext = XtWidgetToApplicationContext( toplevel );

  SoDB::init();
  SoNodeKit::init();
  SoInteraction::init();

  SoDB::getSensorManager()->setChangedCallback( SoXt::sensorQueueChanged, NULL);

} // init()

// *************************************************************************

void
SoXt::mainLoop( // static
  void )
{
  XtAppMainLoop( SoXt::xtAppContext );
} // mainLoop()

void
SoXt::nextEvent( // static
  XtAppContext context,
  XEvent * event )
{
  SOGUI_STUB();
} // nextEvent()

Boolean
SoXt::dispatchEvent( // static
  XEvent * event )
{
  SOGUI_STUB();
  return False;
} // dispatchEvent()

// *************************************************************************

XtAppContext
SoXt::getAppContext( // static
  void )
{
  return SoXt::xtAppContext;
} // getAppContext()

Display *
SoXt::getDisplay( // static
  void )
{
  return SoXt::display;
} // getDisplay()

Widget
SoXt::getTopLevelWidget( // static
  void )
{
  return SoXt::mainWidget;
} // getTopLevelWidget()

// *************************************************************************

void
SoXt::show( // static
  Widget widget )
{
  if ( XtIsTopLevelShell( widget ) ) {
    XtRealizeWidget( widget );
  } else {
    XtManageChild( widget );
  }
} // show()

void
SoXt::hide( // static
  Widget widget )
{
  XtUnrealizeWidget( widget );
} // hide()

// *************************************************************************

XmString
SoXt::encodeString( // static
  char * string )
{
  return XmStringCreateLocalized(string);
} // encodeString()

char *
SoXt::decodeString( // static
  XmString xstring )
{
  SOGUI_STUB();
  return 0;
} // decodeString()

// *************************************************************************

void
SoXt::setWidgetSize( // static
  Widget widget,
  const SbVec2s size )
{
  XtVaSetValues( widget,
    XtNwidth, (Dimension) size[0], XtNheight, (Dimension) size[1], NULL);
} // setWidgetSize()

SbVec2s
SoXt::getWidgetSize( // static
  Widget widget )
{
  int w = 0, h = 0;
  XtVaGetValues( widget, XtNwidth, &w, XtNheight, &h, NULL );
  return SbVec2s( w, h );
} // getWidgetSize()

// *************************************************************************

Widget
SoXt::getShellWidget( // static
  Widget widget )
{
  if ( XtIsTopLevelShell( SoXt::mainWidget ) )
    return SoXt::mainWidget;
  SOGUI_STUB();
  return (Widget) NULL;
} // getShellWidget()

// *************************************************************************

void
SoXt::createSimpleErrorDialog( // static
  Widget widget,
  char * title,
  char * string1,
  char * string2 )
{
  SOGUI_STUB();
} // createSimpleErrorDialog()

void
SoXt::getPopupArgs( // static
  Display * display,
  int screen,
  ArgList args,
  int * n )
{
  SOGUI_STUB();
} // getPopupArgs()

// *************************************************************************

void
SoXt::registerColormapLoad( // static
  Widget widget,
  Widget shell )
{
  SOGUI_STUB();
} // registerColormapLoad()

void
SoXt::addColormapToShell( // static
  Widget widget,
  Widget shell )
{
  SOGUI_STUB();
} // addColormapToShell()

void
SoXt::removeColormapFromShell( // static
  Widget widget,
  Widget shell )
{
  SOGUI_STUB();
} // removeColormapFromShell()

// *************************************************************************

void
SoXt::addExtensionEventHandler( // static
  Widget widget,
  int extensionEventType,
  XtEventHandler proc,
  XtPointer clientData )
{
  SOGUI_STUB();
} // addExtensionEventHandler()

void
SoXt::removeExtensionEventHandler( // static
  Widget widget,
  int extensionEventType,
  XtEventHandler proc,
  XtPointer clientData )
{
  SOGUI_STUB();
} // removeExtensionEventHandler()

Widget
SoXt::getwidget( // static
  unsigned int what )
{
  SOGUI_STUB();
  return (Widget) NULL;
} // getwidget()

// *************************************************************************

void
SoXt::getExtensionEventHandler( // static
  XEvent * event,
  Widget & widget,
  XtEventHandler & proc,
  XtPointer & clientData )
{
  SOGUI_STUB();
} // getExtensionEventHandler()

// *************************************************************************

/*!
*/

void
SoXt::timerSensorCB( // static, private
  XtPointer closure,
  XtIntervalId * id )
{
  SoDB::getSensorManager()->processTimerQueue();
  SoXt::sensorQueueChanged( NULL );
  SoXt::timerSensorId = 0;
  SoXt::timerSensorActive = FALSE;
} // timerSensorCB()

/*!
*/

void
SoXt::delaySensorCB( // static, private
  XtPointer closure,
  XtIntervalId * id )
{
  SoDB::getSensorManager()->processDelayQueue(FALSE);
  SoXt::sensorQueueChanged( NULL );
  SoXt::delaySensorId = 0;
  SoXt::delaySensorActive = FALSE;
} // delaySensorCB()

/*!
*/

void
SoXt::idleSensorCB( // static, private
  XtPointer closure,
  XtIntervalId * id )
{
  SoDB::getSensorManager()->processDelayQueue(TRUE);
  SoXt::sensorQueueChanged( NULL );
  SoXt::idleSensorId = 0;
  SoXt::idleSensorActive = FALSE;
} // idleSensorCB()

// *************************************************************************

/*!
  This callback handles events from sensors in the scene graph, needed to
  deal with scene interaction.
*/

void
SoXt::sensorQueueChanged( // static, private
  void * user )
{
  SoSensorManager * sensormanager = SoDB::getSensorManager();

  SbTime timevalue;
  if ( sensormanager->isTimerSensorPending( timevalue ) ) {
    SbTime interval = timevalue - SbTime::getTimeOfDay();
    if ( SoXt::timerSensorActive )
      XtRemoveTimeOut( SoXt::timerSensorId );
    SoXt::timerSensorId = XtAppAddTimeOut( SoXt::getAppContext(),
                              interval.getMsecValue(),
                              SoXt::timerSensorCB, NULL );
    SoXt::timerSensorActive = TRUE;
  } else if ( SoXt::timerSensorActive ) {
    XtRemoveTimeOut( SoXt::timerSensorId );
    SoXt::timerSensorId = 0;
    SoXt::timerSensorActive = FALSE;
  }

  if ( sensormanager->isDelaySensorPending() ) {
    if ( ! SoXt::idleSensorActive ) {
      SoXt::idleSensorId = XtAppAddTimeOut( SoXt::getAppContext(), 0,
                               SoXt::idleSensorCB, NULL );
      SoXt::idleSensorActive = TRUE;
    }

    if ( ! SoXt::delaySensorActive ) {
      SoXt::delaySensorId = XtAppAddTimeOut( SoXt::getAppContext(),
                                SoDB::getDelaySensorTimeout().getMsecValue(),
                                SoXt::delaySensorCB, NULL );
      SoXt::delaySensorActive = TRUE;
    }
  } else {
    if ( SoXt::idleSensorActive ) {
      XtRemoveTimeOut( SoXt::idleSensorId );
      SoXt::idleSensorId = 0;
      SoXt::idleSensorActive = FALSE;
    }
    if ( SoXt::delaySensorActive ) {
      XtRemoveTimeOut( SoXt::delaySensorId );
      SoXt::delaySensorId = 0;
      SoXt::delaySensorActive = FALSE;
    }
  }
} // sensorQueueChange()

// *************************************************************************
