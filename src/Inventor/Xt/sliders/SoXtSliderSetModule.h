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

#include <Inventor/Xt/SoXtSliderSet.h>

class SoXtSlider;

// *************************************************************************

class SoXtSliderSetModule : public SoXtSliderSet {
  typedef SoXtSliderSet inherited;

public:
  SoXtSliderSetModule(
    const Widget parent = NULL,
    const char * const name = NULL,
    const SbBool embed = TRUE,
    SoNode * const node = NULL );
  virtual ~SoXtSliderSetModule(void);

  int getNumSliders(void) const;

  void setFolding(const SbBool enable);
  SbBool isFolded(void) const;

protected:
  SoXtSliderSetModule(
    const Widget parent,
    const char * const name,
    const SbBool embed,
    SoNode * const node,
    const SbBool build );

  virtual void getLayoutSize( int & width, int & height );
//  virtual const char * getModuleTitle(void) const = 0;
//  virtual const char * getSliderTitle(int slider) const = 0;

  virtual const char * getDefaultWidgetName(void) const;

  Widget buildWidget( Widget parent );

  virtual void valueChanged( float value, int slider );

private:
  Widget form;           // XmForm :
  Widget head;           //   XmForm :
  Widget styleToggle;    //     XmPushButton
  Widget foldingToggle;  //     XmToggleButton
  Widget body;           //   XmForm

  int numSliders;
  SoXtSlider ** sliders;

  SbBool folded;

private:
  void constructor( const SbBool build );

  static void toggleFoldingCB( Widget, XtPointer, XtPointer );
  static void toggleStyleCB( Widget, XtPointer, XtPointer );

  static void valueChangedCB( void * user, float value, int slider );

}; // SoXtSliderSetModule

// *************************************************************************

#endif // ! SOXT_SLIDERSETMODULE_H
