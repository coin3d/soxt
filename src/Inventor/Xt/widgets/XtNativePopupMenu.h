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

#ifndef SOXT_XTNATIVEPOPUPMENU_H
#define SOXT_XTNATIVEPOPUPMENU_H

#include <X11/Intrinsic.h>

#include <Inventor/SoLists.h>

#include <Inventor/Xt/SoXtBasic.h>
#include <Inventor/Xt/widgets/SoAnyPopupMenu.h>

// *************************************************************************

struct MenuRecord;
struct ItemRecord;

class XtNativePopupMenu : public SoAnyPopupMenu {
  typedef SoAnyPopupMenu inherited;

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
  Widget traverseBuild(Widget parent, MenuRecord *  menu, int indent);

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
