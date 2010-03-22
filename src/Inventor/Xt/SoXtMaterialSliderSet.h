/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

// $Id$

#ifndef SOXT_MATERIALSLIDERSET_H
#define SOXT_MATERIALSLIDERSET_H

#include <Inventor/Xt/SoXtSliderSet.h>

// *************************************************************************

class SOXT_DLL_API SoXtMaterialSliderSet : public SoXtSliderSet {
  SOXT_OBJECT_HEADER(SoXtMaterialSliderSet, SoXtSliderSet);

public:
  SoXtMaterialSliderSet(
    Widget parent = NULL,
    const char * const name = NULL,
    SbBool embed = TRUE,
    SoNode * const node = NULL);
  ~SoXtMaterialSliderSet(void);

protected:
  SoXtMaterialSliderSet(
    Widget parent,
    const char * const name,
    SbBool embed,
    SoNode * const node,
    SbBool build);

  Widget buildWidget(Widget parent);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

private:
  void constructor(SbBool build);

}; // class SoXtMaterialSliderSet

// *************************************************************************

#endif // ! SOXT_MATERIALSLIDERSET_H
