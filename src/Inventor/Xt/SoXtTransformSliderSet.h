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

#ifndef SOXT_TRANSFORMSLIDERSET_H
#define SOXT_TRANSFORMSLIDERSET_H

#include <Inventor/Xt/SoXtSliderSet.h>

class SoNode;

// *************************************************************************

class SOXT_DLL_API SoXtTransformSliderSet : public SoXtSliderSet {
  SOXT_OBJECT_HEADER(SoXtTransformSliderSet, SoXtSliderSet);

public:
  SoXtTransformSliderSet(
    Widget parent = NULL,
    const char * const name = NULL,
    const SbBool embed = TRUE,
    SoNode * const node = NULL);
  ~SoXtTransformSliderSet(void);

protected:
  SoXtTransformSliderSet(
    Widget parent,
    const char * const name,
    const SbBool embed,
    SoNode * const node,
    const SbBool build);

  Widget buildWidget(Widget parent);

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

private:
  void constructor(SbBool build);

}; // class SoXtTransformSliderSet

// *************************************************************************

#endif // ! SOXT_TRANSFORMSLIDERSET_H
