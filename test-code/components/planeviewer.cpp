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

#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtPlaneViewer.h>

#include "computils.h"

// *************************************************************************

int
main(
  int argc,
  char ** argv )
{
  Widget window = SoXt::init( argv[0] );

  SoNode * root = get_scene_graph( argc, argv );
  if ( ! root ) return -1;

  SoXtPlaneViewer * viewer = new SoXtPlaneViewer( window );
  viewer->setSceneGraph( root );
  viewer->show();
  SoXt::show( window );

  SoXt::mainLoop();

  delete viewer;
  return 0;
} // main()

// *************************************************************************
