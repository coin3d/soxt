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

#if SOXT_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOXT_DEBUG

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xmu/Editres.h>
#include <X11/Xmu/StdCmap.h>
#include <Xm/Xm.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/misc/SoBasic.h>
#include <Inventor/SbPList.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/sensors/SoSensorManager.h>

#include <soxtdefs.h>

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
SbPList *     SoXt::eventHandlers = NULL;

char *        SoXt::appName = NULL;
char *        SoXt::appClass = NULL;

// *************************************************************************

/*!
  This method will fill in the integers pointed to by the arguments with the
  corresponding part of the version id.  NULL pointers are ignored.

  This method is not part of the original SoXt API from SGI.
*/

void
SoXt::getVersionInfo( // static
  int * const major,
  int * const minor,
  int * const micro )
{
  if ( major ) *major = SOXT_MAJOR_VERSION;
  if ( minor ) *minor = SOXT_MINOR_VERSION;
  if ( micro ) *micro = SOXT_MICRO_VERSION;
} // getVersionInfo()

/*!
  This method returns a string containing the version id of the library.

  This method is not part of the original SoXt API from SGI.
*/

const char *
SoXt::getVersionString( // static
  void )
{
  return SOXT_VERSION;
} // getVersionString()

// *************************************************************************

/*!
*/

Widget
SoXt::init( // static
  const char * const appName,
  const char * const appClass )
{
  int argc = 1;
  char * argv[] = { (char *) appName, NULL };
  return SoXt::init( argc, argv, appName, appClass );
} // init()

/*!
*/

Widget
SoXt::init( // static
  int & argc,
  char ** argv,
  const char * const appName,
  const char * const appClass )
{
  if ( appName )
    SoXt::appName = strcpy( new char [strlen(appName) + 1], appName );
  if ( appClass )
    SoXt::appClass = strcpy( new char [strlen(appClass) + 1], appClass );

  XtAppContext tempcontext; // SoXt::xtAppContext is set later

  Display * display = NULL;
  int depth = 0;
  Visual * visual = NULL;
  Colormap colormap = 0;

  SoXt::selectBestVisual( display, visual, colormap, depth );

  Widget toplevel = (Widget) NULL;
  if ( visual ) {
    toplevel = XtVaOpenApplication(
      &tempcontext, SoXt::appClass, NULL, 0, &argc, argv,
      SoXt::fallback_resources, topLevelShellWidgetClass,
      XmNvisual, visual,
      XmNdepth, depth,
      XmNcolormap, colormap,
      NULL );
  } else {
    SoDebugError::postInfo( "SoXt::init", "default toplevel! (error)" );
    toplevel = XtVaOpenApplication(
      &tempcontext, SoXt::appClass, NULL, 0, &argc, argv,
      SoXt::fallback_resources, topLevelShellWidgetClass,
      NULL );
  }


  SoXt::init( toplevel );
  return toplevel;
} // init()

/*!
*/

void
SoXt::init( // static
  Widget toplevel )
{
#if SOXT_DEBUG
  setbuf( stdout, NULL );
  setbuf( stderr, NULL );
#endif // SOXT_DEBUG

  SoXt::mainWidget = toplevel;
  SoXt::display = XtDisplay( toplevel );
  SoXt::xtAppContext = XtWidgetToApplicationContext( toplevel );

  SoDB::init();
  SoNodeKit::init();
  SoInteraction::init();

  SoDB::getSensorManager()->setChangedCallback( SoXt::sensorQueueChanged, NULL);

  XtAppSetFallbackResources( SoXt::getAppContext(), SoXt::fallback_resources );

  XtEventHandler editres_hook = (XtEventHandler) _XEditResCheckMessages;
  XtAddEventHandler( toplevel, (EventMask) 0, True, editres_hook, NULL );
} // init()

// *************************************************************************

/*
  Internal function for translating type code to event name.
*/

static const char *
_eventName(
  const int type )
{
  static const char * names[LASTEvent];
  static int first = 1;
  if ( first ) {
    for ( int i = 0; i < LASTEvent; i++ )
      names[i] = "<not set>";
    names[KeyPress]          = "KeyPress";
    names[KeyRelease]        = "KeyRelease";
    names[ButtonPress]       = "ButtonPress";
    names[ButtonRelease]     = "ButtonRelease";
    names[MotionNotify]      = "MotionNotify";
    names[EnterNotify]       = "EnterNotify";
    names[LeaveNotify]       = "LeaveNotify";
    names[FocusIn]           = "FocusIn";
    names[FocusOut]          = "FocusOut";
    names[KeymapNotify]      = "KeymapNotify";
    names[Expose]            = "Expose";
    names[GraphicsExpose]    = "GraphicsExpose";
    names[NoExpose]          = "NoExpose";
    names[VisibilityNotify]  = "VisibilityNotify";
    names[DestroyNotify]     = "DestroyNotify";
    names[CreateNotify]      = "CreateNotify";
    first = 0;
  }
  if ( type >= 0 && type < LASTEvent )
    return names[type];
  return "<unknown>";
} // _eventName()

/*!
  This function should be used instead of XtAppMainLoop().  The current
  implementation is no different, but in the future we will add support
  for input device extensions in this event dispatch loop.

  If you need your own event dispatching loop, base your code on this one,
  and use SoXt::dispatchEvent() when appropriate.
*/

void
SoXt::mainLoop( // static
  void )
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXt::mainLoop", "[enter]" );
#endif // SOXT_DEBUG
  XEvent event;
  XtAppContext context = SoXt::getAppContext();
  Boolean exit = XtAppGetExitFlag( context );
  while ( ! exit ) {
    SoXt::nextEvent( context, &event );
    Boolean dispatched = SoXt::dispatchEvent( &event );
#if SOXT_DEBUG && 0
    if ( ! dispatched ) {
      SoDebugError::postInfo( "SoXt::mainLoop", "no event handler (%2d : %s)",
        event.type, _eventName( event.type ) );
    }
#endif // SOXT_DEBUG
    exit = XtAppGetExitFlag( context );
  }
#if SOXT_DEBUG
  SoDebugError::postInfo( "SoXt::mainLoop", "[exit]" );
#endif // SOXT_DEBUG
} // mainLoop()

/*!
  This method calls XtAppNextEvent() to fill in the event structure with
  the next event for the given context.
*/

void
SoXt::nextEvent( // static
  XtAppContext context,
  XEvent * event )
{
  XtAppNextEvent( context, event );
} // nextEvent()

/*!
  This method dispatches the event by calling XtDispatchEvent().  Special
  handling of extension input devices will be handled in this method in
  the future.
*/

Boolean
SoXt::dispatchEvent( // static
  XEvent * event )
{
  Boolean dispatched = XtDispatchEvent( event );
  if ( ! dispatched ) {
    XtEventHandler handler;
    XtPointer data;
    Widget widget;
    SoXt::getExtensionEventHandler( event, widget, handler, data );
    if ( handler ) {
      Boolean dispatch = False;
      (*handler)( widget, data, event, &dispatch );
      if ( dispatch == False )
        dispatched = True;
    }
  }
  return dispatched;
} // dispatchEvent()

// *************************************************************************

/*!
*/

XtAppContext
SoXt::getAppContext( // static
  void )
{
  return SoXt::xtAppContext;
} // getAppContext()

/*!
*/

Display *
SoXt::getDisplay( // static
  void )
{
  return SoXt::display;
} // getDisplay()

/*!
*/

Widget
SoXt::getTopLevelWidget( // static
  void )
{
  return SoXt::mainWidget;
} // getTopLevelWidget()

// *************************************************************************

/*!
*/

const char *
SoXt::getAppName( // static
  void )
{
  return SoXt::appName;
} // getAppName()

/*!
*/

const char *
SoXt::getAppClass( // static
  void )
{
  return SoXt::appClass;
} // getAppClass()

// *************************************************************************

/*!
*/

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

/*!
*/

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
  XmStringGetLtoR( xstring, XmSTRING_DEFAULT_CHARSET, &str );
  return str;
} // decodeString()

// *************************************************************************

/*!
*/

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

/*!
*/

SbVec2s
SoXt::getWidgetSize( // static
  Widget widget )
{
  int w = 0, h = 0;
  XtVaGetValues( widget, XtNwidth, &w, XtNheight, &h, NULL );
  return SbVec2s( w, h );
} // getWidgetSize()

// *************************************************************************

/*!
*/

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
  return (Widget) NULL;
} // getShellWidget()

// *************************************************************************

/*!
*/

void
SoXt::createSimpleErrorDialog( // static
  Widget widget,
  char * title,
  char * string1,
  char * string2 )
{
  SOXT_STUB();
} // createSimpleErrorDialog()

/*!
*/

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

/*!
*/

void
SoXt::registerColormapLoad( // static
  Widget widget,
  Widget shell )
{
  SOXT_STUB();
} // registerColormapLoad()

/*!
*/

void
SoXt::addColormapToShell( // static
  Widget widget,
  Widget shell )
{
  SOXT_STUB();
} // addColormapToShell()

/*!
*/

void
SoXt::removeColormapFromShell( // static
  Widget widget,
  Widget shell )
{
  SOXT_STUB();
} // removeColormapFromShell()

// *************************************************************************

typedef struct _EventHandlerInfo {
  int type;
  Widget widget;
  XtEventHandler handler;
  XtPointer data;
} EventHandlerInfo;

/*!
*/

void
SoXt::addExtensionEventHandler( // static
  Widget widget,
  int type,
  XtEventHandler proc,
  XtPointer data )
{
  EventHandlerInfo * info = new EventHandlerInfo;
  info->type = type;
  info->widget = widget;
  info->handler = proc;
  info->data = data;

  if ( SoXt::eventHandlers == NULL )
    SoXt::eventHandlers = new SbPList;

#if SOXT_DEBUG
  const int handlers = SoXt::eventHandlers->getLength();
  for ( int i = 0; i < handlers; i++ ) {
    EventHandlerInfo * query = (EventHandlerInfo *) (*SoXt::eventHandlers)[i];
    if ( query->type == type )
      SoDebugError::postWarning( "SoXt::addExtensionEventHandler",
        "handler of type %d already exists, shadowing the new handler" );
  }
#endif // SOXT_DEBUG

  SoXt::eventHandlers->append( (void *) info );
} // addExtensionEventHandler()

/*!
*/

void
SoXt::removeExtensionEventHandler( // static
  Widget widget,
  int type,
  XtEventHandler proc,
  XtPointer data )
{
  if ( SoXt::eventHandlers == NULL ) {
#if SOXT_DEBUG
    SoDebugError::postInfo( "SoXt::removeExtensionEventHandler",
      "no extension event handlers registered." );
#endif // SOXT_DEBUG
    return;
  }
  int handlers = SoXt::eventHandlers->getLength();
  for ( int i = 0; i < handlers; i++ ) {
    EventHandlerInfo * info = (EventHandlerInfo *) (*SoXt::eventHandlers)[i];
    if ( info->widget == widget && info->type == type &&
         info->handler == proc && info->data == data ) {
      SoXt::eventHandlers->remove(i);
      delete info;
      return;
    }
  }
#if SOXT_DEBUG
  SoDebugError::postInfo( "SoXt::removeExtensionEventHandler",
    "no such extension event handler registered." );
#endif // SOXT_DEBUG
} // removeExtensionEventHandler()

/*!
*/

void
SoXt::getExtensionEventHandler( // static, protected
  XEvent * event,
  Widget & widget,
  XtEventHandler & proc,
  XtPointer & data )
{
  proc = (XtEventHandler) NULL;
  data = (XtPointer) NULL;
  widget = (Widget) NULL;

  if ( SoXt::eventHandlers == NULL )
    return;

  const int handlers = SoXt::eventHandlers->getLength();
  for ( int i = 0; i < handlers; i++ ) {
    EventHandlerInfo * info = (EventHandlerInfo *) (*SoXt::eventHandlers)[i];
    if ( info->type == event->type ) {
      widget = info->widget;
      proc = info->handler;
      data = info->data;
    }
  }
} // getExtensionEventHandler()

// *************************************************************************

/*!
*/

Widget
SoXt::getwidget( // static
  unsigned int what )
{
  SOXT_STUB();
  return (Widget) NULL;
} // getwidget()

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
  \internal
*/

static const char *
_visualClassName(
  const int vclass )
{
  switch ( vclass ) {
  case StaticGray:   return "StaticGray";
  case GrayScale:    return "GrayScale";
  case StaticColor:  return "StaticColor";
  case PseudoColor:  return "PseudoColor";
  case TrueColor:    return "TrueColor";
  case DirectColor:  return "DirectColor";
  default:           return "<unknown>";
  }
} // _visualClassName()

/*!
  This function tries to find the best visual type, depth, and colormap
  combination for the display.  The display argument may be given, or set
  to NULL - then the default display will be used and returned.

  When SoXt doesn't get it's graphics displayed correctly, this routine may
  be the root of the problem.  Let's hope it solves more problems than it
  introduces...

  Misc Links:

  "Beyond the Default Visual", by John Cwikla
    http://www.motifzone.com/tmd/articles/DefaultVisual/DefaultVisual.html

*/

void
SoXt::selectBestVisual( // static
  Display *& dpy,
  Visual *& visual,
  Colormap & colormap,
  int & depth )
{
  if ( dpy == NULL )
     dpy = XOpenDisplay( NULL );
  assert( dpy != NULL );

  int snum = XDefaultScreen( dpy );

  if ( XDefaultDepth( dpy, snum ) >= 24 ) { // me like...
    depth = XDefaultDepth( dpy, snum );
    visual = XDefaultVisual( dpy, snum );
    colormap = XDefaultColormap( dpy, snum );
    return;
  }

#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXt::selectBestVisual",
    "we're beyond the default visual" );
#endif // SOXT_DEBUG

  static struct {
    int depth;
    int vclass;
  } pri[] = { // how things are prioritized
    { 24, DirectColor },
    { 24, TrueColor },
    { 24, PseudoColor },
    { 16, DirectColor },
    { 16, TrueColor },
    { 16, PseudoColor },
    { 12, DirectColor },
    { 12, TrueColor },
    { 12, PseudoColor },
    { 8, PseudoColor },
    { 0, 0 }
  };

  XVisualInfo vinfo;
  for ( int i = 0; pri[i].depth != 0; i++ ) {
    if ( XMatchVisualInfo( dpy, snum, pri[i].depth, pri[i].vclass, &vinfo ) ) {
#if SOXT_DEBUG
      SoDebugError::postInfo( "SoXt::selectBestVisual",
        "visual: depth=%d, class=%s", vinfo.depth,
        _visualClassName( vinfo.c_class ) );
#endif // SOXT_DEBUG
      visual = vinfo.visual;
      depth = vinfo.depth;

      int numcmaps;
      XStandardColormap * stdcolormaps = NULL;

      if ( XmuLookupStandardColormap( dpy, vinfo.screen, vinfo.visualid,
             vinfo.depth, XA_RGB_DEFAULT_MAP, False, True )
        && XGetRGBColormaps( dpy, RootWindow( dpy, vinfo.screen ),
             &stdcolormaps, &numcmaps, XA_RGB_DEFAULT_MAP ) ) {
        SbBool found = FALSE;
        for ( i = 0; i < numcmaps && ! found; i++ ) {
          if ( stdcolormaps[i].visualid == vinfo.visualid ) {
            colormap = stdcolormaps[i].colormap;
            found = TRUE;
          }
        }
        if ( ! found ) {
          SoDebugError::postInfo( "SoXt::selectBestVisual",
            "standard RGB colormaps did not work with visual - creating own colormap" );
          colormap = XCreateColormap(
            dpy, RootWindow( dpy, vinfo.screen ), vinfo.visual, AllocNone );
        }
      } else {
        SoDebugError::postInfo( "SoXt::selectBestVisual",
          "no standard RGB colormaps - creating own colormap" );
        colormap = XCreateColormap(
          dpy, RootWindow( dpy, vinfo.screen ), vinfo.visual, AllocNone );
      }
      XtFree( (char *) stdcolormaps );
      return;
    }
  }
#if SOXT_DEBUG
  SoDebugError::postInfo( "SoXt::selectBestVisual", "yikes!" );
#endif // SOXT_DEBUG
} // selectBestVisual()

// *************************************************************************

/*!
  \var SoXt::fallback_resources

  This is an array of X resources.
  They are not in use yet - the implementation is purely experimental.
*/

String
SoXt::fallback_resources[] =
{
#define _COMPONENT "*SoXtGLWidget"
  _COMPONENT     ".background:"				"white", // not used
#undef _COMPONENT

#define _COMPONENT "*SoXtRenderArea"
  _COMPONENT     ".border:"				"false",
  _COMPONENT     ".borderThickness:"			"2",
  _COMPONENT     ".background:"				"black",
#undef _COMPONENT

#define _COMPONENT "*SoXtExaminerViewer"
  _COMPONENT	".title:"				"Examiner Viewer",
  _COMPONENT	"*LeftWheelLabel.labelString:"		"Rot Y",
  _COMPONENT	"*BottomWheelLabel.labelString:"	"Rot X",
  _COMPONENT	"*RightWheelLabel.dollyString:"		"Dolly",
  _COMPONENT	"*RightWheelLabel.zoomString:"		"Zoom",
#undef _COMPONENT

#define _COMPONENT "*SoXtPlaneViewer"
  _COMPONENT	".title:"				"Plane Viewer",
  _COMPONENT	"*LeftWheelLabel.labelString:"		"Trans Y",
  _COMPONENT	"*BottomWheelLabel.labelString:"	"Trans X",
  _COMPONENT	"*RightWheelLabel.dollyString:"		"Dolly",
  _COMPONENT	"*RightWheelLabel.zoomString:"		"Zoom",
#undef _COMPONENT

#define _COMPONENT "*SoXtWalkViewer"
  _COMPONENT	".title:"				"Walk Viewer",
#undef _COMPONENT

#define _COMPONENT "*SoXtFlyViewer"
  _COMPONENT	".title:"				"Fly Viewer",
  _COMPONENT	".LeftWheelLabel:"			"Tilt",
  _COMPONENT	".BottomWheelLabel:"			"Rotate",
  _COMPONENT	".RotateWheelLabel:"			"Dolly",
#undef _COMPONENT

#define _COMPONENT "*SoXtDirectionalLightEditor"
#undef _COMPONENT

#define _COMPONENT "*SoXtMaterialEditor"
#undef _COMPONENT

#define _COMPONENT "*SoXtLightSliderSet"
  _COMPONENT	".title:"				"Light Slider Set",
  _COMPONENT	".module1:"				"LightIntensity",
  _COMPONENT	".module2:"				"LightColor",
#undef _COMPONENT

#define _COMPONENT "*SoXtMaterialSliderSet"
  _COMPONENT	".title:"				"Material Slider Set",
  _COMPONENT	".module1:"				"AmbientColor",
  _COMPONENT	".module2:"				"DiffuseColor",
  _COMPONENT	".module3:"				"SpecularColor",
  _COMPONENT	".module4:"				"EmissiveColor",
  _COMPONENT	".module5:"				"Shininess",
  _COMPONENT	".module6:"				"Transparency",
#undef _COMPONENT

#define _COMPONENT "*SoXtTransformSliderSet"
  _COMPONENT	".title:"				"Transform Slider Set",
  _COMPONENT	".module1:"				"Translate",
  _COMPONENT	".module2:"				"Scale",
  _COMPONENT	".module3:"				"Rotate",
  _COMPONENT	".module4:"				"Center",
#undef _COMPONENT

#define _COMPONENT "*SoXtMaterialList"
#undef _COMPONENT

#define _COMPONENT "*SoXtPrintDialog"
  _COMPONENT	"*title:"				"Print",
#undef _COMPONENT

#define _COMPONENT "*SoXtAmbientColorSliderModule"
  _COMPONENT	".title:"				"AMBIENT COLOR",
  _COMPONENT	".slider1Title:"			"Red",
  _COMPONENT	".slider1Field:"			"ambientColor[0]",
  _COMPONENT	".slider2Title:"			"Green",
  _COMPONENT	".slider2Field:"			"ambientColor[1]",
  _COMPONENT	".slider3Title:"			"Blue",
  _COMPONENT	".slider3Field:"			"ambientColor[2]",
#undef _COMPONENT

#define _COMPONENT "*SoXtCenterSliderModule"
  _COMPONENT	".title:"				"CENTER",
#undef _COMPONENT

#define _COMPONENT "*SoXtDiffuseColorSliderModule"
  _COMPONENT	".title:"				"DIFFUSE COLOR",
#undef _COMPONENT

#define _COMPONENT "*SoXtEmissiveColorSliderModule"
  _COMPONENT	".title:"				"EMISSIVE COLOR",
#undef _COMPONENT

#define _COMPONENT "*SoXtLightColorSliderModule"
  _COMPONENT	".title:"				"COLOR",
  _COMPONENT	".slider1Title:"			"Red",
  _COMPONENT	".slider1Field:"			"color[0]",
  _COMPONENT	".slider2Title:"			"Red",
  _COMPONENT	".slider2Field:"			"color[1]",
  _COMPONENT	".slider3Title:"			"Red",
  _COMPONENT	".slider3Field:"			"color[2]",
#undef _COMPONENT

#define _COMPONENT "*SoXtLightIntensitySliderModule"
  _COMPONENT	".title:"				"INTENSITY",
  _COMPONENT	".slider1Title:"			"Intensity",
  _COMPONENT	".slider1Field:"			"intensity",
#undef _COMPONENT

#define _COMPONENT "*SoXtRotateSliderModule"
  _COMPONENT	".title:"				"ROTATION",
#undef _COMPONENT

#define _COMPONENT "*SoXtScaleSliderModule"
  _COMPONENT	".title:"				"SCALE",
#undef _COMPONENT

#define _COMPONENT "*SoXtShininessSliderModule"
  _COMPONENT	".title:"				"SHININESS",
#undef _COMPONENT

#define _COMPONENT "*SoXtSpecularColorSliderModule"
  _COMPONENT	".title:"				"SPECULAR COLOR",
#undef _COMPONENT

#define _COMPONENT "*SoXtTranslateSliderModule"
  _COMPONENT	".title:"				"TRANSLATE",
#undef _COMPONENT

#define _COMPONENT "*SoXtTransparencySliderModule"
  _COMPONENT	".title:"				"TRANSPARENCY",
#undef _COMPONENT

  NULL
}; // fallback_resources

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

