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

//  $Id$

#ifndef SOXT_COMPONENT_H
#define SOXT_COMPONENT_H

#include <X11/Intrinsic.h>

#include <Inventor/SbBasic.h>
#include <Inventor/SbLinear.h>

#include <Inventor/Xt/SoXtBasic.h>

class SbPList;
class SoXtComponent;

// *************************************************************************

typedef void SoXtComponentCB( void * closure, SoXtComponent * component );
typedef void SoXtComponentVisibilityCB( void * closure, SbBool enable );

class SOXT_DLL_EXPORT SoXtComponent {
public:
  virtual ~SoXtComponent(void);

  virtual void show(void);
  virtual void hide(void);
  SbBool isVisible(void);

  Widget getWidget(void) const;
  Widget baseWidget(void) const;
  Widget getBaseWidget(void) const;

  SbBool isTopLevelShell(void) const;
  Widget getShellWidget(void) const;
  Widget getParentWidget(void) const;

  Display * getDisplay(void);

  void setSize( const SbVec2s size );
  SbVec2s getSize(void);
  void fitSize( const SbVec2s size );

  const char * getWidgetName(void) const;
  const char * getClassName(void) const;
  void setTitle( const char * const title );
  const char * getTitle(void) const;
  void setIconTitle( const char * const title );
  const char * getIconTitle(void) const;

  void setWindowCloseCallback(
    SoXtComponentCB * callback, void * closure = NULL );
  void addWindowCloseCallback(
    SoXtComponentCB * callback, void * closure = NULL );
  void removeWindowCloseCallback(
    SoXtComponentCB * callback, void * closure = NULL );

  static SoXtComponent * getComponent( Widget widget );

protected:
  SoXtComponent(
    const Widget parent = NULL,
    const char * const name = NULL,
    const SbBool embed = TRUE );

  void setBaseWidget( Widget widget );
  void setClassName( const char * const name );

  virtual void windowCloseAction(void);
  virtual void afterRealizeHook(void);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  void registerWidget( Widget widget );
  void unregisterWidget( Widget widget );

  void addVisibilityChangeCallback(
    SoXtComponentVisibilityCB * callback, void * closure = NULL );
  void removeVisibilityChangeCallback(
    SoXtComponentVisibilityCB * callback, void * closure = NULL );

  void invokeVisibilityChangeCallbacks( const SbBool enable ) const;
  void invokeWindowCloseCallbacks(void) const;

  void openHelpCard( const char * name );

  static char * getlabel( unsigned int what );

  SbBool firstRealize;

private:
  Widget parent;
  Widget widget;
  char * widgetName;
  char * widgetClass;
  char * title;
  char * iconTitle;

  SbBool embedded;
  Widget constructorParent;

  SbVec2s size;

  static SbPList * widgets;
  static SbPList * components;

  SbPList * close_callbacks;
  SbPList * visibility_callbacks;

}; // class SoXtComponent

// *************************************************************************

#endif // ! SOXT_COMPONENT_H
