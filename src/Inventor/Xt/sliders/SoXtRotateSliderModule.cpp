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

#include <Inventor/Xt/sliders/SoXtRotateSliderModule.h>

/*!
  \class SoXtRotateSliderModule Inventor/Xt/sliders/SoXtRotateSliderModule.h
  \brief The SoXtRotateSliderModule class is yet to be documented.

  This class is internal to SoXt (consider it private) and may change
  interface without notice.
*/

// *************************************************************************

/*!
*/

SoXtRotateSliderModule::SoXtRotateSliderModule(
  const Widget parent, 
  const char * const name, 
  const SbBool embed, 
  SoNode * const node )
: inherited( parent, name, embed, node, FALSE )
{
} // SoXtRotateSliderModule()

/*!
*/

SoXtRotateSliderModule::~SoXtRotateSliderModule(
  void )
{
} // ~SoXtRotateSliderModule()

// *************************************************************************

/*!
*/

void
SoXtRotateSliderModule::valueChanged( // virtual, protected
  float value,
  int slider )
{
} // valueChanged()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtRotateSliderModuleRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG
