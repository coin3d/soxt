/**************************************************************************\
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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#include <math.h>
#include <stdlib.h>

#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText3.h>

// *************************************************************************

SoNode *
shape_to_scene_graph(
  SoShape * shape )
{
  SoSeparator * root = new SoSeparator;
  SoBaseColor * basecol = new SoBaseColor;
  basecol->rgb.setValue( float(rand())/float(RAND_MAX),
                         float(rand())/float(RAND_MAX),
                         float(rand())/float(RAND_MAX) );
  root->addChild( basecol );
  root->addChild( shape );
  return (SoNode *) root;
} // shape_to_scene_graph()

// *************************************************************************

SoNode *
get_scene_graph(
  int argc,
  char ** argv )
{
  srand( (unsigned int) time(NULL) );

  if ( argc > 1 ) {
#if HAVE_DIME2SO
#endif
#if HAVE_PROFIT2SO
    if ( prf_openflight_check_file( argv[1] ) )
      return prf_openinventor_import( argv[1] );
#endif
    SoInput in;
    if ( ! strcmp( argv[1], "--cube" ) ) {
      return shape_to_scene_graph( new SoCube );
    } else if ( ! strcmp( argv[1], "--sphere" ) ) {
      return shape_to_scene_graph( new SoSphere );
    } else if ( ! strcmp( argv[1], "--cone" ) ) {
      return shape_to_scene_graph( new SoCone );
    } else if ( ! strcmp( argv[1], "--cylinder" ) ) {
      return shape_to_scene_graph( new SoCylinder );
    } else if ( ! strcmp( argv[1], "--text3" ) ) {
      SoText3 * textnode = new SoText3;
      if ( argc > 2 ) textnode->string.setValue( argv[2] );
      else textnode->string.setValue( "Coin" );
      textnode->parts.setValue( SoText3::ALL );
      return shape_to_scene_graph( textnode );
    } else if ( ! in.openFile( argv[1] ) ) {
      return NULL;
    }
    return SoDB::readAll( &in );
  }

  int randval = rand() % 4;
  switch ( randval ) {
  case 0:  return shape_to_scene_graph( new SoCone );
  case 1:  return shape_to_scene_graph( new SoCube );
  case 2:  return shape_to_scene_graph( new SoCylinder );
  default: return shape_to_scene_graph( new SoSphere );
  }
} // get_scene_graph()

// *************************************************************************
