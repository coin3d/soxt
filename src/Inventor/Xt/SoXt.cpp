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

#if SOXT_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOXT_DEBUG

/*!
  \class SoXt Inventor/Xt/SoXt.h
  \brief The SoXt class provides initialization and run-time management
  functions for the SoXt library.

  All methods in SoXt are static methods.
*/

// *************************************************************************

#include <assert.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xmu/Editres.h>
#include <X11/Xmu/StdCmap.h>
#include <X11/Xmu/Atoms.h>
#include <Xm/Xm.h>
#include <Xm/Protocols.h>
#include <Xm/MessageB.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/misc/SoBasic.h>
#include <Inventor/SbPList.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/sensors/SoSensorManager.h>
#include <Inventor/SbTime.h>

#include <soxtdefs.h>

#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtObject.h>
#include <Inventor/Xt/devices/SoXtDevice.h>
#include <Inventor/Xt/SoXtComponent.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

// *************************************************************************

/*!
  \mainpage

  SoXt is an Xt/Motif glue library for Coin.  It can also be used
  on top of Open Inventor, and is designed to be source code
  compatible with SGI's InventorXt library (which is a goal that
  is still way off).  You will need either Motif
  (<a href="http://www.opengroup.org/openmotif/">Open Motif</a>)
  or <a href="http://www.lesstif.org/">Lesstif</a> to use SoXt.

  SoXt is in alpha release, meaning that it is buggy and lacks
  functionality.  In other words, hope for the best but expect the
  worst.

  The corresponding documentation for Coin is located at
  http://doc.coin3d.org/Coin/
*/

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

static Atom   WM_PROTOCOLS = 0;
static Atom   WM_DELETE_WINDOW = 0;

// *************************************************************************

/*!
  This method will fill in the integers pointed to by the arguments with the
  corresponding part of the version id.  NULL pointers are ignored.

  This method is not part of the original SoXt API from SGI.
*/

void
SoXt::getVersionInfo(// static
  int * const major,
  int * const minor,
  int * const micro)
{
  if (major) *major = SOXT_MAJOR_VERSION;
  if (minor) *minor = SOXT_MINOR_VERSION;
  if (micro) *micro = SOXT_MICRO_VERSION;
} // getVersionInfo()

/*!
  This method returns a string containing the version id of the library.

  This method is not part of the original SoXt API from SGI.
*/

const char *
SoXt::getVersionString(// static
  void)
{
  static const char version[] = SOXT_VERSION;
  return version;
} // getVersionString()

// *************************************************************************

/*!
  This function initializes the SoXt library.

  The returned Widget is a toplevel shell widget for the application, which
  can be used as a shell for the main component.
*/

Widget
SoXt::init(// static
  const char * const appName,
  const char * const appClass)
{
  int argc = 1;
  char * argv[] = { (char *) appName, NULL };
  return SoXt::init(argc, argv, appName, appClass);
} // init()

/*!
  This function initializes the SoXt library.

  The returned Widget is a toplevel shell widget for the application, which
  can be used as a shell for the main component.
*/

Widget
SoXt::init(// static
  int & argc,
  char ** argv,
  const char * const appName,
  const char * const appClass)
{
  // FIXME: as far as I can see, no SoXt::init() method in InventorXt
  // matches the signature of this constructor. So we should probably
  // mark this as SoXt/Coin-only (but look over InventorXt again).
  //
  // Also investigate our other So*-libraries to see if they contain
  // this Coin-specific init() method. 20010919 mortene.

  if (appName)
    SoXt::appName = strcpy(new char [strlen(appName) + 1], appName);
  if (appClass)
    SoXt::appClass = strcpy(new char [strlen(appClass) + 1], appClass);

  XtAppContext tempcontext; // SoXt::xtAppContext is set later

  Display * display = NULL;
  int depth = 0;
  Visual * visual = NULL;
  Colormap colormap = 0;

  SoXt::selectBestVisual(display, visual, colormap, depth);

  Widget toplevel = (Widget) NULL;
  if (visual) {
    toplevel = XtVaOpenApplication(
      &tempcontext, SoXt::appClass, NULL, 0, &argc, argv,
      SoXt::fallback_resources, topLevelShellWidgetClass,
      XmNvisual, visual,
      XmNdepth, depth,
      XmNcolormap, colormap,
      NULL);
  } else {
    SoDebugError::postInfo("SoXt::init", "default toplevel! (error)");
    toplevel = XtVaOpenApplication(
      &tempcontext, SoXt::appClass, NULL, 0, &argc, argv,
      SoXt::fallback_resources, topLevelShellWidgetClass,
      NULL);
  }
  if (appName)
    XtVaSetValues(toplevel, XmNtitle, SoXt::appName, NULL);

  SoXt::init(toplevel);
  return toplevel;
} // init()

// documented in common/SoGuiObject.cpp.in
void
SoXtObject::init(// static
  void)
{
  SoXtObject::initClass();
  SoXtDevice::initClasses();
  SoXtComponent::initClasses();
} // init()

/*
  \internal
*/

static
void
wm_close_handler(
  Widget widget,
  XtPointer user,
  XEvent * e,
  Boolean * dispatch)
{
  if (e->type == ClientMessage) {
    XClientMessageEvent * event = (XClientMessageEvent *) e;
    if (WM_PROTOCOLS == None) {
      WM_PROTOCOLS = XInternAtom(SoXt::getDisplay(), "WM_PROTOCOLS", True);
      if (WM_PROTOCOLS == None) WM_PROTOCOLS = (Atom) -1;
    }
    if (WM_DELETE_WINDOW == None) {
      WM_DELETE_WINDOW = XInternAtom(SoXt::getDisplay(),
        "WM_DELETE_WINDOW", True);
      if (WM_DELETE_WINDOW == None) WM_DELETE_WINDOW = (Atom) -1;
    }
    if (event->message_type == WM_PROTOCOLS &&
         (unsigned) event->data.l[0] == WM_DELETE_WINDOW) {
      XtAppSetExitFlag(SoXt::getAppContext());
      *dispatch = False;
    }
  }
} // wm_close_handler()

/*!
  This function initializes the SoXt library.

  It should be called for applications that want to create the toplevel
  shell widget themselves.
*/

void
SoXt::init(// static
  Widget toplevel)
{
#if SOXT_DEBUG
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);
#endif // SOXT_DEBUG

  SoXt::mainWidget = toplevel;
  SoXt::display = XtDisplay(toplevel);
  SoXt::xtAppContext = XtWidgetToApplicationContext(toplevel);

  SoDB::init();
  SoNodeKit::init();
  SoInteraction::init();
  SoXtObject::init();

  SoDB::getSensorManager()->setChangedCallback(SoXt::sensorQueueChanged, NULL);

  XtAppSetFallbackResources(SoXt::getAppContext(), SoXt::fallback_resources);

  XtAddEventHandler(toplevel, (EventMask) 0, True, wm_close_handler, NULL);

#if SOXT_DEBUG
  XtEventHandler editres_hook = (XtEventHandler) _XEditResCheckMessages;
  XtAddEventHandler(toplevel, (EventMask) 0, True, editres_hook, NULL);
#endif // SOXT_DEBUG
} // init()

// *************************************************************************

/*
  This is a function for translating X11 event type id to event name.

  \internal
*/

static const char *
_eventName(
  const int type)
{
  static const char * names[LASTEvent];
  static int first = 1;
  if (first) {
    for (int i = 0; i < LASTEvent; i++)
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
  if (type >= 0 && type < LASTEvent)
    return names[type];
  return "<unknown>";
} // _eventName()

/*!
  This function should be used instead of XtAppMainLoop().  The current
  implementation is no different than the standard event loop, but in the
  future it will add support for input device extensions.

  If you need your own event dispatching loop, base your code on this one,
  and use SoXt::dispatchEvent() when appropriate.
*/

void
SoXt::mainLoop(// static
  void)
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXt::mainLoop", "[enter]");
#endif // SOXT_DEBUG
  XEvent event;
  XtAppContext context = SoXt::getAppContext();
  Boolean exit = XtAppGetExitFlag(context);
  while (! exit) {
    SoXt::nextEvent(context, &event);
    Boolean dispatched = SoXt::dispatchEvent(&event);
#if SOXT_DEBUG && 0
    if (! dispatched) {
      SoDebugError::postInfo("SoXt::mainLoop", "no event handler (%2d : %s)",
        event.type, _eventName(event.type));
    }
#endif // SOXT_DEBUG
    exit = XtAppGetExitFlag(context);
  }
} // mainLoop()

/*!
  This function calls XtAppNextEvent() to fill in the event structure with
  the next event for the given context.
*/

void
SoXt::nextEvent(// static
  XtAppContext context,
  XEvent * event)
{
  XtAppNextEvent(context, event);
} // nextEvent()

/*!
  This method dispatches the event by calling XtDispatchEvent().  Special
  handling of extension input devices will be handled in this method in
  the future.
*/

Boolean
SoXt::dispatchEvent(// static
  XEvent * event)
{
  Boolean dispatched = XtDispatchEvent(event);
  if (! dispatched) {
    XtEventHandler handler;
    XtPointer data;
    Widget widget;
    SoXt::getExtensionEventHandler(event, widget, handler, data);
    if (handler) {
      Boolean dispatch = False;
      (*handler)(widget, data, event, &dispatch);
      if (dispatch == False)
        dispatched = True;
    }
  }
  return dispatched;
} // dispatchEvent()

/*!
  This method makes the event loop stop processing events and exit so the
  program can finish.

  This is an SoXt extension not found in InventorXt.

  \sa SoXt::mainLoop
*/

void
SoXt::exitMainLoop(
  void)
{
  XtAppSetExitFlag(SoXt::getAppContext());
} // exitMainLoop()

// *************************************************************************

/*!
  This function returns the Xt Application Context for the application.
*/

XtAppContext
SoXt::getAppContext(// static
  void)
{
  return SoXt::xtAppContext;
} // getAppContext()

/*!
  This function returns the X Display of the application.
*/

Display *
SoXt::getDisplay(// static
  void)
{
  return SoXt::display;
} // getDisplay()

/*!
  This function returns the toplevel shell for the application.
*/

Widget
SoXt::getTopLevelWidget(// static
  void)
{
  return SoXt::mainWidget;
} // getTopLevelWidget()

// *************************************************************************

/*!
  This function returns the application name, given to SoXt::init.
*/

const char *
SoXt::getAppName(// static
  void)
{
  return SoXt::appName;
} // getAppName()

/*!
  This function returns the application class, given to SoXt::init.
*/

const char *
SoXt::getAppClass(// static
  void)
{
  return SoXt::appClass;
} // getAppClass()

// *************************************************************************

/*!
  This function realizes the given \a widget.
*/

void
SoXt::show(// static
  Widget widget)
{
  if (XtIsTopLevelShell(widget)) {
    XtRealizeWidget(widget);
  } else {
    XtManageChild(widget);
  }
} // show()

/*!
  This function hides the given \a widget.
*/

void
SoXt::hide(// static
  Widget widget)
{
  if (XtIsTopLevelShell(widget)) {
    XtUnrealizeWidget(widget);
  } else {
    XtUnmanageChild(widget);
  }
} // hide()

// *************************************************************************

/*!
  Create and return a localized string from \a string.

  The caller is responsible for freeing the returned XmString by using
  XmStringFree() or just plain XtFree((char *) string).
*/

XmString
SoXt::encodeString(// static
  const char * const string)
{
  return XmStringCreateLocalized((char *) string);
} // encodeString()

/*!
  Decode a left-to-right localized string into a simple character
  array, and return its memory pointer.

  The caller is responsible for freeing the returned character string
  with XtFree() to avoid memory leaks.
*/

char *
SoXt::decodeString(// static
  XmString xstring)
{
  char * str;
  /* set str to point to the text */
  XmStringGetLtoR(xstring, XmSTRING_DEFAULT_CHARSET, &str);
  return str;
} // decodeString()

// *************************************************************************

/*!
  This function resizes the widget to the given \a size.
*/

void
SoXt::setWidgetSize(// static
  Widget widget,
  const SbVec2s size)
{
  if (! widget) {
#if SOXT_DEBUG
    SoDebugError::postInfo("SoXt::setWidgetSize", "called with no widget");
#endif // SOXT_DEBUG
    return;
  }
  XtVaSetValues(widget,
    XmNwidth, size[0],
    XmNheight, size[1],
    NULL);
} // setWidgetSize()

/*!
  This function returns the size of the given widget.
*/

SbVec2s
SoXt::getWidgetSize(// static
  Widget widget)
{
  if (! widget) {
#if SOXT_DEBUG
    SoDebugError::postInfo("SoXt::getWidgetSize", "called with no widget");
#endif // SOXT_DEBUG
    return SbVec2s(0, 0);
  }
  Dimension width, height;
  XtVaGetValues(widget,
    XtNwidth, &width,
    XtNheight, &height,
    NULL);
  return SbVec2s(width, height);
} // getWidgetSize()

// *************************************************************************

/*!
  This function returns the shell of the given \a widget.
*/

Widget
SoXt::getShellWidget(// static
  Widget widget)
{
  Widget p = widget;
  while (p != (Widget) NULL) {
    if (XtIsShell(p))
      return p;
    p = XtParent(p);
  }
#if SOXT_DEBUG
  SoDebugError::postInfo("SoXt::getShellWidget",
    "couldn't find shell for widget at %p", widget);
#endif // SOXT_DEBUG
  return (Widget) NULL;
} // getShellWidget()

// *************************************************************************

/*
  Internal callback.
*/

static
void
close_dialog_cb(
  Widget,
  XtPointer closure,
  XtPointer)
{
  Widget dialogshell = (Widget) closure;
  XtPopdown(dialogshell);
  XtDestroyWidget(dialogshell);
} // close_dialog_cb()

/*!
  This function creates a simple error dialog window and places it on the
  display over the given \a parent widget.  Placement is not implemented at
  the moment.
*/

void
SoXt::createSimpleErrorDialog(// static
  Widget parent,
  const char * title,
  const char * string1,
  const char * string2)
{
  Arg args[10];
  int argc = 0;

  Display * dpy = SoXt::getDisplay();
  Visual * vis;
  Colormap cmap;
  int depth;

  if (! title) title = "";
  if (! string1) string1 = "";

  SoXt::selectBestVisual(dpy, vis, cmap, depth);

  Widget errdialog = XtVaCreatePopupShell("errordialog",
    topLevelShellWidgetClass, parent,
    XmNvisual, vis,
    XmNcolormap, cmap,
    XmNdepth, depth,
    XtVaTypedArg,
      XmNtitle, XmRString,
      title, strlen(title)+1,
    XmNresizable, False,
    NULL);

  Widget root = XtVaCreateManagedWidget("root",
    xmFormWidgetClass, errdialog,
    NULL);

  Widget label1 = XtVaCreateManagedWidget("label1",
    xmLabelWidgetClass, root,
    XmNleftAttachment, XmATTACH_FORM,
    XmNleftOffset, 20,
    XmNtopAttachment, XmATTACH_FORM,
    XmNtopOffset, 10,
    XmNrightAttachment, XmATTACH_FORM,
    XmNrightOffset, 20,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      string1, strlen(string1),
    NULL);

  Widget label2 = (Widget) NULL;
  if (string2 != NULL) {
    label2 = XtVaCreateManagedWidget("label2",
      xmLabelWidgetClass, root,
      XmNleftAttachment, XmATTACH_FORM,
      XmNleftOffset, 20,
      XmNtopAttachment, XmATTACH_WIDGET,
      XmNtopWidget, label1,
      XmNtopOffset, 5,
      XmNrightAttachment, XmATTACH_FORM,
      XmNrightOffset, 20,
      XtVaTypedArg,
        XmNlabelString, XmRString,
        string2, strlen(string2),
      NULL);
  }

  Widget close = XtVaCreateManagedWidget("close",
    xmPushButtonWidgetClass, root,
    XmNtopAttachment, XmATTACH_WIDGET,
    XmNtopWidget, string2 ? label2 : label1,
    XmNtopOffset, 5,
    XmNrightAttachment, XmATTACH_FORM,
    XmNrightOffset, 10,
    XmNbottomAttachment, XmATTACH_FORM,
    XmNbottomOffset, 10,
    XtVaTypedArg,
      XmNlabelString, XmRString,
      " Close ", 5,
    NULL);

  XtAddCallback(close, XmNactivateCallback, close_dialog_cb, errdialog);

  XtPopup(errdialog, XtGrabNone);

  Dimension width = 0;
  Dimension height = 0;
  XtVaGetValues(root,
    XmNwidth, &width,
    XmNheight, &height,
    NULL);
  XtVaSetValues(errdialog,
    XmNheight, height,
    XmNwidth, width,
    XmNminHeight, height,
    XmNmaxHeight, height,
    XmNminWidth, width,
    NULL);

} // createSimpleErrorDialog()

// *************************************************************************

/*!
  This function should add the necessary arguments to the \a args argument
  list so the popup shell gets the same visual and colormap as the rest
  of the application.

  This function is not implemented.
*/

void
SoXt::getPopupArgs(// static
  Display * display,
  int screen,
  ArgList args,
  int * n)
{
  SOXT_STUB();
} // getPopupArgs()

/*!
  This function is not implemented.
*/

void
SoXt::registerColormapLoad(// static
  Widget widget,
  Widget shell)
{
  SOXT_STUB();
} // registerColormapLoad()

/*!
  This function is not implemented.
*/

void
SoXt::addColormapToShell(// static
  Widget widget,
  Widget shell)
{
  SOXT_STUB();
} // addColormapToShell()

/*!
  This function is not implemented.
*/

void
SoXt::removeColormapFromShell(// static
  Widget widget,
  Widget shell)
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
  This function adds an extension event handler to the application, which
  will be considered in the SoXt::mainLoop event dispatching loop.

  \sa SoXt::removeExtensionEventHandler
  \sa SoXt::getExtensionEventHandler
*/

void
SoXt::addExtensionEventHandler(// static
  Widget widget,
  int type,
  XtEventHandler proc,
  XtPointer data)
{
  EventHandlerInfo * info = new EventHandlerInfo;
  info->type = type;
  info->widget = widget;
  info->handler = proc;
  info->data = data;

  if (SoXt::eventHandlers == NULL)
    SoXt::eventHandlers = new SbPList;

#if SOXT_DEBUG
  const int handlers = SoXt::eventHandlers->getLength();
  for (int i = 0; i < handlers; i++) {
    EventHandlerInfo * query = (EventHandlerInfo *) (*SoXt::eventHandlers)[i];
    if (query->type == type)
      SoDebugError::postWarning("SoXt::addExtensionEventHandler",
        "handler of type %d already exists, shadowing the new handler");
  }
#endif // SOXT_DEBUG

  SoXt::eventHandlers->append((void *) info);
} // addExtensionEventHandler()

/*!
  This method removes an extension event handler.

  \sa SoXt::addExtensionEventHandler
  \sa SoXt::getExtensionEventHandler
*/

void
SoXt::removeExtensionEventHandler(// static
  Widget widget,
  int type,
  XtEventHandler proc,
  XtPointer data)
{
  if (SoXt::eventHandlers == NULL) {
#if SOXT_DEBUG
    SoDebugError::postInfo("SoXt::removeExtensionEventHandler",
      "no extension event handlers registered.");
#endif // SOXT_DEBUG
    return;
  }
  int handlers = SoXt::eventHandlers->getLength();
  for (int i = 0; i < handlers; i++) {
    EventHandlerInfo * info = (EventHandlerInfo *) (*SoXt::eventHandlers)[i];
    if (info->widget == widget && info->type == type &&
         info->handler == proc && info->data == data) {
      SoXt::eventHandlers->remove(i);
      delete info;
      return;
    }
  }
#if SOXT_DEBUG
  SoDebugError::postInfo("SoXt::removeExtensionEventHandler",
    "no such extension event handler registered.");
#endif // SOXT_DEBUG
} // removeExtensionEventHandler()

/*!
  This method returns the extension event handler for the given \a event.

  \sa SoXt::addExtensionEventHandler
  \sa SoXt::removeExtensionEventHandler
*/

void
SoXt::getExtensionEventHandler(// static, protected
  XEvent * event,
  Widget & widget,
  XtEventHandler & proc,
  XtPointer & data)
{
  proc = (XtEventHandler) NULL;
  data = (XtPointer) NULL;
  widget = (Widget) NULL;

  if (SoXt::eventHandlers == NULL)
    return;

  const int handlers = SoXt::eventHandlers->getLength();
  for (int i = 0; i < handlers; i++) {
    EventHandlerInfo * info = (EventHandlerInfo *) (*SoXt::eventHandlers)[i];
    if (info->type == event->type) {
      widget = info->widget;
      proc = info->handler;
      data = info->data;
    }
  }
} // getExtensionEventHandler()

// *************************************************************************

/*!
  This function is not implemented.
*/

Widget
SoXt::getwidget(// static
  unsigned int what)
{
  SOXT_STUB();
  return (Widget) NULL;
} // getwidget()

// *************************************************************************

/*!
  \internal
*/

void
SoXt::timerSensorCB(// static, private
  XtPointer closure,
  XtIntervalId * id)
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXt::timerSensorCB", "called");
#endif // SOXT_DEBUG
  SoXt::timerSensorId = 0;
  SoXt::timerSensorActive = FALSE;
  SoDB::getSensorManager()->processTimerQueue();
  SoXt::sensorQueueChanged(NULL);
} // timerSensorCB()

/*!
  \internal
*/

void
SoXt::delaySensorCB(// static, private
  XtPointer closure,
  XtIntervalId * id)
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXt::delaySensorCB", "called");
#endif // SOXT_DEBUG
  SoXt::delaySensorId = 0;
  SoXt::delaySensorActive = FALSE;
  SoDB::getSensorManager()->processDelayQueue(FALSE);
  SoXt::sensorQueueChanged(NULL);
} // delaySensorCB()

/*!
  \internal
*/

Boolean
SoXt::idleSensorCB(// static, private
  XtPointer closure)
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXt::idleSensorCB", "called");
#endif // SOXT_DEBUG
  SoXt::idleSensorId = 0;
  SoXt::idleSensorActive = FALSE;
  SoDB::getSensorManager()->processDelayQueue(TRUE);
  SoXt::sensorQueueChanged(NULL);
  return True;
} // idleSensorCB()

// *************************************************************************

/*!
  This callback handles events from sensors in the scene graph, needed to
  deal with scene interaction.
*/

void
SoXt::sensorQueueChanged(// static, private
  void *) // user)
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXt::sensorQueueChanged", "start");
#endif // SOXT_DEBUG
  SoSensorManager * sensormanager = SoDB::getSensorManager();

  SbTime timevalue;
  if (sensormanager->isTimerSensorPending(timevalue)) {
    SbTime interval = timevalue - SbTime::getTimeOfDay();

#if SOXT_DEBUG && 0
    SoDebugError::postInfo("SoXt::sensorQueueChanged",
      "interval: %f (msec: %d)", interval.getValue(),
      interval.getMsecValue());
#endif // debug

    // On a system with some load, the interval value can easily get
    // negative. For Xt, this means it will never trigger -- which
    // causes all kinds of problems. Setting it to 0 will make it
    // trigger more-or-less immediately.
    if (interval.getValue() < 0.0) interval.setValue(0.0);

    if (SoXt::timerSensorActive) XtRemoveTimeOut(SoXt::timerSensorId);

    SoXt::timerSensorId = XtAppAddTimeOut(SoXt::getAppContext(),
      interval.getMsecValue(), SoXt::timerSensorCB, NULL);
    SoXt::timerSensorActive = TRUE;

  } else if (SoXt::timerSensorActive) {
    XtRemoveTimeOut(SoXt::timerSensorId);
    SoXt::timerSensorId = 0;
    SoXt::timerSensorActive = FALSE;
  }

  if (sensormanager->isDelaySensorPending()) {
    if (! SoXt::idleSensorActive) {
      SoXt::idleSensorId =
        XtAppAddWorkProc(SoXt::getAppContext(), SoXt::idleSensorCB, NULL);
      SoXt::idleSensorActive = TRUE;
    }

    if (! SoXt::delaySensorActive) {
      unsigned long timeout = SoDB::getDelaySensorTimeout().getMsecValue();
      SoXt::delaySensorId =
        XtAppAddTimeOut(SoXt::getAppContext(), timeout, SoXt::delaySensorCB,
          NULL);
      SoXt::delaySensorActive = TRUE;
    }
  } else {
    if (SoXt::idleSensorActive) {
      XtRemoveWorkProc(SoXt::idleSensorId);
      SoXt::idleSensorId = 0;
      SoXt::idleSensorActive = FALSE;
    }
    if (SoXt::delaySensorActive) {
      XtRemoveTimeOut(SoXt::delaySensorId);
      SoXt::delaySensorId = 0;
      SoXt::delaySensorActive = FALSE;
    }
  }
} // sensorQueueChange()

// *************************************************************************

/*
  \internal
*/

static const char *
_visualClassName(
  const int vclass)
{
  switch (vclass) {
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

  This function is not standard for the SGI SoXt API.
*/

void
SoXt::selectBestVisual(// static
  Display * & dpy,
  Visual * & visual,
  Colormap & colormap,
  int & depth)
{
  if (dpy == NULL) {
     dpy = XOpenDisplay(NULL);
#if SOXT_DEBUG && 0
     // This is _extremely_ useful for debugging X errors: activate
     // this code (flip the "0" above to "1"), recompile libSoXt, then
     // run the application code in a debugger with a breakpoint set
     // at _XError. Now you can backtrace to the exact source location
     // of the failing X request.
     if (dpy) {
       SoDebugError::postInfo("SoXt::selectBestVisual",
         "Turning on X synchronization.");
       XSynchronize(dpy, True);
     }
#endif // SOXT_DEBUG
  }
  if (dpy == NULL) {
    SoDebugError::postInfo("SoXt::selectBestVisual",
      "Failed to open display.");
    exit(-1);
  }

  int snum = XDefaultScreen(dpy);

  if (XDefaultDepth(dpy, snum) >= 16) { // me like...
    depth = XDefaultDepth(dpy, snum);
    visual = XDefaultVisual(dpy, snum);
    colormap = XDefaultColormap(dpy, snum);
    return;
  }

#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXt::selectBestVisual",
    "we're beyond the default visual");
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
  for (int i = 0; pri[i].depth != 0; i++) {
    if (XMatchVisualInfo(dpy, snum, pri[i].depth, pri[i].vclass, &vinfo)) {
#if SOXT_DEBUG && 0
      SoDebugError::postInfo("SoXt::selectBestVisual",
        "visual: depth=%d, class=%s", vinfo.depth,
        _visualClassName(vinfo.c_class));
#endif // SOXT_DEBUG
      visual = vinfo.visual;
      depth = vinfo.depth;

      int numcmaps;
      XStandardColormap * stdcolormaps = NULL;

      if (XmuLookupStandardColormap(dpy, vinfo.screen, vinfo.visualid,
             vinfo.depth, XA_RGB_DEFAULT_MAP, False, True)
        && XGetRGBColormaps(dpy, RootWindow(dpy, vinfo.screen),
             &stdcolormaps, &numcmaps, XA_RGB_DEFAULT_MAP)) {
        SbBool found = FALSE;
        for (i = 0; i < numcmaps && ! found; i++) {
          if (stdcolormaps[i].visualid == vinfo.visualid) {
            colormap = stdcolormaps[i].colormap;
            found = TRUE;
          }
        }
        if (! found) {
          SoDebugError::postInfo("SoXt::selectBestVisual",
            "standard RGB colormaps did not work with visual - creating own colormap");
          colormap = XCreateColormap(
            dpy, RootWindow(dpy, vinfo.screen), vinfo.visual, AllocNone);
        }
      } else {
        SoDebugError::postInfo("SoXt::selectBestVisual",
          "no standard RGB colormaps - creating own colormap");
        colormap = XCreateColormap(
          dpy, RootWindow(dpy, vinfo.screen), vinfo.visual, AllocNone);
      }
      XtFree((char *) stdcolormaps);
      return;
    }
  }
#if SOXT_DEBUG
  SoDebugError::postInfo("SoXt::selectBestVisual", "yikes!");
#endif // SOXT_DEBUG
} // selectBestVisual()

// *************************************************************************

/*!
  \var SoXt::fallback_resources

  This is an array of X resources.
  They are not really in use yet - the implementation is mostly experimental,
  and resource names will change in the future.
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
  _COMPONENT	".title:"				"Light Sliders",
  _COMPONENT	".sliderset1:"				"LightIntensity",
  _COMPONENT	".sliderset2:"				"LightColor",
#undef _COMPONENT

#define _COMPONENT "*SoXtMaterialSliderSet"
  _COMPONENT	".title:"				"Material Sliders",
  _COMPONENT	".sliderset1:"				"AmbientColor",
  _COMPONENT	".sliderset2:"				"DiffuseColor",
  _COMPONENT	".sliderset3:"				"SpecularColor",
  _COMPONENT	".sliderset4:"				"EmissiveColor",
  _COMPONENT	".sliderset5:"				"Shininess",
  _COMPONENT	".sliderset6:"				"Transparency",
#undef _COMPONENT

#define _COMPONENT "*SoXtTransformSliderSet"
  _COMPONENT	".title:"				"Transform Sliders",
  _COMPONENT	".sliderset1:"				"Translations",
  _COMPONENT	".sliderset2:"				"Scales",
  _COMPONENT	".sliderset3:"				"Rotations",
  _COMPONENT	".sliderset4:"				"ScaleOrientation",
  _COMPONENT	".sliderset5:"				"Center",
#undef _COMPONENT

#define _COMPONENT "*SoXtMaterialList"
#undef _COMPONENT

#define _COMPONENT "*SoXtPrintDialog"
  _COMPONENT	"*title:"				"Print",
#undef _COMPONENT

// *************************************************************************

#define _COMPONENT "*SoXtAmbientColorSliderModule"
  _COMPONENT	".title:"				"AMBIENT COLOR",
  _COMPONENT	".slider1Title:"			"Ambient Red",
  _COMPONENT	".slider1Field:"			"ambientColor[0]",
  _COMPONENT	".slider2Title:"			"Ambient Green",
  _COMPONENT	".slider2Field:"			"ambientColor[1]",
  _COMPONENT	".slider3Title:"			"Ambient Blue",
  _COMPONENT	".slider3Field:"			"ambientColor[2]",
#undef _COMPONENT

#define _COMPONENT "*SoXtCenterSliderModule"
  _COMPONENT	".title:"				"CENTER",
  _COMPONENT	".slider1Title:"			"X Center",
  _COMPONENT	".slider1Field:"			"center[0]",
  _COMPONENT	".slider2Title:"			"Y Center",
  _COMPONENT	".slider2Field:"			"center[1]",
  _COMPONENT	".slider3Title:"			"Z Center",
  _COMPONENT	".slider3Field:"			"center[2]",
#undef _COMPONENT

#define _COMPONENT "*SoXtDiffuseColorSliderModule"
  _COMPONENT	".title:"				"DIFFUSE COLOR",
  _COMPONENT	".slider1Title:"			"Diffuse Red",
  _COMPONENT	".slider1Field:"			"diffuseColor[0]",
  _COMPONENT	".slider2Title:"			"Diffuse Green",
  _COMPONENT	".slider2Field:"			"diffuseColor[1]",
  _COMPONENT	".slider3Title:"			"Diffuse Blue",
  _COMPONENT	".slider3Field:"			"diffuseColor[2]",
#undef _COMPONENT

#define _COMPONENT "*SoXtEmissiveColorSliderModule"
  _COMPONENT	".title:"				"EMISSIVE COLOR",
  _COMPONENT	".slider1Title:"			"Emissive Red",
  _COMPONENT	".slider1Field:"			"emissiveColor[0]",
  _COMPONENT	".slider2Title:"			"Emissive Green",
  _COMPONENT	".slider2Field:"			"emissiveColor[1]",
  _COMPONENT	".slider3Title:"			"Emissive Blue",
  _COMPONENT	".slider3Field:"			"emissiveColor[2]",
#undef _COMPONENT

#define _COMPONENT "*SoXtLightColorSliderModule"
  _COMPONENT	".title:"				"COLOR",
  _COMPONENT	".slider1Title:"			"Red",
  _COMPONENT	".slider1Field:"			"color[0]",
  _COMPONENT	".slider2Title:"			"Green",
  _COMPONENT	".slider2Field:"			"color[1]",
  _COMPONENT	".slider3Title:"			"Blue",
  _COMPONENT	".slider3Field:"			"color[2]",
#undef _COMPONENT

#define _COMPONENT "*SoXtLightIntensitySliderModule"
  _COMPONENT	".title:"				"INTENSITY",
  _COMPONENT	".slider1Title:"			"Intensity",
  _COMPONENT	".slider1Field:"			"intensity",
#undef _COMPONENT

#define _COMPONENT "*SoXtRotationsSliderModule"
  _COMPONENT	".title:"				"ROTATIONS",
  _COMPONENT	".slider1Title:"			"X Rotate",
  _COMPONENT	".slider1Field:"			"rotation[0]",
  _COMPONENT	".slider2Title:"			"Y Rotate",
  _COMPONENT	".slider2Field:"			"rotation[1]",
  _COMPONENT	".slider3Title:"			"Z Rotate",
  _COMPONENT	".slider3Field:"			"rotation[2]",
#undef _COMPONENT

#define _COMPONENT "*SoXtScalesSliderModule"
  _COMPONENT	".title:"				"SCALES",
  _COMPONENT	".slider1Title:"			"X Scale",
  _COMPONENT	".slider1Field:"			"scaleFactor[0]",
  _COMPONENT	".slider2Title:"			"Y Scale",
  _COMPONENT	".slider2Field:"			"scaleFactor[1]",
  _COMPONENT	".slider3Title:"			"Z Scale",
  _COMPONENT	".slider3Field:"			"scaleFactor[2]",
#undef _COMPONENT

#define _COMPONENT "*SoXtScaleOrientationSliderModule"
  _COMPONENT	".title:"				"SCALE ORIENTATION",
  _COMPONENT	".slider1Title:"			"X Rotate",
  _COMPONENT	".slider1Field:"			"scaleOrientation[0]",
  _COMPONENT	".slider2Title:"			"Y Rotate",
  _COMPONENT	".slider2Field:"			"scaleOrientation[1]",
  _COMPONENT	".slider3Title:"			"Z Rotate",
  _COMPONENT	".slider3Field:"			"scaleOrientation[2]",
#undef _COMPONENT

#define _COMPONENT "*SoXtShininessSliderModule"
  _COMPONENT	".title:"				"SHININESS",
  _COMPONENT	".slider1Title:"			"Shininess",
  _COMPONENT	".slider1Field:"			"shininess",
#undef _COMPONENT

#define _COMPONENT "*SoXtSpecularColorSliderModule"
  _COMPONENT	".title:"				"SPECULAR COLOR",
  _COMPONENT	".slider1Title:"			"Specular Red",
  _COMPONENT	".slider1Field:"			"specularColor[0]",
  _COMPONENT	".slider2Title:"			"Specular Green",
  _COMPONENT	".slider2Field:"			"specularColor[1]",
  _COMPONENT	".slider3Title:"			"Specular Blue",
  _COMPONENT	".slider3Field:"			"specularColor[2]",
#undef _COMPONENT

#define _COMPONENT "*SoXtTranslationsSliderModule"
  _COMPONENT	".title:"				"TRANSLATIONS",
  _COMPONENT	".slider1Title:"			"X Translation",
  _COMPONENT	".slider1Field:"			"translation[0]",
  _COMPONENT	".slider2Title:"			"Y Translation",
  _COMPONENT	".slider2Field:"			"translation[1]",
  _COMPONENT	".slider3Title:"			"Z Translation",
  _COMPONENT	".slider3Field:"			"translation[2]",
#undef _COMPONENT

#define _COMPONENT "*SoXtTransparencySliderModule"
  _COMPONENT	".title:"				"TRANSPARENCY",
  _COMPONENT	".slider1Title:"			"Transparency",
  _COMPONENT	".slider1Field:"			"transparency",
#undef _COMPONENT

  NULL
}; // fallback_resources

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

