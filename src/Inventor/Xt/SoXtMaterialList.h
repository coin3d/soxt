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

#ifndef SOXT_MATERIALLIST_H
#define SOXT_MATERIALLIST_H

#include <Inventor/Xt/SoXtComponent.h>

class SoMaterial;
class SoAnyMaterialList;

typedef void SoXtMaterialListCB( void * closure, const SoMaterial * material );

// *************************************************************************

class SOXT_DLL_EXPORT SoXtMaterialList : public SoXtComponent {
  SOXT_OBJECT_HEADER(SoXtMaterialList, SoXtComponent);

public:
  SoXtMaterialList(
    Widget parent = NULL,
    const char * const name = NULL,
    SbBool embed = TRUE,
    const char * const dir = NULL );
  ~SoXtMaterialList(void);

  void addCallback(
    SoXtMaterialListCB * const callback,
    void * const closure = NULL );
  void removeCallback(
    SoXtMaterialListCB * const callback,
    void * const closure = NULL );

protected:
  SoXtMaterialList(
    Widget parent,
    const char * const name,
    SbBool embed,
    const char * const dir,
    SbBool build );

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  Widget buildWidget( Widget parent );
  Widget buildPulldownMenu( Widget parent );

private:
  void constructor( const char * const dir, const SbBool build );

  void selectionCallback( int i );
  static void selection_cb( Widget, XtPointer, XtPointer );

  void paletteMenuCallback( Widget );
  static void palette_menu_cb( Widget, XtPointer, XtPointer );

  Widget listwidget;

  SoAnyMaterialList * common;

}; // public SoXtMaterialList

// *************************************************************************

#endif // ! SOXT_MATERIALLIST_H
