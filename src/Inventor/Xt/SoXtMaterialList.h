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

#ifndef SOXT_MATERIALLIST_H
#define SOXT_MATERIALLIST_H

#include <Inventor/Xt/SoXtComponent.h>

class SoMaterial;

// *************************************************************************

typedef void SoXtMaterialListCB( void * user, const SoMaterial * material );

class SoXtMaterialList : public SoXtComponent {
  typedef SoXtComponent inherited;

public:
  SoXtMaterialList(
    Widget parent = NULL,
    const char * const name = NULL,
    SbBool inParent = TRUE,
    const char * const dir = NULL );
  ~SoXtMaterialList(void);

  void addCallback(
    SoXtMaterialListCB * const callback,
    void * const userdata = NULL );
  void removeCallback(
    SoXtMaterialListCB * const callback,
    void * const userdata = NULL );

protected:
  SoXtMaterialList(
    Widget parent,
    const char * const name,
    SbBool inParent,
    const char * const dir,
    SbBool build );

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  Widget buildWidget( Widget parent );
  Widget buildPulldownMenu( Widget parent );

private:
  void constructor( SbBool build );

}; // public SoXtMaterialList

// *************************************************************************

#endif // ! SOXT_MATERIALLIST_H
