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
