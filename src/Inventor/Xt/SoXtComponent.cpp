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

// *************************************************************************

/*!
  \class SoXtComponent Inventor/Xt/SoXtComponent.h
  \brief The SoXtComponent class is the base class for all SoXt components.
  \ingroup components
*/

// *************************************************************************

#include <assert.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/Xmu/Editres.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/MessageB.h>

#include <Inventor/misc/SoBasic.h>
#include <Inventor/SoLists.h>
#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/SoXt.h>

#include <Inventor/Xt/SoXtComponent.h>

// *************************************************************************

// static variables
SbPList * SoXtComponent::widgets = NULL;
SbPList * SoXtComponent::components = NULL;

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

SoXtComponent::SoXtComponent( // protected
  const Widget parent,
  const char * const name,
  const SbBool embed )
{
  this->constructorParent = parent;

  this->title = NULL;
  this->iconTitle = NULL;
  this->widgetName = NULL;
  this->widgetClass = NULL;
  this->firstRealize = TRUE;
  this->widget = NULL;

  this->close_callbacks = NULL;
  this->visibility_callbacks = NULL;

  this->size = SbVec2s( -1, -1 );

  if ( name && strlen(name) > 0 )
    this->widgetName = strcpy( new char [ strlen( name ) + 1 ], name );

  if ( (parent == (Widget) NULL) || ! embed ) {
    // create own shell

    Visual * visual = NULL; 
    Colormap colormap = 0;
    int depth = 0;
    Display * dpy = SoXt::getDisplay();

    if ( parent ) {
      Widget shell = parent;
      while ( ! XtIsShell( shell ) && shell != (Widget) NULL )
        shell = XtParent( shell );
      assert( shell != (Widget) NULL );
      dpy = XtDisplay(shell);
      XtVaGetValues( shell, 
        XmNvisual, &visual,
        XmNcolormap, &colormap,
        XmNdepth, &depth,
        NULL );
    } else {
      SoXt::selectBestVisual( dpy, visual, colormap, depth );
    }
    assert( dpy != NULL );

    this->parent = XtVaAppCreateShell(
      SoXt::getAppName(), // didn't work
      SoXt::getAppClass(),
      topLevelShellWidgetClass,
      dpy,
      XmNvisual, visual,
      XmNcolormap, colormap,
      XmNdepth, depth,
      NULL );

#if SOXT_DEBUG
    XtEventHandler editres_hook = (XtEventHandler) _XEditResCheckMessages;
    XtAddEventHandler( this->parent, (EventMask) 0, True, editres_hook, NULL );
#endif // SOXT_DEBUG

    this->embedded = FALSE;
  } else {
    this->parent = parent;
    this->embedded = TRUE;
  }
  if ( parent && XtIsShell( parent ) )
    this->embedded = FALSE;
} // SoXtComponent()

/*!
  The destructor.
*/

SoXtComponent::~SoXtComponent( // virtual
  void )
{
  delete [] this->widgetName;
  delete [] this->widgetClass;
  delete [] this->title;
  delete [] this->iconTitle;
  if ( this->close_callbacks != NULL ) {
    const int num = this->close_callbacks->getLength();
    for ( int i = 0; i < num; i++ ) {
      SoXtWindowCloseCallbackInfo * info =
        (SoXtWindowCloseCallbackInfo *) (*this->close_callbacks)[i];
      delete info;
    }
    delete this->close_callbacks;
  }
  if ( this->visibility_callbacks != NULL ) {
    const int num = this->visibility_callbacks->getLength();
    for ( int i = 0; i < num; i++ ) {
      SoXtComponentVisibilityCallbackInfo * info =
        (SoXtComponentVisibilityCallbackInfo *)
          (*this->visibility_callbacks)[i];
      delete info;
    }
    delete this->visibility_callbacks;
  }
} // ~SoXtComponent()

// *************************************************************************

/*!
  This method realizes the component.

  topLevelShell widgets will be realized and mapped.
  non-toplevel components will just be managed.
*/

void
SoXtComponent::show( // virtual
  void )
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtComponent::show", "[enter]" );
#endif // SOXT_DEBUG
  if ( this->isTopLevelShell() ) {
    Widget shell = this->getShellWidget();
    XtRealizeWidget( shell );
    if ( this->firstRealize == TRUE ) {
      this->afterRealizeHook();
      this->firstRealize = FALSE;
    }
    XtMapWidget( shell );
  } else {
    XtManageChild( this->getBaseWidget() );
  }
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtComponent::show", "[exit]" );
#endif // SOXT_DEBUG
} // show()

/*!
  This method hides the component.

  topLevelShell widgets will be unrealized.
  non-toplevel components will just be unmanaged.
*/

void
SoXtComponent::hide( // virtual
  void )
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtComponent::hide", "[enter]" );
#endif // SOXT_DEBUG
  if ( this->isTopLevelShell() ) {
    Widget shell = this->getShellWidget();
    XtUnmapWidget( shell );
    XtUnrealizeWidget( shell );
  } else {
    XtUnmanageChild( this->getBaseWidget() );
  }
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtComponent::hide", "[exit]" );
#endif // SOXT_DEBUG
} // hide()

/*!
  This method returns TRUE if component is shown, and FALSE if it is hidden.

  This method is not implemented.
*/

SbBool
SoXtComponent::isVisible(
  void )
{
//  tracking calls to show/hide is not the answer - use X calls to see if
//  mapped.
  SOXT_STUB();
  return TRUE;
} // isVisible()

// *************************************************************************

/*!
  This method returns the base widget of the component.

  \sa SoXtComponent::getBaseWidget()
*/

Widget
SoXtComponent::getWidget(
  void ) const
{
  return this->getBaseWidget();
} // getWidget()

/*!
  This method returns the base widget of the component.

  \sa SoXtComponent::getBaseWidget()
*/

Widget
SoXtComponent::baseWidget(
  void ) const
{
  return this->getBaseWidget();
} // baseWidget()

/*!
  This method returns the base widget of the component.

  \sa SoXtComponent::getBaseWidget()
*/

Widget
SoXtComponent::getBaseWidget(
  void ) const
{
  return this->widget;
} // getBaseWidget()

/*!
  This method returns whether the component was created as a toplevel shell
  or not.
*/

SbBool
SoXtComponent::isTopLevelShell(
  void ) const
{
  return this->embedded ? FALSE : TRUE;
} // isTopLevelShell()

/*!
  This method returns the shell widget of the component, but only if it was
  created as a toplevel shell.  This method will return NULL for embedded
  components.
*/

Widget
SoXtComponent::getShellWidget(
  void ) const
{
  return this->isTopLevelShell() ? this->parent : (Widget) NULL;
} // getShellWidget()

/*!
  This method returns the parent widget of the component widget.
  If the component created its own toplevel shell, this method returns the
  the shell widget.  If the component is embedded, this method returns the
  widget given in the \a parent argument of the constructor.
*/

Widget      
SoXtComponent::getParentWidget(
  void ) const
{
  return this->parent;
} // getParentWidget()

// *************************************************************************

/*!
  This method sets the size of the component.

  The method assumes the caller knows what he is doing.
*/

void    
SoXtComponent::setSize(
  const SbVec2s size )
{
  this->size = size;
  Widget widget;
  if ( this->isTopLevelShell() )
    widget = this->getShellWidget();
  else
    widget = this->getBaseWidget();
  if ( ! widget )
    return;

  int argc = 0;
  Arg args[2];
  if ( size[0] != -1 ) {
    XtSetArg( args[argc], XmNwidth, size[0] );
    argc++;
  }
  if ( size[1] != -1 ) {
    XtSetArg( args[argc], XmNheight, size[1] );
    argc++;
  }
  XtSetValues( widget, args, argc );
} // setSize()

/*!
  This method returns the size of the component.

  The size that is returned is a cached size value, not a value fetched
  from the GUI system.
*/

SbVec2s   
SoXtComponent::getSize(
  void )
{
  return this->size;
} // getSize()

/*!
  This method tries to resize the component window, using \a size as the
  minimum requirements.

  This method is not part of the Open Inventor SoXt API.
*/

void
SoXtComponent::fitSize(
  const SbVec2s size )
{
  if ( this->isTopLevelShell() || (this->parent && XtIsShell(this->parent)) ) {
    XtWidgetGeometry geometry;
    XtQueryGeometry( this->getBaseWidget(), NULL, &geometry );
    this->size[0] = SoXtMax( (short) geometry.width, size[0] );
    this->size[1] = SoXtMax( (short) geometry.height, size[1] );
    XtVaSetValues( this->getShellWidget(),
      XmNwidth, this->size[0],
      XmNheight, this->size[1],
      NULL );
  }
} // fitSize()

// *************************************************************************

/*!
  This method returns the display the component is sent to.
*/

Display *
SoXtComponent::getDisplay(
  void )
{
#if SOXT_DEBUG
  if ( ! this->getBaseWidget() )
    SoDebugError::postInfo( "SoXtComponent::getDisplay",
      "component has no base widget" );
#endif // SOXT_DEBUG
  return this->getBaseWidget() ?
    XtDisplay( this->getBaseWidget() ) : (Display *) NULL;
} // getDisplay()

// *************************************************************************

/*!
  This method sets the title of the component.

  The title will appear on the window title bar, if the component manages
  its own window.
*/

void      
SoXtComponent::setTitle(
  const char * const title )
{
  if ( this->title && strlen(this->title) >= strlen(title) ) {
    strcpy( this->title, (char *) title );
  } else {
    delete [] this->title;
    this->title = strcpy( new char [strlen(title)+1], title );
  }

  Widget shell = this->isTopLevelShell() ?
    this->getShellWidget() : (Widget) NULL;
  if ( ! shell )
    return;
  XtVaSetValues( shell,
    XmNtitle, this->title,
    NULL );
} // setTitle()

/*!
  This method returns the title of the component.

  If a title has been set, that title will be returned.
  If no title has been set, the default title is returned.
*/

const char *
SoXtComponent::getTitle(
  void ) const
{
  // FIXME: use SoXtResource to see if title is set?
  return this->title ? this->title : this->getDefaultTitle();
}

/*!
  This method sets the title of the icon representation of the window.

  The title will appear on the window icon, if the component manages its
  own window.
*/

void      
SoXtComponent::setIconTitle(
  const char * const title )
{
  if ( this->iconTitle && strlen(this->iconTitle) >= strlen(title) ) {
    strcpy( this->iconTitle, (char *) title );
  } else {
    delete [] this->iconTitle;
    this->iconTitle = strcpy( new char [strlen(title)+1], title );
  }
  Widget shell = this->isTopLevelShell() ?
    this->getShellWidget() : (Widget) NULL;
  if ( ! shell )
    return;
  XtVaSetValues( shell,
    XtNiconName, this->iconTitle,
    NULL );
}

/*!
  This method returns the icon title for the component.

  If an icon title has been set, that icon title is returned.
  If no icon title has been set, the default icon title is returned.
*/

const char * 
SoXtComponent::getIconTitle(
  void ) const
{
  // FIXME: use SoXtResource to see if iconName is set?
  return this->iconTitle ? this->iconTitle : this->getDefaultIconTitle();
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
  void * closure )
{
  this->addWindowCloseCallback( callback, closure );
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
  void * closure )
{
  if ( this->close_callbacks == NULL )
    this->close_callbacks = new SbPList;
  SoXtWindowCloseCallbackInfo * info = new SoXtWindowCloseCallbackInfo;
  info->callback = callback;
  info->closure = closure;
  this->close_callbacks->append( info );
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
  void * closure )
{
  if ( this->close_callbacks != NULL ) {
    const int num = this->close_callbacks->getLength();
    for ( int i = 0; i < num; i++ ) {
      SoXtWindowCloseCallbackInfo * info =
        (SoXtWindowCloseCallbackInfo *) (*this->close_callbacks)[i];
      if ( info->callback == callback && info->closure == closure ) {
        this->close_callbacks->remove( i );
        delete info;
        return;
      }
    }
  }
#if SOXT_DEBUG
  SoDebugError::post( "SoXtComponent::removeWindowCloseCallback",
    "trying to remove nonexisting callback" );
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
SoXtComponent::invokeWindowCloseCallbacks( // protected
  void ) const
{
  if ( this->close_callbacks == NULL )
    return;
  const int num = this->close_callbacks->getLength();
  for ( int i = 0; i < num; i++ ) {
    SoXtWindowCloseCallbackInfo * info =
      (SoXtWindowCloseCallbackInfo *) (*this->close_callbacks)[i];
    // Cast required for AIX
    info->callback( info->closure, (SoXtComponent *) this );
  }
} // invokeWindowCloseCallbacks()

// *************************************************************************

/*!
  This method returns a pointer to the SoXtComponent object which the
  \a widget argument is registered for.
*/

SoXtComponent *
SoXtComponent::getComponent( // static
  Widget widget )
{
  assert( SoXtComponent::widgets != NULL );
  int pos = SoXtComponent::widgets->find( (void *) widget );
  if ( pos == -1 )
    return NULL;
  return (SoXtComponent *) (*SoXtComponent::components)[pos];
} // getComponent()

/*!
  This method returns the name of the component.
*/

const char *
SoXtComponent::getWidgetName(
  void ) const
{
  return this->widgetName ? this->widgetName : this->getDefaultWidgetName();
} // getWidgetName()

/*!
  This method returns the class name of the component.
*/

const char *
SoXtComponent::getClassName(
  void ) const
{
  return this->widgetClass;
} // getClassName()

/*!
  This method sets the base widget of the component.
*/

void
SoXtComponent::setBaseWidget( // protected
  Widget widget )
{
  this->widget = widget;
  if ( this->size[0] != -1 )
    XtVaSetValues( this->widget, XtNwidth, this->size[0], NULL );
  if ( this->size[1] != -1 )
    XtVaSetValues( this->widget, XtNheight, this->size[1], NULL );
  // register widget?
} // setBaseWidget()

/*!
  This method sets the class name of the widget.
*/

void
SoXtComponent::setClassName( // protected
  const char * const name )
{
  if ( this->widgetClass && strlen(this->widgetClass) >= strlen(name) ) {
    strcpy( this->widgetClass, (char *) name );
  } else {
    delete [] this->widgetClass;
    this->widgetClass = strcpy( new char [strlen(name)+1], name );
  }
} // setClassName()

/*!
  This method should be hooked up to window close events.
  If the main window is closed, the program exits.  If a sub-window is
  closed, it is just hidden.
*/

void
SoXtComponent::windowCloseAction( // virtual, protected
  void )
{
  if ( this->getShellWidget() == SoXt::getTopLevelWidget() ) {
    XtAppSetExitFlag( SoXt::getAppContext() );
  } else {
    this->hide();
  }
} // windowCloseAction()

/*!
  This method is a hook that is called when the component is realized.

  Invocation of this hook is not implemented yet.
*/

void
SoXtComponent::afterRealizeHook( // virtual, protected
  void )
{
#if SOXT_DEBUG && 0
  SoDebugError::postInfo( "SoXtComponent::afterRealizeHook", "invoked" );
#endif // SOXT_DEBUG
  if ( this->isTopLevelShell() ) {

    XtVaSetValues( this->getShellWidget(),
      XmNtitle, this->getTitle(),
      XmNiconName, this->getIconTitle(),
      NULL );

    if ( this->size[0] > 0 ) {
      XtVaSetValues( this->getShellWidget(),
        XmNwidth, this->size[0],
        XmNheight, this->size[1],
        NULL );
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
SoXtComponent::getDefaultWidgetName( // virtual, protected
  void ) const
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
SoXtComponent::getDefaultTitle( // virtual, protected
  void ) const
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
SoXtComponent::getDefaultIconTitle( // virtual, protected
  void ) const
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
SoXtComponent::registerWidget( // protected
  Widget widget )
{
  if ( SoXtComponent::widgets == NULL ) {
    SoXtComponent::widgets = new SbPList;
    SoXtComponent::components = new SbPList;
  }
  SoXtComponent::widgets->append( (void *) widget );
  SoXtComponent::components->append( (void *) this );
} // registerWidget()

/*!
  This method unregisters \a widget.

  \sa SoXtComponent::registerWidget()
*/

void
SoXtComponent::unregisterWidget( // protected
  Widget widget )
{
  assert( SoXtComponent::widgets != NULL );
  assert( widget != NULL );
  int pos = SoXtComponent::widgets->find( (void *) widget );
  if ( pos == -1 ) {
    SoDebugError::post( "SoXtComponent::unregisterWidget",
      "widget (%s) not registered", XtName( widget ) );
  }
  assert( SoXtComponent::components != NULL );
  SoXtComponent::widgets->remove( pos );
  SoXtComponent::components->remove( pos );
} // unregisterWidget()

// *************************************************************************

/*!
  This method adds a callback that will be invoked when the components
  visibility changes.

  Callback invocation is not implemented yet.

  \sa SoXtComponent::removeVisibilityChangeCallback()
*/

void
SoXtComponent::addVisibilityChangeCallback( // protected
  SoXtComponentVisibilityCB * callback,
  void * closure )
{
  if ( this->visibility_callbacks == NULL )
    this->visibility_callbacks = new SbPList;
  SoXtComponentVisibilityCallbackInfo * info =
    new SoXtComponentVisibilityCallbackInfo;
  this->visibility_callbacks->append( info );
} // addVisibilityChangeCallback()

/*!
  This method removes a callback from the list of callbacks that are to be
  invoked when the component visibility changes.
  
  \sa SoXtComponent::addVisibilityChangeCallback()
*/

void
SoXtComponent::removeVisibilityChangeCallback( // protected
  SoXtComponentVisibilityCB * callback,
  void * closure )
{
  if ( this->visibility_callbacks != NULL ) {
    const int num = this->visibility_callbacks->getLength();
    for ( int i = 0; i < num; i++ ) {
      SoXtComponentVisibilityCallbackInfo * info =
        (SoXtComponentVisibilityCallbackInfo *)
          (*this->visibility_callbacks)[i];
      if ( info->callback == callback && info->closure == closure ) {
        this->visibility_callbacks->remove( i );
        delete info;
        return;
      }
    }
  }
#if SOXT_DEBUG
  SoDebugError::post( "SoXtComponent::removeVisibilityChangeCallback",
    "Tried to remove nonexistent callback." );
#endif // SOXT_DEBUG
} // removeVisibilityChangeCallback()

/*!
  This method invokes all the visibility-change callbacks.

  This method is not part of the Open Inventor API.

  \sa SoXtComponent::addVisibilityChangeCallback()
  \sa SoXtComponent::removeVisibilityChangeCallback()
*/

void
SoXtComponent::invokeVisibilityChangeCallbacks( // protected
  const SbBool enable ) const
{
  if ( this->visibility_callbacks == NULL )
    return;
  const int num = this->visibility_callbacks->getLength();
  for ( int i = 0; i < num; i++ ) {
    SoXtComponentVisibilityCallbackInfo * info =
      (SoXtComponentVisibilityCallbackInfo *)
        (*this->visibility_callbacks)[i];
    info->callback( info->closure, enable );
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

void
SoXtComponent::openHelpCard( // protected
  const char * name )
{
  SoXt::createSimpleErrorDialog( this->getWidget(),
    "Not Implemented",
    "Help Card functionality is not implemented yet." );
} // openHelpCard()

// *************************************************************************

/*!
  This function is not implemented yet.
*/

char *
SoXtComponent::getlabel( // static, protected
  unsigned int what )
{
  SOXT_STUB();
  return "(null)";
} // getlabel()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtComponentRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

