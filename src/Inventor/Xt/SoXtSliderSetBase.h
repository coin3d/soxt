/**************************************************************************\
 *
 *  This file is part of the Coin family of 3D visualization libraries.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and / or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use this library in software that is incompatible
 *  with the GNU GPL, and / or you would like to take advantage of the
 *  additional benefits with regard to our support services, please
 *  contact Systems in Motion about acquiring a Coin Professional
 *  Edition License.  See <URL:http://www.coin3d.org> for more
 *  information.
 *
 *  Systems in Motion, Abels gate 5, Teknobyen, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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
