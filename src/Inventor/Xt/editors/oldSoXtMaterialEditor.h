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

#ifndef SOXT_MATERIALEDITOR_H
#define SOXT_MATERIALEDITOR_H

#include <Inventor/Xt/SoXtComponent.h>

class SoMaterial;
class SoVRMLMaterial;

// *************************************************************************

typedef void SoXtMaterialEditorCB(void * userdata,
  const SoMaterial * material);

class SOXT_DLL_API SoXtMaterialEditor : public SoXtComponent {
  SOXT_OBJECT_HEADER(SoXtMaterialEditor, SoXtComponent);

public:
  enum UpdateFrequency {
    CONTINUOUS, AFTER_ACCEPT
  };

  SoXtMaterialEditor(
    Widget parent = NULL,
    const char * name = NULL,
    SbBool embed = TRUE);
  ~SoXtMaterialEditor(void);

  void attach(SoMaterial * material, int index = 0);
  void attach(SoVRMLMaterial * material);
  void detach(void);
  SbBool isAttached(void);

  void addMaterialChangedCallback(
    SoXtMaterialEditorCB * callback, void * user = NULL);
  void removeMaterialChangedCallback(
    SoXtMaterialEditorCB * callback, void * user = NULL);

  void setUpdateFrequency(SoXtMaterialEditor::UpdateFrequency freq);
  SoXtMaterialEditor::UpdateFrequency getUpdateFrequency(void) const;

  virtual void show(void);
  virtual void hide(void);

  void setMaterial(const SoMaterial & mtl);
  void setMaterial(const SoVRMLMaterial & mtl);
  const SoMaterial & getMaterial(void) const;
  SbBool isAttachedVRML(void);

protected:
  SoXtMaterialEditor(
    Widget parent,
    const char * const name,
    SbBool embed,
    SbBool build);

  Widget buildWidget(Widget parent);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

private:
  void constructor(SbBool build);

}; // class SoXtMaterialEditor

// *************************************************************************

#endif // ! SOXT_MATERIALEDITOR_H
