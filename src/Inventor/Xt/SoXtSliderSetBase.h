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

#ifndef SOXT_SLIDERSETBASE_H
#define SOXT_SLIDERSETBASE_H

#include <Inventor/Xt/SoXtComponent.h>

class SoNode;

// *************************************************************************

class SOXT_DLL_API SoXtSliderSetBase : public SoXtComponent {
  SOXT_OBJECT_ABSTRACT_HEADER(SoXtSliderSetBase, SoXtComponent);

public:
  virtual void setNode(SoNode * node);
  SoNode * getNode(void) const;

  virtual void getLayoutSize(int & width, int & height);
  virtual void show(void);

protected:
  SoXtSliderSetBase(
    const Widget parent = NULL,
    const char * const name = NULL,
    const SbBool embed = TRUE,
    SoNode * const node = NULL);
  virtual ~SoXtSliderSetBase(void);

private:
  SoNode * node;

}; // class SoXtSliderSetBase

// *************************************************************************

#endif // ! SOXT_SLIDERSETBASE_H
