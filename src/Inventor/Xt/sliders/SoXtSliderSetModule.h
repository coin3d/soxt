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

#ifndef SOXT_SLIDERSETMODULE_H
#define SOXT_SLIDERSETMODULE_H

#include <Inventor/Xt/SoXtSliderSetBase.h>

class SoXtSliderManager;

// *************************************************************************

class SoXtSliderSetModule : public SoXtSliderSetBase {
  typedef SoXtSliderSetBase inherited;

public:
  void setSliderSetSize( int sliders );
  int getSliderSetSize(void) const;

protected:
  SoXtSliderSetModule(
    const Widget parent,
    const char * const name,
    const SbBool embed,
    SoNode * const node,
    const SbBool build );

  virtual ~SoXtSliderSetModule(void);

  virtual const char * getDefaultWidgetName(void) const;
  virtual void valueChanged( float value, int slider ) = 0;

  Widget buildWidget( Widget parent );

private:
  static void valueChangedCB( void * user, float value, int slider );

  SoXtSliderManager * sliderset;
  int slidersetSize;

}; // SoXtSliderSetModule

// *************************************************************************

#endif // ! SOXT_SLIDERSETMODULE_H
