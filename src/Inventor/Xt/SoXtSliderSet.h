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

#ifndef SOXT_SLIDERSET_H
#define SOXT_SLIDERSET_H

#include <Inventor/Xt/SoXtSliderSetBase.h>

class SoXtSlider;
struct SoXtSliderSetClosure;

// *************************************************************************

class SOXT_DLL_EXPORT SoXtSliderSet : public SoXtSliderSetBase {
  typedef SoXtSliderSetBase inherited;

public:
  SoXtSliderSet(
    const Widget parent,
    const char * const name,
    const SbBool embed,
    SoNode * const node );

  virtual void setNode( SoNode * node );
  void updateLayout( void );

protected:
  SoXtSliderSet(
    const Widget parent,
    const char * const name,
    const SbBool embed,
    SoNode * const node,
    const SbBool build );
  ~SoXtSliderSet(void);

  Widget buildWidget( Widget parent );

private:
  void constructor( const SbBool build );

  void buildSliderModule( const Widget rowcolumn );
  void buildSliderSet( const Widget rowcolumn );

  void folding( SoXtSliderSetClosure * closure, Widget button );
  static void folding_cb( Widget, XtPointer, XtPointer );
  void style( SoXtSliderSetClosure * closure );
  static void style_cb( Widget, XtPointer, XtPointer );
  void slider( SoXtSliderSetClosure * closure, char * title, float value );
  static void slider_cb( void * closure, char * title, float value );

private:
  int numSliders;
  SoXtSlider ** sliders;

  SbPList * closures;

}; // class SoXtSliderSet

// *************************************************************************

#endif // ! SOXT_SLIDERSET_H
