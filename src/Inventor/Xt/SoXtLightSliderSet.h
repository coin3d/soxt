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

#ifndef SOXT_LIGHTSLIDERSET_H
#define SOXT_LIGHTSLIDERSET_H

#include <Inventor/Xt/SoXtSliderSet.h>

// *************************************************************************

class SOXT_DLL_EXPORT SoXtLightSliderSet : public SoXtSliderSet {
  typedef SoXtSliderSet inherited;

public:
  SoXtLightSliderSet(
    const Widget parent = NULL,
    const char * const name = NULL,
    const SbBool embed = TRUE,
    SoNode * const node = NULL );
  ~SoXtLightSliderSet(void);

protected:
  SoXtLightSliderSet(
    const Widget parent,
    const char * const name,
    const SbBool embed,
    SoNode * const node,
    const SbBool build );

  Widget buildWidget( Widget parent );

  virtual const char * getDefaultWidgetName(void) const;
  virtual const char * getDefaultTitle(void) const;
  virtual const char * getDefaultIconTitle(void) const;

private:
  void constructor( const SbBool build );

}; // class SoXtLightSliderSet

// *************************************************************************

#endif // ! SOXT_LIGHTSLIDERSET_H
