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

#include <Inventor/Xt/widgets/SoXtPopupMenu.h>

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
  \class SoXtPopupMenu Inventor/Qt/widgets/SoXtPopupMenu.h
  \brief The SoXtPopupMenu class implements a common interface for popup
  menu management for all the Coin GUI toolkit libraries.
*/

// *************************************************************************

SoXtPopupMenu::SoXtPopupMenu(
  void )
{
  this->menus = new SbPList;
  this->items = new SbPList;
  this->dirty = TRUE;
  this->popup = (Widget) NULL;
} // SoXtPopupMenu()

SoXtPopupMenu::~SoXtPopupMenu( // virtual
  void )
{
  const int numMenus = this->menus->getLength();
//  QPopupMenu * popup = NULL;
  int i;
  for ( i = 0; i < numMenus; i++ ) {
    MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
//    if ( rec->menuid == 0 ) popup = rec->menu;
    delete [] rec->name;
    delete [] rec->title;
//    if ( rec->parent == NULL ) delete rec->menu; // menu not attached
    delete rec;
  }

  const int numItems = this->items->getLength();
  for ( i = 0; i < numItems; i++ ) {
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
  MenuRecord * rec = this->createMenuRecord( name );
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
  int i;
  for ( i = 0; i < numMenus; i++ )
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
  ItemRecord * rec = this->createItemRecord( name );
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
  int i;
  for ( i = 0; i < numItems; i++ )
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
//  inherited::SetMenuItemEnabled( itemid, enabled );
  ItemRecord * rec = this->getItemRecord( itemid );
  if ( rec == NULL )
    return;
  rec->flags |= ITEM_ENABLED;
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
  return ( rec->flags & ITEM_ENABLED ) ? TRUE : FALSE;
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

  if ( rec->item != NULL )
    XmToggleButtonSetState( rec->item, marked ? True : False, False );

  if ( marked )
    this->SetRadioGroupMarkedItem( itemid );
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
  return (rec->flags & ITEM_MARKED) ? TRUE : FALSE;
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
  if ( pos == -1 ) {
    int max = 0;
    int i;
    const int numItems = this->items->getLength();
    for ( i = 0; i < numItems; i++ ) {
      ItemRecord * rec = (ItemRecord *) (*this->items)[i];
      if ( rec->parent == super ) {
        if ( rec->pos >= max )
          max = rec->pos + 1;
      }
    } 
    const int numMenus = this->menus->getLength();
    for ( i = 0; i < numMenus; i++ ) {
      MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
      if ( rec->parent == super ) {
        if ( rec->pos >= max )
          max = rec->pos + 1;
      }
    } 
    sub->pos = max;
    sub->parent = super;
  } else {
    int i;
    const int numItems = this->items->getLength();
    for ( i = 0; i < numItems; i++ ) {
      ItemRecord * rec = (ItemRecord *) (*this->items)[i];
      if ( rec->parent == super ) {
        if ( rec->pos >= pos )
          rec->pos = rec->pos + 1;
      }
    } 
    const int numMenus = this->menus->getLength();
    for ( i = 0; i < numMenus; i++ ) {
      MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
      if ( rec->parent == super ) {
        if ( rec->pos >= pos )
          rec->pos = rec->pos + 1;
      }
    } 
    sub->pos = pos;
    sub->parent = super;
  }
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
  if ( pos == -1 ) {
    int max = 0;
    int i;
    const int numItems = this->items->getLength();
    for ( i = 0; i < numItems; i++ ) {
      ItemRecord * rec = (ItemRecord *) (*this->items)[i];
      if ( rec->parent == menu ) {
        if ( rec->pos >= max )
          max = rec->pos + 1;
      }
    } 
    const int numMenus = this->menus->getLength();
    for ( i = 0; i < numMenus; i++ ) {
      MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
      if ( rec->parent == menu ) {
        if ( rec->pos >= max )
          max = rec->pos + 1;
      }
    } 
    item->pos = max;
    item->parent = menu;
  } else {
    int i;
    const int numItems = this->items->getLength();
    for ( i = 0; i < numItems; i++ ) {
      ItemRecord * rec = (ItemRecord *) (*this->items)[i];
      if ( rec->parent == menu ) {
        if ( rec->pos >= pos )
          rec->pos = rec->pos + 1;
      }
    } 
    const int numMenus = this->menus->getLength();
    for ( i = 0; i < numMenus; i++ ) {
      MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
      if ( rec->parent == menu ) {
        if ( rec->pos >= pos )
          rec->pos = rec->pos + 1;
      }
    } 
    item->pos = pos;
    item->parent = menu;
  }
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
  ItemRecord * sep = this->createItemRecord( "separator" );
  sep->flags |= ITEM_SEPARATOR;
  if ( pos == -1 ) {
    int max = 0;
    int i;
    const int numItems = this->items->getLength();
    for ( i = 0; i < numItems; i++ ) {
      ItemRecord * rec = (ItemRecord *) (*this->items)[i];
      if ( rec->parent == menu ) {
        if ( rec->pos >= max )
          max = rec->pos + 1;
      }
    } 
    const int numMenus = this->menus->getLength();
    for ( i = 0; i < numMenus; i++ ) {
      MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
      if ( rec->parent == menu ) {
        if ( rec->pos >= max )
          max = rec->pos + 1;
      }
    } 
    sep->pos = max;
    sep->parent = menu;
  } else {
    int i;
    const int numItems = this->items->getLength();
    for ( i = 0; i < numItems; i++ ) {
      ItemRecord * rec = (ItemRecord *) (*this->items)[i];
      if ( rec->parent == menu ) {
        if ( rec->pos >= pos )
          rec->pos = rec->pos + 1;
      }
    } 
    const int numMenus = this->menus->getLength();
    for ( i = 0; i < numMenus; i++ ) {
      MenuRecord * rec = (MenuRecord *) (*this->menus)[i];
      if ( rec->parent == menu ) {
        if ( rec->pos >= pos )
          rec->pos = rec->pos + 1;
      }
    } 
    sep->pos = pos;
    sep->parent = menu;
  }
  this->items->append( sep );
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

void
SoXtPopupMenu::PopUp(
  Widget inside,
  int x,
  int y )
{
  MenuRecord * root = this->getMenuRecord( 0 );
  if ( root == NULL ) {
#if SOXT_DEBUG
    SoDebugError::postInfo( "SoXtPopupMenu::PopUp", "no root menu" );
#endif // SOXT_DEBUG
    return;
  }
  // FIXME: build menu
  if ( this->dirty ) {
    if ( this->popup != (Widget) NULL ) {
      // destroy existing menu
    }
    this->popup = this->build( inside );
  }
  this->dirty = FALSE;
  XButtonEvent pos;
  pos.x_root = x;
  pos.y_root = y;
  XmMenuPosition( this->popup, &pos );
  XtManageChild( this->popup );
//  SoDebugError::postInfo( "SoXtPopupMenu::PopUp", "menu popped up" );
} // PopUp()

// *************************************************************************

/*!
*/

Widget
SoXtPopupMenu::traverseBuild(
  Widget parent,
  MenuRecord * menu,
  int indent )
{
  char pre[24];
  int i, j;
  for ( i = 0; i < indent; i++ ) pre[i] = ' ';
  pre[i] = '\0';
  j = 0;
  MenuRecord * sub;
  ItemRecord * item;
  do {
    sub = (MenuRecord *) NULL;
    item = (ItemRecord *) NULL;
    const int numMenus = this->menus->getLength();
    for ( i = 0; i < numMenus; i++ ) {
      sub = (MenuRecord *) (*this->menus)[i];
      if ( sub->pos == j && sub->parent == menu ) {
//        fprintf( stderr, "%s%s {\n", pre, sub->name );
        Widget submenu = XmCreatePulldownMenu( parent, sub->name, NULL, 0 );
        XmString temp = XmStringCreateLocalized( sub->title );
        sub->menu = XtVaCreateManagedWidget( sub->name,
          xmCascadeButtonGadgetClass, parent,
          XmNsubMenuId, submenu,
          XmNlabelString, temp,
          NULL );
        XmStringFree( temp );
        this->traverseBuild( submenu, sub, indent + 2 );
//        fprintf( stderr, "%s}\n", pre );
        break;
      } else {
        sub = (MenuRecord *) NULL;
      }
    }
    if ( sub == NULL ) {
      const int numItems = this->items->getLength();
      for ( i = 0; i < numItems; i++ ) {
        item = (ItemRecord *) (*this->items)[i];
        if ( item->pos == j && item->parent == menu ) {
//          fprintf( stderr, "%s%s\n", pre, item->name );
          if ( item->flags & ITEM_SEPARATOR ) {
            item->item = XtVaCreateManagedWidget( item->title,
              xmSeparatorGadgetClass, parent, NULL );
          } else {
            item->item = XtVaCreateManagedWidget( item->title,
              xmToggleButtonGadgetClass, parent,
              NULL );
            XtAddCallback( item->item, XmNvalueChangedCallback,
                SoXtPopupMenu::itemSelectionCallback, this );
            XmToggleButtonSetState( item->item,
              (item->flags & ITEM_MARKED) ? True : False,
              False );
          }
          break;
        } else {
          item = (ItemRecord *) NULL;
        }
      }
    }
    j++;
  } while ( sub != NULL || item != NULL );
  return parent;
} // traverseBuild()

/*!
*/

Widget
SoXtPopupMenu::build(
  Widget parent )
{
  MenuRecord * root = this->getMenuRecord( 0 );
  assert( root != NULL );
  Widget popup = XmCreatePopupMenu( parent, root->name, NULL, 0 );
//  fprintf( stderr, "%s {\n", root->name );
  (void) this->traverseBuild( popup, root, 2 );
//  fprintf( stderr, "}\n" );
  return popup;
} // build()

// *************************************************************************

/*!
*/

MenuRecord *
SoXtPopupMenu::getMenuRecord(
  int menuid )
{
  const int numMenus = this->menus->getLength();
  int i;
  for ( i = 0; i < numMenus; i++ )
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
  int i;
  for ( i = 0; i < numItems; i++ )
    if ( ((ItemRecord *) (*this->items)[i])->itemid == itemid )
      return (ItemRecord *) (*this->items)[i];
  return (ItemRecord *) NULL;
} // getItemRecord()

// *************************************************************************

/*!
*/

MenuRecord *
SoXtPopupMenu::createMenuRecord(
  char * name )
{
  MenuRecord * rec = new MenuRecord;
  rec->menuid = -1;
  rec->pos = -1;
  rec->name = strcpy( new char [strlen(name)+1], name );
  rec->title = strcpy( new char [strlen(name)+1], name );
  rec->menu = (Widget) NULL;
  rec->parent = NULL;
  return rec;
} // create()

/*!
*/

ItemRecord *
SoXtPopupMenu::createItemRecord(
  char * name )
{
  ItemRecord * rec = new ItemRecord;
  rec->itemid = -1;
  rec->pos = -1;
  rec->flags = 0;
  rec->name = strcpy( new char [strlen(name)+1], name );
  rec->title = strcpy( new char [strlen(name)+1], name );
  rec->item = (Widget) NULL;
  rec->parent = NULL;
  return rec;
} // create()

// *************************************************************************

/*!
*/

void
SoXtPopupMenu::itemSelection( // private
  Widget w,
  XtPointer call )
{
  if ( w == NULL )
    return;
  XmToggleButtonCallbackStruct * data = (XmToggleButtonCallbackStruct *) call;
  const int numItems = this->items->getLength();
  int i;
  for ( i = 0; i < numItems; i++ ) {
    ItemRecord * rec = (ItemRecord *) (*this->items)[i];
    if ( rec->item == w ) {
      int groupid = this->GetRadioGroup( rec->itemid );
      if ( data->set && groupid != -1 ) {
        this->SetMenuItemMarked( rec->itemid, TRUE );
        this->InvokeMenuSelection( rec->itemid );
      } else {
        if ( groupid == -1 ) {
          this->SetMenuItemMarked( rec->itemid, FALSE );
          this->InvokeMenuSelection( rec->itemid );
        } else if ( this->GetRadioGroupSize( groupid ) > 1 ) {
          this->SetMenuItemMarked( rec->itemid, TRUE );
          this->InvokeMenuSelection( rec->itemid );
        } else {
          this->SetMenuItemMarked( rec->itemid, FALSE );
          this->InvokeMenuSelection( rec->itemid );
        }
      }
    }
  }
} // itemSelection()

/*!
*/

void
SoXtPopupMenu::itemSelectionCallback( // private, static
  Widget w,
  XtPointer client_data,
  XtPointer call_data )
{
  assert( client_data != NULL );
  SoXtPopupMenu * popup = (SoXtPopupMenu *) client_data;
  popup->itemSelection( w, call_data );
} // itemSelectionCallback()

// *************************************************************************