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

#ifndef SOXT_SCALESLIDERMODULE_H
#define SOXT_SCALESLIDERMODULE_H

#include <Inventor/Xt/sliders/SoXtSliderSetModule.h>

// *************************************************************************

class SoXtScaleSliderModule : public SoXtSliderSetModule {
  typedef SoXtSliderSetModule inherited;

public:
  SoXtScaleSliderModule(
    Widget parent = NULL,
    const char * const name = NULL,
    const SbBool embed = TRUE,
    SoNode * const node = NULL );
  ~SoXtScaleSliderModule(void);

private:
  virtual void valueChanged( float value, int slider );

}; // class SoXtScaleSliderModule

// *************************************************************************

#endif // ! SOXT_SCALESLIDERMODULE_H
