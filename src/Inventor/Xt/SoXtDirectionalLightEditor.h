/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

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
  void * user, const SoDirectionalLight * light);

class SOXT_DLL_API SoXtDirectionalLightEditor : public SoXtComponent {
  SOXT_OBJECT_HEADER(SoXtDirectionalLightEditor, SoXtComponent);

public:
  SoXtDirectionalLightEditor(
    Widget parent = NULL,
    const char * const name = NULL,
    SbBool embed = TRUE);
  ~SoXtDirectionalLightEditor(void);

  void attach(SoPath * pathToLight);
  void detach(void);
  SbBool isAttached(void);

  void setLight(const SoDirectionalLight & light);
  const SoDirectionalLight & getLight(void) const;

  void addLightChangedCallback(
    SoXtDirectionalLightEditorCB * callback, void * user = NULL);
  void removeLightChangedCallback(
    SoXtDirectionalLightEditorCB * callback, void * user = NULL);

  virtual void show(void);
  virtual void hide(void);

protected:
  SoXtDirectionalLightEditor(
    Widget parent,
    const char * const name,
    SbBool embed,
    SbBool build);

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

  void copyLight(SoDirectionalLight * dst, const SoDirectionalLight * src);
  void updateLocalComponents(void);

private:
  Widget buildWidget(Widget parent);
  void constructor(SbBool build);

}; // class SoXtDirectionalLightEditor

// *************************************************************************

#endif // ! SOXT_DIRECTIONALLIGHTEDITOR_H
