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

// $Id$

#ifndef SOXT_COMPONENT_H
#define SOXT_COMPONENT_H

#include <X11/Intrinsic.h>

#include <Inventor/SbBasic.h>
#include <Inventor/SbLinear.h>

#include <Inventor/Xt/SoXtObject.h>

class SbPList;
class SoXtComponent;

// *************************************************************************

typedef void SoXtComponentCB(void * closure, SoXtComponent * component);
typedef void SoXtComponentVisibilityCB(void * closure, SbBool enable);

class SOXT_DLL_API SoXtComponent : public SoXtObject {
  SOXT_OBJECT_ABSTRACT_HEADER(SoXtComponent, SoXtObject);

public:
  virtual ~SoXtComponent(void);

  virtual void show(void);
  virtual void hide(void);

  SbBool isFullScreen(void) const;
  SbBool setFullScreen(const SbBool onoff);

  SbBool isVisible(void);
  Widget getWidget(void) const;
  Widget baseWidget(void) const;
  Widget getBaseWidget(void) const;

  SbBool isTopLevelShell(void) const;
  Widget getShellWidget(void) const;
  Widget getParentWidget(void) const;

  Display * getDisplay(void);

  void setSize(const SbVec2s size);
  SbVec2s getSize(void);
  void fitSize(const SbVec2s size);
  virtual void sizeChanged(const SbVec2s size);

  const char * getWidgetName(void) const;
  const char * getClassName(void) const;
  void setTitle(const char * const title);
  const char * getTitle(void) const;
  void setIconTitle(const char * const title);
  const char * getIconTitle(void) const;

  void setWindowCloseCallback(
    SoXtComponentCB * callback, void * closure = NULL);
  void addWindowCloseCallback(
    SoXtComponentCB * callback, void * closure = NULL);
  void removeWindowCloseCallback(
    SoXtComponentCB * callback, void * closure = NULL);

  static SoXtComponent * getComponent(Widget widget);

  static void initClasses(void);

protected:
  SoXtComponent(
    const Widget parent = NULL,
    const char * const name = NULL,
    const SbBool embed = TRUE);

  void setBaseWidget(Widget widget);
  void setClassName(const char * const name);

  virtual void windowCloseAction(void);
  virtual void afterRealizeHook(void);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  void registerWidget(Widget widget);
  void unregisterWidget(Widget widget);

  void addVisibilityChangeCallback(
    SoXtComponentVisibilityCB * callback, void * closure = NULL);
  void removeVisibilityChangeCallback(
    SoXtComponentVisibilityCB * callback, void * closure = NULL);

  void invokeVisibilityChangeCallbacks(const SbBool enable) const;
  void invokeWindowCloseCallbacks(void) const;

  void openHelpCard(const char * name);

  static char * getlabel(unsigned int what);

  SbBool firstRealize;

  virtual Boolean eventHandler(Widget widget, XEvent * event);

private:
  class SoXtComponentP * pimpl;

  static void event_handler(Widget, XtPointer, XEvent *, Boolean *);

}; // class SoXtComponent

// *************************************************************************

#endif // ! SOXT_COMPONENT_H
