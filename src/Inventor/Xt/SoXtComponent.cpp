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

// *************************************************************************

/*!
  \class SoXtComponent Inventor/Xt/SoXtComponent.h
  \brief The SoXtComponent class is the base class for all SoXt components.
  \ingroup components
*/

// *************************************************************************

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/misc/SoBasic.h>
#include <Inventor/SoLists.h>
#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXt.h>

#include <Inventor/Xt/SoXtComponent.h>
#include <Inventor/Xt/SoXtGLWidget.h>
#include <Inventor/Xt/SoXtRenderArea.h>
#include <Inventor/Xt/viewers/SoXtViewer.h>
#include <Inventor/Xt/viewers/SoXtFullViewer.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>
#include <Inventor/Xt/viewers/SoXtPlaneViewer.h>
#include <Inventor/Xt/viewers/SoXtConstrainedViewer.h>
#include <Inventor/Xt/viewers/SoXtWalkViewer.h>
#include <Inventor/Xt/viewers/SoXtFlyViewer.h>
#include <Inventor/Xt/SoXtSliderSetBase.h>
#include <Inventor/Xt/SoXtSliderSet.h>
#include <Inventor/Xt/SoXtLightSliderSet.h>
#include <Inventor/Xt/SoXtMaterialSliderSet.h>
#include <Inventor/Xt/SoXtTransformSliderSet.h>
#include <Inventor/Xt/SoXtMaterialList.h>
#include <Inventor/Xt/SoXtMaterialEditor.h>
#include <Inventor/Xt/SoXtPrintDialog.h>
#include <Inventor/Xt/SoXtCursor.h>

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <X11/cursorfont.h>
#ifdef HAVE_LIBXMU
#include <X11/Xmu/Editres.h>
#endif // HAVE_LIBXMU

#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/MessageB.h>

#include <assert.h>
#include <string.h>

// *************************************************************************

// The private data for the SoXtComponent.

class SoXtComponentP {
public:
  // Constructor.
  SoXtComponentP(const SoXtComponent * o)
    : widget(NULL), widgetname(NULL), widgetclass(NULL), title(NULL),
      icontitle(NULL), size(SbVec2s(-1, -1)),
      closecbs(NULL), visibilitycbs(NULL),
      visibilitystate(FALSE), fullscreen(FALSE)
  {
    this->owner = o;
  }


  static Cursor getNativeCursor(Display * d,
                                const SoXtCursor::CustomCursor * cc);

  Widget parent;
  Widget widget;
  char * widgetname;
  char * widgetclass;
  char * title;
  char * icontitle;

  SbBool embedded;

  SbVec2s size;

  static SbPList * widgets;
  static SbPList * components;

  SbPList * closecbs;
  SbPList * visibilitycbs;
  SbBool visibilitystate, fullscreen;

private:
  const SoXtComponent * owner;
  static SbDict * cursordict;
};

SbPList * SoXtComponentP::widgets = NULL;
SbPList * SoXtComponentP::components = NULL;
SbDict * SoXtComponentP::cursordict = NULL;

#define PRIVATE(o) (o->pimpl)

// *************************************************************************

SOXT_OBJECT_ABSTRACT_SOURCE(SoXtComponent);

/*!
  \internal
  This function initializes the type system for all the component classes.
  It is called indirectly when calling SoXt::init().

  \sa SoXt::init, SoXtDevice::initClasses
*/

void
SoXtComponent::initClasses(
  void)
{
  SoXtComponent::initClass();
  SoXtGLWidget::initClass();
  SoXtRenderArea::initClass();
  SoXtViewer::initClass();
  SoXtFullViewer::initClass();
  SoXtExaminerViewer::initClass();
  SoXtPlaneViewer::initClass();
  SoXtConstrainedViewer::initClass();
  SoXtWalkViewer::initClass();
  SoXtFlyViewer::initClass();
  SoXtSliderSetBase::initClass();
  SoXtSliderSet::initClass();
  SoXtLightSliderSet::initClass();
  SoXtMaterialSliderSet::initClass();
  SoXtTransformSliderSet::initClass();
  SoXtMaterialList::initClass();
  SoXtMaterialEditor::initClass();
  SoXtPrintDialog::initClass();
} // initClasses()

// *************************************************************************

struct SoXtWindowCloseCallbackInfo {
  SoXtComponentCB * callback;
  void * closure;
};

struct SoXtComponentVisibilityCallbackInfo {
  SoXtComponentVisibilityCB * callback;
  void * closure;
};

/*!
  \var SoXtComponent::firstRealize

  This member is used to detect the first realization of the component.
*/

// *************************************************************************

/*!
  This is a protected constructor, used only by derived classes.

  The \a parent argument is the parent widget of the component.  If you
  don't supply a parent, the main window (the one given to or returned
  from SoXt::init()) is used (and the \a embed argument is set to FALSE).

  The \a name argument is the name of the component.  It will decide which
  X resources the component will use, so be careful with what you set it
  to.  If you don't supply one, the name will default to something, depending
  on the inheritance hierarchy.  If you supply a name and don't set up your
  own X resources, the component will at the least be full of bogus labels.

  The \a embed argument tells wether the component should be embedded in
  the \a parent widget or should create its own shell.  This flag is only
  checked if the \a parent widget argument is specified (not NULL).

  If you create a non-embedded component, the component will create its
  own shell, which will be of the topLevelShellWidgetClass type.  If you
  embed the component, the component will create an XmFormWidgetClass
  type widget inside the \a parent widget, which you can get the handle
  og by calling SoXtComponent::getBaseWidget().  You do not need (nor
  should you) create an empty form widget for the component in other
  words.
*/

SoXtComponent::SoXtComponent(const Widget parent,
                             const char * const name,
                             const SbBool embed) // protected
{
  PRIVATE(this) = new SoXtComponentP(this);

  this->firstRealize = TRUE;

  if (name && strlen(name) > 0)
    PRIVATE(this)->widgetname = strcpy(new char [ strlen(name) + 1 ], name);

  if ((parent == (Widget) NULL) || ! embed) {
    // create own shell

    Visual * visual = NULL;
    Colormap colormap = 0;
    int depth = 0;
    Display * dpy = SoXt::getDisplay();

    if (parent) {
      Widget shell = parent;
      while (! XtIsShell(shell) && shell != (Widget) NULL)
        shell = XtParent(shell);
      assert(shell != (Widget) NULL);
      dpy = XtDisplay(shell);
      XtVaGetValues(shell,
        XmNvisual, &visual,
        XmNcolormap, &colormap,
        XmNdepth, &depth,
        NULL);
    } else {
      SoXt::selectBestVisual(dpy, visual, colormap, depth);
    }
    assert(dpy != NULL);

    PRIVATE(this)->parent = XtVaAppCreateShell(
      SoXt::getAppName(), // didn't work
      SoXt::getAppClass(),
      topLevelShellWidgetClass,
      dpy,
      XmNvisual, visual,
      XmNcolormap, colormap,
      XmNdepth, depth,
      NULL);

#ifdef HAVE_LIBXMU
#if SOXT_DEBUG
    XtEventHandler editres_hook = (XtEventHandler) _XEditResCheckMessages;
    XtAddEventHandler(PRIVATE(this)->parent, (EventMask) 0, True, editres_hook, NULL);
#endif // SOXT_DEBUG
#endif // HAVE_LIBXMU

    PRIVATE(this)->embedded = FALSE;
  } else {
    PRIVATE(this)->parent = parent;
    PRIVATE(this)->embedded = TRUE;
  }
  if (parent && XtIsShell(parent))
    PRIVATE(this)->embedded = FALSE;

  if (XtIsShell(PRIVATE(this)->parent))
    XtInsertEventHandler(PRIVATE(this)->parent, (EventMask) StructureNotifyMask, False,
      SoXtComponent::event_handler, (XtPointer) this, XtListTail);
} // SoXtComponent()

/*!
  The destructor.
*/

SoXtComponent::~SoXtComponent(// virtual
  void)
{
  delete [] PRIVATE(this)->widgetname;
  delete [] PRIVATE(this)->widgetclass;
  delete [] PRIVATE(this)->title;
  delete [] PRIVATE(this)->icontitle;
  if (PRIVATE(this)->closecbs != NULL) {
    const int num = PRIVATE(this)->closecbs->getLength();
    for (int i = 0; i < num; i++) {
      SoXtWindowCloseCallbackInfo * info =
        (SoXtWindowCloseCallbackInfo *) (*PRIVATE(this)->closecbs)[i];
      delete info;
    }
    delete PRIVATE(this)->closecbs;
  }
  if (PRIVATE(this)->visibilitycbs != NULL) {
    const int num = PRIVATE(this)->visibilitycbs->getLength();
    for (int i = 0; i < num; i++) {
      SoXtComponentVisibilityCallbackInfo * info =
        (SoXtComponentVisibilityCallbackInfo *)
          (*PRIVATE(this)->visibilitycbs)[i];
      delete info;
    }
    delete PRIVATE(this)->visibilitycbs;
  }

  delete PRIVATE(this);
} // ~SoXtComponent()

// *************************************************************************

/*!
  This method realizes the component.

  topLevelShell widgets will be realized and mapped.
  non-toplevel components will just be managed.
*/

void
SoXtComponent::show(// virtual
  void)
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtComponent::show", "[enter]");
#endif // SOXT_DEBUG
  if (this->isTopLevelShell()) {
    Widget shell = this->getShellWidget();
    XtRealizeWidget(shell);
    if (this->firstRealize == TRUE) {
      this->afterRealizeHook();
      this->firstRealize = FALSE;
    }
    XtMapWidget(shell);
  } else {
    XtManageChild(this->getBaseWidget());
  }
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtComponent::show", "[exit]");
#endif // SOXT_DEBUG
} // show()

/*!
  This method hides the component.

  topLevelShell widgets will be unrealized.
  non-toplevel components will just be unmanaged.
*/

void
SoXtComponent::hide(// virtual
  void)
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtComponent::hide", "[enter]");
#endif // SOXT_DEBUG
  if (this->isTopLevelShell()) {
    Widget shell = this->getShellWidget();
    XtUnmapWidget(shell);
    XtUnrealizeWidget(shell);
  } else {
    XtUnmanageChild(this->getBaseWidget());
  }
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtComponent::hide", "[exit]");
#endif // SOXT_DEBUG
} // hide()

/*!
  This method returns TRUE if component is shown, and FALSE if it is hidden.

  This method is not implemented.
*/

SbBool
SoXtComponent::isVisible(
  void)
{
  return PRIVATE(this)->visibilitystate;
} // isVisible()

// *************************************************************************

/*!
  This method returns the base widget of the component.

  \sa SoXtComponent::getBaseWidget()
*/

Widget
SoXtComponent::getWidget(
  void) const
{
  return this->getBaseWidget();
} // getWidget()

/*!
  This method returns the base widget of the component.

  \sa SoXtComponent::getBaseWidget()
*/

Widget
SoXtComponent::baseWidget(
  void) const
{
  return this->getBaseWidget();
} // baseWidget()

/*!
  This method returns the base widget of the component.

  \sa SoXtComponent::getBaseWidget()
*/

Widget
SoXtComponent::getBaseWidget(
  void) const
{
  return PRIVATE(this)->widget;
} // getBaseWidget()

/*!
  This method returns whether the component was created as a toplevel shell
  or not.
*/

SbBool
SoXtComponent::isTopLevelShell(
  void) const
{
  return PRIVATE(this)->embedded ? FALSE : TRUE;
} // isTopLevelShell()

/*!
  This method returns the shell widget of the component, but only if it was
  created as a toplevel shell.  This method will return NULL for embedded
  components.
*/

Widget
SoXtComponent::getShellWidget(
  void) const
{
  return this->isTopLevelShell() ? PRIVATE(this)->parent : (Widget) NULL;
} // getShellWidget()

/*!
  This method returns the parent widget of the component widget.  If
  the component created its own toplevel shell, this method returns
  the the shell widget.

  If the component is embedded, this method returns the widget given
  in the \a parent argument of the constructor.
*/
// FIXME: ^^^ this last statement doesn't seem valid -- investigate.
// 20011012 mortene.
Widget
SoXtComponent::getParentWidget(
  void) const
{
  return PRIVATE(this)->parent;
} // getParentWidget()

// *************************************************************************

/*!
  This method sets the size of the component.

  The method assumes the caller knows what he is doing.
*/

void
SoXtComponent::setSize(
  const SbVec2s size)
{
  PRIVATE(this)->size = size;
  Widget widget;
  if (this->isTopLevelShell())
    widget = this->getShellWidget();
  else
    widget = this->getBaseWidget();
  if (! widget)
    return;

  int argc = 0;
  Arg args[2];
  if (size[0] != -1) {
    XtSetArg(args[argc], XmNwidth, size[0]);
    argc++;
  }
  if (size[1] != -1) {
    XtSetArg(args[argc], XmNheight, size[1]);
    argc++;
  }
  XtSetValues(widget, args, argc);
  this->sizeChanged(size);
} // setSize()

/*!
  This method returns the size of the component.

  The size that is returned is a cached size value, not a value fetched
  from the GUI system.
*/
SbVec2s
SoXtComponent::getSize(void) const
{
  return PRIVATE(this)->size;
} // getSize()

/*!
  This method tries to resize the component window, using \a size as the
  minimum requirements.

  This method is not part of the Open Inventor SoXt API.
*/

void
SoXtComponent::fitSize(
  const SbVec2s size)
{
  if (this->isTopLevelShell() || (PRIVATE(this)->parent && XtIsShell(PRIVATE(this)->parent))) {
    XtWidgetGeometry geometry;
    XtQueryGeometry(this->getBaseWidget(), NULL, &geometry);
    PRIVATE(this)->size[0] = SoXtMax((short) geometry.width, size[0]);
    PRIVATE(this)->size[1] = SoXtMax((short) geometry.height, size[1]);
    XtVaSetValues(this->getShellWidget(),
      XmNwidth, PRIVATE(this)->size[0],
      XmNheight, PRIVATE(this)->size[1],
      NULL);
  }
} // fitSize()

/*!
  Called internally from within the SoXt library when the widget
  embedded in a component changes it size. that is usually triggered
  by end-user interaction.
*/
void         // virtual
SoXtComponent::sizeChanged(const SbVec2s & size)
{
  // Since SoXtComponent doesn't manage any internal widgets, this
  // default implementation does nothing.
  //
  // Derived components should implement this to resize internal
  // widgets and pass the [modified] size down to the parent class
  // afterwards.
}

// *************************************************************************

/*!
  This method returns the display the component is sent to.
*/

Display *
SoXtComponent::getDisplay(
  void)
{
#if SOXT_DEBUG
  if (! this->getBaseWidget())
    SoDebugError::postInfo("SoXtComponent::getDisplay",
      "component has no base widget");
#endif // SOXT_DEBUG
  return this->getBaseWidget() ?
    XtDisplay(this->getBaseWidget()) : (Display *) NULL;
} // getDisplay()

// *************************************************************************

/*!
  This method sets the title of the component.

  The title will appear on the window title bar, if the component manages
  its own window.
*/

void
SoXtComponent::setTitle(
  const char * const title)
{
  if (PRIVATE(this)->title && strlen(PRIVATE(this)->title) >= strlen(title)) {
    strcpy(PRIVATE(this)->title, (char *) title);
  } else {
    delete [] PRIVATE(this)->title;
    PRIVATE(this)->title = strcpy(new char [strlen(title)+1], title);
  }

  Widget shell = this->isTopLevelShell() ?
    this->getShellWidget() : (Widget) NULL;
  if (! shell)
    return;
  XtVaSetValues(shell,
    XmNtitle, PRIVATE(this)->title,
    NULL);
} // setTitle()

/*!
  This method returns the title of the component.

  If a title has been set, that title will be returned.
  If no title has been set, the default title is returned.
*/

const char *
SoXtComponent::getTitle(
  void) const
{
  // FIXME: use SoXtResource to see if title is set?
  return PRIVATE(this)->title ? PRIVATE(this)->title : this->getDefaultTitle();
}

/*!
  This method sets the title of the icon representation of the window.

  The title will appear on the window icon, if the component manages its
  own window.
*/

void
SoXtComponent::setIconTitle(
  const char * const title)
{
  if (PRIVATE(this)->icontitle && strlen(PRIVATE(this)->icontitle) >= strlen(title)) {
    strcpy(PRIVATE(this)->icontitle, (char *) title);
  } else {
    delete [] PRIVATE(this)->icontitle;
    PRIVATE(this)->icontitle = strcpy(new char [strlen(title)+1], title);
  }
  Widget shell = this->isTopLevelShell() ?
    this->getShellWidget() : (Widget) NULL;
  if (! shell)
    return;
  XtVaSetValues(shell,
    XtNiconName, PRIVATE(this)->icontitle,
    NULL);
}

/*!
  This method returns the icon title for the component.

  If an icon title has been set, that icon title is returned.
  If no icon title has been set, the default icon title is returned.
*/

const char *
SoXtComponent::getIconTitle(
  void) const
{
  // FIXME: use SoXtResource to see if iconName is set?
  return PRIVATE(this)->icontitle ? PRIVATE(this)->icontitle : this->getDefaultIconTitle();
} // getIconTitle()

// *************************************************************************

/*!
  This method adds window close callbacks to the Component window.

  Note that window close callback invokation has not been implemented yet.

  \sa SoXtComponent::addWindowCloseCallback()
  \sa SoXtComponent::removeWindowCloseCallback()
  \sa SoXtComponent::invokeWindowCloseCallback()
*/

void
SoXtComponent::setWindowCloseCallback(
  SoXtComponentCB * callback,
  void * closure)
{
  this->addWindowCloseCallback(callback, closure);
} // setWindowCloseAction()

/*!
  This method adds a close callback for the component window.

  This method is not standard for Open Inventor.

  \sa SoXtComponent::setWindowCloseCallback()
  \sa SoXtComponent::removeWindowCloseCallback()
  \sa SoXtComponent::invokeWindowCloseCallback()
*/

void
SoXtComponent::addWindowCloseCallback(
  SoXtComponentCB * callback,
  void * closure)
{
  if (PRIVATE(this)->closecbs == NULL)
    PRIVATE(this)->closecbs = new SbPList;
  SoXtWindowCloseCallbackInfo * info = new SoXtWindowCloseCallbackInfo;
  info->callback = callback;
  info->closure = closure;
  PRIVATE(this)->closecbs->append(info);
} // addWindowCloseCallback()

/*!
  This method removes a close callback for the component window.

  This method is not standard for Open Inventor.

  \sa SoXtComponent::addWindowCloseCallback()
  \sa SoXtComponent::setWindowCloseCallback()
  \sa SoXtComponent::invokeWindowCloseCallback()
*/

void
SoXtComponent::removeWindowCloseCallback(
  SoXtComponentCB * callback,
  void * closure)
{
  if (PRIVATE(this)->closecbs != NULL) {
    const int num = PRIVATE(this)->closecbs->getLength();
    for (int i = 0; i < num; i++) {
      SoXtWindowCloseCallbackInfo * info =
        (SoXtWindowCloseCallbackInfo *) (*PRIVATE(this)->closecbs)[i];
      if (info->callback == callback && info->closure == closure) {
        PRIVATE(this)->closecbs->remove(i);
        delete info;
        return;
      }
    }
  }
#if SOXT_DEBUG
  SoDebugError::post("SoXtComponent::removeWindowCloseCallback",
    "trying to remove nonexisting callback");
#endif // SOXT_DEBUG
} // removeWindowCloseCallback()

/*!
  This method invokes the close callbacks for the component window.

  This method is not standard for Open Inventor.

  \sa SoXtComponent::addWindowCloseCallback()
  \sa SoXtComponent::setWindowCloseCallback()
  \sa SoXtComponent::removeWindowCloseCallback()
*/

void
SoXtComponent::invokeWindowCloseCallbacks(// protected
  void) const
{
  if (PRIVATE(this)->closecbs == NULL)
    return;
  const int num = PRIVATE(this)->closecbs->getLength();
  for (int i = 0; i < num; i++) {
    SoXtWindowCloseCallbackInfo * info =
      (SoXtWindowCloseCallbackInfo *) (*PRIVATE(this)->closecbs)[i];
    // Cast required for AIX
    info->callback(info->closure, (SoXtComponent *) this);
  }
} // invokeWindowCloseCallbacks()

// *************************************************************************

/*!
  This method returns a pointer to the SoXtComponent object which the
  \a widget argument is registered for.
*/

SoXtComponent *
SoXtComponent::getComponent(// static
  Widget widget)
{
  assert(SoXtComponentP::widgets != NULL);
  int pos = SoXtComponentP::widgets->find((void *) widget);
  if (pos == -1)
    return NULL;
  return (SoXtComponent *) (*SoXtComponentP::components)[pos];
} // getComponent()

/*!
  This method returns the name of the component.
*/

const char *
SoXtComponent::getWidgetName(
  void) const
{
  return PRIVATE(this)->widgetname ? PRIVATE(this)->widgetname : this->getDefaultWidgetName();
} // getWidgetName()

/*!
  This method returns the class name of the component.
*/

const char *
SoXtComponent::getClassName(
  void) const
{
  return PRIVATE(this)->widgetclass;
} // getClassName()

/*!
  This method sets the base widget of the component.
*/
void         // protected
SoXtComponent::setBaseWidget(Widget widget)
{
  const EventMask events = StructureNotifyMask | VisibilityChangeMask;

//  ExposureMask | VisibilityChangeMask | ResizeRedirectMask |
//    FocusChangeMask;
//    EnterWindowMask | LeaveWindowMask |

  if (PRIVATE(this)->widget) {
    // remove event handler
  }

  PRIVATE(this)->widget = widget;

  // really resize widget?  after all, size has been touched...
  if (PRIVATE(this)->size[0] != -1)
    XtVaSetValues(PRIVATE(this)->widget, XtNwidth, PRIVATE(this)->size[0], NULL);
  if (PRIVATE(this)->size[1] != -1)
    XtVaSetValues(PRIVATE(this)->widget, XtNheight, PRIVATE(this)->size[1], NULL);
  // register widget?

  XtInsertEventHandler(PRIVATE(this)->widget, events, False,
    SoXtComponent::event_handler, (XtPointer) this, XtListTail);
} // setBaseWidget()

/*!
  This method sets the class name of the widget.
*/

void
SoXtComponent::setClassName(// protected
  const char * const name)
{
  if (PRIVATE(this)->widgetclass && strlen(PRIVATE(this)->widgetclass) >= strlen(name)) {
    strcpy(PRIVATE(this)->widgetclass, (char *) name);
  } else {
    delete [] PRIVATE(this)->widgetclass;
    PRIVATE(this)->widgetclass = strcpy(new char [strlen(name)+1], name);
  }
} // setClassName()

/*!
  This method should be hooked up to window close events.
  If the main window is closed, the program exits.  If a sub-window is
  closed, it is just hidden.
*/

void
SoXtComponent::windowCloseAction(// virtual, protected
  void)
{
  if (this->getShellWidget() == SoXt::getTopLevelWidget()) {
    XtAppSetExitFlag(SoXt::getAppContext());
  } else {
    this->hide();
  }
} // windowCloseAction()

/*!
  This method is a hook that is called when the component is realized.

  Invocation of this hook is not implemented yet.
*/

void
SoXtComponent::afterRealizeHook(// virtual, protected
  void)
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo("SoXtComponent::afterRealizeHook", "invoked");
#endif // SOXT_DEBUG
  if (this->isTopLevelShell()) {

    XtVaSetValues(this->getShellWidget(),
      XmNtitle, this->getTitle(),
      XmNiconName, this->getIconTitle(),
      NULL);

    if (PRIVATE(this)->size[0] > 0) {
      XtVaSetValues(this->getShellWidget(),
        XmNwidth, PRIVATE(this)->size[0],
        XmNheight, PRIVATE(this)->size[1],
        NULL);
    }
  }
} // afterRealizeHook()

// *************************************************************************

/*!
  This method returns the default name for the component widget.

  It should be overloaded by SoXtComponent-derived classes so the
  topmost widget in the component gets a proper name.
*/

const char *
SoXtComponent::getDefaultWidgetName(// virtual, protected
  void) const
{
  static const char defaultWidgetName[] = "SoXtComponent";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
  This method returns the default window title for the component.

  It should be overloaded by SoXtComponent-derived classes so the window
  and popup menu will get a proper title.
*/

const char *
SoXtComponent::getDefaultTitle(// virtual, protected
  void) const
{
  static const char defaultTitle[] = "Xt Component";
  return defaultTitle;
} // getDefaultTitle()

/*!
  This method returns the default title for icons for the component window.

  It should be overloaded by SoXtComponent-derived classes so icons will
  get proper titles.
*/

const char *
SoXtComponent::getDefaultIconTitle(// virtual, protected
  void) const
{
  static const char defaultIconTitle[] = "Xt Component";
  return defaultIconTitle;
} // getDefaultIconTitle()

// *************************************************************************

/*!
  This method registers the widget as part of the component.

  All components should at least register it's base widget.  This database
  is used by the SoXtResource class.

  \sa SoXtComponent::unregisterWidget()
*/

// FIXME: Should base widgets get registered when setBaseWidget is called?

void
SoXtComponent::registerWidget(// protected
  Widget widget)
{
  if (SoXtComponentP::widgets == NULL) {
    SoXtComponentP::widgets = new SbPList;
    SoXtComponentP::components = new SbPList;
  }
  SoXtComponentP::widgets->append((void *) widget);
  SoXtComponentP::components->append((void *) this);
} // registerWidget()

/*!
  This method unregisters \a widget.

  \sa SoXtComponent::registerWidget()
*/

void
SoXtComponent::unregisterWidget(// protected
  Widget widget)
{
  assert(SoXtComponentP::widgets != NULL);
  assert(widget != NULL);
  int pos = SoXtComponentP::widgets->find((void *) widget);
  if (pos == -1) {
    SoDebugError::post("SoXtComponent::unregisterWidget",
      "widget (%s) not registered", XtName(widget));
  }
  assert(SoXtComponentP::components != NULL);
  SoXtComponentP::widgets->remove(pos);
  SoXtComponentP::components->remove(pos);
} // unregisterWidget()

// *************************************************************************

/*!
  This method adds a callback that will be invoked when the components
  visibility changes.

  Callback invocation is not implemented yet.

  \sa SoXtComponent::removeVisibilityChangeCallback()
*/

void
SoXtComponent::addVisibilityChangeCallback(// protected
  SoXtComponentVisibilityCB * callback,
  void * closure)
{
  if (PRIVATE(this)->visibilitycbs == NULL)
    PRIVATE(this)->visibilitycbs = new SbPList;
  SoXtComponentVisibilityCallbackInfo * info =
    new SoXtComponentVisibilityCallbackInfo;
  PRIVATE(this)->visibilitycbs->append(info);
} // addVisibilityChangeCallback()

/*!
  This method removes a callback from the list of callbacks that are to be
  invoked when the component visibility changes.

  \sa SoXtComponent::addVisibilityChangeCallback()
*/

void
SoXtComponent::removeVisibilityChangeCallback(// protected
  SoXtComponentVisibilityCB * callback,
  void * closure)
{
  if (PRIVATE(this)->visibilitycbs != NULL) {
    const int num = PRIVATE(this)->visibilitycbs->getLength();
    for (int i = 0; i < num; i++) {
      SoXtComponentVisibilityCallbackInfo * info =
        (SoXtComponentVisibilityCallbackInfo *)
          (*PRIVATE(this)->visibilitycbs)[i];
      if (info->callback == callback && info->closure == closure) {
        PRIVATE(this)->visibilitycbs->remove(i);
        delete info;
        return;
      }
    }
  }
#if SOXT_DEBUG
  SoDebugError::post("SoXtComponent::removeVisibilityChangeCallback",
    "Tried to remove nonexistent callback.");
#endif // SOXT_DEBUG
} // removeVisibilityChangeCallback()

/*!
  This method invokes all the visibility-change callbacks.

  This method is not part of the Open Inventor API.

  \sa SoXtComponent::addVisibilityChangeCallback()
  \sa SoXtComponent::removeVisibilityChangeCallback()
*/

void
SoXtComponent::invokeVisibilityChangeCallbacks(// protected
  const SbBool enable) const
{
  if (PRIVATE(this)->visibilitycbs == NULL)
    return;
  const int num = PRIVATE(this)->visibilitycbs->getLength();
  for (int i = 0; i < num; i++) {
    SoXtComponentVisibilityCallbackInfo * info =
      (SoXtComponentVisibilityCallbackInfo *)
        (*PRIVATE(this)->visibilitycbs)[i];
    info->callback(info->closure, enable);
  }
} // invokeVisibilityChangeCallbacks()

// *************************************************************************

/*!
  This method is used to open component help cards.  \a name is the name of
  a file that will be searched for in the following path:

  .:$SO_HELP_DIR:/{prefix}/share/Coin/help

  If no card is found, an error dialog will appear.

  This method is not implemented yet.

  \sa SoXtViewer::openViewerHelpCard()
*/

void         // protected
SoXtComponent::openHelpCard(const char * const name)
{
  SoXt::createSimpleErrorDialog(this->getWidget(),
                                "Not Implemented",
                                "Help Card functionality is not implemented yet.");
} // openHelpCard()

// *************************************************************************

/*!
  This function is not implemented yet.
*/

char *
SoXtComponent::getlabel(// static, protected
  unsigned int what)
{
  SOXT_STUB();
  return "(null)";
} // getlabel()

// *************************************************************************

/*!
  FIXME: write doc
*/
Boolean      // protected, virtual
SoXtComponent::sysEventHandler(Widget widget, XEvent * event)
{
  if (widget == PRIVATE(this)->widget) { // base widget
#if SOXT_DEBUG && 0
    SoDebugError::postInfo("SoXtComponent::sysEventHandler",
                           "base widget event (%d)", event->type);
#endif // SOXT_DEBUG
    if (event->type == ConfigureNotify) {
      XConfigureEvent * conf = (XConfigureEvent *) event;
      if (PRIVATE(this)->size != SbVec2s(conf->width, conf->height)) {
        PRIVATE(this)->size = SbVec2s(conf->width, conf->height);
        this->sizeChanged(PRIVATE(this)->size);
      }
    } else if (event->type == MapNotify) {
      Dimension width = 0, height = 0;
      XtVaGetValues(this->getBaseWidget(),
        XmNwidth, &width,
        XmNheight, &height,
        NULL);
      PRIVATE(this)->size = SbVec2s(width, height);
      this->sizeChanged(PRIVATE(this)->size);
    } else if (event->type == VisibilityNotify) {
//    SoDebugError::postInfo("SoXtComponent::sysEventHandler", "Visibility");
      XVisibilityEvent * visibility = (XVisibilityEvent *) event;
      SbBool newvisibility = TRUE;
      if (visibility->state == VisibilityFullyObscured)
        newvisibility = FALSE;
      if (PRIVATE(this)->visibilitystate != newvisibility) {
        PRIVATE(this)->visibilitystate = newvisibility;
        this->invokeVisibilityChangeCallbacks(PRIVATE(this)->visibilitystate);
      }
    }
  } else if (this->isTopLevelShell() && widget == this->getShellWidget()) {
#if SOXT_DEBUG && 0
    SoDebugError::postInfo("SoXtComponent::sysEventHandler",
                           "shell widget event (%d)", event->type);
#endif // SOXT_DEBUG

    if (event->type == ConfigureNotify) {
      XConfigureEvent * conf = (XConfigureEvent *) event;
      if (PRIVATE(this)->size != SbVec2s(conf->width, conf->height)) {
        PRIVATE(this)->size = SbVec2s(conf->width, conf->height);
        XtVaSetValues(this->getBaseWidget(),
          XmNwidth, PRIVATE(this)->size[0],
          XmNheight, PRIVATE(this)->size[1],
          NULL);
        this->sizeChanged(PRIVATE(this)->size);
      }
    }
  } else {
#if SOXT_DEBUG && 0
    SoDebugError::postInfo("SoXtComponent::sysEventHandler",
                           "[removing] event handler for unknown widget");
#endif // SOXT_DEBUG
  }
  return True;
}

/*!
  This static callback invokes SoXtComponent::sysEventHandler.

  \sa sysEventHandler
*/
void
SoXtComponent::event_handler(Widget widget,
                             XtPointer closure,
                             XEvent * event,
                             Boolean * dispatch)
{
  assert(closure != NULL);
  SoXtComponent * component = (SoXtComponent *) closure;
  *dispatch = component->sysEventHandler(widget, event);
} // event_handler()

/*!
  Toggle full screen mode for this component, if possible.

  Returns \c FALSE if operation failed.  This might happen if the
  toolkit doesn't support attempts at making the component cover the
  complete screen or if the component is not a toplevel window.
*/
SbBool
SoXtComponent::setFullScreen(const SbBool onoff)
{
  if (onoff == PRIVATE(this)->fullscreen) { return TRUE; }
  SOXT_STUB();
  return FALSE;
}

/*!
  Returns if this widget/component is in full screen mode.
*/
SbBool
SoXtComponent::isFullScreen(void) const
{
  return PRIVATE(this)->fullscreen;
}

// Converts from the common generic cursor format to a X11 Cursor
// instance.
Cursor
SoXtComponentP::getNativeCursor(Display * d,
                                const SoXtCursor::CustomCursor * cc)
{
  if (SoXtComponentP::cursordict == NULL) { // first call, initialize
    SoXtComponentP::cursordict = new SbDict; // FIXME: mem leak. 20011121 mortene.
  }

  void * qc;
  SbBool b = SoXtComponentP::cursordict->find((unsigned long)cc, qc);
  if (b) { return (Cursor)qc; }

  // FIXME: translate from bitmap to native format. 20011127 mortene.

  // FIXME: currently a memory leak here. 20011121 mortene.
  Cursor c = XCreateFontCursor(d, XC_hand2);
  // FIXME: use a better dict class -- the void* cast is ugly.
  // 20011127 mortene.
  assert(sizeof(Cursor) <= sizeof(void*));
  SoXtComponentP::cursordict->enter((unsigned long)cc, (void *)c);
  return c;
}

/*!
  Sets the cursor for this component.
*/
void 
SoXtComponent::setComponentCursor(const SoXtCursor & cursor)
{
  SoXtComponent::setWidgetCursor(this->getWidget(), cursor);
}

/*!
  Set cursor for a native widget in the underlying toolkit.
*/
void
SoXtComponent::setWidgetCursor(Widget w, const SoXtCursor & cursor)
{
  Display * d = SoXt::getDisplay();

  if (cursor.getShape() == SoXtCursor::CUSTOM_BITMAP) {
    const SoXtCursor::CustomCursor * cc = &cursor.getCustomCursor();
    XDefineCursor(d, XtWindow(w), SoXtComponentP::getNativeCursor(d, cc));
  }
  else {
    Cursor c;
    switch (cursor.getShape()) {
    case SoXtCursor::DEFAULT:
      XUndefineCursor(d, XtWindow(w));
      break;

    case SoXtCursor::BUSY:
      // FIXME: plug memory leak. 20011127 mortene.
      c = XCreateFontCursor(d, XC_clock);
      XDefineCursor(d, XtWindow(w), c);
      break;

    case SoXtCursor::BLANK:
      // FIXME: implement. 20011127 mortene.
      SOXT_STUB();
      break;

    case SoXtCursor::CROSSHAIR:
      // FIXME: plug memory leak. 20011127 mortene.
      c = XCreateFontCursor(d, XC_crosshair);
      XDefineCursor(d, XtWindow(w), c);
      break;

    case SoXtCursor::UPARROW:
      // FIXME: plug memory leak. 20011127 mortene.
      c = XCreateFontCursor(d, XC_based_arrow_up);
      // FIXME: perhaps this one is better?:  20011127 mortene.
//        Cursor c = XCreateFontCursor(d, XC_sb_up_arrow);
      XDefineCursor(d, XtWindow(w), c);
      break;

    default:
      assert(FALSE && "unsupported cursor shape type");
      break;
    }
  }
}

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtComponentRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG
