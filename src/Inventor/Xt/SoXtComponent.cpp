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
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtComponent.h>

// *************************************************************************

// static variables
SbPList * SoXtComponent::widgets = NULL;
SbPList * SoXtComponent::components = NULL;

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

    XSync( dpy, False );
    SoDebugError::postInfo( "", "creating shell" );
    this->parent = XtVaAppCreateShell(
      SoXt::getAppName(), // didn't work
      SoXt::getAppClass(),
      topLevelShellWidgetClass,
      dpy,
      XmNvisual, visual,
      XmNcolormap, colormap,
      XmNdepth, depth,
      NULL );
    XSync( dpy, False );
    SoDebugError::postInfo( "", "shell created" );

    XtEventHandler editres_hook = (XtEventHandler) _XEditResCheckMessages;
    XtAddEventHandler( this->parent, (EventMask) 0, True, editres_hook, NULL );

    this->embedded = FALSE;
  } else {
    this->parent = parent;
    this->embedded = TRUE;
  }
  if ( parent ) {
    if ( parent == SoXt::getTopLevelWidget() )
      this->embedded = FALSE;
//    else if ( XtIsShell(parent) )
//      this->embedded = FALSE;
  }

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
} // ~SoXtComponent()

// *************************************************************************

/*!
  This method shows the component.  topLevelShell widgets will be realized
  and mapped.  non-toplevel components will just be managed.
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
  This method hides the component.  topLevelShell widgets will be unmapped
  and destroyed.  non-toplevel components will just be unmanaged.
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
*/

SbBool
SoXtComponent::isVisible(
  void )
{
  SOXT_STUB();
  return TRUE;
} // isVisible()

/*!
*/

Widget
SoXtComponent::getWidget(
  void ) const
{
  return this->getBaseWidget();
} // getWidget()

/*!
*/

Widget
SoXtComponent::baseWidget(
  void ) const
{
  return this->getBaseWidget();
} // baseWidget()

/*!
*/

Widget
SoXtComponent::getBaseWidget(
  void ) const
{
  return this->widget;
} // getBaseWidget()

/*!
  This method returns wether the component was created as a toplevel shell
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
  widget given in the \a parent argument of the SoXtComponent constructor.
*/

Widget      
SoXtComponent::getParentWidget(
  void ) const
{
  return this->parent;
} // getParentWidget()

/*!
*/

void    
SoXtComponent::setSize(
  const SbVec2s size )
{
  if ( this->isTopLevelShell() ) {
    XtVaSetValues( this->getShellWidget(),
      XmNwidth, size[0],
      XmNheight, size[1],
      NULL );
  }
  this->size = size;
} // setSize()

/*!
*/

SbVec2s   
SoXtComponent::getSize(
  void )
{
  return this->size;
} // getSize()

/*!
  This method returns the display the component is sent to.
*/

Display *
SoXtComponent::getDisplay(
  void )
{
  return this->getBaseWidget() ?
    XtDisplay( this->getBaseWidget() ) : (Display *) NULL;
} // getDisplay()

// *************************************************************************

/*!
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

  if ( this->isTopLevelShell() ) {
    Widget shell = this->getShellWidget();
    if ( shell ) {
      XtVaSetValues( shell,
        XmNtitle, this->title,
        NULL );
    }
  }
} // setTitle()

/*!
*/

const char *
SoXtComponent::getTitle(
  void ) const
{
  // FIXME: use SoXtResource to see if title is set
  return this->title ? this->title : this->getDefaultTitle();
}

/*!
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
  // If the component has a parent, set the parent's icon title, otherwise
  // set our widget's icon title
  if (parent || widget) {
    // FIXME: doesn't work if the widget is already realized. 20000324 mortene.
     XtVaSetValues(parent ? parent : widget,
                   XtNiconName, this->iconTitle,
                   0, 0);
  }
}

/*!
*/

const char * 
SoXtComponent::getIconTitle(
  void ) const
{
  // FIXME: use SoXtResource to see if iconName is set
  return this->iconTitle ? this->iconTitle : this->getDefaultIconTitle();
} // getIconTitle()

/*!
  This method should be 
*/

void      
SoXtComponent::setWindowCloseCallback(
  SoXtComponentCB * func,
  void * data )
{
  SOXT_STUB();
} // setWindowCloseAction()

/*!
  This method returns the SoXtComponent object \a widget is registered
  for.
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
*/

const char *
SoXtComponent::getWidgetName(
  void ) const
{
  return this->widgetName ? this->widgetName : this->getDefaultWidgetName();
} // getWidgetName()

/*!
*/

const char *
SoXtComponent::getClassName(
  void ) const
{
  return this->widgetClass;
} // getClassName()

/*!
*/

void
SoXtComponent::setBaseWidget( // protected
  Widget widget )
{
  this->widget = widget;
  if ( this->size[0] != -1 )
    XtVaSetValues( this->widget,
        XtNwidth, this->size[0], XtNheight, this->size[1], NULL );
} // setBaseWidget()

/*!
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
    exit( 0 );
  } else {
    this->hide();
  }
} // windowCloseAction()

/*!
  Hook called when window is realized.
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

/*!
  This method returns the default name for the component widget.
  It should be overloaded by SoXtComponent-derived classes so the topmost
  widget in the component gets a proper name.
*/

const char *
SoXtComponent::getDefaultWidgetName( // virtual, protected
  void ) const
{
  static const char defaultWidgetName[] = "SoXtComponent";
  return defaultWidgetName;
} // getDefaultWidgetName()

/*!
  This method returns the default window title for the component.  It
  should be overloaded by SoXtComponent-derived classes so the window
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
  This method registers the widget as part of the component.  All components
  should at least register it's base widget.  This database is used by the
  SoXtResource class.
*/

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
*/

void
SoXtComponent::addVisibilityChangeCallback( // protected
  SoXtComponentVisibilityCB * func,
  void * user )
{
  SOXT_STUB();
} // addVisibilityChangeCallback()

void
SoXtComponent::removeVisibilityChangeCallback( // protected
  SoXtComponentVisibilityCB * func,
  void * user )
{
  SOXT_STUB();
} // removeVisibilityChangeCallback()

/*!
  This method is used to open component help cards.  \a name is the name of
  a file that will be searched for in the following path:

  .:$SO_HELP_DIR:/{prefix}/share/Coin/help

  If no card is found, an error dialog will appear.

  This method is not implemented yet.
*/

void
SoXtComponent::openHelpCard( // protected
  const char * name )
{
  SoXt::createSimpleErrorDialog( this->getWidget(), "Not Supported",
    "Sorry.  Help cards are not supported by SoXt yet." );
} // openHelpCard()

/*!
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

