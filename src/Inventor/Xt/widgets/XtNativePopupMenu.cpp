/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#include <assert.h>
#include <stdio.h>

#include <Xm/RowColumn.h>
#include <Xm/SeparatoG.h>
#include <Xm/PushBG.h>
#include <Xm/ToggleBG.h>
#include <Xm/ToggleB.h>
#include <Xm/CascadeBG.h>

#include <Inventor/SoLists.h>
#include <Inventor/errors/SoDebugError.h>

#include <soxtdefs.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/widgets/XtNativePopupMenu.h>

#define SOXT_POPUPMENU_DEBUG 0

// *************************************************************************

struct MenuRecord {
  int menuid;
  int pos;
  char * name;
  char * title;
  Widget menu;
  MenuRecord * parent;
}; // struct MenuRecord

struct ItemRecord {
  int itemid;
  int flags;
  int pos;
  char * name;
  char * title;
  Widget item;
  MenuRecord * parent;
}; // struct ItemRecord

#define ITEM_MARKED       0x0001
#define ITEM_SEPARATOR    0x0002
#define ITEM_ENABLED      0x0004

// *************************************************************************

/*!
  \class XtNativePopupMenu Inventor/Qt/widgets/XtNativePopupMenu.h
  \brief The XtNativePopupMenu class implements a common interface for popup
  menu management for all the Coin GUI toolkit libraries.
*/

// *************************************************************************

XtNativePopupMenu::XtNativePopupMenu(
  void)
{
  this->menus = new SbPList;
  this->items = new SbPList;
  this->dirty = TRUE;
  this->popup = (Widget) NULL;
} // XtNativePopupMenu()

XtNativePopupMenu::~XtNativePopupMenu(// virtual
  void)
{
  const int numMenus = this->menus->getLength();
//  QPopupMenu * popup = NULL;
  int i;
  for (i = 0; i < numMenus; i++) {
    MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
//    if (rec->menuid == 0) popup = rec->menu;
    delete [] rec->name;
    delete [] rec->title;
//    if (rec->parent == NULL) delete rec->menu; // menu not attached
    delete rec;
  }

  const int numItems = this->items->getLength();
  for (i = 0; i < numItems; i++) {
    ItemRecord * rec = (ItemRecord *) (*this->items)[i];
    delete [] rec->name;
    delete [] rec->title;
    delete rec;
  }

  // delete root popup menu
//  delete popup;
} // ~XtNativePopupMenu()

// *************************************************************************

/*!
*/

int
XtNativePopupMenu::newMenu(
  const char * name,
  int menuid)
{
  int id = menuid;
  if (id == -1) {
    id = 1;
    while (this->getMenuRecord(id) != NULL) id++;
  } else {
    if (this->getMenuRecord(id) != NULL) {
#if SOXT_DEBUG
      SoDebugError::postInfo("XtNativePopupMenu::NewMenu",
        "requested menuid already taken");
#endif // SOXT_DEBUG
      return -1;
    }
  }
  // id contains ok ID
  MenuRecord * rec = this->createMenuRecord(name);
  rec->menuid = id;
  this->menus->append((void *) rec);
  return id;
} // newMenu()

/*!
*/

int
XtNativePopupMenu::getMenu(
  const char * name)
{
  const int numMenus = this->menus->getLength();
  int i;
  for (i = 0; i < numMenus; i++)
    if (strcmp(((MenuRecord *) (*this->menus)[i])->name, name) == 0)
      return ((MenuRecord *) (*this->menus)[i])->menuid;
  return -1;
} // getMenu()

/*!
*/

void
XtNativePopupMenu::setMenuTitle(
  int menuid,
  const char * title)
{
  MenuRecord * rec = this->getMenuRecord(menuid);
  if (rec == NULL) {
    SoDebugError::postWarning("XtNativePopupMenu::SetMenuTitle",
      "no such menu (%d.title = \"%s\")", menuid, title);
    return;
  }
  delete [] rec->title;
  rec->title = strcpy(new char [strlen(title)+1], title);
//  if (rec->parent)
//    rec->parent->changeItem(rec->menuid, QString(rec->title));
} // setMenuTitle()

/*!
*/

const char *
XtNativePopupMenu::getMenuTitle(
  int menuid)
{
  MenuRecord * rec = this->getMenuRecord(menuid);
  if (rec == NULL)
    return NULL;
  return rec->title;
} // getMenuTitle()

// *************************************************************************

/*!
*/

int
XtNativePopupMenu::newMenuItem(
  const char * name,
  int itemid)
{
  // FIXME: this function is the same in the other So-libraries --
  // move to common abstraction layer SoGuiPopupMenu of
  // possible. 20031012 mortene.

  int id = itemid;
  if (id == -1) {
    id = 1;
    while (this->getItemRecord(id) != NULL) id++;
  } else {
    if (this->getItemRecord(itemid) != NULL) {
#if SOXT_DEBUG
      SoDebugError::postInfo("XtNativePopupMenu::NewMenuItem",
        "requested itemid already taken");
#endif // SOXT_DEBUG
      return -1;
    }
  }
  ItemRecord * rec = this->createItemRecord(name);
  rec->itemid = id;
  this->items->append(rec);
  return id;
} // newMenuItem()

/*!
*/

int
XtNativePopupMenu::getMenuItem(
  const char * name)
{
  const int numItems = this->items->getLength();
  int i;
  for (i = 0; i < numItems; i++)
    if (strcmp(((ItemRecord *) (*this->items)[i])->name, name) == 0)
      return ((ItemRecord *) (*this->items)[i])->itemid;
  return -1;
} // getMenuItem()

/*!
*/

void
XtNativePopupMenu::setMenuItemTitle(
  int itemid,
  const char * title)
{
  ItemRecord * rec = this->getItemRecord(itemid);
  if (rec == NULL)
    return;
  delete [] rec->title;
  rec->title = strcpy(new char [strlen(title)+1], title);
//  if (rec->parent)
//    rec->parent->changeItem(rec->itemid, QString(rec->title));
} // setMenuItemTitle()

/*!
*/

const char *
XtNativePopupMenu::getMenuItemTitle(
  int itemid)
{
  ItemRecord * rec = this->getItemRecord(itemid);
  if (rec == NULL) return NULL;
  return rec->title;
} // getMenuItemTitle()

/*!
*/

void
XtNativePopupMenu::setMenuItemEnabled(// virtual
  int itemid,
  SbBool enabled)
{
  ItemRecord * rec = this->getItemRecord(itemid);
  if (rec == NULL) {
#if SOXT_DEBUG
    SoDebugError::postInfo("XtNativePopupMenu::SetMenuItemEnabled",
      "no such menu item");
#endif // SOXT_DEBUG
    return;
  }
  if (enabled)
    rec->flags |= ITEM_ENABLED;
  else
    rec->flags &= ~ITEM_ENABLED;
  if (rec->item != (Widget) NULL)
    XtVaSetValues(rec->item, XmNsensitive, enabled ? True : False, NULL);
} // setMenuItemEnabled()

/*!
*/

SbBool
XtNativePopupMenu::getMenuItemEnabled(
  int itemid)
{
  ItemRecord * rec = this->getItemRecord(itemid);
  if (rec == NULL)
    return FALSE;
  return (rec->flags & ITEM_ENABLED) ? TRUE : FALSE;
} // getMenuItemEnabled()

/*!
*/

void
XtNativePopupMenu::_setMenuItemMarked(int itemid, SbBool marked)
{
  ItemRecord * rec = this->getItemRecord(itemid);
  if (rec == NULL)
    return;
  if (marked)
    rec->flags |= ITEM_MARKED;
  else
    rec->flags &= ~ITEM_MARKED;

  if (rec->item != NULL)
    XmToggleButtonSetState(rec->item, marked ? True : False, False);
}

/*!
*/

SbBool
XtNativePopupMenu::getMenuItemMarked(
  int itemid)
{
  ItemRecord * rec = this->getItemRecord(itemid);
  if (rec == NULL)
    return FALSE;
  return (rec->flags & ITEM_MARKED) ? TRUE : FALSE;
} // getMenuItemMarked()

// *************************************************************************

/*!
*/

void
XtNativePopupMenu::addMenu(
  int menuid,
  int submenuid,
  int pos)
{
  MenuRecord * super = this->getMenuRecord(menuid);
  MenuRecord * sub = this->getMenuRecord(submenuid);
  if (super == NULL || sub == NULL) {
#if SOXT_DEBUG
    SoDebugError::postInfo("XtNativePopupMenu::AddMenu",
      "no such menu (super = 0x%08x, sub = 0x%08x)", super, sub);
#endif // SOXT_DEBUG
    return;
  }
  if (pos == -1) {
    int max = 0;
    int i;
    const int numItems = this->items->getLength();
    for (i = 0; i < numItems; i++) {
      ItemRecord * rec = (ItemRecord *) (*this->items)[i];
      if (rec->parent == super) {
        if (rec->pos >= max)
          max = rec->pos + 1;
      }
    } 
    const int numMenus = this->menus->getLength();
    for (i = 0; i < numMenus; i++) {
      MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
      if (rec->parent == super) {
        if (rec->pos >= max)
          max = rec->pos + 1;
      }
    } 
    sub->pos = max;
    sub->parent = super;
  } else {
    int i;
    const int numItems = this->items->getLength();
    for (i = 0; i < numItems; i++) {
      ItemRecord * rec = (ItemRecord *) (*this->items)[i];
      if (rec->parent == super) {
        if (rec->pos >= pos)
          rec->pos = rec->pos + 1;
      }
    } 
    const int numMenus = this->menus->getLength();
    for (i = 0; i < numMenus; i++) {
      MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
      if (rec->parent == super) {
        if (rec->pos >= pos)
          rec->pos = rec->pos + 1;
      }
    } 
    sub->pos = pos;
    sub->parent = super;
  }
} // addMenu()

/*!
*/

void
XtNativePopupMenu::addMenuItem(
  int menuid,
  int itemid,
  int pos)
{
  MenuRecord * menu = this->getMenuRecord(menuid);
  ItemRecord * item = this->getItemRecord(itemid);
  if (menu == NULL || item == NULL) {
#if SOXT_DEBUG
    SoDebugError::postInfo("XtNativePopupMenu::AddMenuItem",
      "no such item (menu = 0x%08x, item = 0x%08x)", menu, item);
#endif // SOXT_DEBUG
    return;
  }
  if (pos == -1) {
    int max = 0;
    int i;
    const int numItems = this->items->getLength();
    for (i = 0; i < numItems; i++) {
      ItemRecord * rec = (ItemRecord *) (*this->items)[i];
      if (rec->parent == menu) {
        if (rec->pos >= max)
          max = rec->pos + 1;
      }
    } 
    const int numMenus = this->menus->getLength();
    for (i = 0; i < numMenus; i++) {
      MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
      if (rec->parent == menu) {
        if (rec->pos >= max)
          max = rec->pos + 1;
      }
    } 
    item->pos = max;
    item->parent = menu;
  } else {
    int i;
    const int numItems = this->items->getLength();
    for (i = 0; i < numItems; i++) {
      ItemRecord * rec = (ItemRecord *) (*this->items)[i];
      if (rec->parent == menu) {
        if (rec->pos >= pos)
          rec->pos = rec->pos + 1;
      }
    } 
    const int numMenus = this->menus->getLength();
    for (i = 0; i < numMenus; i++) {
      MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
      if (rec->parent == menu) {
        if (rec->pos >= pos)
          rec->pos = rec->pos + 1;
      }
    } 
    item->pos = pos;
    item->parent = menu;
  }
} // addMenuItem()

void
XtNativePopupMenu::addSeparator(
  int menuid,
  int pos)
{
  MenuRecord * menu = this->getMenuRecord(menuid);
  if (menu == NULL) {
    SoDebugError::postWarning("XtNativePopupMenu::AddSeparator",
      "no such menu (%d)", menuid);
    return;
  }
  ItemRecord * sep = this->createItemRecord("separator");
  sep->flags |= ITEM_SEPARATOR;
  if (pos == -1) {
    int max = 0;
    int i;
    const int numItems = this->items->getLength();
    for (i = 0; i < numItems; i++) {
      ItemRecord * rec = (ItemRecord *) (*this->items)[i];
      if (rec->parent == menu) {
        if (rec->pos >= max)
          max = rec->pos + 1;
      }
    } 
    const int numMenus = this->menus->getLength();
    for (i = 0; i < numMenus; i++) {
      MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
      if (rec->parent == menu) {
        if (rec->pos >= max)
          max = rec->pos + 1;
      }
    } 
    sep->pos = max;
    sep->parent = menu;
  } else {
    int i;
    const int numItems = this->items->getLength();
    for (i = 0; i < numItems; i++) {
      ItemRecord * rec = (ItemRecord *) (*this->items)[i];
      if (rec->parent == menu) {
        if (rec->pos >= pos)
          rec->pos = rec->pos + 1;
      }
    } 
    const int numMenus = this->menus->getLength();
    for (i = 0; i < numMenus; i++) {
      MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
      if (rec->parent == menu) {
        if (rec->pos >= pos)
          rec->pos = rec->pos + 1;
      }
    } 
    sep->pos = pos;
    sep->parent = menu;
  }
  this->items->append(sep);
} // addSeparator()

/*!
  This method removes the submenu with the given \a menuid.

  A removed menu can be attached again later - its menuid will still be
  allocated.
*/

void
XtNativePopupMenu::removeMenu(
  int menuid)
{
  MenuRecord * rec = this->getMenuRecord(menuid);
  if (rec == NULL) {
#if SOXT_DEBUG
    SoDebugError::postInfo("XtNativePopupMenu::RemoveMenu", "no such menu");
#endif // SOXT_DEBUG
    return;
  }
  if (rec->menuid == 0) {
#if SOXT_DEBUG
    SoDebugError::postInfo("XtNativePopupMenu::RemoveMenu", "can't remove root");
#endif // SOXT_DEBUG
    return;
  }
  // if (rec->parent == NULL) {
// #if SOXT_DEBUG
  //   SoDebugError::postInfo("XtNativePopupMenu::RemoveMenu", "menu not attached");
// #endif // SOXT_DEBUG
  //   return;
  // }
  // rec->parent->removeItem(rec->menuid);
  // rec->parent = NULL;
} // removeMenu()

/*!
  This method removes the menu item with the given \a itemid.

  A removed menu item can be attached again later - its itemid will still
  be allocated.
*/

void
XtNativePopupMenu::removeMenuItem(
  int itemid)
{
  ItemRecord * rec = this->getItemRecord(itemid);
  if (rec == NULL) {
#if SOXT_DEBUG
    SoDebugError::postInfo("XtNativePopupMenu::RemoveMenu", "no such item");
#endif // SOXT_DEBUG
    return;
  }
  int idx = this->items->find(rec);
  assert(idx != -1);
  this->items->removeFast(idx);
  delete [] rec->name;
  delete [] rec->title;
  delete rec;
  this->dirty = TRUE;
} // removeMenuItem()

// *************************************************************************

// Doc in superclass.
void
XtNativePopupMenu::popUp(Widget inside, int x, int y)
{
  assert(inside != NULL);

  MenuRecord * root = this->getMenuRecord(0);
  if (root == NULL) {
#if SOXT_DEBUG
    SoDebugError::postInfo("XtNativePopupMenu::PopUp", "no root menu");
#endif // SOXT_DEBUG
    return;
  }
  // FIXME: build menu
  if (this->dirty) {
    if (this->popup != (Widget) NULL) {
      // FIXME: destroy existing menu widget hierarchy
    }
    this->popup = this->build(inside);
  }
  this->dirty = FALSE;

  // Find global mouse pointer coordinates.
  Display * display = XtDisplay(inside);
  XButtonEvent pos;
  Window dummyarg;
  Bool b = XTranslateCoordinates(display,
                                 XtWindow(inside),
                                 DefaultRootWindow(display),
                                 x, y, &pos.x_root, &pos.y_root, &dummyarg);
  assert(b == True); // or we've got a bug

  XmMenuPosition(this->popup, &pos);
  XtManageChild(this->popup);
} // popUp()

// *************************************************************************

/*!
*/

Widget
XtNativePopupMenu::traverseBuild(
  Widget parent,
  MenuRecord * menu,
  int indent)
{
  assert(indent < 24);
  int i;
#if SOXT_POPUPMENU_DEBUG
  char pre[24];
  for (i = 0; i < indent; i++) pre[i] = ' ';
  pre[i] = '\0';
#endif // SOXT_POPUPMENU_DEBUG
  int j = 0;
  MenuRecord * sub;
  ItemRecord * item;
  do {
    sub = (MenuRecord *) NULL;
    item = (ItemRecord *) NULL;
    const int numMenus = this->menus->getLength();
    for (i = 0; i < numMenus; i++) {
      sub = (MenuRecord *) (*this->menus)[i];
      if ((sub->pos == j) && (sub->parent == menu)) {
#if SOXT_POPUPMENU_DEBUG
        fprintf(stderr, "%s%s {\n", pre, sub->name);
#endif // SOXT_POPUPMENU_DEBUG
        Display * dpy = SoXt::getDisplay();
        const int screen = DefaultScreen(dpy);
        Visual * visual = DefaultVisual(dpy, screen);
        Colormap colormap = DefaultColormap(dpy, screen);
        int depth = DefaultDepth(dpy, screen);
        Arg args[10];
        int argc = 0;
        XtSetArg(args[argc], XmNvisual, visual); argc++;
        XtSetArg(args[argc], XmNdepth, depth); argc++;
        XtSetArg(args[argc], XmNcolormap, colormap); argc++;
        Widget submenu = XmCreatePulldownMenu(parent, sub->name, args, argc);
        sub->menu = XtVaCreateManagedWidget(sub->name,
          xmCascadeButtonGadgetClass, parent,
          XmNsubMenuId, submenu,
          XtVaTypedArg,
            XmNlabelString, XmRString,
            sub->title, strlen(sub->title) + 1,
          NULL);
        this->traverseBuild(submenu, sub, indent + 2);
#if SOXT_POPUPMENU_DEBUG
        fprintf(stderr, "%s}\n", pre);
#endif // SOXT_POPUPMENU_DEBUG
        break;
      } else {
        sub = (MenuRecord *) NULL;
      }
    }
    if (sub == NULL) {
      const int numItems = this->items->getLength();
      for (i = 0; i < numItems; i++) {
        item = (ItemRecord *) (*this->items)[i];
        if ((item->pos == j) && (item->parent == menu)) {
#if SOXT_POPUPMENU_DEBUG
          fprintf(stderr, "%s%s\n", pre, item->name);
#endif // SOXT_POPUPMENU_DEBUG
          if (item->flags & ITEM_SEPARATOR) {
            item->item = XtVaCreateManagedWidget(item->title,
              xmSeparatorGadgetClass, parent, NULL);
          } else {
            item->item = XtVaCreateManagedWidget(item->title,
              xmToggleButtonGadgetClass, parent,
              XmNsensitive, (item->flags & ITEM_ENABLED) ? True : False,
              XtVaTypedArg,
                XmNlabelString, XmRString,
                item->title, strlen(item->title)+1,
              NULL);
            XtAddCallback(item->item, XmNvalueChangedCallback,
                XtNativePopupMenu::itemSelectionCallback, this);
            XmToggleButtonSetState(item->item,
              (item->flags & ITEM_MARKED) ? True : False,
              False);
          }
          break;
        } else {
          item = (ItemRecord *) NULL;
        }
      }
    }
    j++;
  } while ((sub != NULL) || (item != NULL));
  return parent;
} // traverseBuild()

/*!
*/

Widget
XtNativePopupMenu::build(
  Widget parent)
{
  MenuRecord * root = this->getMenuRecord(0);
  assert(root != NULL);

#if SOXT_POPUPMENU_DEBUG
  setbuf(stderr, NULL);
  fprintf(stderr, "building popup menu\n");
#endif // SOXT_POPUPMENU_DEBUG

  Display * dpy = SoXt::getDisplay();
  const int screen = DefaultScreen(dpy);
  Visual * visual = DefaultVisual(dpy, screen);
  Colormap colormap = DefaultColormap(dpy, screen);
  int depth = DefaultDepth(dpy, screen);

  Arg args[10];
  int argc = 0;
  XtSetArg(args[argc], XmNvisual, visual); argc++;
  XtSetArg(args[argc], XmNdepth, depth); argc++;
  XtSetArg(args[argc], XmNcolormap, colormap); argc++;
  Widget popup = XmCreatePopupMenu(parent, root->name, args, argc);

#if SOXT_POPUPMENU_DEBUG
  fprintf(stderr, "%s {\n", root->name);
#endif // SOXT_POPUPMENU_DEBUG
  (void) this->traverseBuild(popup, root, 2);
#if SOXT_POPUPMENU_DEBUG
  fprintf(stderr, "}\n");
#endif // SOXT_POPUPMENU_DEBUG
  return popup;
} // build()

// *************************************************************************

/*!
*/

MenuRecord *
XtNativePopupMenu::getMenuRecord(
  int menuid)
{
  const int numMenus = this->menus->getLength();
  int i;
  for (i = 0; i < numMenus; i++)
    if (((MenuRecord *) (*this->menus)[i])->menuid == menuid)
      return (MenuRecord *) (*this->menus)[i];
  return (MenuRecord *) NULL;
} // getMenuRecord()

/*!
*/

ItemRecord *
XtNativePopupMenu::getItemRecord(
  int itemid)
{
  const int numItems = this->items->getLength();
  int i;
  for (i = 0; i < numItems; i++)
    if (((ItemRecord *) (*this->items)[i])->itemid == itemid)
      return (ItemRecord *) (*this->items)[i];
  return (ItemRecord *) NULL;
} // getItemRecord()

// *************************************************************************

/*!
*/

MenuRecord *
XtNativePopupMenu::createMenuRecord(
  const char * name)
{
  MenuRecord * rec = new MenuRecord;
  rec->menuid = -1;
  rec->pos = -1;
  rec->name = strcpy(new char [strlen(name)+1], name);
  rec->title = strcpy(new char [strlen(name)+1], name);
  rec->menu = (Widget) NULL;
  rec->parent = NULL;
  return rec;
} // create()

/*!
*/

ItemRecord *
XtNativePopupMenu::createItemRecord(
  const char * name)
{
  ItemRecord * rec = new ItemRecord;
  rec->itemid = -1;
  rec->pos = -1;
  rec->flags = 0 | ITEM_ENABLED;
  rec->name = strcpy(new char [strlen(name)+1], name);
  rec->title = strcpy(new char [strlen(name)+1], name);
  rec->item = (Widget) NULL;
  rec->parent = NULL;
  return rec;
} // create()

// *************************************************************************

/*!
*/

void
XtNativePopupMenu::itemSelection(// private
  Widget w,
  XtPointer call)
{
  if (w == NULL)
    return;
  XmToggleButtonCallbackStruct * data = (XmToggleButtonCallbackStruct *) call;
  const int numItems = this->items->getLength();
  int i;
  for (i = 0; i < numItems; i++) {
    ItemRecord * rec = (ItemRecord *) (*this->items)[i];
    if (rec->item == w) {
      int groupid = this->getRadioGroup(rec->itemid);
      if (data->set && groupid != -1) {
        this->setMenuItemMarked(rec->itemid, TRUE);
        this->invokeMenuSelection(rec->itemid);
      } else {
        if (groupid == -1) {
          this->setMenuItemMarked(rec->itemid, FALSE);
          this->invokeMenuSelection(rec->itemid);
        } else if (this->getRadioGroupSize(groupid) > 1) {
          this->setMenuItemMarked(rec->itemid, TRUE);
          this->invokeMenuSelection(rec->itemid);
        } else {
          this->setMenuItemMarked(rec->itemid, FALSE);
          this->invokeMenuSelection(rec->itemid);
        }
      }
    }
  }
} // itemSelection()

/*!
*/

void
XtNativePopupMenu::itemSelectionCallback(// private, static
  Widget w,
  XtPointer client_data,
  XtPointer call_data)
{
  assert(client_data != NULL);
  XtNativePopupMenu * popup = (XtNativePopupMenu *) client_data;
  popup->itemSelection(w, call_data);
} // itemSelectionCallback()

// *************************************************************************

