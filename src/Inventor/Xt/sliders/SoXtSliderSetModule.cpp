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

#include <assert.h>

#include <Inventor/Xt/widgets/compound/SoXtSliderManager.h>

#include <Inventor/Xt/sliders/SoXtSliderSetModule.h>

/*!
  \class SoXtSliderSetModule Inventor/Xt/sliders/SoXtSliderSetModule.h
  \brief The SoXtSliderSetModule class is yet to be documented.

  This class is internal to SoXt and may change interface without notice.
*/

// *************************************************************************

/*!
  Protected constructor used by derived classes.
*/

SoXtSliderSetModule::SoXtSliderSetModule( // protected
  const Widget parent,
  const char * const name,
  const SbBool embed,
  SoNode * const node,
  const SbBool build )
: inherited( parent, name, embed, node )
{
  this->sliderset = NULL;
  this->slidersetSize = 1;
  this->setClassName( this->getDefaultWidgetName() );

  if ( build ) {
    Widget module = this->buildWidget( this->getParentWidget() );
    this->setBaseWidget( module );
  }
} // SoXtSliderSetModule()

/*!
  Destructor.
*/

SoXtSliderSetModule::~SoXtSliderSetModule( // protected
  void )
{
} // ~SoXtSliderSetModule()

// *************************************************************************

/*!
*/

void
SoXtSliderSetModule::setSliderSetSize(
  int sliders )
{
  assert( this->sliderset == NULL );
  this->slidersetSize = sliders;
} // setSliderSetSize()

/*!
*/

int
SoXtSliderSetModule::getSliderSetSize(
  void ) const
{
  return this->slidersetSize;
} // getSliderSetSize()

// *************************************************************************

/*!
*/

Widget
SoXtSliderSetModule::buildWidget(
  Widget parent )
{
  assert( this->sliderset == NULL );
  this->sliderset = new SoXtSliderManager( parent,
    this->getDefaultWidgetName(), this->slidersetSize );
  Widget sliders = this->sliderset->getBaseWidget();
  this->registerWidget( sliders );
  return sliders;
} // buildWidget()

// *************************************************************************

/*!
*/

const char *
SoXtSliderSetModule::getDefaultWidgetName( // virtual
  void ) const
{
  static const char defaultWidgetName[] = "SoXtSliderSet";
  return defaultWidgetName;
} // getDefaultWidgetName()

// *************************************************************************

/*!
  \fn void SoXtSliderSetModule::valueChanged( float, int ) = 0
*/

/*!
*/

void
SoXtSliderSetModule::valueChangedCB(
  void * user,
  float value,
  int slider )
{
  assert( user != NULL );
  SoXtSliderSetModule * const sliderset = (SoXtSliderSetModule *) user;
  sliderset->valueChanged( value, slider );
} // valueChangedCB()

// *************************************************************************

#if SOXT_DEBUG
static const char * getSoXtSliderSetModuleRCSId(void) { return rcsid; }
#endif // SOXT_DEBUG

