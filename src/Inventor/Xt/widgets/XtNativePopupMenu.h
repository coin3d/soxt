/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

// $Id$

#ifndef SOXT_XTNATIVEPOPUPMENU_H
#define SOXT_XTNATIVEPOPUPMENU_H

#include <X11/Intrinsic.h>

#include <Inventor/SoLists.h>

#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/widgets/SoXtPopupMenu.h>

// *************************************************************************

struct MenuRecord;
struct ItemRecord;

class XtNativePopupMenu : public SoXtPopupMenu {
  typedef SoXtPopupMenu inherited;

public:
  XtNativePopupMenu(void);
  virtual ~XtNativePopupMenu(void);

  virtual int newMenu(const char * name, int menuid = -1);
  virtual int getMenu(const char * name);
  virtual void setMenuTitle(int id, const char * title);
  virtual const char * getMenuTitle(int id);

  virtual int newMenuItem(const char * name, int itemid = -1);
  virtual int getMenuItem(const char * name);
  virtual void setMenuItemTitle(int itemid, const char * title);
  virtual const char * getMenuItemTitle(int itemid);
  virtual void setMenuItemEnabled(int itemid, SbBool enabled);
  virtual SbBool getMenuItemEnabled(int itemid);
  virtual SbBool getMenuItemMarked(int itemid);

  virtual void addMenu(int menuid, int submenuid, int pos = -1);
  virtual void addMenuItem(int menuid, int itemid, int pos = -1);
  virtual void addSeparator(int menuid, int pos = -1);
  virtual void removeMenu(int menuid);
  virtual void removeMenuItem(int itemid);

  virtual void popUp(Widget inside, int x, int y);

protected:
  virtual void _setMenuItemMarked(int itemid, SbBool marked);

  MenuRecord * getMenuRecord(int menuid);
  ItemRecord * getItemRecord(int itemid);
  MenuRecord * createMenuRecord(const char * name);
  ItemRecord * createItemRecord(const char * name);

  Widget build(Widget parent);
  Widget traverseBuild(Widget parent, MenuRecord * menu, int indent);

  void itemSelection(Widget w, XtPointer call);
  static void itemSelectionCallback(Widget, XtPointer client, XtPointer call);

private:
  SbPList * menus;
  SbPList * items;
  Widget popup;
  SbBool dirty;

}; // class XtNativePopupMenu

// *************************************************************************

#endif // ! SOXT_XTNATIVEPOPUPMENU_H
