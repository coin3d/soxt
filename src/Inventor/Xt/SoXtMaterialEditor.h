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

#ifndef SOXT_MATERIALEDITOR_H
#define SOXT_MATERIALEDITOR_H

#include <Inventor/Xt/SoXtComponent.h>

class SoMaterial;
class SoVRMLMaterial;

// *************************************************************************

typedef void SoXtMaterialEditorCB( void * userdata,
  const SoMaterial * material );

class SOXT_DLL_EXPORT SoXtMaterialEditor : public SoXtComponent {
  typedef SoXtComponent inherited;

public:
  enum UpdateFrequency {
    CONTINUOUS, AFTER_ACCEPT
  };

  SoXtMaterialEditor(
    Widget parent = NULL,
    const char * name = NULL,
    SbBool inParent = TRUE );
  ~SoXtMaterialEditor(void);

  void attach( SoMaterial * material, int index = 0 );
  void attach( SoVRMLMaterial * material );
  void detach(void);
  SbBool isAttached(void);

  void addMaterialChangedCallback(
    SoXtMaterialEditorCB * callback, void * user = NULL );
  void removeMaterialChangedCallback(
    SoXtMaterialEditorCB * callback, void * user = NULL );

  void setUpdateFrequency( SoXtMaterialEditor::UpdateFrequency freq );
  SoXtMaterialEditor::UpdateFrequency getUpdateFrequency(void) const;

  virtual void show(void);
  virtual void hide(void);

  void setMaterial( const SoMaterial & mtl );
  void setMaterial( const SoVRMLMaterial & mtl );
  const SoMaterial & getMaterial(void) const;
  SbBool isAttachedVRML(void);

protected:
  SoXtMaterialEditor(
    Widget parent,
    const char * const name,
    SbBool inParent,
    SbBool build );

  Widget buildWidget( Widget parent );

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

private:
  void constructor( SbBool build );

}; // class SoXtMaterialEditor

// *************************************************************************

#endif // ! SOXT_MATERIALEDITOR_H
