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

#ifndef SOXT_EXAMINERVIEWER_H
#define SOXT_EXAMINERVIEWER_H

#include <Inventor/Xt/viewers/SoXtFullViewer.h>

// *************************************************************************

class SoXtExaminerViewer : public SoXtFullViewer {
  typedef SoXtFullViewer inherited;

public:
  SoXtExaminerViewer( Widget parent = (Widget) NULL,
      const char * name = NULL, SbBool inParent = TRUE,
      SoXtFullViewer::BuildFlag flag = BUILD_ALL,
      SoXtViewer::Type type = BROWSER );
  ~SoXtExaminerViewer(void);

protected:
  SoXtExaminerViewer( Widget parent, const char * name,
      SbBool inParent, SoXtFullViewer::BuildFlag flag,
      SoXtViewer::Type type, SbBool build );

private:
  void constructor( SbBool build );

}; // class SoXtExaminerViewer

// *************************************************************************

#endif // ! SOXT_EXAMINERVIEWER_H
