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
#include <X11/Xmu/Editres.h>
#include <Xm/Xm.h>

#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/misc/SoBasic.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/sensors/SoSensorManager.h>

#include <Inventor/Xt/SoXtBasic.h>
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
    NULL, 0, &argc, argv, SoXt::fallback_resources, NULL, 0 );
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

  XtAppSetFallbackResources( SoXt::getAppContext(), SoXt::fallback_resources );

  XtAddEventHandler( toplevel, (EventMask) 0, True,
    (void (*)(Widget, void *, XEvent *, Boolean *)) _XEditResCheckMessages,
    NULL );
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
  SOXT_STUB();
} // nextEvent()

Boolean
SoXt::dispatchEvent( // static
  XEvent * event )
{
  SOXT_STUB();
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
  if ( XtIsTopLevelShell( widget ) ) {
    XtUnrealizeWidget( widget );
  } else {
    XtUnmanageChild( widget );
  }
} // hide()

// *************************************************************************

/*!
  Create and return a localized string from \a string.

  The caller is responsible for freeing the returned XmString.
*/

XmString
SoXt::encodeString( // static
  const char * const string )
{
  return XmStringCreateLocalized( (char *) string );
} // encodeString()

/*!
  Decode a left-to-right localized string into a simple character
  array, and return its memory pointer.

  The caller is responsible for freeing the returned character string
  with XtFree() to avoid memory leaks.
*/

char *
SoXt::decodeString( // static
  XmString xstring )
{
  char * str;
  /* set str to point to the text */
  XmStringGetLtoR(xstring, XmSTRING_DEFAULT_CHARSET, &str);
  return str;
} // decodeString()

// *************************************************************************

void
SoXt::setWidgetSize( // static
  Widget widget,
  const SbVec2s size )
{
  XtVaSetValues( widget,
    XtNwidth, (Dimension) size[0],
    XtNheight, (Dimension) size[1],
    NULL );
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
  Widget p = widget;
  while ( p != (Widget) NULL ) {
    if ( XtIsShell(p) )
      return p;
    p = XtParent(p);
  }
  SOXT_STUB();
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
  SOXT_STUB();
} // createSimpleErrorDialog()

void
SoXt::getPopupArgs( // static
  Display * display,
  int screen,
  ArgList args,
  int * n )
{
  SOXT_STUB();
} // getPopupArgs()

// *************************************************************************

void
SoXt::registerColormapLoad( // static
  Widget widget,
  Widget shell )
{
  SOXT_STUB();
} // registerColormapLoad()

void
SoXt::addColormapToShell( // static
  Widget widget,
  Widget shell )
{
  SOXT_STUB();
} // addColormapToShell()

void
SoXt::removeColormapFromShell( // static
  Widget widget,
  Widget shell )
{
  SOXT_STUB();
} // removeColormapFromShell()

// *************************************************************************

void
SoXt::addExtensionEventHandler( // static
  Widget widget,
  int extensionEventType,
  XtEventHandler proc,
  XtPointer clientData )
{
  SOXT_STUB();
} // addExtensionEventHandler()

void
SoXt::removeExtensionEventHandler( // static
  Widget widget,
  int extensionEventType,
  XtEventHandler proc,
  XtPointer clientData )
{
  SOXT_STUB();
} // removeExtensionEventHandler()

Widget
SoXt::getwidget( // static
  unsigned int what )
{
  SOXT_STUB();
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
  SOXT_STUB();
} // getExtensionEventHandler()

// *************************************************************************

/*!
*/

void
SoXt::timerSensorCB( // static, private
  XtPointer closure,
  XtIntervalId * id )
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXt::timerSensorCB", "called" );
#endif // SOXT_DEBUG
  SoXt::timerSensorId = 0;
  SoXt::timerSensorActive = FALSE;
  SoDB::getSensorManager()->processTimerQueue();
  SoXt::sensorQueueChanged( NULL );
} // timerSensorCB()

/*!
*/

void
SoXt::delaySensorCB( // static, private
  XtPointer closure,
  XtIntervalId * id )
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXt::delaySensorCB", "called" );
#endif // SOXT_DEBUG
  SoXt::delaySensorId = 0;
  SoXt::delaySensorActive = FALSE;
  SoDB::getSensorManager()->processDelayQueue( FALSE );
  SoXt::sensorQueueChanged( NULL );
} // delaySensorCB()

/*!
*/

Boolean
SoXt::idleSensorCB( // static, private
  XtPointer closure )
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXt::idleSensorCB", "called" );
#endif // SOXT_DEBUG
  SoXt::idleSensorId = 0;
  SoXt::idleSensorActive = FALSE;
  SoDB::getSensorManager()->processDelayQueue(TRUE);
  SoXt::sensorQueueChanged( NULL );
  return True;
} // idleSensorCB()

// *************************************************************************

/*!
  This callback handles events from sensors in the scene graph, needed to
  deal with scene interaction.
*/

void
SoXt::sensorQueueChanged( // static, private
  void * ) // user )
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXt::sensorQueueChanged", "called" );
#endif // SOXT_DEBUG
  SoSensorManager * sensormanager = SoDB::getSensorManager();

  SbTime timevalue;
  if ( sensormanager->isTimerSensorPending( timevalue ) ) {
    SbTime interval = timevalue - SbTime::getTimeOfDay();

#if SOXT_DEBUG && 0 // debug
    SoDebugError::postInfo("SoXt::sensorQueueChanged",
                           "interval: %f (msec: %d)",
                           interval.getValue(),
                           interval.getMsecValue());
#endif // debug

    // On a system with some load, the interval value can easily get
    // negative. For Xt, this means it will never trigger -- which
    // causes all kinds of problems. Setting it to 0 will make it
    // trigger more-or-less immediately.
    if (interval.getValue() < 0.0) interval.setValue(0.0);

    if ( SoXt::timerSensorActive ) XtRemoveTimeOut( SoXt::timerSensorId );

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
      SoXt::idleSensorId =
        XtAppAddWorkProc( SoXt::getAppContext(), SoXt::idleSensorCB, NULL );
      SoXt::idleSensorActive = TRUE;
    }

    if ( ! SoXt::delaySensorActive ) {
      unsigned long timeout = SoDB::getDelaySensorTimeout().getMsecValue();
      SoXt::delaySensorId =
        XtAppAddTimeOut( SoXt::getAppContext(), timeout,
                         SoXt::delaySensorCB, NULL );
      SoXt::delaySensorActive = TRUE;
    }
  } else {
    if ( SoXt::idleSensorActive ) {
      XtRemoveWorkProc( SoXt::idleSensorId );
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

/*!
  \var SoXt::fallback_resources

  This is an array of X resources.
  They are not in use yet - the implementation is purely experimental.
*/

String
SoXt::fallback_resources[] =
{
#define _COMPONENT "*SoXtGLWidget*"
  _COMPONENT     "background:"				"white", // not used
#undef _COMPONENT

#define _COMPONENT "*SoXtRenderArea*"
  _COMPONENT     "border:"				"false",
  _COMPONENT     "borderThickness:"			"2",
  _COMPONENT     "background:"				"black",
#undef _COMPONENT

#define _COMPONENT "*SoXtExaminerViewer*"
  _COMPONENT	"title:"				"Examiner Viewer",
  _COMPONENT	"LeftWheelLabel.labelString:"		"Rot Y",
  _COMPONENT	"BottomWheelLabel.labelString:"		"Rot X",
  _COMPONENT	"RightWheelLabel.dollyString:"		"Dolly",
  _COMPONENT	"RightWheelLabel.zoomString:"		"Zoom",
#undef _COMPONENT

#define _COMPONENT "*SoXtPlaneViewer*"
  _COMPONENT	"title:"				"Plane Viewer",
  _COMPONENT	"LeftWheelLabel.labelString:"		"Trans Y",
  _COMPONENT	"BottomWheelLabel.labelString:"		"Trans X",
  _COMPONENT	"RightWheelLabel.dollyString:"		"Dolly",
  _COMPONENT	"RightWheelLabel.zoomString:"		"Zoom",
#undef _COMPONENT

#define _COMPONENT "*SoXtWalkViewer*"
  _COMPONENT	"title:"				"Walk Viewer",
#undef _COMPONENT

#define _COMPONENT "*SoXtFlyViewer*"
  _COMPONENT	"title:"				"Fly Viewer",
#undef _COMPONENT

#define _COMPONENT "*SoXtDirectionalLightEditor*"
#undef _COMPONENT

#define _COMPONENT "*SoXtMaterialEditor*"
#undef _COMPONENT

#define _COMPONENT "*SoXtLightSliderSet*"
#undef _COMPONENT

#define _COMPONENT "*SoXtMaterialSliderSet*"
#undef _COMPONENT

#define _COMPONENT "*SoXtTransformSliderSet*"
#undef _COMPONENT

#define _COMPONENT "*SoXtMaterialList*"
#undef _COMPONENT

#define _COMPONENT "*SoXtPrintDialog*"
  _COMPONENT	"title:"				"Print",
#undef _COMPONENT

  NULL
}; // fallback_resources

// *************************************************************************
