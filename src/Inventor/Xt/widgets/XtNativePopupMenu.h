/**************************************************************************\
 *
 *  This file is part of the Coin family of 3D visualization libraries.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and / or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use this library in software that is incompatible
 *  with the GNU GPL, and / or you would like to take advantage of the
 *  additional benefits with regard to our support services, please
 *  contact Systems in Motion about acquiring a Coin Professional
 *  Edition License.  See <URL:http://www.coin3d.org> for more
 *  information.
 *
 *  Systems in Motion, Abels gate 5, Teknobyen, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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
