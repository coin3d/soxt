/**************************************************************************\
 *
 *  This file is part of a set of example programs for the Coin library.
 *  Copyright (C) 2000-2003 by Systems in Motion. All rights reserved.
 *
 *                   <URL:http://www.coin3d.org>
 *
 *  This sourcecode can be redistributed and/or modified under the
 *  terms of the GNU General Public License version 2 as published by
 *  the Free Software Foundation. See the file COPYING at the root
 *  directory of the distribution for more details.
 *
 *  As a special exception, all sourcecode of the demo examples can be
 *  used for any purpose for licensees of the Coin Professional
 *  Edition License, without the restrictions of the GNU GPL. See our
 *  web pages for information about how to acquire a Professional Edition
 *  License.
 *
 *  Systems in Motion, <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#include <Inventor/nodes/SoCube.h>

#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>

// *************************************************************************

int
main(
  int argc,
  char ** argv )
{
  Widget window = SoXt::init( argv[0] );

  SoNode * root = new SoCube;

  SoXtExaminerViewer * viewer = new SoXtExaminerViewer( window );

  viewer->setSceneGraph( root );
  viewer->show();
  SoXt::show( window );

  SoXt::mainLoop();

  delete viewer;
  return 0;
} // main()

// *************************************************************************
