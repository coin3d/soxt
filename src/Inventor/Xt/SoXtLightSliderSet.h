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
    Widget parent = NULL,
    const char * const name = NULL,
    SbBool inParent = TRUE,
    SoNode * node = NULL );
  ~SoXtLightSliderSet(void);

protected:
  SoXtLightSliderSet(
    Widget parent,
    const char * const name,
    SbBool inParent,
    SoNode * node,
    SbBool build );

  void constructor( SbBool build );

  Widget buildWidget( Widget parent );

}; // class SoXtLightSliderSet

// *************************************************************************

#endif // ! SOXT_LIGHTSLIDERSET_H
