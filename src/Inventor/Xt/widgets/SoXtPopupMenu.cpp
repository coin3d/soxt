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

#include <Inventor/SoLists.h>
#include <Inventor/errors/SoDebugError.h>

#include <Inventor/Xt/widgets/SoXtPopupMenu.h>

// *************************************************************************

struct MenuRecord {
  static MenuRecord * create( char * name );
  int menuid;
  char * name;
  char * title;
//  QPopupMenu * menu;
//  QPopupMenu * parent;
}; // struct MenuRecord

struct ItemRecord {
  static ItemRecord * create( char * name );
  int itemid;
  int flags;
  char * name;
  char * title;
//  QPopupMenu * parent;
}; // struct ItemRecord

#define ITEM_MARKED       0x0001
#define ITEM_SEPARATOR    0x0002

// *************************************************************************

/*!
  \class SoXtPopupMenu Inventor/Qt/widgets/SoXtPopupMenu.h
  \brief The SoXtPopupMenu class implements a common interface for popup
  menu management for all the Coin GUI toolkit libraries.
*/

// *************************************************************************

SoXtPopupMenu::SoXtPopupMenu(
  SoAnyPopupMenu * handler )
{
  this->handler = handler;
  this->menus = new SbPList;
  this->items = new SbPList;
} // SoXtPopupMenu()

SoXtPopupMenu::~SoXtPopupMenu(
  void )
{
  const int numMenus = this->menus->getLength();
//  QPopupMenu * popup = NULL;
  for ( int i = 0; i < numMenus; i++ ) {
    MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
//    if ( rec->menuid == 0 ) popup = rec->menu;
    delete [] rec->name;
    delete [] rec->title;
//    if ( rec->parent == NULL ) delete rec->menu; // menu not attached
    delete rec;
   }

  const int numItems = this->items->getLength();
  for ( int i = 0; i < numItems; i++ ) {
    ItemRecord * rec = (ItemRecord *) (*this->items)[i];
    delete [] rec->name;
    delete [] rec->title;
    delete rec;
  }

  // delete root popup menu
//  delete popup;
} // ~SoXtPopupMenu()

// *************************************************************************

/*!
*/

int
SoXtPopupMenu::NewMenu(
  char * name,
  int menuid )
{
  int id = menuid;
  if ( id == -1 ) {
    id = 1;
    while ( this->getMenuRecord( id ) != NULL ) id++;
  } else {
    if ( this->getMenuRecord( id ) != NULL ) {
#if SOXT_DEBUG
      SoDebugError::postInfo( "SoXtPopupMenu::NewMenu",
        "requested menuid already taken" );
#endif // SOXT_DEBUG
      return -1;
    }
  }
  // id contains ok ID
  MenuRecord * rec = MenuRecord::create( name );
  rec->menuid = id;
  this->menus->append( (void *) rec );
  return id;
} // NewMenu()

/*!
*/

int
SoXtPopupMenu::GetMenu(
  char * name )
{
  const int numMenus = this->menus->getLength();
  for ( int i = 0; i < numMenus; i++ )
    if ( strcmp( ((MenuRecord *) (*this->menus)[i])->name, name ) == 0 )
      return ((MenuRecord *) (*this->menus)[i])->menuid;
  return -1;
} // GetMenu()

/*!
*/

void
SoXtPopupMenu::SetMenuTitle(
  int menuid,
  char * title )
{
  MenuRecord * rec = this->getMenuRecord( menuid );
  if ( rec == NULL ) {
    SoDebugError::postWarning( "SoXtPopupMenu::SetMenuTitle",
      "no such menu (%d.title = \"%s\")", menuid, title );
    return;
  }
  delete [] rec->title;
  rec->title = strcpy( new char [strlen(title)+1], title );
//  if ( rec->parent )
//    rec->parent->changeItem( rec->menuid, QString( rec->title ) );
} // SetMenuTitle()

/*!
*/

char *
SoXtPopupMenu::GetMenuTitle(
  int menuid )
{
  MenuRecord * rec = this->getMenuRecord( menuid );
  if ( rec == NULL )
    return NULL;
  return rec->title;
} // GetMenuTitle()

// *************************************************************************

/*!
*/

int
SoXtPopupMenu::NewMenuItem(
  char * name,
  int itemid )
{
  int id = itemid;
  if ( id == -1 ) {
    id = 1;
    while ( this->getItemRecord( itemid ) != NULL ) id++;
  } else {
    if ( this->getItemRecord( itemid ) != NULL ) {
#if SOXT_DEBUG
      SoDebugError::postInfo( "SoXtPopupMenu::NewMenuItem",
        "requested itemid already taken" );
#endif // SOXT_DEBUG
      return -1;
    }
  }
  ItemRecord * rec = ItemRecord::create( name );
  rec->itemid = id;
  this->items->append( rec );
  return id;
} // NewMenuItem()

/*!
*/

int
SoXtPopupMenu::GetMenuItem(
  char * name )
{
  const int numItems = this->items->getLength();
  for ( int i = 0; i < numItems; i++ )
    if ( strcmp( ((ItemRecord *) (*this->items)[i])->name, name ) == 0 )
      return ((ItemRecord *) (*this->items)[i])->itemid;
  return -1;
} // GetMenuItem()

/*!
*/

void
SoXtPopupMenu::SetMenuItemTitle(
  int itemid,
  char * title )
{
  ItemRecord * rec = this->getItemRecord( itemid );
  if ( rec == NULL )
    return;
  delete [] rec->title;
  rec->title = strcpy( new char [strlen(title)+1], title );
//  if ( rec->parent )
//    rec->parent->changeItem( rec->itemid, QString( rec->title ) );
} // SetMenuItemTitle()

/*!
*/

char *
SoXtPopupMenu::GetMenuItemTitle(
  int itemid )
{
  ItemRecord * rec = this->getItemRecord( itemid );
  if ( rec == NULL ) return NULL;
  return rec->title;
} // GetMenuItemTitle()

/*!
*/

void
SoXtPopupMenu::SetMenuItemEnabled(
  int itemid,
  SbBool enabled )
{
  ItemRecord * rec = this->getItemRecord( itemid );
  if ( rec == NULL )
    return;
//  rec->parent->setItemEnabled( rec->itemid, enabled ? true : false );
} // SetMenuItemEnabled()

/*!
*/

SbBool
SoXtPopupMenu::GetMenuItemEnabled(
  int itemid )
{
  ItemRecord * rec = this->getItemRecord( itemid );
  if ( rec == NULL )
    return FALSE;
//  return rec->parent->isItemEnabled( rec->itemid ) ? TRUE : FALSE;
  return TRUE;
} // GetMenuItemEnabled()

/*!
*/

void
SoXtPopupMenu::SetMenuItemMarked(
  int itemid,
  SbBool marked )
{
  ItemRecord * rec = this->getItemRecord( itemid );
  
  if ( rec == NULL )
    return;
  if ( marked )
    rec->flags |= ITEM_MARKED;
  else
    rec->flags &= ~ITEM_MARKED;
//  if ( rec->parent == NULL )
//    return;
//  rec->parent->setItemChecked( rec->itemid, marked ? true : false );
} // SetMenuItemMarked()

/*!
*/

SbBool
SoXtPopupMenu::GetMenuItemMarked(
  int itemid )
{
  ItemRecord * rec = this->getItemRecord( itemid );
  if ( rec == NULL )
    return FALSE;
//  if ( rec->parent == NULL )
//    return (rec->flags & ITEM_MARKED) ? TRUE : FALSE;
//  return rec->parent->isItemChecked( rec->itemid ) ? TRUE : FALSE;
  return FALSE;
} // GetMenuItemMarked()

// *************************************************************************

/*!
*/

void
SoXtPopupMenu::AddMenu(
  int menuid,
  int submenuid,
  int pos )
{
  MenuRecord * super = this->getMenuRecord( menuid );
  MenuRecord * sub = this->getMenuRecord( submenuid );
  if ( super == NULL || sub == NULL ) {
#if SOXT_DEBUG
    SoDebugError::postInfo( "SoXtPopupMenu::AddMenu",
      "no such menu (super = 0x%08x, sub = 0x%08x)", super, sub );
#endif // SOXT_DEBUG
    return;
  }
//  if ( pos == -1 )
//    super->menu->insertItem( QString( sub->title ), sub->menu, sub->menuid );
//  else
//    super->menu->insertItem( QString( sub->title ),
//                             sub->menu, sub->menuid, pos );
//  sub->parent = super->menu;
} // AddMenu()

/*!
*/

void
SoXtPopupMenu::AddMenuItem(
  int menuid,
  int itemid,
  int pos )
{
  MenuRecord * menu = this->getMenuRecord( menuid );
  ItemRecord * item = this->getItemRecord( itemid );
  if ( menu == NULL || item == NULL ) {
#if SOXT_DEBUG
    SoDebugError::postInfo( "SoXtPopupMenu::AddMenuItem",
      "no such item (menu = 0x%08x, item = 0x%08x)", menu, item );
#endif // SOXT_DEBUG
    return;
  }
//  if ( pos == -1 )
//    menu->menu->insertItem( QString( item->title ), item->itemid );
//  else
//    menu->menu->insertItem( QString( item->title ), item->itemid, pos );
//  item->parent = menu->menu;
//  if ( item->flags & ITEM_MARKED )
//    item->parent->setItemChecked( item->itemid, true );
} // AddMenuItem()

void
SoXtPopupMenu::AddSeparator(
  int menuid,
  int pos )
{
  MenuRecord * menu = this->getMenuRecord( menuid );
  if ( menu == NULL ) {
    SoDebugError::postWarning( "SoXtPopupMenu::AddSeparator",
      "no such menu (%d)", menuid );
    return;
  }
  ItemRecord * rec = ItemRecord::create( "separator" );
//  menu->menu->insertSeparator( pos );
  rec->flags |= ITEM_SEPARATOR;
  this->items->append( rec );
} // AddSeparator()

/*!
  This method removes the submenu with the given \a menuid.

  A removed menu can be attached again later - its menuid will still be
  allocated.
*/

void
SoXtPopupMenu::RemoveMenu(
  int menuid )
{
  MenuRecord * rec = this->getMenuRecord( menuid );
  if ( rec == NULL ) {
#if SOXT_DEBUG
    SoDebugError::postInfo( "SoXtPopupMenu::RemoveMenu", "no such menu" );
#endif // SOXT_DEBUG
    return;
  }
  if ( rec->menuid == 0 ) {
#if SOXT_DEBUG
    SoDebugError::postInfo( "SoXtPopupMenu::RemoveMenu", "can't remove root" );
#endif // SOXT_DEBUG
    return;
  }
//  if ( rec->parent == NULL ) {
//#if SOXT_DEBUG
//    SoDebugError::postInfo( "SoXtPopupMenu::RemoveMenu", "menu not attached" );
//#endif // SOXT_DEBUG
//    return;
//  }
//  rec->parent->removeItem( rec->menuid );
//  rec->parent = NULL;
} // RemoveMenu()

/*!
  This method removes the menu item with the given \a itemid.

  A removed menu item can be attached again later - its itemid will still
  be allocated.
*/

void
SoXtPopupMenu::RemoveMenuItem(
  int itemid )
{
  ItemRecord * rec = this->getItemRecord( itemid );
  if ( rec == NULL ) {
#if SOXT_DEBUG
    SoDebugError::postInfo( "SoXtPopupMenu::RemoveMenu", "no such item" );
#endif // SOXT_DEBUG
    return;
  }
//  if ( rec->parent == NULL ) {
//#if SOXT_DEBUG
//    SoDebugError::postInfo( "SoXtPopupMenu::RemoveMenu", "item not attached" );
//#endif // SOXT_DEBUG
//    return;
//  }
//  rec->parent->removeItem( rec->itemid );
//  rec->parent = NULL;
} // RemoveMenuItem()

// *************************************************************************

/*!
  This method invokes the popup menu.

  If -1 is returned, no menu item was selected.  The itemid of the selected
  item is returned otherwise.
*/

int
SoXtPopupMenu::PopUp(
  int screenx,
  int screeny )
{
  MenuRecord * rec = this->getMenuRecord( 0 );
//  return rec->menu->exec( QPoint( screenx, screeny ) );
  return -1;
} // PopUp()

// *************************************************************************

/*!
*/

MenuRecord *
SoXtPopupMenu::getMenuRecord(
  int menuid )
{
  const int numMenus = this->menus->getLength();
  for ( int i = 0; i < numMenus; i++ )
    if ( ((MenuRecord *) (*this->menus)[i])->menuid == menuid )
      return (MenuRecord *) (*this->menus)[i];
  return (MenuRecord *) NULL;
} // getMenuRecord()

/*!
*/

ItemRecord *
SoXtPopupMenu::getItemRecord(
  int itemid )
{
  const int numItems = this->items->getLength();
  for ( int i = 0; i < numItems; i++ )
    if ( ((ItemRecord *) (*this->items)[i])->itemid == itemid )
      return (ItemRecord *) (*this->items)[i];
  return (ItemRecord *) NULL;
} // getItemRecord()

// *************************************************************************

/*!
*/

MenuRecord *
MenuRecord::create(
  char * name )
{
  MenuRecord * rec = new MenuRecord;
  rec->menuid = -1;
  rec->name = strcpy( new char [strlen(name)+1], name );
  rec->title = strcpy( new char [strlen(name)+1], name );
//  rec->menu = new QPopupMenu( (QWidget *) NULL, name );
//  rec->parent = NULL;
  return rec;
} // create()

/*!
*/

ItemRecord *
ItemRecord::create(
  char * name )
{
  ItemRecord * rec = new ItemRecord;
  rec->itemid = -1;
  rec->flags = 0;
  rec->name = strcpy( new char [strlen(name)+1], name );
  rec->title = strcpy( new char [strlen(name)+1], name );
//  rec->parent = NULL;
  return rec;
} // create()

// *************************************************************************
