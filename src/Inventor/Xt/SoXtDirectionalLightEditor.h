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

#ifndef SOXT_DIRECTIONALLIGHTEDITOR_H
#define SOXT_DIRECTIONALLIGHTEDITOR_H

#include <Inventor/nodes/SoDirectionalLight.h>

#include <Inventor/Xt/SoXtComponent.h>

class SoPath;
class SoSeparator;
class SoCamera;
class SoPerspectiveCamera;
class SoNodeSensor;
class SoDirectionalLightManip;
class SoCallbackList;
class SoXtClipboard;
class SoXtRenderArea;
class _SoXtColorEditor;
class _SoXtColorSlider;

// *************************************************************************

typedef void SoXtDirectionalLightEditorCB(
  void * user, const SoDirectionalLight * light );

class SOXT_DLL_API SoXtDirectionalLightEditor : public SoXtComponent {
  SOXT_OBJECT_HEADER(SoXtDirectionalLightEditor, SoXtComponent);

public:
  SoXtDirectionalLightEditor(
    Widget parent = NULL,
    const char * const name = NULL,
    SbBool embed = TRUE );
  ~SoXtDirectionalLightEditor(void);

  void attach( SoPath * pathToLight );
  void detach(void);
  SbBool isAttached(void);

  void setLight( const SoDirectionalLight & light );
  const SoDirectionalLight & getLight(void) const;

  void addLightChangedCallback(
    SoXtDirectionalLightEditorCB * callback, void * user = NULL);
  void removeLightChangedCallback(
    SoXtDirectionalLightEditorCB * callback, void * user = NULL );

  virtual void show(void);
  virtual void hide(void);

protected:
  SoXtDirectionalLightEditor(
    Widget parent,
    const char * const name,
    SbBool embed,
    SbBool build );

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

  SoDirectionalLight * dirLight;
  SoSeparator * root;
  SoSeparator * litStuff;

  SoPerspectiveCamera * myCamera;
  SoCamera * cameraToWatch;

  _SoXtColorEditor * colorEditor;
  _SoXtColorSlider * intensitySlider;
  SoXtRenderArea * renderArea;
  SoNodeSensor * lightSensor;
  SoNodeSensor * cameraSensor;
  SoDirectionalLightManip * dirLightManip;
  static char * geomBuffer;
  SbBool ignoreCallback;
  SoCallbackList * callbackList;

  SoXtClipboard * clipboard;

  void copyLight( SoDirectionalLight * dst, const SoDirectionalLight * src );
  void updateLocalComponents(void);

private:
  Widget buildWidget( Widget parent );
  void constructor( SbBool build );

}; // class SoXtDirectionalLightEditor

// *************************************************************************

#endif // ! SOXT_DIRECTIONALLIGHTEDITOR_H
