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

#if SOXT_DEBUG
static const char rcsid[] =
  "$Id$";
#endif // SOXT_DEBUG

#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/PushB.h>
#include <Xm/ToggleB.h>

#include <Inventor/Xt/widgets/compound/SoXtSlider.h>

#include <Inventor/Xt/widgets/compound/SoXtSliderManager.h>

/*!
  \class SoXtSliderManager Inventor/Xt/widgets/compound/SoXtSliderManager.h
  \brief The SoXtSliderManager class manages a set of sliders, and a mechanism
  to fold and unfold the slider set.

  This class is internal to SoXt and may change interface without notice.
*/

// *************************************************************************

/*!
*/

SoXtSliderManager::SoXtSliderManager(
  Widget parent,
  const char * const name,
  int sliders )
{
  this->form = NULL;
  this->head = NULL;
  this->body = NULL;
  this->openToggle = NULL;
  this->styleToggle = NULL;

  this->numSliders = sliders;
  this->sliders = NULL;
} // SoXtSliderManager()

/*!
*/

SoXtSliderManager::~SoXtSliderManager(
  void )
{
} // ~SoXtSliderManager()

// *************************************************************************

/*!
  This method returns the base widget of the whole slider set.
*/

Widget
SoXtSliderManager::getBaseWidget(
  void ) const
{
  return this->form;
} // getBaseWidget()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtSliderManagerRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

