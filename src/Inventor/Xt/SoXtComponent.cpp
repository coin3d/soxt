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
  this->widgetName = name;
} // SoXtComponent()

SoXtComponent::~SoXtComponent( // virtual
  void )
{
} // ~SoXtComponent()

void
SoXtComponent::show( // virtual
  void )
{
  SoXt::show( this->widget );
}

void
SoXtComponent::hide( // virtual
  void )
{
  SoXt::hide( this->widget );
}

SbBool
SoXtComponent::isVisible(
  void )
{
}

Widget
SoXtComponent::getWidget(
  void ) const
{
  return this->widget;
}

Widget
SoXtComponent::baseWidget(
  void ) const
{
  return this->widget;
}

SbBool
SoXtComponent::isTopLevelShell(
  void ) const
{
}

Widget
SoXtComponent::getShellWidget(
  void ) const
{
}

Widget      
SoXtComponent::getParentWidget(
  void ) const
{
  return this->parent;
}

void    
SoXtComponent::setSize(
  const SbVec2s size )
{
  SoXt::setWidgetSize( this->widget, size );
  this->size = size;
}

SbVec2s   
SoXtComponent::getSize(
  void )
{
  return this->size;
}

Display *
SoXtComponent::getDisplay(
  void )
{
}

void      
SoXtComponent::setTitle(
  const char * title )
{
}

const char *
SoXtComponent::getTitle(
  void ) const
{
}

void      
SoXtComponent::setIconTitle(
  const char * title )
{
}

const char * 
SoXtComponent::getIconTitle(
  void ) const
{
}

void      
SoXtComponent::setWindowCloseCallback(
  SoXtComponentCB * func,
  void * data )
{
}

SoXtComponent *
SoXtComponent::getComponent( // static
  Widget widget )
{
}

const char *     
SoXtComponent::getWidgetName(
  void ) const
{
  return this->widgetName;
}

const char *
SoXtComponent::getClassName(
  void ) const
{
  return this->widgetClass;
}

void
SoXtComponent::setBaseWidget( // protected
  Widget widget )
{
  this->widget = widget;
}

void
SoXtComponent::setClassName( // protected
  const char * name )
{
}

void
SoXtComponent::windowCloseAction( // virtual, protected
  void )
{
}

void
SoXtComponent::afterRealizeHook( // virtual, protected
  void )
{
}

const char *
SoXtComponent::getDefaultWidgetName( // virtual, protected
  void ) const
{
}

const char *
SoXtComponent::getDefaultTitle( // virtual, protected
  void ) const
{
}

const char *
SoXtComponent::getDefaultIconTitle( // virtual, protected
  void ) const
{
}

void
SoXtComponent::registerWidget( // protected
  Widget widget )
{
}

void
SoXtComponent::unregisterWidget( // protected
  Widget widget )
{
}

void
SoXtComponent::addVisibilityChangeCallback( // protected
  SoXtComponentVisibilityCB * func,
  void * user )
{
}

void
SoXtComponent::removeVisibilityChangeCallback( // protected
  SoXtComponentVisibilityCB * func,
  void * user )
{
}

void
SoXtComponent::openHelpCard( // protected
  const char * name )
{
}

char *
SoXtComponent::getlabel( // static, protected
  unsigned int what )
{
}

// *************************************************************************
