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

#include <string.h>

#include <Xm/Xm.h>

#include <Inventor/misc/SoBasic.h>

#include <common/soguidefs.h> // SOGUI_STUB()
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtComponent.h>

// *************************************************************************

SoXtComponent::SoXtComponent( // protected
  Widget parent,
  const char * name,
  SbBool buildInsideParent )
{
  this->size = SbVec2s( -1, -1 );
  this->widget = (Widget) NULL;
  this->parent = parent;
  if ( ! name )
    this->widgetName = 
        strcpy( new char [strlen(this->getDefaultWidgetName())+1],
                this->getDefaultWidgetName() );
  else
    this->widgetName = strcpy( new char [strlen(name)+1], name );

  this->widgetClass = NULL;
  this->setClassName( this->getDefaultWidgetName() );
  this->title = NULL;
  this->setTitle( this->getDefaultTitle() );
  this->iconTitle = NULL;
  this->setIconTitle( this->getDefaultIconTitle() );

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

void
SoXtComponent::show( // virtual
  void )
{
  if ( this->widget != (Widget) NULL )
    SoXt::show( this->widget );
} // show()

void
SoXtComponent::hide( // virtual
  void )
{
  if ( this->widget != (Widget) NULL )
    SoXt::hide( this->widget );
} // hide()

SbBool
SoXtComponent::isVisible(
  void )
{
  SOGUI_STUB();
  return TRUE;
} // isVisible()

Widget
SoXtComponent::getWidget(
  void ) const
{
  return this->widget;
} // getWidget()

Widget
SoXtComponent::baseWidget(
  void ) const
{
  return this->widget;
} // baseWidget()

SbBool
SoXtComponent::isTopLevelShell(
  void ) const
{
  SOGUI_STUB();
  return FALSE;
} // isTopLevelShell()

Widget
SoXtComponent::getShellWidget(
  void ) const
{
  SOGUI_STUB();
  return (Widget) NULL;
} // getShellWidget()

Widget      
SoXtComponent::getParentWidget(
  void ) const
{
  return this->parent;
} // getParentWidget()

void    
SoXtComponent::setSize(
  const SbVec2s size )
{
  if ( this->parent != NULL )
    SoXt::setWidgetSize( this->parent, size );
  this->size = size;
} // setSize()

SbVec2s   
SoXtComponent::getSize(
  void )
{
  return this->size;
} // getSize()

Display *
SoXtComponent::getDisplay(
  void )
{
  return SoXt::getDisplay();
} // getDisplay()

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
  // If the component has a parent, set the parent's title, otherwise
  // set our widget's title
  if (parent || widget) {
    // FIXME: doesn't work if the widget is already realized. 20000324 mortene.
     XtVaSetValues(parent ? parent : widget,
                   XmNtitle, this->title,
                   0, 0);
  }
} // setTitle()

const char *
SoXtComponent::getTitle(
  void ) const
{
  return this->title;
}

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

const char * 
SoXtComponent::getIconTitle(
  void ) const
{
  return this->iconTitle;
} // getIconTitle()

void      
SoXtComponent::setWindowCloseCallback(
  SoXtComponentCB * func,
  void * data )
{
  SOGUI_STUB();
}

SoXtComponent *
SoXtComponent::getComponent( // static
  Widget widget )
{
  SOGUI_STUB();
  return NULL;
} // getComponent()

const char *
SoXtComponent::getWidgetName(
  void ) const
{
  return this->widgetName;
} // getWidgetName()

const char *
SoXtComponent::getClassName(
  void ) const
{
  return this->widgetClass;
} // getClassName()

void
SoXtComponent::setBaseWidget( // protected
  Widget widget )
{
  this->widget = widget;
  if ( this->size[0] != -1 )
    XtVaSetValues( this->widget,
        XtNwidth, this->size[0], XtNheight, this->size[1], NULL );
} // setBaseWidget()

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

void
SoXtComponent::windowCloseAction( // virtual, protected
  void )
{
  SOGUI_STUB();
} // windowCloseAction()

void
SoXtComponent::afterRealizeHook( // virtual, protected
  void )
{
  SOGUI_STUB();
} // afterRealizeHook()

const char *
SoXtComponent::getDefaultWidgetName( // virtual, protected
  void ) const
{
  static const char defaultWidgetName[] = "Xt Component";
  return defaultWidgetName;
} // getDefaultWidgetName()

const char *
SoXtComponent::getDefaultTitle( // virtual, protected
  void ) const
{
  static const char defaultTitle[] = "Xt Component";
  return defaultTitle;
} // getDefaultTitle()

const char *
SoXtComponent::getDefaultIconTitle( // virtual, protected
  void ) const
{
  static const char defaultIconTitle[] = "Xt Component";
  return defaultIconTitle;
} // getDefaultIconTitle()

void
SoXtComponent::registerWidget( // protected
  Widget widget )
{
  SOGUI_STUB();
} // registerWidget()

void
SoXtComponent::unregisterWidget( // protected
  Widget widget )
{
  SOGUI_STUB();
} // unregisterWidget()

void
SoXtComponent::addVisibilityChangeCallback( // protected
  SoXtComponentVisibilityCB * func,
  void * user )
{
  SOGUI_STUB();
} // addVisibilityChangeCallback()

void
SoXtComponent::removeVisibilityChangeCallback( // protected
  SoXtComponentVisibilityCB * func,
  void * user )
{
  SOGUI_STUB();
} // removeVisibilityChangeCallback()

void
SoXtComponent::openHelpCard( // protected
  const char * name )
{
  SOGUI_STUB();
} // openHelpCard()

char *
SoXtComponent::getlabel( // static, protected
  unsigned int what )
{
  SOGUI_STUB();
  return "(null)";
} // getlabel()

// *************************************************************************
