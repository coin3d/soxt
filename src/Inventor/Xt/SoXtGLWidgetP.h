/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#ifndef SOXTGLWIDGETP_H
#define SOXTGLWIDGETP_H

#include <Inventor/Xt/SoGuiGLWidgetP.h>

// ************************************************************************

// This class contains private data and methods used within the
// SoXtGLWidget class.

class SoXtGLWidgetP : public SoGuiGLWidgetP
{
public:
  SoXtGLWidgetP(SoXtGLWidget * publ);
  ~SoXtGLWidgetP();

  void initNormalContext(void);

  SbVec2s glsize; // cached GL widget size

  // FIXME: none of these are really supported, and the initialization
  // values are probably wrong. 20011012 mortene.
  int normalcolormapsize;
  int overlaycolormapsize;
  unsigned long transparentpixel;

  SbBool border;
  int borderwidth;

  Widget glxwidget;
  Widget glxmanager;
  GLXContext normalcontext;
  XVisualInfo * normalvisual;
  GLXContext overlaycontext;
  XVisualInfo * overlayvisual;
  SbBool doublebuffer;
  SbBool firstexpose;
  SbBool quadbuffer;
  SbBool alphachannel;
  SbBool stencilbuffer;
  SbBool accumbuffer;
  SbBool needrebuild;
  Colormap colormap;

  void createVisual(void);
  void cleanupVisual(void);

  void buildGLWidget(void);
  void cleanupGLWidget(void);

  static void exposeCB(Widget w, XtPointer closure, XtPointer call_data);

  int buildGLAttrs(int * attrs, int trynum);

  void buildContext(void);
  void cleanupContext(void);

protected:
  virtual SbBool isDirectRendering(void);


};

// ************************************************************************

#endif // !SOXTGLWIDGETP_H
