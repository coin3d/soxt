/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#ifndef SOXTEXAMINERVIEWERP_H
#define SOXTEXAMINERVIEWERP_H

#include <Inventor/Xt/viewers/SoGuiExaminerViewerP.h>

class SoXtExaminerViewer;

// ************************************************************************

// This class contains private data and methods used within the
// SoXtExaminerViewer class.

class SoXtExaminerViewerP : public SoGuiExaminerViewerP
{
public:
  SoXtExaminerViewerP(SoXtExaminerViewer * publ);
  ~SoXtExaminerViewerP();

  void camerabuttonClicked(void);
  static void camerabuttonCB(Widget, XtPointer, XtPointer);

  void constructor(const SbBool build);

  Widget camerabutton;
  struct {
    Pixmap ortho, ortho_ins;
    Pixmap perspective, perspective_ins;
    Pixmap nocam, nocam_ins;
  } camerapixmaps;
};

// ************************************************************************

#endif // ! SOXTEXAMINERVIEWERP_H
